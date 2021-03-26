/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/

#include <topaz/modules/audio.h>
#include <topaz/modules/resources.h>
#include <topaz/containers/table.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/backends/audio_manager.h>
#include <topaz/assets/sound.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <topaz/asset.h>

#ifdef TOPAZDC_DEBUG
#include <stdio.h>
#endif
/*

    Soundbank
    ---------
    
    
    temporary holding area for sounds that are playing.
    

*/

typedef struct TopazAudioSoundBank TopazAudioSoundBank;
typedef struct {
    uint16_t slot;
    uint16_t bank;
} TASBID;

// Creates a new sound bank
TopazAudioSoundBank * topaz_asb_create(topaz_t *);

// Returns a unique ID corresponding to a sound asset within the soundbank.
// When locked, the sound is marked as "in use"
// NOTE: topaz_asb_lock should be called before a sound is sent to the processor.
////
// Locks are only performed by the AudioManager.
TASBID topaz_asb_lock_ref(TopazAudioSoundBank *, topazAsset_t *);

// Unlocks a sounds within the bank.
// Unlocks are only performed by the AudioManager.
void topaz_asb_lock_unref(TopazAudioSoundBank *, TASBID id);

// Gets the samples for a sound. Thread-safe.
topazArray_t * topaz_asb_get_samples(TopazAudioSoundBank *, TASBID);

// For all fully unlocked sounds that arent in use, will check through 
// topaz' resources to make sure the sounds still exist. If they no longer exist,
// the IDs and slots will be reclaimed for future sounds.
//
// NOTE: can be expensive, so only call occasionally. Under most normal circumstances,
// this wont even be needed since most uses will not remove sound assets.
void topaz_asb_clean(TopazAudioSoundBank *);






/*

    Processor / Manager communication

*/


typedef enum {
    // adds a new sound for playback
    // id: the assigned ID of the sound for interaction / reference.
    //     This id will always refer to the same sound until UINT32_MAX 
    //     sounds have been produced.
    // sound: the ID from the soundbank. This can be used to retrieve the sound's samples 
    //      from the soundbank.
    // value0: the channel to play on. 
    //
    APU__SOUND_START,
    

    // sets the volume for a sound clip.
    // id: sound clip id
    // value0: sound amt, clipped (0, 1)
    APU__SOUND_SET_VOLUME,


    // sets the panning for a sound clip.
    // id: sound clip id
    // value0: sound amt, clipped (0, 1)
    APU__SOUND_SET_PANNING,

    // sets the repeat state for a sound clip.
    // id: sound clip id
    // value0: 0 for no repeat, 1 for repeat.
    APU__SOUND_SET_REPEAT,

    // seeks the clip.
    // id: sound clip id
    // value0: 0 for the start, 1 for the end.
    // Note: setting seek to 1 will result in stopping of the sound.  
    APU__SOUND_SEEK,

    // Pauses the clip, suspending it in the playback state 
    // id: sound clip id    
    APU__SOUND_PAUSE,

    // Pauses the clip, suspending it in the playback state 
    // id: sound clip id        
    APU__SOUND_RESUME,
    
    
    // Sets the volume for a channel. Should be clipped.
    // id: channel
    // value0: amount
    APU__CHANNEL_SET_VOLUME,

    // Sets the panning for a channel. Should be clipped.
    // id: channel
    // value0: amount
    APU__CHANNEL_SET_PANNING,


    // Stops a channel from being active, stopping all active streams 
    // on the channel and removing them.
    // id: channel
    APU__CHANNEL_HALT,



    
    
    // Informs the manager that a sound was successfully stopped.
    // this will let the manager "unlock" the related sound within the bank.
    // this is the ONLY time when this should happen.
    // id: sound clip id 
    // sound: soundbank id
    // 
    APU__RESPONSE__SOUND_FINISHED
    


} TopazAudioOpCode;


// individual command for either the processor
// or the manager.
typedef struct {
    TopazAudioOpCode cmd;
    uint32_t id;
    TASBID sound;
    float value0;
    float value1;
} TopazAudioInstruction;


// A contains of commands to the audio processor
typedef struct {
    // unique ID of this packet. Should be assigned by the 
    // CREATOR of the packet only. Overflow is OK as long as 
    // packets are sequential.
    uint64_t id;
    
    // array of TopazAudioInstruction objects.
    topazArray_t * instructions;    
} TopazAudioStatePacket;




