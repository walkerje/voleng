//
// Created by jesse on 2/27/21.
//

#include <voleng/display.hpp>
#include <voleng/engine.hpp>
#include <GLFW/glfw3.h>

namespace vlg{
    namespace{//Input callbacks to pass input events to the main notification queue.
        using namespace input;
        
        void passMouseButtonEvent(GLFWwindow* window, int button, int action, int mods){
            MouseButtonNotification* note = new MouseButtonNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->button = button;
            note->action = action;
            note->mods = mods;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
        
        void passMouseScrollEvent(GLFWwindow* window, double xoffs, double yoffs){
            MouseScrollNotification* note = new MouseScrollNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->xoffs = xoffs;
            note->yoffs = yoffs;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
        
        void passMouseMotionEvent(GLFWwindow* window, double xPos, double yPos){
            MouseMotionNotification* note = new MouseMotionNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->xpos = xPos;
            note->ypos = yPos;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
        
        void passMouseEnterEvent(GLFWwindow* window, int entered){
            MouseEnterNotification* note = new MouseEnterNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->entered = entered;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
        
        void passKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods){
            KeyNotification* note = new KeyNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->key = key;
            note->scancode = scancode;
            note->down = action == GLFW_PRESS;
            note->mods = mods;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
        
        void passCharEvent(GLFWwindow* window, unsigned int codepoint){
            CharNotification* note = new CharNotification;
            note->display = (Display*)glfwGetWindowUserPointer(window);
            note->codepoint = codepoint;
            VLGEngine::getInstance().getNotificationQueue().enqueueNotification(note);
        }
    }
    
    Display::Display(const std::string &title, vlg::Display::Extent size) {
        wPtr = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer((GLFWwindow*)wPtr, this);
        
        glfwSetMouseButtonCallback((GLFWwindow*)wPtr, passMouseButtonEvent);
        glfwSetScrollCallback((GLFWwindow*)wPtr,      passMouseScrollEvent);
        glfwSetCursorPosCallback((GLFWwindow*)wPtr,   passMouseMotionEvent);
        glfwSetCursorEnterCallback((GLFWwindow*)wPtr, passMouseEnterEvent);
        glfwSetKeyCallback((GLFWwindow*)wPtr,         passKeyEvent);
        glfwSetCharCallback((GLFWwindow*)wPtr,        passCharEvent);
        
        //TODO: More callback types & Controller support.
    }
    
    Display::~Display() {
        glfwDestroyWindow((GLFWwindow*)wPtr);
        wPtr = nullptr;
    }
    
    bool Display::isClosing() const {
        return glfwWindowShouldClose((GLFWwindow*)wPtr);
    }
    
    Display::Position Display::getPosition() const {
        Display::Position position;
        glfwGetWindowPos((GLFWwindow*)wPtr, &position.x, &position.y);
        return position;
    }
    
    Display::Extent Display::getExtent() const {
        Display::Extent extent;
        glfwGetWindowSize((GLFWwindow*)wPtr, &extent.x, &extent.y);
        return extent;
    }

    Display::Extent Display::getFramebufferExtent() const {
        Display::Extent extent;
        glfwGetFramebufferSize((GLFWwindow*)wPtr, &extent.x, &extent.y);
        return extent;
    }
    
    bool Display::getFullscreen() const {
        return glfwGetWindowMonitor((GLFWwindow*)wPtr) != nullptr;
    }
    
    void Display::setFullscreen(bool fullscreen) {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* primaryVidmode = glfwGetVideoMode(primaryMonitor);

        const Extent extent = getExtent();
        const int posX = primaryVidmode->width / 2 - (extent.x / 2);
        const int posY = primaryVidmode->height / 2 - (extent.y / 2);
        
        glfwSetWindowMonitor((GLFWwindow*) wPtr, fullscreen ? primaryMonitor : nullptr, posX, posY, extent.x, extent.y, GLFW_DONT_CARE);
    }
    
    void Display::setVSync(bool vsync) {
        //TODO: VSync Specification
    }
    
    bool Display::getVSync() const {
        return false;
    }
    
    bool Display::getHidden() {
        return glfwGetWindowAttrib((GLFWwindow*)wPtr, GLFW_VISIBLE) == GLFW_FALSE;
    }
    
    void Display::hide() {
        glfwHideWindow((GLFWwindow*)wPtr);
    }
    
    void Display::show() {
        glfwShowWindow((GLFWwindow*)wPtr);
    }
    
    void Display::setTitle(const std::string &title) {
        glfwSetWindowTitle((GLFWwindow*)wPtr, (this->title = title).c_str());
    }
    
    void Display::pollEvents(){
        glfwPollEvents();
    }
}