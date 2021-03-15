//
// Created by jesse on 3/1/21.
//

#include <voleng/job.hpp>
#include <voleng/engine.hpp>
#include <voleng/job_pool.hpp>

namespace vlg{
    void Job::finish(vlg::EngineSystem *targetSystem, Poco::Notification *finishNote) {
        setState(State::Finished);
        if(finishNote == nullptr)
            return;
        (targetSystem ? targetSystem->getNotificationQueue()
            : VLGEngine::getInstance().getNotificationQueue()).enqueueNotification(finishNote);
        
        owner->freeJobs.push(this);//free the job, guaranteed safe due to clarified ownership.
    }
}