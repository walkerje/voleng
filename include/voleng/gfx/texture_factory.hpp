//
// Created by jesse on 3/1/21.
//

#pragma once

#include "../job.hpp"
#include "../resource_factory.hpp"
#include "texture.hpp"
#include "texture_load_job.hpp"

#include <Poco/Buffer.h>
#include <Poco/Notification.h>

namespace vlg{
    namespace gfx{
        class GraphicsEngineSystem;
        
        /**
         * The TextureFactory serves as the only method of loading, unloading, and instantiating textures.
         * Loading and unloading can be done synchronously and asynchronously.
         */
        class TextureFactory : public ResourceFactory<Texture>{
        public:
            //The destructor removes notification listeners from the graphics engine system.
            ~TextureFactory();
            
            /**
             * Loads the specified texture resource.
             * @param res 
             */
            void load(Resource* res) const;

            /**
             * Queues a job to load the specified texture resource.
             * Notifies the graphics system upon completion.
             * @param resource 
             */
            void loadAsync(Resource* resource) const;

            /**
             * Performs an unload operation for the specified resource pointer.
             * This is done on the invoking thread, and has no guarantee of safety.
             * It's generally safest to unload resources with the async API.
             * @param resource 
             */
            void unload(Resource* resource) const;

            /**
             * Notifies the graphics system that the specified texture resource can be unloaded.
             * @param resource 
             */
            void unloadAsync(Resource* resource) const;
            
            Resource* createInstance() const{
                return new Texture(currentInfo);
            }
        private:
            
            //We need to declare some notifications here to be hooked to the Graphics system notification queue...
            //these are guaranteed to be called from the rendering thread.
            void handleTextureUpdate(TextureUpdateNotification* note);
            void handleTextureUnload(TextureUnloadNotification* note);
            
            friend class GraphicsEngineSystem;
            
            TextureFactory();//Adds notification listeners to the graphics system.
        };
    }
}