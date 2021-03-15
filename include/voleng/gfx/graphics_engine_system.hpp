//
// Created by jesse on 3/1/21.
//

#pragma once

#include "../engine_system.hpp"

#include <atomic>
#include <Poco/RunnableAdapter.h>
#include <Poco/Thread.h>

namespace vlg{
    class VLGEngine;
    
    namespace gfx{
        class GraphicsEngineSystem : public EngineSystem{
        public:
            static constexpr char NAME[] = "Graphics";
            
            ~GraphicsEngineSystem() = default;
            
            bool init(Configuration& config);
            
            void deinit();
            
        private:
            friend class vlg::VLGEngine;
            
            //The render thread function encompasses the entirety of the rendering thread
            void renderThreadFunc();
            
            void renderThreadInit();
            
            void renderThreadDeinit();

            Poco::RunnableAdapter<GraphicsEngineSystem> threadRunner;
            Poco::Thread renderThread;
            std::atomic_bool threadRunning = false;//used to indicate that the render thread is running
                                                   //and if it should stop. Atomic for well-defined behavior.
            
            GraphicsEngineSystem() 
                : EngineSystem(NAME),
                  threadRunner(*this, &GraphicsEngineSystem::renderThreadFunc){
            }
        };
    }
}