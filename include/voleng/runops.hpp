//
// Created by jesse on 2/26/21.
//

#pragma once

#include "app.hpp"
#include <string>

namespace vlg{
    /**
     * Run Options passed to the engine on initialization.
     */
    struct RunOptions{
        std::string windowTitle = "Voleng Engine";  //default window title
        std::string windowIconFile = "icon.png";    //path to default window icon (change to bitmap pointer??)
        std::string logFile = "voleng.log";         //path to the output log file. TODO: circular logging?
        std::string logFormat = "[%s](%m/%d/%y %H:%M:%S): %t";
        std::string configFile = "config.xml";      //path to config file
        std::string dataDir = "data";               //path to packaged resources
        
        int argc = 0;                   //argc from main function
        const char** argv = nullptr;    //argv from main function
    };
}