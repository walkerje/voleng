//
// Created by jesse on 3/2/21.
//

#include <voleng/audio/voice.hpp>

#include <soloud/soloud.h>

namespace vlg{
    namespace audio{
        void Voice::setVolume(float f) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setVolume(handle, f);
        }
        
        float Voice::getVolume() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getVolume(handle);
        }
        
        void Voice::setPan(float f) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setPan(handle, f);
        }
        
        float Voice::getPan() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getPan(handle);
        }
        
        void Voice::setLoop(bool loop) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setLooping(handle, loop);
        }
        
        bool Voice::getLoop() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getLooping(handle);
        }
        
        bool Voice::isProtected() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getProtectVoice(handle);
        }
        
        void Voice::setProtected(bool pr) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setProtectVoice(handle, pr);
        }
        
        void Voice::setLoopPoint(double val) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setLoopPoint(handle, val);
        }
        
        double Voice::getLoopPoint() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getLoopPoint(handle);
        }
        
        void Voice::setInaudibleBehaviour(bool tick, bool kill) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setInaudibleBehavior(handle, tick, kill);
        }
        
        bool Voice::getPaused() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getPause(handle);
        }
        
        void Voice::setPaused(bool v) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->setPause(handle, v);
        }
        
        void Voice::setProperties(const AbstractVoiceProperties &properties) {
            setVolume(properties.getVolume());
            setPan(properties.getPan());
            setLoop(properties.getLoop());
            setProtected(properties.isProtected());
            setLoopPoint(properties.getLoopPoint());
            setPaused(properties.getPaused());
        }
        
        bool Voice::isValid() const {
            return backend ? ((SoLoud::Soloud*)backend)->isValidVoiceHandle(handle) : false;
        }
        
        void Voice::seek(double t) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->seek(handle, t);
        }
        
        void Voice::fadeVolume(float to, double duration) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->fadeVolume(handle, to, duration);
        }
        
        void Voice::fadeVolumeToStop(double duration) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->fadeVolume(handle, 0, duration);
            sl->scheduleStop(handle, duration);
        }
        
        double Voice::getSeekPosition() {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            return sl->getStreamTime(handle);
        }
        
        void Voice::stop() {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->stop(handle);
        }
        
        void Voice3D::setVelocity(Velocity v) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->set3dSourceVelocity(handle, v.x, v.y, v.z);
        }
        
        void Voice3D::setPosition(Position p) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backend;
            sl->set3dSourcePosition(handle, p.x, p.y, p.z);
        }
        
    }
}