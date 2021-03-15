#include <voleng/gfx/texture_load_job.hpp>
#include <voleng/gfx/texture_factory.hpp>

namespace vlg{
    namespace gfx{
        //VERY simple job here.
        //Just calls the synchronous load from the factory.
        void TextureLoadJob::run() {
            setState(State::Working);
            factory->load(texture);
            finish();
        }
    }
}