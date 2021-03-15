//
// Created by jesse on 2/26/21.
//

#include <voleng/engine.hpp>

#include <Poco/File.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Notification.h>

#include <GLFW/glfw3.h>
#include <physfs.h>
#include <Poco/Observer.h>

namespace vlg{
    namespace{
        void catchGLFWError(int ec, const char* description){
            VLGEngine::getInstance().getLogger().error("GLFW Error #%d: %s", ec, description);
        }
    }
    
    VLGEngine VLGEngine::instance;
    
    VLGEngine& VLGEngine::getInstance() {
        return instance;
    }
    
    void VLGEngine::handleDeferredNotification(VLGEngine::DeferredNotification *note) {
        //send the deferred message...
        note->target->getNotificationQueue().enqueueNotification(note->notification);
    }

    int VLGEngine::run(const RunOptions &options, Application *application) {
        runOptions = options;
        app = application;

        running = true;
        
        //deferred notification handler...
        noteCenter.addObserver(Poco::Observer(*this, &VLGEngine::handleDeferredNotification));
        
        exitStatus = preInit();
        if(exitStatus != EXIT_SUCCESS)
            return exitStatus;
        
        init();
        
        getLogger().information("Starting %s...", app->getName());
        app->state = &(app->onRun(*config));
        
        mainloop();
        
        getLogger().information("Stopping %s...", app->getName());
        app->onShutdown();
        
        preDeinit();
        deinit();
        
        return exitStatus;
    }
    
    int VLGEngine::preInit() {
        if(Poco::File(runOptions.configFile).exists())
            config = new Configuration(runOptions.configFile);
        else
            config = new Configuration();
        
        //ensure data directory exists...
        Poco::File(runOptions.dataDir).createDirectories();
            
        {//Configure the root logger, whose config is passed down to all other logger instances by default.
            Poco::AutoPtr<Poco::ConsoleChannel>     logConsoleChannel;
            Poco::AutoPtr<Poco::SimpleFileChannel>  logFileChannel;
            Poco::AutoPtr<Poco::SplitterChannel>    logChannelSplitter;
            Poco::AutoPtr<Poco::PatternFormatter>   logPatternFormatter;
            Poco::AutoPtr<Poco::FormattingChannel>  logChannelFormatter;
            
            logPatternFormatter = new Poco::PatternFormatter();
            logChannelSplitter  = new Poco::SplitterChannel();
            logConsoleChannel   = new Poco::ConsoleChannel;
            logFileChannel      = new Poco::SimpleFileChannel(runOptions.logFile);

            logPatternFormatter->setProperty("pattern", runOptions.logFormat);
            logChannelFormatter = new Poco::FormattingChannel(logPatternFormatter, logChannelSplitter);
            logChannelSplitter->addChannel(logConsoleChannel);
            logChannelSplitter->addChannel(logFileChannel);

            Poco::Logger::root().setChannel(logChannelFormatter);
            
            logger = &Poco::Logger::get("VolEng");
            app->logger = &Poco::Logger::get(app->getName());
        }
        
        getLogger().information("Starting VolEng...");
        
        //initialize the backend utilities
        
        //PHYSFS, GLFW, etc...
        if(PHYSFS_init(runOptions.argv[0]) == 0){
            const char* errMsg = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
            getLogger().error("Failed to initialize PhysFS Filesystem backend: %s", errMsg);
            return EXIT_FAILURE;
        }
        
        glfwSetErrorCallback(catchGLFWError);
        if(glfwInit() != GLFW_TRUE){
            getLogger().error("Failed to initialize GLFW backend.");
            PHYSFS_deinit();
            return EXIT_FAILURE;
        }
        
        //Set window hints for all created application displays...
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        
        return EXIT_SUCCESS;
    }
    
    void VLGEngine::init() {
        //initialize any engine systems here...
        
        //simple initializations first...
        jobPool = new JobPool();
        resources = new ResourceCenter();
        
        {   //Then continue with the application display.
            Display::Extent displaySize;
            displaySize.x = config->getInt(cfg::KEY_DISPLAY_WIDTH, cfg::VAL_DISPLAY_WIDTH);
            displaySize.y = config->getInt(cfg::KEY_DISPLAY_HEIGHT, cfg::VAL_DISPLAY_HEIGHT);
            
            Display* display = app->display = new Display(runOptions.windowTitle, displaySize);
            
            const bool isFullscreen = config->getBool(cfg::KEY_DISPLAY_FULLSCREEN, cfg::VAL_DISPLAY_FULLSCREEN);
            const bool isVSync = config->getBool(cfg::KEY_DISPLAY_VSYNC, cfg::VAL_DISPLAY_VSYNC);

            //TODO: Display icon setting. Need access to bitmaps from the Resource API. Maybe add bitmap resource...?
            
            display->setFullscreen(isFullscreen);
            display->setVSync(isVSync);
        }
        
        {   //Systems!
            audioSystem = new audio::AudioEngineSystem;
            audioSystem->init(*config);
            
            graphicsSystem = new gfx::GraphicsEngineSystem;
            graphicsSystem->init(*config);
        }
    }
    
    void VLGEngine::mainloop() {
        app->display->show();
        
        while(running && !app->display->isClosing()){
            Display::pollEvents();
            jobPool->update();
            noteQueue.dispatch(noteCenter);
            audioSystem->update();
        }
    }
    
    void VLGEngine::preDeinit() {
        //de-initialize any engine systems here...
        
        //CLEAN UP ALL LEFTOVER RESOURCES THAT HAVEN'T BEEN MANUALLY UNLOADED.
        //This loop waits for all resources to get to a state where they can be unloaded,
        //then unloads them, while updating the job pool and the note queue.
        //All engine systems should also be running normally while this is happening.
        //Coincidentally, this also ensures the job queue is devoid of all jobs relating to resources.
        while(!resources->tryUnloadAll()){
            jobPool->update();              //some operations REQUIRE the thread pool update to work correctly...
            audioSystem->update();
            noteQueue.dispatch(noteCenter); //same thing here, the central note queue MUST be updated.
            Poco::Thread::current()->yield();//this will spin a fair bit... yield here.
        }
        delete resources;
        
        delete jobPool;//Job pool being deleted here is VERY important. DO NOT MOVE THIS!!!
        
        {   //Engine systems!
            graphicsSystem->deinit();
            delete graphicsSystem;
            
            audioSystem->deinit();
            delete audioSystem;
        }
    }
    
    void VLGEngine::deinit() {
        //de-initialize backend utilities here
        //ensure they are de-initialized in opposite order
        glfwTerminate();
        PHYSFS_deinit();
    }
}