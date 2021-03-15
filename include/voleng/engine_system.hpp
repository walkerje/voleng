//
// Created by jesse on 2/26/21.
//

#pragma once

#include "config.hpp"

#include <string>
#include <Poco/Logger.h>
#include <Poco/NotificationCenter.h>
#include <Poco/NotificationQueue.h>

namespace vlg{
    class EngineSystem{
    public:
        virtual ~EngineSystem() = default;
        
        /**
         * Initializes this engine system.
         * @param config the configuration to initialize this system with.
         * @return a boolean indicating if the initialization was successful.
         */
        virtual bool init(Configuration& config) = 0;
        
        /**
         * De-initializes this engine system.
         */
        virtual void deinit() = 0;

        /**
         * Basic update implementation.
         * Dispatches all currently queued notifications.
         */
        virtual void update(){
            noteQueue.dispatch(noteCenter);
        }
        
        /**
         * @return a boolean indicating if this engine system is initialized.
         */
        bool getInitialized() const{
            return isInitialized;
        }
        
        /**
         * @return a read-only reference to the name string of this engine system. 
         */
        const std::string& getName() const{
            return systemName;
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
         * @return a read-write reference to the logger used by this engine system.
         */
        Poco::Logger& getLogger(){
            return logger;
        }
    protected:
        EngineSystem(const std::string& systemName)
            : systemName(systemName), logger(Poco::Logger::get("VolEng - " + systemName)){}
        
        bool isInitialized = false;
    private:
        Poco::NotificationQueue noteQueue;
        Poco::NotificationCenter noteCenter;
        
        std::string systemName;
        Poco::Logger& logger;
    };
}