// facilitates input / output of commands in a thread-safe-ish way
typedef struct TopazAudioStateStream TopazAudioStateStream;

// Creates a new instance.
static TopazAudioStateStream * topaz_sstream_create();

// Adds an additional command to the current pending packet
// CAN ONLY BE CALLED ON THE CREATION THREAD.
static void topaz_sstream_push_instruction(TopazAudioStateStream * state, TopazAudioInstruction);

// Consolidates instructions to prepare them to be pulled from another StateStream.
static int topaz_sstream_finalize(TopazAudioStateStream * state);


// Transfers the outgoing packet from one dispatcher to another.
// Upon success, TRUE is returned.
static int topaz_sstream_transfer_packet(TopazAudioStateStream * from, TopazAudioStateStream * to);


// Retrieves the last transfered packet from the audio state if available.
static TopazAudioStatePacket * topaz_sstream_get_incoming(TopazAudioStateStream * state);

// Lets the state stream know that there is no longer a need for the incoming reference.
static void topaz_sstream_release_incoming(TopazAudioStateStream * state);











/*
    Audio Processor (likely on separate thread.)


*/
typedef struct TopazAudioProcessor TopazAudioProcessor;


// Creates a new APU
static TopazAudioProcessor * topaz_apu_create(topazAudio_t *);


// Gets the state stream for the APU.
static TopazAudioStateStream * topaz_apu_get_state_stream(TopazAudioProcessor *);

// sent to the audio manager
// Likely run on a separate thread.
static void topaz_apu_main(
    topazAudioManager_t *,
    uint32_t sampleCount,
    float * samples,
    void * tap
);






struct topazAudio_t {
    // command processors
    TopazAudioStateStream * cmd;
    
    // all sounds 
    TopazAudioSoundBank * soundbank;

    // audio processor instance, likely running on separate thread
    TopazAudioProcessor * apu;

    // the actual audio manager.
    topazAudioManager_t * manager;

    // whether audio is streaming or not.
    int streaming;
    
    // active sound object ID pool
    uint32_t aspool;    
    

    topaz_t * ctx;  
};








topazAudio_t * topaz_audio_create(
    /// The context to use.
    topaz_t * context
) {
    topazAudio_t * out = calloc(1, sizeof(topazAudio_t));
    out->ctx = context;

    {
        topazAudioManagerAPI_t api = {};
        topazSystem_Backend_t * ref = topaz_system_create_backend(
            topaz_context_get_system(context), 
            TOPAZ_STR_CAST("audioManager"), 
            &api
        );
        out->manager = topaz_audio_manager_create(
            context,
            ref,
            api
        );
    }

    out->apu = topaz_apu_create(out);
    out->soundbank = topaz_asb_create(context);
    out->cmd = topaz_sstream_create();
    return out;
}



void topaz_audio_destroy(
    topazAudio_t * audio
) {
    audio = NULL;
    audio->cmd = NULL;
    // NOT DONE YET, WILL CRASH
}


void topaz_audio_update(
    topazAudio_t * audio
) {
    // make sure we're connected before doing anything more.
    if (!audio->streaming) {
        audio->streaming = topaz_audio_manager_connect(
            audio->manager,
            topaz_apu_main,
            audio->apu
        );
    }

    // package and condense instructions for the apu to pick up at its leisure.
    topaz_sstream_finalize(audio->cmd);

    // pull responses from the apu
    if (topaz_sstream_transfer_packet(topaz_apu_get_state_stream(audio->apu), audio->cmd)) {
        TopazAudioStatePacket * p = topaz_sstream_get_incoming(audio->cmd);
        if (p) {
            TopazAudioInstruction * inst;
            uint32_t i;
            uint32_t len = topaz_array_get_size(p->instructions);
            for(i = 0; i < len; ++i) {
                inst = &topaz_array_at(p->instructions, TopazAudioInstruction, i);
                switch(inst->cmd) {

                  // release (decrement) lock.
                  case APU__RESPONSE__SOUND_FINISHED:
                    topaz_asb_lock_unref(audio->soundbank, inst->sound);
                    break;
                  default:
                    #ifdef TOPAZDC_DEBUG
                        printf("Unknown notifcation from topaz APU received: %d\n", inst->cmd);
                    #endif
                
                }
            }


            topaz_sstream_release_incoming(audio->cmd);
        }
    }

}




