#pragma once

#include <Poco/Notification.h>

namespace vlg{
    class Display;
    
    namespace input{
        struct MouseButtonNotification : public Poco::Notification{
            Display* display;
            int button;
            int action;
            int mods;
        };

        struct MouseScrollNotification : public Poco::Notification{
            Display* display;
            double xoffs;
            double yoffs;
        };
        
        struct MouseMotionNotification : public Poco::Notification{
            Display* display;
            double xpos;
            double ypos;
        };

        struct MouseEnterNotification : public Poco::Notification{
            Display* display;
            bool entered;
        };

        struct KeyNotification : public Poco::Notification{
            Display* display;
            int key;
            int scancode;
            int mods;
            bool down;
        };
        
        struct CharNotification : public Poco::Notification{
            Display* display;
            unsigned int codepoint; 
        };
    }
}