//
// Created by jesse on 3/2/21.
//

#pragma once

#include "../engine_system.hpp"
#include "sound_effect.hpp"
#include "music.hpp"
#include "voice.hpp"

namespace vlg{
    class VLGEngine;
    
    namespace audio{
        class AudioEngineSystem : public EngineSystem{
        public:
            static constexpr char NAME[] = "Audio";
            
            ~AudioEngineSystem(){}

            /**
             * Initializes this engine system.
             * @param config the configuration to initialize this system with.
             * @return a boolean indicating if the initialization was successful.
             */
            bool init(Configuration& config);

            /**
             * De-initializes this engine system.
             */
            void deinit();
            
            /**
             * Fades the global volume to a specified value over a specified duration (seconds).
             * @param to the destination volume
             * @param duration the duration, in seconds, of the fade.
             */
            void fadeVolume(float to, double duration);

            /**
             * Sets the global volume of the audio system.
             * Normalized in range 0...1
             * @param f 
             */
            void setVolume(float f);
            
            /**
             * @return the global volume of the audio system, normalized in range 0...1
             */
            float getVolume() const;
            
            /**
             * Sets a boolean indicating if all voices should be paused.
             * @param value 
             */
            void setPauseAll(bool value);
            
            /**
             * Sets the position of the audio listener.
             * @param position 
             */
            void setListenerPosition(const Voice3D::Position& position);
            
            /**
             * Sets the point that the listener is facing towards.
             * @param facing 
             */
            void setListenerFacing(const Voice3D::Position& facing);
            
            /**
             * Sets the "up" vector of the audio listener.
             * @param up 
             */
            void setListenerUp(const Voice3D::Velocity& up);
            
            /**
             * Sets the velocity of the audio listener.
             * @param vel 
             */
            void setListenerVelocity(const Voice3D::Velocity& vel);
            
            /**
             * Plays the specified sound effect.
             * Returns a voice pointer that is valid while the effect is playing or is paused.
             * Uses the properties of the sound effect itself, applied to the voice.
             * @param effect The sound effect to play.
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice* play(SoundEffect& effect);
            
            /**
             * Plays the specified sound effect
             * @param effect The sound effect to play.
             * @param properties The properties to assign to the effect's voice.
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice* play(SoundEffect& effect, const VoiceProperties& properties);
            
            /**
             * Plays the specified sound effect at the specified position.
             * Returns a voice pointer that is valid while the effect is playing or is paused.
             * Uses the properties of the sound effect itself, applied to the voice.
             * @param effect The sound effect to play.
             * @param position The 3D position to play the effect at.
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice3D* play3D(SoundEffect& effect, Voice3D::Position position);
            
            /**
             * Plays the specified sound effect at the specified position.
             * Returns a voice pointer that is valid while the effect is playing or is paused.
             * Uses the properties of the sound effect itself, applied to the voice.
             * @param effect The sound effect to play.
             * @param position The 3D position to play the effect at.
             * @param velocity The velocity of the voice.
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice3D* play3D(SoundEffect& effect, Voice3D::Position position, Voice3D::Velocity velocity);
            
            /**
             * Plays the specified sound effect at the specified position.
             * Returns a voice pointer that is valid while the effect is playing or is paused.
             * @param effect The sound effect to play.
             * @param position The 3D position to play the effect at.
             * @param properties The properties to assign to the used voice.
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice3D* play3D(SoundEffect& effect, Voice3D::Position position, const VoiceProperties& properties);
            
            /**
             * Plays the specified sound effect at the specified position with the specified velocity.
             * Returns a voice pointer that is valid while the effect is playing or is paused.
             * Uses the properties of the sound effect itself, applied to the voice.
             * @param effect 
             * @param position 
             * @param velocity 
             * @param properties 
             * @return a pointer to the voice the effect will play from. Do not delete.
             */
            Voice3D* play3D(SoundEffect& effect, Voice3D::Position position, Voice3D::Velocity velocity, const VoiceProperties& properties);
            
            /**
             * Plays the specified music.
             * Returns a voice pointer that is valid while the music is playing or is paused.
             * Uses the properties of the music itself, applied to the voice.
             * @param music music to play
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice* play(Music& music);
            
            /**
             * Plays the specified music. Applies the specified properties to the resulting voice.
             * @param music 
             * @param properties 
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice* play(Music& music, const VoiceProperties& properties);

            /**
             * Plays the specified music at the specified position.
             * Uses the properties of the music itself, applied to the voice.
             * @param music the music to play
             * @param position the position to play the music at
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice3D* play3D(Music& music, Voice3D::Position position);
            
            /**
             * Plays the specified music at the specified position.
             * Uses the properties of the music itself, applied to the voice.
             * @param music the music to play
             * @param position the position to play the music at
             * @param velocity the velocity of the resulting voice
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice3D* play3D(Music& music, Voice3D::Position position, Voice3D::Velocity velocity);
            
            /**
             * Plays the specifeid music at the specified position.
             * Applies the specified properties to the resulting voice.
             * @param music the music to play
             * @param position the position to play the music at
             * @param properties the velocity of the resulting voice.
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice3D* play3D(Music& music, Voice3D::Position position, const VoiceProperties& properties);
            
            /**
             * Plays the specified music at the specified position, with the specified velocity.
             * Applies the specified properties to the resulting voice.
             * @param music the music to play
             * @param position the position to play the music at
             * @param velocity the velocity of the resulting voice
             * @param properties the properties to apply to the resulting voice.
             * @return a pointer to the voice the music will play from. Do not delete.
             */
            Voice3D* play3D(Music& music, Voice3D::Position position, Voice3D::Velocity velocity, const VoiceProperties& properties);
            
            /**
             * Stops all voices currently playing the specified sound effect.
             * @param effect 
             */
            void stopSource(SoundEffect& effect);
            
            /**
             * Stops all voices currently playing the specified sound effect.
             * @param effect 
             */
            void stopSource(Music& effect);
            
            /**
             * Stops all voices that are currently playing.
             */
            void stopAll();
        private:
            friend class vlg::VLGEngine;
            //all voices are technically 3D voices because the 3D implementation sits
            //at the top of the voice class hierarchy...
            //See here: http://sol.gfxile.net/soloud/voicemanagement.html
            Voice3D voices[1024];
            
            AudioEngineSystem() : EngineSystem(NAME){}
            void* backendPtr = nullptr;
        };
    }
}