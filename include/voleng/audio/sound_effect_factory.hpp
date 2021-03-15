//
// Created by jesse on 3/2/21.
//

#pragma once

#include "../resource_factory.hpp"
#include "sound_effect.hpp"
#include "sound_effect_load_job.hpp"

#include <Poco/Notification.h>

namespace vlg{
    namespace audio{
        class SoundEffectFactory : public ResourceFactory<SoundEffect>{
        public:
            SoundEffectFactory();
            ~SoundEffectFactory();
            
            /**
             * Performs a load operation for the specified resource pointer.
             * This is done on the invoking thread, and has no guarantee of safety.
             * It's generally safest to load resources with async API.
             * @param resource 
             */
            void load(Resource* resource) const;

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
                return new SoundEffect(currentInfo);
            }
            
        private:
            struct SoundEffectUnloadNotification : public Poco::Notification{
                SoundEffect* effect;
            };
            
            void handleEffectUnload(SoundEffectUnloadNotification* note);
        };
    }
}