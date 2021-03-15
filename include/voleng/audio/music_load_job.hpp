//
// Created by jesse on 3/2/21.
//

#pragma once

#include "../job.hpp"

namespace vlg{
    namespace audio{
        class Music;
        class MusicFactory;
        
        class MusicLoadJob : public Job{
        public:
            MusicLoadJob(JobPool& pool, Music* music, const MusicFactory* factory)
            : Job(pool), music(music), factory(factory){}

            /**
             * Loads the sound effect this job was constructed with.
             */
            void run();
        private:
            Music* music;
            const MusicFactory* factory;
        };
    }
}