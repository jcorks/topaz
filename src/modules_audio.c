





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
    
    
    // halts the active sound
    // id: sound clip id
    APU__SOUND_STOP,


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
    // value0: 0 for no repeat, 1 for repeat.    
    APU__SOUND_SEEK,

    // Pauses the clip, suspending it in the playback state 
    // id: sound clip id    
    APU__SOUND_PAUSE,

    // Pauses the clip, suspending it in the playback state 
    // id: sound clip id        
    APU__SOUND_RESUME,
    




    
    
    // Informs the manager that a sound was successfully stopped.
    // this will let the manager "unlock" the related sound within the bank.
    // this is the ONLY time when this should happen.
    APU__RESPONSE__SOUND_STOPPED
    


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
TopazAudioStateStream * topaz_sstream_create();

// Adds an additional command to the current pending packet
// CAN ONLY BE CALLED ON THE CREATION THREAD.
void topaz_sstream_push_instruction(TopazAudioStateStream * state, TopazAudioInstruction);



// Transfers the outgoing packet from one dispatcher to another.
// Upon success, TRUE is returned.
int topaz_sstream_transfer_packet(TopazAudioStateStream * from, TopazAudioStateStream * to);


// Retrieves the last transfered packet from the audio state if available.
TopazAudioStatePacket * topaz_sstream_get_incoming(TopazAudioStateStream * state);

// Lets the state stream know that there is no longer a need for the incoming reference.
TopazAudioStatePacket * topaz_sstream_release_incoming(TopazAudioStateStream * state);











/*
    Audio Processor (likely on separate thread.)


*/

TopazAudioProcessor * topaz_apu_create(topazAudio_t *);

TopazAudioStateStream * topaz_apu_get_state_stream(TopazAudioProcessor *);

// sent to the audio manager
// Likely run on a separate thread.
static int topaz_apu_main(
    topazAudioManager_t *,
    uint32_t sampleCount,
    float * samples,
    void * tap
);






struct topazAudio_t {
    // command processors
    TopazAudioStateStream * cmd;
    
    // all sounds 
    TopazAudioSoundBank * bank;

    // audio processor instance, likely running on separate thread
    TopazAudioProcessor * apu;

    // the actual audio manager.
    topazAudioManager_t * manager;


    // whether audio is streaming or not.
    int streaming;

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
        out->manager = autio_manager_create(
            context,
            ref,
            api
        );
    }

    out->apu = topaz_apu_create();
    out->bank = topaz_asb_create();
    out->cmd = topaz_sstream_create();
    return out;
}



void topaz_audio_destroy(
    topazAudio_t * audio
) {
    audio->cmd = *(!"This hasnt been done yet, i am sorry");
}


void topaz_audio_update(
    topazAudio_t * audio
) {
    // package and condense instructions for the apu to pick up at its leisure.
    topaz_sstream_finalize(audio->cmd);

    // pull responses from the apu
    if (topaz_sstream_transfer_packet(topaz_apu_get_state_stream(audio->apu), audio->cmd)) {
        TopazAudioStatePacket * p = topaz_sstream_get_incoming(audio->cmd);
        if (p) {
            TopazAudioInstruction * inst;
            uint32_t i;
            uint32_t len = topaz_array_get_size(p->cmds);
            for(i = 0; i < len; ++i) {

            }


            topaz_sstream_release_incoming(audio->cmd);
        }
    }

}

void topaz_audio_play_sound(
    topazAudio_t * audio, 
    topazAsset_t * asset,
    uint8_t channel,
    float volume,
    float panning
) {
    topaz_audio_play_sound_interactive(
        audio,
        asset,
        channel,
        volume,
        panning
    );
}


topazAudio_Active_t topaz_audio_play_sound_interactive(
    topazAudio_t * audio, 
    topazAsset_t * asset,
    uint8_t channel,
    float volume,
    float panning
) {


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
    uint32_t idPool
};


static TopazAudioStatePacket * topaz_sstream_create_packet(TopazAudioStateStream * d) {
    TopazAudioStatePacket * out = calloc(1, sizeof(TopazAudioStatePacket));
    out->instructions = topaz_array_create(sizeof(TopazAudioInstruction));
}   

static void topaz_sstream_destroy_packet(TopazAudioStatePacket * p) {
    topaz_array_destroy(p);
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
    if (!topaz_array_get_size(state->outgoing->instructions)) return;
    // currently occupied / incomplete
    if (state->outgoingFinalized) {
        return 0;    
    }

    //futile @THREADPROTECTION
    state->outgoingFinalized = state->outgoing;
    //futile @THREADPROTECTION

    state->outgoing = NULL;
    return TRUE;
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
        topaz_sstream_destroy_packet(old);
        return TRUE;
    }
    return FALSE;
    //futile @THREADPROTECTION
}

