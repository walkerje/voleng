//
// Created by jesse on 3/2/21.
//

#pragma once

#include <glm/vec3.hpp>

namespace vlg{
    namespace audio{
        class AbstractVoiceProperties{
        public:
            virtual float getVolume() const = 0;
            virtual void setVolume(float f) = 0;

            virtual float getPan() const = 0;
            virtual void setPan(float f) = 0;
            
            virtual bool getLoop() const = 0;
            virtual void setLoop(bool loop) = 0;

            virtual double getLoopPoint() const = 0;
            virtual void setLoopPoint(double val) = 0;

            virtual bool isProtected() const = 0;
            virtual void setProtected(bool pr) = 0;
            
            virtual bool getPaused() const = 0;
            virtual void setPaused(bool v) = 0;
            
            virtual void setInaudibleBehaviour(bool tick, bool kill) = 0;
            
            virtual ~AbstractVoiceProperties() = default;
            
        protected:
            AbstractVoiceProperties() = default;
        };
        
        class VoiceProperties : public AbstractVoiceProperties{
        public:
            VoiceProperties(){}
            virtual ~VoiceProperties() = default;
            
            float getVolume() const{
                return volume;
            }
            
            void setVolume(float f){
                volume = f;
            }
            
            float getPan() const{
                return pan;
            }
            
            void setPan(float p){
                pan = p;
            }
            
            bool getLoop() const{
                return loop;
            }
            
            void setLoop(bool l){
                loop = l;
            }
            
            double getLoopPoint() const{
                return loopPoint;
            }
            
            void setLoopPoint(double pt){
                loopPoint = pt;
            }
            
            bool isProtected() const{
                return protectedVoice;
            }
            
            void setProtected(bool pr){
                protectedVoice = pr;
            }
            
            void setInaudibleBehaviour(bool tick, bool kill){
                inaudibleTick = tick;
                inaudibleKill = kill;
            }
            
            void setPaused(bool v){
                paused = v;
            }
            
            bool getPaused() const{
                return paused;
            }
            
            bool getInaudibleTick() const{
                return inaudibleTick;
            }
            
            bool getInaudibleKill() const{
                return inaudibleKill;
            }
        private:
            float volume = 1.0f;
            float pan = 0.0f;
            bool loop = false;
            bool paused = false;
            double loopPoint = -1.0;
            bool protectedVoice = false;
            
            bool inaudibleTick = false;
            bool inaudibleKill = false;
        };
    }
}