uint32_t topaz_audio_play_sound(
    topazAudio_t * audio, 
    topazAsset_t * asset,
    uint8_t channel
) {

    TopazAudioInstruction inst;
    inst.cmd = APU__SOUND_START;
    inst.sound = topaz_asb_lock_ref(audio->soundbank, asset);
    inst.id = ++audio->aspool;
    inst.value0 = channel;
    topaz_sstream_push_instruction(audio->cmd, inst);
    
    
    inst.cmd = APU__SOUND_SET_VOLUME;
    inst.value0 = 1;
    topaz_sstream_push_instruction(audio->cmd, inst);
    
    inst.cmd = APU__SOUND_SET_PANNING;
    inst.value0 = 0.5;
    topaz_sstream_push_instruction(audio->cmd, inst);

    return inst.id;
}





void topaz_audio_channel_halt(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel
) {
    TopazAudioInstruction inst;
    inst.cmd = APU__CHANNEL_HALT;
    inst.id = channel;
    topaz_sstream_push_instruction(audio->cmd, inst);
}



void topaz_audio_channel_set_volume(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
    /// The amount, from 0 to 1. Will be clamped.
    float amt
) {
    TopazAudioInstruction inst;
    inst.cmd = APU__CHANNEL_SET_VOLUME;
    inst.id = channel;
    inst.value0 = amt;
    if (inst.value0 < 0) inst.value0 = 0;
    if (inst.value0 < 1) inst.value0 = 1;
    topaz_sstream_push_instruction(audio->cmd, inst);
}


void topaz_audio_channel_set_panning(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
    /// The amount, from 0 to 1. Will be clamped.
    float amt
) {
    TopazAudioInstruction inst;
    inst.cmd = APU__CHANNEL_SET_PANNING;
    inst.id = channel;
    inst.value0 = amt;
    if (inst.value0 < 0) inst.value0 = 0;
    if (inst.value0 < 1) inst.value0 = 1;
    topaz_sstream_push_instruction(audio->cmd, inst);
}



topazAudioManager_t * topaz_audio_get_manager(
    /// The audio module to query.
    topazAudio_t * audio
) {
    return audio->manager;
}

   

















//////////// implementation

//////////////////////
///// audio state dispatcher


struct TopazAudioStateStream {
    // Only one incoming at a time until it is processed.
    // Once its retrieved using transfer_packet(), 
    // 1) incomingLast is freed
    // 2) incoming is transfered to incomingLast
    // 3) incoming is set NULL
    TopazAudioStatePacket * incoming;
    TopazAudioStatePacket * incomingLast;

    
    // the prepared, finalized state to be sent to a different dispatcher.
    // Once the user finalize()s, 2 things can occur:
    // IFF the outgoingFinalized instance is NULL (as in either was sent successfully or 
    // first time finalizing), the current outgoing instance is copied to the 
    // outgoingFinalized slot. 
    // IFF the outgoingFinalized is NOT NULL, additional commands are 
    // merged with "outgoing"  
    // 
    // outgoingFinalized is not editable by this instance once created and set.
    TopazAudioStatePacket * outgoingFinalized;

    // Modifiable instance.
    TopazAudioStatePacket * outgoing;
    
    // local ID pool. Only modified on the same thread as the creation thread.
    uint32_t idPool;
};


static TopazAudioStatePacket * topaz_sstream_create_packet(TopazAudioStateStream * d) {
    TopazAudioStatePacket * out = calloc(1, sizeof(TopazAudioStatePacket));
    out->instructions = topaz_array_create(sizeof(TopazAudioInstruction));
    return out;
}   

static void topaz_sstream_destroy_packet(TopazAudioStatePacket * p) {
    topaz_array_destroy(p->instructions);
    free(p);
}


// Creates a new instance.
TopazAudioStateStream * topaz_sstream_create() {
    TopazAudioStateStream * d = calloc(1, sizeof(TopazAudioStateStream));    
    return d;
}


