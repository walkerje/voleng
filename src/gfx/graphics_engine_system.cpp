//
// Created by jesse on 3/1/21.
//

#include <voleng/engine.hpp>
#include <voleng/gfx/graphics_engine_system.hpp>
#include <voleng/gfx/texture_factory.hpp>

#include "glad.h"
#include <GLFW/glfw3.h>

namespace vlg{
    namespace gfx{
        bool GraphicsEngineSystem::init(Configuration &config) {
            renderThread.start(threadRunner);
            getLogger().information("Initializing Rendering Thread...");
            while(!threadRunning.load(std::memory_order::memory_order_relaxed))//wait for a finished initialization...
                Poco::Thread::current()->yield();
            isInitialized = true;
            getLogger().information("Initialized.");
            return true;
        }

        void GraphicsEngineSystem::renderThreadInit() {
            Display& display = VLGEngine::getInstance().getApplication().getDisplay();
            GLFWwindow* wPtr = (GLFWwindow*)display.getWindowPointer();

            glfwMakeContextCurrent(wPtr);
            
            //TODO: Error Checking on GL procedure address loading.
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            
            //Add resource factories...
            VLGEngine::getInstance().getResourceCenter().setFactory<Texture>(ResourceType::Texture, new TextureFactory());
        }
        
        void GraphicsEngineSystem::renderThreadFunc() {
            renderThreadInit();

            Display& display = VLGEngine::getInstance().getApplication().getDisplay();
            GLFWwindow* wPtr = (GLFWwindow*)display.getWindowPointer();
            
            threadRunning.store(true, std::memory_order::memory_order_acq_rel);
            
            while(threadRunning.load(std::memory_order::memory_order_relaxed)){
                update();
                
                glClearColor(1,1,1,1);
                glClear(GL_COLOR_BUFFER_BIT);
                
                glfwSwapBuffers(wPtr);
            }
            
            renderThreadDeinit();
        }
        
        void GraphicsEngineSystem::renderThreadDeinit() {
            glfwMakeContextCurrent(nullptr);
        }
        
        void GraphicsEngineSystem::deinit() {
            threadRunning.store(false, std::memory_order::memory_order_acq_rel);
            renderThread.join();
            isInitialized = false;
            getLogger().information("De-initialized.");
        }
    }
}