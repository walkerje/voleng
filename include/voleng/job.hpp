//
// Created by jesse on 3/1/21.
//

#pragma once

#include <atomic>
#include <Poco/Notification.h>
#include <Poco/Runnable.h>

namespace vlg{
    class EngineSystem;
    class JobPool;
    
    class Job : public Poco::Runnable{
    public:
        enum class State{
            Idle,
            Working,
            Finished
        };
        
        Job(JobPool& owner)
            : owner(&owner), state(State::Idle){}
        
        /**
         * @return the current state of this job.
         */
        State getState() const{
            return state.load(std::memory_order::memory_order_relaxed);
        }
            
        /**
         * Runs this task.
         * Inheritors must define this function, and use the state/finish utilities accordingly.
         */
        virtual void run() = 0;
    protected:
        
        /**
         * Marks this job as "finished", and optionally sends a notification to a specific engine system.
         * The notification pointer is owned by the notification system following this call.
         * @param finishNote A pointer, which is now owned by the notification system.
         */
        void finish(EngineSystem* targetSystem, Poco::Notification* finishNote);
        
        /**
         * Marks this job as "finished" and sends a notification to the engine core.
         * The notification pointer is owned by the notification system following this call.
         * @param notification 
         */
        inline void finish(Poco::Notification* notification){
            finish(nullptr, notification);
        }
        
        /**
         * Marks this job as "finished".
         */
        inline void finish(){
            finish(nullptr, nullptr);
        }
        
        /**
         * Sets the current state of this job. Protected to only allow
         * inheritors to assign their states.
         * @param st 
         */
        void setState(State st){
            state.store(st);
        }
        
    private:
        JobPool* owner;
        std::atomic<State> state;
    };
}