void topaz_sstream_push_instruction(TopazAudioStateStream * state, TopazAudioInstruction command) {
    if (!state->outgoing) {        
        state->outgoing = topaz_sstream_create_packet(state);
    }
    topaz_array_push(state->outgoing->instructions, command);
}


int topaz_sstream_finalize(TopazAudioStateStream * state) {
    if (!(state->outgoing && topaz_array_get_size(state->outgoing->instructions))) return 1;
    // currently occupied / incomplete
    if (state->outgoingFinalized) {
        return 0;    
    }

    //futile @THREADPROTECTION
    state->outgoingFinalized = state->outgoing;
    //futile @THREADPROTECTION

    state->outgoing = NULL;
    return 1;
}

int topaz_sstream_transfer_packet(TopazAudioStateStream * from, TopazAudioStateStream * to) {
    //futile @THREADPROTECTION
    // try again
    if (from->outgoingFinalized &&
        !to->incoming) {
        
        TopazAudioStatePacket * p = from->outgoingFinalized;
        from->outgoingFinalized = NULL; // now from's thread can finalize again. 
                                        // its finalized is forgotten. Its up to "to" to free it
        TopazAudioStatePacket * old = to->incomingLast;
        to->incomingLast = to->incoming;                   
        to->incoming = p;
        if (old)
            topaz_sstream_destroy_packet(old);
        return 1;
    }
    return 0;
    //futile @THREADPROTECTION
}

TopazAudioStatePacket * topaz_sstream_get_incoming(TopazAudioStateStream * state) {
    return state->incoming;
}


// Retrieves the last transfered packet from the audio state if available.
void topaz_sstream_release_incoming(TopazAudioStateStream * state) {
    //futile @THREADPROTECTION
    TopazAudioStatePacket * n = state->incoming;
    if (n) {
        state->incomingLast = n;
        state->incoming = NULL;
    }
    //futile @THREADPROTECTION    
}












///////////////////////
///// soundbank

#define TASBPARTITION_SIZE 256


typedef struct {
    topazAsset_t * slots[TASBPARTITION_SIZE];
} TASBPartition;

struct TopazAudioSoundBank {
    // current known banks. TASBPartition *
    // banksLast contains previous state array for better thread safety
    topazArray_t * banks;
    topazArray_t * banksLast;
    
    
    // asset -> sbid
    topazTable_t * asset2id;

    // assetName -> asset. For checking whether to keep in soundbank when cleaning.
    topazTable_t * name2asset;
    
    // ctx. Mostly used for cleanup.
    topaz_t * ctx;
    
    // IDs ready to be recycled
    topazArray_t * deadIDs;
    
};

TopazAudioSoundBank * topaz_asb_create(topaz_t * ctx) {
    TopazAudioSoundBank * bank = calloc(1, sizeof(TopazAudioSoundBank));
    bank->banks = topaz_array_create(sizeof(TASBPartition*));
    bank->asset2id = topaz_table_create_hash_pointer();
    bank->deadIDs = topaz_array_create(sizeof(TASBID));
    bank->name2asset = topaz_table_create_hash_topaz_string();
    bank->ctx = ctx;
    return bank;
}

// Returns a unique ID corresponding to a sound asset within the soundbank.
// When locked, the sound is marked as "in use"
// NOTE: topaz_asb_lock should be called before a sound is sent to the processor.
//
// Locks are only performed by the AudioManager.
TASBID topaz_asb_lock_ref(TopazAudioSoundBank * bank, topazAsset_t * asset) {
    topaz_sound_tag_in_use(asset);
    TASBID * id = topaz_table_find(bank->asset2id, asset);
    // new asset
    if (!id) {
        uint32_t recyclable = topaz_array_get_size(bank->deadIDs);
        id = malloc(sizeof(TASBID));
        
        topaz_table_insert(bank->name2asset, topaz_asset_get_name(asset), asset);

        // recycle old slot
        if (recyclable) {
            *id = topaz_array_at(bank->deadIDs, TASBID, recyclable-1);
            topaz_array_set_size(bank->deadIDs, recyclable-1);


        // create new slot.
        } else {   
            int needsNewBank = 1;         
            if (topaz_array_get_size(bank->banks)) {
                TASBPartition * p = topaz_array_at(bank->banks, TASBPartition *, topaz_array_get_size(bank->banks)-1);
                uint32_t i;
                for(i = 0; i < TASBPARTITION_SIZE; ++i) {
                    if (!p->slots[i]) {
                        id->bank = topaz_array_get_size(bank->banks)-1;
                        id->slot = i;
                        needsNewBank = 0;
                        break;
                    }
                }        
            }
            if (needsNewBank) {
                TASBPartition * p = calloc(1, sizeof(TASBPartition));
                id->bank = topaz_array_get_size(bank->banks);
                id->slot = 0;


                ///// futile @THREADPROTECTION
                topazArray_t * old = bank->banksLast;
                if (old) {
                    bank->banksLast = NULL;
                    topaz_array_destroy(old);
                }
                topazArray_t * newArray = topaz_array_clone(bank->banks);
                topaz_array_push(newArray, p);
                bank->banksLast = bank->banks;
                bank->banks = newArray;
                ///// futile @THREADPROTECTION                               
            } 
        }
        
        topaz_table_insert(bank->asset2id, asset, id);
        topaz_array_at(bank->banks, TASBPartition *, id->bank)->slots[id->slot] = asset;
    }
    return *id;
}

