//
// Created by jesse on 3/2/21.
//

#include <voleng/audio/music_factory.hpp>
#include <voleng/audio/music_load_job.hpp>
#include <voleng/engine.hpp>

#include <soloud/soloud_wavstream.h>

#include <physfs.h>
#include <Poco/Observer.h>

namespace vlg{
    namespace audio{
        MusicFactory::MusicFactory() {
            VLGEngine& engine = VLGEngine::getInstance();
            AudioEngineSystem& sys = engine.getAudioSystem();

            sys.getNotificationCenter().addObserver(
                    Poco::Observer(*this, &MusicFactory::handleMusicUnload)
            );
        }

        MusicFactory::~MusicFactory() {
            VLGEngine& engine = VLGEngine::getInstance();
            AudioEngineSystem& sys = engine.getAudioSystem();

            sys.getNotificationCenter().removeObserver(
                    Poco::Observer(*this, &MusicFactory::handleMusicUnload)
            );
        }

        void MusicFactory::load(Resource *resource) const {
            Music& sfx = *((Music*)resource);
            SoLoud::WavStream* wavPointer = new SoLoud::WavStream;

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

        void MusicFactory::loadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            JobPool& jobPool = engine.getJobPool();
            Music& sfx = *((Music*)resource);
            sfx.currentState.store(Resource::State::LoadPending);
            jobPool.enqueue(new MusicLoadJob(jobPool, &sfx, this));
        }

        void MusicFactory::unload(Resource *resource) const {
            Music& sfx = *((Music*)resource);
            sfx.currentState.store(Resource::State::Unloading);
            SoLoud::WavStream* wavPointer = (SoLoud::WavStream*)sfx.handle;
            delete wavPointer;
            sfx.handle = nullptr;
            sfx.currentState.store(Resource::State::Unloaded);
        }

        void MusicFactory::unloadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            Music& music = *((Music*)resource);
            music.currentState.store(Resource::State::UnloadPending);

            //Post unload notification to the audio system...
            MusicUnloadNotification* note = new MusicUnloadNotification;
            note->music = &music;
            engine.getAudioSystem().getNotificationQueue().enqueueNotification(note);
        }

        void MusicFactory::handleMusicUnload(MusicUnloadNotification *note) {
            unload(note->music);//called when the audio system is "updated".
        }
    }
}