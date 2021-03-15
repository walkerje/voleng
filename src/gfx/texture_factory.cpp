//
// Created by jesse on 3/1/21.
//

#include <voleng/gfx/texture_factory.hpp>
#include <voleng/engine.hpp>
#include <Poco/Observer.h>
#include <physfs.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad.h"


namespace vlg{
    namespace gfx{
        TextureFactory::TextureFactory(){
            VLGEngine& engine = VLGEngine::getInstance();
            engine.getGraphicsSystem().getNotificationCenter().addObserver(
                    Poco::Observer(*this, &TextureFactory::handleTextureUpdate)
                    );

            engine.getGraphicsSystem().getNotificationCenter().addObserver(
                    Poco::Observer(*this, &TextureFactory::handleTextureUnload)
            );
        }
        
        TextureFactory::~TextureFactory(){
            VLGEngine& engine = VLGEngine::getInstance();
            
            engine.getGraphicsSystem().getNotificationCenter().removeObserver(
                    Poco::Observer(*this, &TextureFactory::handleTextureUpdate)
            );
            
            engine.getGraphicsSystem().getNotificationCenter().removeObserver(
                    Poco::Observer(*this, &TextureFactory::handleTextureUpdate)
            );
        }
        
        void TextureFactory::load(Resource *res) const {
            Texture& texture = *((Texture*)res);
            VLGEngine& engine = VLGEngine::getInstance();
            texture.currentState.store(Resource::State::Loading);
            TextureUpdateNotification* update = new TextureUpdateNotification;
            update->texture = (Texture*)res;
            
            const ResourceInfo& info = texture.getInfo();
            
            update->pixels.resize(info.size, false);
            PHYSFS_file* imgFile = PHYSFS_openRead(info.fsPath.c_str());
            PHYSFS_readBytes(imgFile, update->pixels.begin(), info.size);
            PHYSFS_close(imgFile);
            
            stbi_uc* pixels = stbi_load_from_memory((stbi_uc *)(update->pixels.begin()), info.size, &texture.size.x, &texture.size.y, nullptr, 4);
            update->pixels.resize(texture.size.x * texture.size.y * 4, false);
            update->pixels.assign((const char *)(pixels), update->pixels.size());
            stbi_image_free(pixels);
            
            engine.getGraphicsSystem().getNotificationQueue().enqueueNotification(update);
        }
        
        void TextureFactory::loadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            JobPool& jobPool = engine.getJobPool();
            ((Texture*)resource)->currentState = Resource::State::LoadPending;
            jobPool.enqueue(new TextureLoadJob(jobPool, (Texture*)resource, this));
        }
        
        void TextureFactory::unload(Resource *res) const {
            Texture& texture = *((Texture*)res);
            texture.currentState = Resource::State::Unloaded;//mark it as unloaded early...
            //having async on this isn't really necessarily as important as getting
            //notification order correct.
            unloadAsync(res);
        }
        
        void TextureFactory::unloadAsync(Resource *resource) const {
            VLGEngine& engine = VLGEngine::getInstance();
            TextureUnloadNotification* note = new TextureUnloadNotification;
            note->texture = (Texture*)resource;
            note->texture->currentState = Resource::State::UnloadPending;
            engine.getGraphicsSystem().getNotificationQueue().enqueueNotification(note);
        }

        void TextureFactory::handleTextureUpdate(vlg::gfx::TextureUpdateNotification *note) {
            Texture& texture = *(note->texture);
            const Texture::Extent extent = texture.getExtent();
            if(texture.getHandle() == Texture::INVALID_HANDLE)
                glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle);
            glTextureStorage2D(texture.handle, 1, GL_RGBA8, extent.x, extent.y);
            glTextureSubImage2D(texture.handle, 0, 0, 0, extent.x, extent.y, GL_RGBA, GL_UNSIGNED_BYTE, note->pixels.begin());
            note->texture->currentState.store(Resource::State::Loaded);
        }
        
        void TextureFactory::handleTextureUnload(vlg::gfx::TextureUnloadNotification *note) {
            Texture& texture = *(note->texture);
            texture.currentState = Resource::State::Unloading;
            if(texture.getHandle() != Texture::INVALID_HANDLE){
                glDeleteTextures(1, &texture.handle);
                texture.handle = Texture::INVALID_HANDLE;
            }
            texture.currentState = Resource::State::Unloaded;
        }
    }
}