// Unlocks a sounds within the bank.
// Unlocks are only performed by the AudioManager.
void topaz_asb_lock_unref(TopazAudioSoundBank * bank, TASBID id) {
    topazAsset_t * asset = topaz_array_at(bank->banks, TASBPartition *, id.bank)->slots[id.slot];
    topaz_sound_untag_in_use(asset);
}

// Gets the samples for a sound. Thread-safe.
topazArray_t * topaz_asb_get_samples(TopazAudioSoundBank * bank, TASBID id) {
    topazAsset_t * asset = topaz_array_at(bank->banks, TASBPartition *, id.bank)->slots[id.slot];
    return (topazArray_t*)topaz_sound_get_samples(asset);
}






void topaz_asb_clean(TopazAudioSoundBank * bank) {
    topazArray_t * deadAssets = topaz_array_create(sizeof(topazAsset_t *));
    topazArray_t * deadNames  = topaz_array_create(sizeof(topazAsset_t *));

    topazTableIter_t * iter = topaz_table_iter_create();

    topazResources_t * res = topaz_context_get_resources(bank->ctx);

    for(topaz_table_iter_start(iter, bank->name2asset);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)
    ) {
        topazAsset_t * asset       = topaz_table_iter_get_value(iter);
        const topazString_t * str  = topaz_table_iter_get_key(iter);
        if (topaz_resources_fetch_asset(
            res,
            topazAsset_Type_Sound,
            str        
        ) != asset) {
            str = topaz_string_clone(str);
            topaz_array_push(deadAssets, asset);
            topaz_array_push(deadNames, str);
        }
    }
    
    topaz_table_iter_destroy(iter);
    
    uint32_t i;
    uint32_t len = topaz_array_get_size(deadAssets);
    for(i = 0; i < len; ++i) {
        topazAsset_t * asset = topaz_array_at(deadAssets, topazAsset_t *, i);
        topaz_table_remove(bank->asset2id, asset);
        
        topazString_t * str = topaz_array_at(deadNames, topazString_t *, i);
        topaz_table_remove(bank->name2asset, str);
        topaz_string_destroy(str);
    }
    
    topaz_array_destroy(deadAssets);
    topaz_array_destroy(deadNames);
}












////////////////////////////////////
///////////////// active sound 




   
void topaz_audio_playback_set_volume(
    topazAudio_t * audio, 
    uint32_t id,
    float v
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_VOLUME;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
}

void topaz_audio_playback_set_panning(
    topazAudio_t * audio, 
    uint32_t id,
    float v
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_PANNING;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
}

void topaz_audio_playback_set_repeat(
    topazAudio_t * audio, 
    uint32_t id,
    int b
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_REPEAT;
    if (b) cmd.value0 = 1.f;
    else   cmd.value0 = 0.f;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
}

void topaz_audio_playback_seek(
    topazAudio_t * audio, 
    uint32_t id,
    float v
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SEEK;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );

}

