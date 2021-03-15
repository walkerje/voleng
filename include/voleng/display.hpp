#pragma once

#include "input.hpp"

#include <string>
#include <glm/vec2.hpp>

namespace vlg{
    class VLGEngine;
    
    class Display{
    public:
        typedef glm::ivec2 Extent;
        typedef glm::ivec2 Position;
        
        /**
         * @return a boolean indicating if this window is closing.
         */
        bool isClosing() const;
        
        /**
         * @return the position of this display.
         */
        Position getPosition() const;
        
        /**
         * @return the size of this display.
         */
        Extent getExtent() const;
        
        /**
         * @return the size of the framebuffer of this display.
         */
        Extent getFramebufferExtent() const;
        
        /**
         * @return a boolean indicating if this display is fullscreen.
         */
        bool getFullscreen() const;
        
        /**
         * Sets a boolean indicating if this display is fullscreen.
         * @param fullscreen a boolean indicating if this display is fullscreen.
         */
        void setFullscreen(bool fullscreen);
        
        /**
         * @return a boolean indicating if this display is fullscreen.
         */
        bool getVSync() const;
        
        /**
         * Sets a boolean indicating if this display currently has vertical sync enabled.
         * @param vsync 
         */
        void setVSync(bool vsync);
        
        /**
         * @return a boolean indicating if this display is hidden.
         */
        bool getHidden();
        
        /**
         * Shows this display.
         */
        void show();
        
        /**
         * Hides this display.
         */
        void hide();
        
        /**
         * @return the title of this application display.
         */
        const std::string& getTitle() const{
            return title;
        }
        
        /**
         * Sets the title of this application display.
         * @param title the new title.
         */
        void setTitle(const std::string& title);
        
        /**
         * @return an opaque pointer to the underlying window type.
         */
        void* getWindowPointer() const{
            return wPtr;
        }
    private:
        friend class VLGEngine;
        
        /**
         * Polls events for ALL displays, thus being a static private function.
         */
        static void pollEvents();
        
        Display(const std::string& title, Extent size);
        ~Display();
        
        std::string title;
        void* wPtr = nullptr;
    };
}