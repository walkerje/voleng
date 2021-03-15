#include <voleng/audio/sound_effect_factory.hpp>
#include <voleng/engine.hpp>

#include <soloud/soloud_wav.h>

#include <physfs.h>
#include <Poco/Observer.h>

namespace vlg{
    namespace audio{
        SoundEffectFactory::SoundEffectFactory() {
            VLGEngine& engine = VLGEngine::getInstance();
            AudioEngineSystem& sys = engine.getAudioSystem();
            
            sys.getNotificationCenter().addObserver(
                    Poco::Observer(*this, &SoundEffectFactory::handleEffectUnload)
                    );
        }
        
        SoundEffectFactory::~SoundEffectFactory() {
            VLGEngine& engine = VLGEngine::getInstance();
            AudioEngineSystem& sys = engine.getAudioSystem();

            sys.getNotificationCenter().removeObserver(
                    Poco::Observer(*this, &SoundEffectFactory::handleEffectUnload)
            );
        }
        
        void SoundEffectFactory::load(Resource *resource) const {
            SoundEffect& sfx = *((SoundEffect*)resource);
            SoLoud::Wav* wavPointer = new SoLoud::Wav;
            
            const ResourceInfo& info = sfx.getInfo();
            
            sfx.currentState.store(Resource::State::Loading);
            //slurp the file...
            unsigned char* fileBuffer = new unsigned char[info.size];
            PHYSFS_File* filePtr = PHYSFS_openRead(info.fsPath.c_str());
            PHYSFS_readBytes(filePtr, fileBuffer, info.size);
            PHYSFS_close(filePtr);
            
            //SoLoud takes ownership of the file buffer allocation
            //and deletes it automatically.
            wavPointer->loadMem(fileBuffer, info.size, false, true);
            sfx.duration = wavPointer->getLength();
            
            sfx.handle = wavPointer;
            sfx.currentState.store(Resource::State::Loaded);
        }
        
        void SoundEffectFactory::loadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            JobPool& jobPool = engine.getJobPool();
            SoundEffect& sfx = *((SoundEffect*)resource);
            sfx.currentState.store(Resource::State::LoadPending);
            jobPool.enqueue(new SoundEffectLoadJob(jobPool, &sfx, this));
        }
        
        void SoundEffectFactory::unload(Resource *resource) const {
            SoundEffect& sfx = *((SoundEffect*)resource);
            sfx.currentState.store(Resource::State::Unloading);
            SoLoud::Wav* wavPointer = (SoLoud::Wav*)sfx.handle;
            delete wavPointer;
            sfx.handle = nullptr;
            sfx.currentState.store(Resource::State::Unloaded);
        }
        
        void SoundEffectFactory::unloadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            SoundEffect& sfx = *((SoundEffect*)resource);
            sfx.currentState.store(Resource::State::UnloadPending);
            
            //Post unload notification to the audio system...
            SoundEffectUnloadNotification* note = new SoundEffectUnloadNotification;
            note->effect = &sfx;
            engine.getAudioSystem().getNotificationQueue().enqueueNotification(note);
        }
        
        void SoundEffectFactory::handleEffectUnload(SoundEffectUnloadNotification *note) {
            unload(note->effect);//called when the audio system is "updated".
        }
    }
}