///  Halts the sound.
///
void topaz_audio_playback_stop(
    topazAudio_t * audio, 
    uint32_t id
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_REPEAT;
    cmd.id = id;
    cmd.value0 = 0;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
    
    
    cmd.cmd = APU__SOUND_SEEK;
    cmd.value0 = 1;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
}
/// Pauses the sound if it was playing.
///
void topaz_audio_playback_pause(
    topazAudio_t * audio, 
    uint32_t id
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_PAUSE;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );
}

/// Resumes the sound's playback if it was Pause()ed.
///
void topaz_audio_playback_resume(
    topazAudio_t * audio, 
    uint32_t id
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_RESUME;
    cmd.id = id;
    topaz_sstream_push_instruction(
        audio->cmd, 
        cmd
    );

}





////////////////////////////////////
////// audio processor 


typedef struct TAPUChannel TAPUChannel;
typedef struct TAPUStream  TAPUStream;


// an active sound. 
struct TAPUStream {
    // Previous stream within the channel. if first within the channel,
    // prev will be null.
    TAPUStream * prev;

    // Next stream within the channel. if last within the channel,
    // next will be null.
    TAPUStream * next;
    
    // Soundbank id. Needs to be sent back once stream object is finished
    TASBID bankID;
    
    // ID assigned by the audio instance
    uint32_t activeID;

    // raw samples from soundbank asset
    const topazSound_Sample_t * samples;

    // raw sample count
    uint32_t sampleCount;

    // how many samples have been given to the audio processor.
    // once progress >= sampleCount, the stream is removed.
    uint32_t progress;

    // volume
    float volume;

    // panning
    float panning;

    // repeat 
    int repeat;
    
    // whether paused. When in the pause state, no samples will be processed (progress does not change)
    int pause;
};


// a channel
// when active, prev||next will be non-null;
// when active, stream will be non-null

struct TAPUChannel {
    // prev active channel. If first,
    // prev will be NULL
    // When no longer active, prev should be set NULL.
    TAPUChannel * prev;

    // Next active channel. If last, will be null
    // When no longer active, next should be set NULL.
    TAPUChannel * next;


    // First stream active for the channel.
    // If none, will be null
    TAPUStream * stream;
    
    // volume for the whole channel
    float volume;
    
    // panning for the whole channel.
    float panning;

};


struct TopazAudioProcessor {
    // command processor
    TopazAudioStateStream * cmd;
    
    // the audio managing this apu
    topazAudio_t * audio;

    // all channels
    TAPUChannel channel[0xff];

    // chain of active channels.
    TAPUChannel * active;

    //TODO: object pool for TAPUStreams is probably a good idea.
    topazArray_t * streamObjectPool;

    // ID-mapped for quicker access.
    topazTable_t * streamObjectTable;
    

};









TopazAudioProcessor * topaz_apu_create(topazAudio_t * audio) {
    TopazAudioProcessor * apu = calloc(1, sizeof(TopazAudioProcessor));
    apu->cmd = topaz_sstream_create();
    apu->audio = audio;
    apu->streamObjectPool = topaz_array_create(sizeof(TAPUStream *));
    apu->streamObjectTable = topaz_table_create_hash_pointer();
    uint32_t i;
    for(i = 0; i < 0xff; ++i) {
        apu->channel[i].volume = 1.0;
        apu->channel[i].panning = 0.5;
        
    }
    return apu;
}


// Gets the state stream for the APU.
TopazAudioStateStream * topaz_apu_get_state_stream(TopazAudioProcessor * apu) {
    return apu->cmd;
}


static TAPUStream * topaz_apu_stream_new(TopazAudioProcessor * apu, uint32_t id) {
    TAPUStream * out;
    uint32_t len = topaz_array_get_size(apu->streamObjectPool); 
    if (len) {
        out = topaz_array_at(apu->streamObjectPool, TAPUStream *, len-1);
        out->prev = out->next = NULL;
        topaz_array_set_size(apu->streamObjectPool, len-1);
    } else {    
        out = malloc(sizeof(TAPUStream));
    }
    out->activeID = id;
    topaz_table_insert_by_uint(apu->streamObjectTable, id, out);
    return out;
}

static void topaz_apu_stream_retire(TopazAudioProcessor * apu, TAPUStream * stream) {
    topaz_table_remove_by_uint(apu->streamObjectTable, stream->activeID);
    TopazAudioInstruction inst;
    inst.cmd = APU__RESPONSE__SOUND_FINISHED;
    inst.sound = stream->bankID;
    inst.id = stream->activeID;
    topaz_sstream_push_instruction(apu->cmd, inst);
    topaz_array_push(apu->streamObjectPool, stream);
}



