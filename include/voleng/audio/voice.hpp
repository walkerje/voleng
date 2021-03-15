//
// Created by jesse on 3/2/21.
//

#pragma once

#include "voice_properties.hpp"
#include <glm/vec3.hpp>

namespace vlg{
    namespace audio{
        class AudioEngineSystem;
        
        /**
         * The Voice class, as its name would imply,
         * represents an audio voice. These represent audio that has been "played",
         * allowing users to change audio properties after the fact.
         * 
         * Voices inherit properties from their respective sound resource.
         */
        class Voice : public AbstractVoiceProperties{
        public:
            typedef unsigned int Handle;

            virtual ~Voice() = default;
            Voice(const Voice& copy) = delete;
            Voice(Voice&& move) = delete;
            Voice& operator=(Voice&) = delete;
            Voice& operator=(Voice&&) = delete;

            /**
             * @return the volume of this voice in range 0...1
             */
            float getVolume() const;
            
            /**
             * Sets this voice's volume in range 0...1
             * @param volume the new volume
             */
            void setVolume(float volume);

            /**
             * @return the left-right stereo pan in range -1...1
             */
            float getPan() const;
            
            /**
             * Sets the left-right stereo pan in range -1...1 wherein
             * -1 is the left channel and 1 is the right channel.
             * 0 is, of course, dead center.
             * @param f 
             */
            void setPan(float pan);

            /**
             * @return a boolean indicating 
             */
            bool getLoop() const;
            
            /**
             * Sets a boolean indicating if this voice will loop its audio.
             * @param loop a boolean indicating whether or not this voice will loop its audio.
             */
            void setLoop(bool loop);

            /**
             * @return the point in the played resource where the voice restarts while looping.
             */
            double getLoopPoint() const;
            
            /**
             * Sets the point in the played resource where the voice restarts while looping.
             * @param val 
             */
            void setLoopPoint(double val);

            /**
             * Returns a boolean indicating whether or not this voice is protected.
             * Voices that are protected are never removed from the list of valid voices
             * when there are many playing simultaneously.
             * @return a boolean indicating if this voice is protected.
             */
            bool isProtected() const;
            
            /**
             * Sets a boolean indicating whether or not this voice is protected.
             * Voices that are protected are never removed from the list of valid voices
             * when there are many playing simultaneously.
             * @param pr 
             */
            void setProtected(bool pr);

            /**
             * Sets the behavior of this voice if it is inaudible.
             * @param tick whether or not to keep the voice playing
             * @param kill whether or not to stop the voice altogether.
             */
            void setInaudibleBehaviour(bool tick, bool kill);

            /**
             * Gets a boolean indicating whether or not this voice is currently paused.
             */
            bool getPaused() const;
            
            /**
             * Sets a boolean indicating whether or not this voice is currently paused.
             * @param pause whether or not this voice is paused.
             */
            void setPaused(bool pause);
            
            /**
             * Seeks in the audio stream.
             * @param t time to seek to, in seconds.
             */
            void seek(double t);
            
            /**
             * @return the current seeking position, in seconds.
             */
            double getSeekPosition();
            
            /**
             * Fades the volume of this voice to a specified level over a specified duration. 
             * @param to the volume to fade to
             * @param duration the duration to fade over
             */
            void fadeVolume(float to, double duration);
            
            /**
             * Fades this voice to a stop over a specified duration.
             * @param duration to stop the voice over, in seconds.
             */
            void fadeVolumeToStop(double duration);
            
            /**
             * Stops this voice, invalidating it.
             */
            void stop();
            
            /**
             * @return a boolean indicating if this voice has a valid handle.
             */
            bool isValid() const;

            /**
             * @return the handle of this voice.
             */
            Handle getHandle() const{
                return handle;
            }
            
            /**
             * Sets the properties of this voice, derived from the specified set of properties.
             * @param properties the new properties to inherit.
             */
            void setProperties(const AbstractVoiceProperties& properties);
        protected:
            friend class AudioEngineSystem;
            
            Voice(){}
            
            void* backend;
            Handle handle;
        };
        
        class Voice3D : public Voice{
        public:
            typedef glm::vec3 Position;
            typedef glm::vec3 Velocity;
            virtual ~Voice3D() = default;
            
            /**
             * Sets this voice's position, Left handed coordinates.
             * @param p position vector
             */
            void setPosition(Position p);
            
            /**
             * Sets this voice's velocity, Left handed coordinates.
             * @param v 
             */
            void setVelocity(Velocity v);
            
        private:
            friend class AudioEngineSystem;
            Voice3D(){}
        };
    }
    
    using AudioVoice = audio::Voice;
}