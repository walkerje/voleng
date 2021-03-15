//
// Created by jesse on 3/2/21.
//

#include <voleng/audio/audio_engine_system.hpp>
#include <voleng/audio/sound_effect_factory.hpp>
#include <voleng/engine.hpp>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#include <voleng/audio/music_factory.hpp>

namespace vlg{
    namespace audio{
        bool AudioEngineSystem::init(Configuration &config) {
            backendPtr = new SoLoud::Soloud;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->init();
            
            //1024 is the hard limit of virtual voices SoLoud can offer by default.
            //See here: http://sol.gfxile.net/soloud/voicemanagement.html
            for(int i = 0; i < 1024; i++)//pass the backend to ALL of the voices...
                voices[i].backend = sl;
            
            VLGEngine::getInstance().getResourceCenter().setFactory(ResourceType::SoundEffect, new SoundEffectFactory);
            VLGEngine::getInstance().getResourceCenter().setFactory(ResourceType::Music, new MusicFactory);
            return true;
        }
        
        void AudioEngineSystem::deinit() {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->deinit();
            delete sl;
            backendPtr = nullptr;
        }
        
        void AudioEngineSystem::fadeVolume(float to, double duration) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->fadeGlobalVolume(to, duration);
        }
        
        void AudioEngineSystem::setVolume(float f) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->setGlobalVolume(f);
        }
        
        float AudioEngineSystem::getVolume() const {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            return sl->getGlobalVolume();
        }
        
        void AudioEngineSystem::setPauseAll(bool value) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->setPauseAll(value);
        }
        
        void AudioEngineSystem::setListenerPosition(const Voice3D::Position &position) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->set3dListenerPosition(position.x, position.y, position.z);
        }
        
        void AudioEngineSystem::setListenerFacing(const Voice3D::Position &facing) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->set3dListenerAt(facing.x, facing.y, facing.z);
        }

        void AudioEngineSystem::setListenerUp(const vlg::audio::Voice3D::Velocity &up) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->set3dListenerUp(up.x, up.y, up.z);
        }

        void AudioEngineSystem::setListenerVelocity(const Voice3D::Velocity &vel) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->set3dListenerVelocity(vel.x, vel.y, vel.z);
        }
        
        Voice* AudioEngineSystem::play(SoundEffect& effect){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play(*wavPtr, effect.getVolume(), effect.getPan(), effect.getPaused());
            
            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            
            return &voice;
        }
        Voice* AudioEngineSystem::play(SoundEffect& effect, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play(*wavPtr, properties.getVolume(), properties.getPan(), properties.getPaused());
            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }

        Voice3D* AudioEngineSystem::play3D(SoundEffect& effect, Voice3D::Position position){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, 0, 0, 0, effect.getVolume(), effect.getPan(), effect.getPaused());
            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            return &voice;
        }
        Voice3D* AudioEngineSystem::play3D(SoundEffect& effect, Voice3D::Position position, Voice3D::Velocity velocity){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z, effect.getVolume(), effect.getPan(), effect.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            return &voice;
        }
        Voice3D* AudioEngineSystem::play3D(SoundEffect& effect, Voice3D::Position position, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, 0, 0, 0, properties.getVolume(), properties.getPan(), properties.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }
        Voice3D* AudioEngineSystem::play3D(SoundEffect& effect, Voice3D::Position position, Voice3D::Velocity velocity, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            
            SoLoud::Wav* wavPtr = (SoLoud::Wav*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z,
                    properties.getVolume(), properties.getPan(), properties.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }

        Voice* AudioEngineSystem::play(Music& effect){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play(*wavPtr, effect.getVolume(), effect.getPan(), effect.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            return &voice;
        }
        Voice* AudioEngineSystem::play(Music& effect, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play(*wavPtr, properties.getVolume(), properties.getPan(), properties.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }

        Voice3D* AudioEngineSystem::play3D(Music& effect, Voice3D::Position position){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, 0, 0, 0, effect.getVolume(), effect.getPan(), effect.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            return &voice;
        }
        Voice3D* AudioEngineSystem::play3D(Music& effect, Voice3D::Position position, Voice3D::Velocity velocity){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z, effect.getVolume(), effect.getPan(), effect.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(effect);
            return &voice;
        }
        Voice3D* AudioEngineSystem::play3D(Music& effect, Voice3D::Position position, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, 0, 0, 0, properties.getVolume(), properties.getPan(), properties.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }
        
        Voice3D* AudioEngineSystem::play3D(Music& effect, Voice3D::Position position, Voice3D::Velocity velocity, const VoiceProperties& properties){
            if(!effect)
                return nullptr;
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            SoLoud::WavStream* wavPtr = (SoLoud::WavStream*)effect.getHandle();
            auto handle = sl->play3d(*wavPtr, position.x, position.y, position.z, velocity.x, velocity.y, velocity.z,
                                     properties.getVolume(), properties.getPan(), properties.getPaused());

            int id = sl->getVoiceFromHandle_internal(handle);
            Voice3D& voice = voices[id];
            voice.handle = handle;
            voice.setProperties(properties);
            return &voice;
        }

        void AudioEngineSystem::stopSource(vlg::SoundEffect &effect) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->stopAudioSource(*((SoLoud::Wav*)effect.getHandle()));
        }

        void AudioEngineSystem::stopSource(Music &effect) {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->stopAudioSource(*((SoLoud::WavStream*)effect.getHandle()));
        }
        
        void AudioEngineSystem::stopAll() {
            SoLoud::Soloud* sl = (SoLoud::Soloud*)backendPtr;
            sl->stopAll();
        }
    }
}