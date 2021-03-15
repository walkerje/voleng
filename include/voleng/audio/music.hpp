//
// Created by jesse on 3/2/21.
//

#pragma once

#include "voice_properties.hpp"
#include "../resource.hpp"

namespace vlg{
    namespace audio{
        class MusicFactory;

        class Music : public Resource, public VoiceProperties{
        public:
            ~Music(){}

            /**
             * @return the duration of this sound effect, in seconds.
             */
            double getDuration() const{
                return duration;
            }

            /**
             * @return an opaque pointer to the underlying handle.
             */
            void* getHandle() const{
                return handle;
            }
        private:
            friend class MusicFactory;

            Music(const ResourceInfo& info)
                    : Resource(info),
                      handle(nullptr){}

            double duration = 0;
            void* handle;
        };
    }
    
    using audio::Music;
}