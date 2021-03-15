#pragma once

#include <Poco/Util/XMLConfiguration.h>

namespace vlg{
    typedef Poco::Util::XMLConfiguration Configuration;
    
    namespace cfg{
        //Display configuration
        constexpr char KEY_DISPLAY_WIDTH[] = "display.width";
        constexpr int VAL_DISPLAY_WIDTH = 800;
        
        constexpr char KEY_DISPLAY_HEIGHT[] = "display.height";
        constexpr int VAL_DISPLAY_HEIGHT = 600;
        
        constexpr char KEY_DISPLAY_FULLSCREEN[] = "display.fullscreen";
        constexpr bool VAL_DISPLAY_FULLSCREEN = false;
        
        constexpr char KEY_DISPLAY_VSYNC[] = "display.vsync";
        constexpr bool VAL_DISPLAY_VSYNC = true;

        //Audio configuration
        constexpr char KEY_AUDIO_SFXVOL[] = "audio.sfx_volume";
        constexpr float VAL_AUDIO_SFXVOL = 1.0f;
        
        constexpr char KEY_AUDIO_MUSVOL[] = "audio.music_volume";
        constexpr float VAL_AUDIO_MUSVOL = 1.0f;
    }
}