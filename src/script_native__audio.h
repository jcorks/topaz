#include <topaz/modules/audio.h>



TSO_SCRIPT_API_FN(audio_api__play_sound) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    


    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    uint32_t id = topaz_audio_play_sound(
        audio,
        native,
        topaz_script_object_as_int(arg1)
    );
    return topaz_script_object_from_int(script, id);
}


TSO_SCRIPT_API_FN(audio_api__channel_halt) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_channel_halt(
        audio,
        topaz_script_object_as_int(arg0)
    );
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(audio_api__channel_set_volume) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_channel_set_volume(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(audio_api__channel_set_panning) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_channel_set_panning(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(audio_api__playback_set_volume) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_set_volume(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(audio_api__playback_set_panning) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_set_panning(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(audio_api__playback_set_repeat) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_set_repeat(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_int(arg1)
    );
    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(audio_api__playback_seek) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_seek(
        audio,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(audio_api__playback_stop) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_stop(
        audio,
        topaz_script_object_as_int(arg0)
    );
    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(audio_api__playback_resume) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_resume(
        audio,
        topaz_script_object_as_int(arg0)
    );
    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(audio_api__playback_pause) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    
    topazAudio_t * audio = topaz_context_get_audio(((topazScriptManager_t*)context)->ctx);

    topaz_audio_playback_pause(
        audio,
        topaz_script_object_as_int(arg0)
    );
    TSO_NO_RETURN;    
}














static void add_refs__audio_api(topazScript_t * script, topazScriptManager_t * context) {

    TS_MAP_NATIVE_FN("topaz_audio__play_sound", audio_api__play_sound);
    TS_MAP_NATIVE_FN("topaz_audio__channel_halt", audio_api__channel_halt);
    TS_MAP_NATIVE_FN("topaz_audio__channel_set_volume", audio_api__channel_set_volume);
    TS_MAP_NATIVE_FN("topaz_audio__channel_set_panning", audio_api__channel_set_panning);
    TS_MAP_NATIVE_FN("topaz_audio__playback_set_volume", audio_api__playback_set_volume);
    TS_MAP_NATIVE_FN("topaz_audio__playback_set_panning", audio_api__playback_set_panning);
    TS_MAP_NATIVE_FN("topaz_audio__playback_set_repeat", audio_api__playback_set_repeat);
    TS_MAP_NATIVE_FN("topaz_audio__playback_seek", audio_api__playback_seek);
    TS_MAP_NATIVE_FN("topaz_audio__playback_stop", audio_api__playback_stop);
    TS_MAP_NATIVE_FN("topaz_audio__playback_pause", audio_api__playback_pause);
    TS_MAP_NATIVE_FN("topaz_audio__playback_resume", audio_api__playback_resume);
}
