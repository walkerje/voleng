//
// Created by jesse on 3/2/21.
//

#include <voleng/audio/music_load_job.hpp>
#include <voleng/audio/music.hpp>
#include <voleng/audio/music_factory.hpp>

namespace vlg{
    namespace audio{
        void MusicLoadJob::run() {
            setState(State::Working);
            factory->load(music);
            finish();
        }
    }
}