static void topaz_apu_process_commands(TopazAudioProcessor * apu) {
    topaz_sstream_finalize(apu->cmd);

    // pull commands from the audio instance
    if (topaz_sstream_transfer_packet(apu->audio->cmd, apu->cmd)) {
        TopazAudioStatePacket * p = topaz_sstream_get_incoming(apu->cmd);
        if (p) {
            TopazAudioInstruction * inst;
            uint32_t i;
            uint32_t len = topaz_array_get_size(p->instructions);
            for(i = 0; i < len; ++i) {
                inst = &topaz_array_at(p->instructions, TopazAudioInstruction, i);
                switch(inst->cmd) {

                  case APU__SOUND_START: {
                    TAPUChannel * channel = apu->channel+((int)inst->value0);
                    TAPUStream * str = topaz_apu_stream_new(apu, inst->id);
                    topazArray_t * samples = topaz_asb_get_samples(apu->audio->soundbank, inst->sound);
                    str->bankID = inst->sound;
                    if (samples) {
                        str->samples = topaz_array_get_data(samples);
                        str->sampleCount = topaz_array_get_size(samples);
                    } else {
                        str->samples = NULL;
                        str->sampleCount = 0;
                    }
                    str->progress = 0;
                    str->repeat = 0;
                    str->pause = 0;
                    
                    // channel is already active
                    if (channel->stream) {
                        str->next = channel->stream;
                        str->prev = NULL;
                        channel->stream->prev = str;
                        channel->stream = str;
                    // channel wasnt active. Set stream linked list state 
                    // also, add channel to stream list.
                    } else {
                        str->next = NULL;
                        str->prev = NULL;
                        channel->stream = str;
                        
                        // channels already active, add to list
                        if (apu->active) {
                            channel->next = apu->active;
                            channel->prev = NULL;
                            apu->active->prev = channel;
                            apu->active = channel;
                        // only active channel
                        } else {
                            channel->next = NULL;
                            channel->prev = NULL;
                            apu->active = channel;
                        }
                    }
                    break;
                  }
                  
                
                  case APU__SOUND_SET_VOLUME: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->volume = inst->value0;
                    }
                    break;
                  }
                   

                  case APU__SOUND_SET_PANNING: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->panning = inst->value0;
                    }
                    break;                  
                  }

                  case APU__SOUND_SET_REPEAT: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->repeat = inst->value0 > 0.5 ? 1 : 0;
                    }
                    break;                  
                  }
                  
                  case APU__SOUND_SEEK: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->progress = stream->sampleCount * inst->value0;
                    }
                    break;                  
                  }


                  case APU__SOUND_PAUSE: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->pause = 1;
                    }
                    break;                  
                  }
                  
                  case APU__SOUND_RESUME: {
                    TAPUStream * stream = topaz_table_find_by_uint(apu->streamObjectTable, inst->id);
                    if (stream) {
                        stream->pause = 0;
                    }
                    break;                  
                  }


                  case APU__CHANNEL_SET_VOLUME: {
                    TAPUChannel * channel = apu->channel+((uint8_t)inst->id);
                    channel->volume = inst->value0;
                    break;
                  }

                  case APU__CHANNEL_SET_PANNING: {
                    TAPUChannel * channel = apu->channel+((uint8_t)inst->id);
                    channel->panning = inst->value0;
                    break;
                  }


                  case APU__CHANNEL_HALT: {
                    TAPUChannel * channel = apu->channel+((uint8_t)inst->id);
                    TAPUStream * stream = channel->stream;
                    while(stream) {
                        stream->progress = stream->sampleCount;
                        stream = stream->next;
                    }
                    break;
                  }
                  
                  default:
                    #ifdef TOPAZDC_DEBUG
                        printf("Unknown notifcation from topaz APU received: %d\n", inst->cmd);
                    #endif
                }
            }


            topaz_sstream_release_incoming(apu->cmd);
        }
    }

}


static float panning_to_multiplier_l(float panning) {
    float leftPan = (2 - panning*2);
    if (leftPan < 0) leftPan = 0;
    if (leftPan > 1) leftPan = 1;
    return leftPan;
}

