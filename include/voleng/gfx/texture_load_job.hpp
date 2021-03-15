//
// Created by jesse on 3/1/21.
//

#pragma once

#include "texture.hpp"
#include "../job.hpp"

#include <Poco/Notification.h>
#include <Poco/Buffer.h>

namespace vlg{
    namespace gfx{
        struct TextureUpdateNotification : public Poco::Notification{
            Texture* texture;           //the texture to be loaded, assumes extent is correct
            Poco::Buffer<char> pixels = {0};  //RGBA pixel buffer
        };

        struct TextureUnloadNotification : public Poco::Notification{
            Texture* texture;
        };
        
        /**
         * Texture load job. Loads the texture's contents into memory and notifies the
         * Graphics system that an update has occured.
         */
        class TextureLoadJob : public Job{
        public:
            TextureLoadJob(JobPool& pool, Texture* texture, const TextureFactory* factory)
                    : Job(pool), texture(texture), factory(factory){}

            /**
             * Loads the texture this job was constructed with.
             */
            void run();
        private:
            Texture* texture;
            const TextureFactory* factory;
        };
    }
}