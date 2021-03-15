//
// Created by jesse on 2/26/21.
//

#pragma once

#include "config.hpp"
#include "runops.hpp"
#include "app.hpp"
#include "engine_system.hpp"
#include "resource_center.hpp"
#include "job_pool.hpp"

#include "gfx/graphics_engine_system.hpp"
#include "audio/audio_engine_system.hpp"

#include <Poco/AutoPtr.h>

namespace vlg{
    class VLGEngine{
    public:
        ~VLGEngine() = default;
        
        /**
         * Runs the engine.
         * @param options Runtime options, largely for configuring consistent settings that don't change. 
         * @param app
         * @return exit return value (EXIT_SUCCESS, EXIT_FAILURE, etc)
         */
        int run(const RunOptions& options, Application* application);
        
        /**
         * Shuts down the engine. This should be called in the main loop.
         * @param exitCode number returned at exit.
         */
        void shutdown(int exitCode = EXIT_SUCCESS){
            running = false;
            exitStatus = exitCode;
        }
        
        /**
         * @return a read-only reference to the run options the engine has been ran with.
         */
        const RunOptions& getRunOptions(){
            return runOptions;
        }
        
        /**
         * @return a read-write reference to the engine configuration.
         */
        Configuration& getConfiguration(){
            return *config;
        }
        
        /**
         * @return a read-only reference to the engine configuration.
         */
        const Configuration& getConfiguration() const{
            return *config;
        }
        
        /**
         * @return a read-write reference to the application the engine is managing.
         */
        Application& getApplication(){
            return *app;
        }

        /**
         * @return a read-only reference to the application the engine is managing.
         */
        const Application& getApplication() const{
            return *app;
        }
        
        /**
         * @return a read-write reference to the graphics system used by the engine.
         */
        gfx::GraphicsEngineSystem& getGraphicsSystem(){
            return *graphicsSystem;
        }
        
        /**
         * @return a read-write reference to the audio system used by the engine.
         */
        audio::AudioEngineSystem& getAudioSystem(){
            return *audioSystem;
        }
        
        /**
         * @return a read-write reference to the resource center the engine is using.
         */
        ResourceCenter& getResourceCenter() const{
            return *resources;
        }
        
        /**
         * @return a read-write reference to the underlying job pool the engine is using.
         */
        JobPool& getJobPool() const{
            return *jobPool;
        }
        
        /**
         * @return a read-write reference to the logger used by the engine's core.
         */
        Poco::Logger& getLogger(){
            return *logger;
        }

        /**
         * @return a read-write reference to the notification queue used by this engine system.
         */
        Poco::NotificationQueue& getNotificationQueue(){
            return noteQueue;
        }

        /**
         * @return a read-write reference to the notification center used by this engine system.
         */
        Poco::NotificationCenter& getNotificationCenter(){
            return noteCenter;
        }
        
        /**
         * Defers the delivery of a notification to a specific engine system.
         * The notification is later sent to the respective engine system
         * in the next update of the main loop.
         * @param target destination engine system, null
         * @param note 
         */
        void deferNotification(EngineSystem* target, Poco::Notification* note){
            DeferredNotification* deferNote = new DeferredNotification;
            deferNote->target = target;
            deferNote->notification = note;
            noteQueue.enqueueNotification(deferNote);
        }
        
        static VLGEngine& getInstance();
    private:
        struct DeferredNotification : public Poco::Notification{
            EngineSystem* target;
            Poco::Notification* notification;
        };
        
        void handleDeferredNotification(DeferredNotification* note);
        
        VLGEngine(){}
        
        int preInit();
        
        void init();
        
        void mainloop();
        
        void preDeinit();
        
        void deinit();
        
        //Engine systems here.
        gfx::GraphicsEngineSystem* graphicsSystem = nullptr;
        audio::AudioEngineSystem* audioSystem = nullptr;

        Poco::NotificationQueue noteQueue;
        Poco::NotificationCenter noteCenter;
        
        Poco::AutoPtr<Configuration> config;
        Poco::Logger*  logger = nullptr;//default logger to nullptr
        Application*   app    = nullptr;//default app to nullptr
        ResourceCenter* resources = nullptr;//default resource center to nullptr
        JobPool*        jobPool   = nullptr;//default job pool to null
        RunOptions runOptions;
        bool running = false;
        int exitStatus = EXIT_SUCCESS;
        
        static VLGEngine instance;
    };
}