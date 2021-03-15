//
// Created by jesse on 2/28/21.
//

#pragma once

#include "resource_pkg.hpp"

#include <atomic>

namespace vlg{
    class AbstractResourceFactory;
    
    class Resource{
    public:
        enum class State{
            Invalid,        //for resources that are completely invalid.
            Idle,           //default state   
            Unloaded,       //previously unloaded, can be loaded again though...
            Unloading,      //currently unloading
            UnloadPending,  //asynchronous unload is pending
            Loading,        //currently loading
            Loaded,         //previously loaded
            LoadPending     //asynchronous load is pending
        };
        
        virtual ~Resource(){}
        
        /**
         * @return a read-only reference to the resource's information.
         */
        const ResourceInfo& getInfo() const{
            return info;
        }
        
        /**
         * @return the type of resource this is.
         */
        const ResourceType& getType() const{
            return info.type;
        }
        
        /**
         * This function is thread-safe.
         * @return the current state of this resource.
         */
        State getState() const{
            return currentState.load();
        }
        
        /**
         * This function is thread safe. "Pending" means any state that's NOT Loaded, Unloaded, or Idle.
         * @return a boolean indicating if there is a pending operation on this resource.
         */
        bool isPending() const{
            const State state = getState();
            return state == State::LoadPending || state == State::UnloadPending || state == State::Loading || state == State::Unloading;
        }
        
        /**
         * This function is thread-safe.
         * @return a boolean indicating if this resource is loaded.
         */
        operator bool() const{
            return getState() == State::Loaded;
        }
    protected:
        Resource(const ResourceInfo& info)
            : currentState((!info.exists || info.type == ResourceType::Unknown)
                           ? State::Invalid : State::Idle),
              info(info){}

        std::atomic<State> currentState;
    private:
        ResourceInfo info;
    };
}