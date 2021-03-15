//
// Created by jesse on 2/27/21.
//

#pragma once

#include "config.hpp"
#include "appstate.hpp"
#include "display.hpp"

#include <string>

#include <Poco/Logger.h>

namespace vlg{
    class VLGEngine;
    
    class Application{
    public:
        virtual ~Application() = default;
        
        /**
         * Called when the application is first ran, after the engine initializes.
         * @param config the configuration of the engine and application.
         * @return the first state of the application.
         */
        virtual ApplicationState& onRun(const Configuration& config) = 0;
        
        /**
         * Called when the application is shut down, before the engine deinitializes.
         */
        virtual void onShutdown() = 0;

        /**
         * @return a read-only reference to the name string of this application.
         */
        const std::string& getName() const{
            return name;
        }
        
        /**
         * @return a read-write reference to the application's logger.
         */
        Poco::Logger& getLogger(){
            return *logger;
        }
        
        /**
         * Sets the current state of the application.
         * @param otherState 
         */
        void setState(ApplicationState& otherState){
            state = &otherState;
        }
        
        /**
         * @return a read-write reference to the current state of the application.
         */
        ApplicationState& getState(){
            return *state;
        }
        
        /**
         * @return a read-write reference to the display of this application.
         */
        Display& getDisplay(){
            return *display;
        }
        
    protected:
        Application(const std::string& name = "Application")
            : name(name){}
        
    private:
        friend class VLGEngine;
        
        std::string name;
        ApplicationState* state = nullptr;
        Display* display = nullptr;
        Poco::Logger* logger = nullptr;
    };
}