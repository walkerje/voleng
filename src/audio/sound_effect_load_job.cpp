//
// Created by jesse on 3/2/21.
//

#include <voleng/audio/sound_effect_load_job.hpp>
#include <voleng/audio/sound_effect.hpp>
#include <voleng/audio/sound_effect_factory.hpp>

namespace vlg{
    namespace audio{
        void SoundEffectLoadJob::run() {
            setState(State::Working);
            factory->load(effect);
            finish();
        }
    }
}