static float panning_to_multiplier_r(float panning) {
    float rightPan = panning*2;
    if (rightPan < 0) rightPan = 0;
    if (rightPan > 1) rightPan = 1;
    return rightPan;
}



typedef struct {
    float l;
    float r;
} FloatSample;
static void topaz_apu_write_buffer(TopazAudioProcessor * apu, float * samplesRaw, uint32_t sampleCount) {
    uint32_t i;
    FloatSample * samples = (FloatSample*)samplesRaw;
    for(i = 0; i < sampleCount; ++i) {
        samples->l = 0;
        samples->r = 0;
        samples++;
    }
    
    int needsLimiting = 0;
    
    TAPUChannel * channel = apu->active;
    TAPUStream * stream;
    while(channel) {
        stream = channel->stream;
        float leftPanChannel  = panning_to_multiplier_l(channel->panning);
        float rightPanChannel = panning_to_multiplier_r(channel->panning);

        while(stream) {
            if (stream->progress < stream->sampleCount && stream->pause == 0) {
                float leftPan  = panning_to_multiplier_l(stream->panning)*leftPanChannel;
                float rightPan = panning_to_multiplier_r(stream->panning)*rightPanChannel;
                float volume = channel->volume * stream->volume;
            

            
                const topazSound_Sample_t * streamSamples = stream->samples + stream->progress;
                samples = (FloatSample*)samplesRaw;
                for(i = 0; i < sampleCount && i < stream->sampleCount - stream->progress; ++i) {
                    samples->l +=  (streamSamples->leftSample / (float)0xffff) * volume * leftPan;
                    samples->r += (streamSamples->rightSample / (float)0xffff) * volume * rightPan;
                    samples++;
                    streamSamples++;
                }
                stream->progress += i;
                needsLimiting = 1;
            }
            TAPUStream * next = stream->next;

            
            // handle stream finished: remove from channel's chain
            if (stream->progress >= stream->sampleCount) {
                // simply reset progress back to 0.
                if (stream->repeat) {
                    stream->progress = 0;

                // otherwise, cleanup and remove.
                } else {
                    if (channel->stream == stream) {
                        channel->stream = stream->next;
                    }
                    
                    if (stream->prev) {
                        stream->prev->next = stream->next;
                    }
                    
                    if (stream->next) {
                        stream->next->prev = stream->prev;
                    }
                    
                    // notifies the audio instance, adds the stream to the object pool
                    topaz_apu_stream_retire(apu, stream);
                }
            }
            
            stream = next;
        }
    
    
        TAPUChannel * next = channel->next;

        // handle deactivate: remove from active linked list
        if (!channel->stream) {
            if (apu->active == channel) {
                apu->active = channel->next;
            }
            
            if (channel->prev) {        
                channel->prev->next = channel->next;
            }
            
            if (channel->next) {
                channel->next->prev = channel->prev;
            }
            
            channel->next = NULL;
            channel->prev = NULL;
        }
        channel = next;


    }
    
    
    // renormalize
    if (needsLimiting) {
        float highest = 0;
        samples = (FloatSample*)samplesRaw;
        for(i = 0; i < sampleCount; ++i) {
            if (samples->l > highest) highest = samples->l;  
            if (samples->r > highest) highest = samples->r;  
            samples++;
        }
        if (highest > 1.f) {
            samples = (FloatSample*)samplesRaw;
            for(i = 0; i < sampleCount; ++i) {
                samples->l = (samples->l / highest)*2.f - 1.f;
                samples->r = (samples->r / highest)*2.f - 1.f;
                samples++;
            }
        } else {
            samples = (FloatSample*)samplesRaw;
            for(i = 0; i < sampleCount; ++i) {
                samples->l = (samples->l)*2.f - 1.f;
                samples->r = (samples->r)*2.f - 1.f;
                samples++;
            }
        }
    }
}



// sent to the audio manager
// Likely run on a separate thread.
void topaz_apu_main(
    topazAudioManager_t * manager,
    uint32_t sampleCount,
    float * samples,
    void * tap
) {
    TopazAudioProcessor * apu = tap;
    topaz_apu_process_commands(apu);
    topaz_apu_write_buffer(apu, samples, sampleCount);
}







