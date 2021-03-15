//
// Created by jesse on 3/2/21.
//

#pragma once

#include "../job.hpp"

namespace vlg{
    namespace audio{
        class SoundEffect;
        class SoundEffectFactory;
        
        class SoundEffectLoadJob : public Job{
        public:
            SoundEffectLoadJob(JobPool& pool, SoundEffect* effect, const SoundEffectFactory* factory)
            : Job(pool), effect(effect), factory(factory){}

            /**
             * Loads the sound effect this job was constructed with.
             */
            void run();
        private:
            SoundEffect* effect;
            const SoundEffectFactory* factory;
        };
    }
}