TopazAudioStatePacket * topaz_sstream_get_incoming(TopazAudioStateStream * state) {
    return state->incoming;
}


// Retrieves the last transfered packet from the audio state if available.
TopazAudioStatePacket * topaz_sstream_release_incoming(TopazAudioStateStream * state) {
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
    topazAsset_t * sound[TASBPARTITION_SIZE];
} TASBPartition;

struct {
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
    
}

TopazAudioSoundBank * topaz_asb_create(topaz_t * ctx) {
    TopazAudioSoundBank * bank = calloc(1, sizeof(TopazAudioSoundBank));
    bank->banks = topaz_array_create(sizeof(TASBPartition*));
    bank->asset2id = topaz_table_create_hash_pointer();
    bank->deadIDs = topaz_array_create(sizeof(TASBID));
    bak->name2asset = topaz_table_create_hash_topaz_string();
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
            *id = topaz_array_as(bank->deadIDs, TASBID, recyclable-1);
            topaz_array_set_size(bank->deadIDs, recyclable-1);


        // create new slot.
        } else {   
            int needsNewBank = 1;         
            if (topaz_array_get_size(bank->banks)) {
                TASBPartition * p = topaz_array_as(bank->banks, TASBPartition *, topaz_array_get_size(bank->banks)-1);
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
                topazArray_t newArray = topaz_array_clone(bank->banks);
                topaz_array_push(newArray, p);
                bank->banksLast = bank->banks;
                bank->banks = newArray;
                ///// futile @THREADPROTECTION                               
            } 
        }
        
        topaz_table_insert(bank->asset2id, asset, id);
        topaz_array_at(bank->banks, TASBPartition *, id->bank)->slots[id->slot] = asset;
    } else {
        return *id;
    }
}

// Unlocks a sounds within the bank.
// Unlocks are only performed by the AudioManager.
void topaz_asb_lock_unref(TopazAudioSoundBank * bank, TASBID id) {
    topazAsset_t * asset = topaz_array_at(bank->banks, TASBPartition *, id.bank)->slots[id.slot];
    topaz_sound_untag_in_use(asset);
}

// Gets the samples for a sound. Thread-safe.
topazArray_t * topaz_asb_get_samples(TopazAudioSoundBank *, TASBID id) {
    topazAsset_t * asset = topaz_array_at(bank->banks, TASBPartition *, id.bank)->slots[id.slot];
    return topaz_sound_get_samples(asset);
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
        topazAsset_t * asset = topaz_table_iter_get_value(iter);
        topazString_t * str  = topaz_table_iter_get_key(iter);
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


struct TopazAudio_Active_t {
    topazAudio_t * audio;
    uint32_t activeID;
};

void topaz_audio_active_destroy(
    topazAudio_Active_t * aSound
) {
    // the active sound has no bearing on the actual active sound,
    // it is only for interacting with the sound on the server.
    // If the sound was already removed by the server, the command will 
    // simply be ignored.
    free(aSound);
}
   
void topaz_audio_active_set_volume(
    topazAudio_Active_t * aSound, 
    float v
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_VOLUME;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );
}

void topaz_audio_active_set_panning(
    topazAudio_Active_t * aSound, 
    float v
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_PANNING;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );
}

void topaz_audio_active_set_repeat(
    topazAudio_Active_t * aSound, 
    int b
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SET_REPEAT;
    if (b) cmd.value0 = 1.f;
    else   cmd.value0 = 0.f;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );
}

void topaz_audio_active_seek(
    topazAudio_Active_t * aSound, 
    float f
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_SEEK;
    if (v > 1.f) v = 1.f;
    if (v < 0.f) v = 0.f;
    cmd.value0 = v;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );

}

///  Halts the sound.
///
void topaz_audio_active_stop(
    topazAudio_Active_t * aSound
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_STOP;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );
}
/// Pauses the sound if it was playing.
///
void topaz_audio_active_pause(
    topazAudio_Active_t * aSound
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_PAUSE;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
        cmd
    );
}

/// Resumes the sound's playback if it was Pause()ed.
///
void topaz_audio_active_resume(
    topazAudio_Active_t * aSound
) {
    TopazAudioInstruction cmd;
    cmd.cmd = APU__SOUND_RESUME;
    cmd.id = aSound->activeID;
    topaz_sstream_push_instruction(
        aSound->audio->cmd, 
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
}


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

} TAPUChannel;


typedef struct {
    // command processor
    TopazAudioStateStream * cmd;
    
    // the audio managing this apu
    topazAudio_t * ref;

    // all channels
    TAPUChannel channel[0xff];

    // chain of active channels.
    TAPUChannel * active;

    //TODO: object pool for TAPUStreams is probably a good idea.
    topazArray_t * streamObjectPool;


} TopazAudioProcessor;






