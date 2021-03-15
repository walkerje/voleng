//
// Created by jesse on 3/1/21.
//

#pragma once

#include "job.hpp"
#include <mutex>
#include <queue>
#include <Poco/ThreadPool.h>

namespace vlg{
    class Job;
    
    /**
     * The JobPool class serves as the primary management system of miscellaneous jobs
     * that are better off done on a separate thread that doesn't need real-time performance.
     * 
     * Examples of this include resource loading, geometry generation, and network tasks.
     */
    class JobPool{
    public:
        JobPool(){}
        
        ~JobPool(){
            threads.joinAll();
            queueMutex.lock();
            
            //free all queued and "freed" jobs...
            while(!freeJobs.empty()){
                Job* job = freeJobs.front();
                freeJobs.pop();
                delete job;
            }

            //TODO: Deleting the job pool clobbers remaining jobs. Probably should allow them to finish after being queued.
            while(!queuedJobs.empty()){
                Job* job = queuedJobs.front();
                queuedJobs.pop();
                delete job;
            }
            queueMutex.unlock();
        }
        
        /**
         * Enqueues a job onto the thread pool.
         * The specified pointer must be allocated with "new".
         * The Job Pool owns the pointer to the specified job after this is called,
         * and will delete it automatically.
         * @param job 
         */
        void enqueue(Job* job){
            queueMutex.lock();
            queuedJobs.push(job);
            queueMutex.unlock();
        }
        
        /**
         * Updates this Job Pool.
         * This will clean up finished jobs and start every queued job it can.
         */
        void update(){
            threads.collect();//collect idle threads to get an accurate count of available threads.

            queueMutex.lock();            
            while(!freeJobs.empty()){
                Job* job = freeJobs.front();
                freeJobs.pop();
                delete job;
            }
            
            if(queuedJobs.empty() || threads.available() == 0){
                queueMutex.unlock();
                return;
            }

            //while there are threads available and there are queued jobs...
            while(threads.available() > 0 && !queuedJobs.empty()){
                Job* job = queuedJobs.front();
                queuedJobs.pop();
                threads.start(*job);//start every job we can!
                //the job should add itself to the "free" list upon completion.
            }
            queueMutex.unlock();
        }
        
        /**
         * Executes all jobs, or otherwise waits for them to finish.
         * This is a blocking call that can REALLY take a long time.
         * Use sparingly.
         */
        void flush(){
            while(!queuedJobs.empty()){
                //wait for jobs to finish...
                if(threads.available() == 0){
                    Poco::Thread::current()->yield();
                    continue;
                }
                Job* job = queuedJobs.front();
                queuedJobs.pop();
                threads.start(*job);
            }
        }
        
        /**
         * @return the size, in total number of jobs, of the job queue.
         */
        unsigned int getQueueSize() const{
            return queuedJobs.size();
        }
        
        /**
         * @return the total number of jobs, queued and working.
         */
        unsigned int getTotalJobs() const{
            return threads.used() + getQueueSize();
        }
        
        /**
         * @return the total number of active jobs.
         */
        unsigned int getActiveJobs() const{
            return threads.used();
        }
        
        /**
         * @return a read-only reference to the underlying thread pool.
         */
        const Poco::ThreadPool& getThreads() const{
            return threads;
        }
    private:
        friend class Job;
        
        std::mutex queueMutex;
        std::queue<Job*> queuedJobs, freeJobs;
        Poco::ThreadPool threads;
    };
}