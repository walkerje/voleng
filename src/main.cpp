#include <voleng/voleng.hpp>
#include <Poco/Observer.h>

//To implement another resource type, I need to:
//Create and register its factory in the ResourceCenter instance of the engine, derived from ResourceFactory.
//Create its type, derived from the Resource class
//Create related load/unload jobs as needed.


//Keep entity logic EXCLUSIVELY in Lua.
//Allow engine to read ENTITY ATTACHMENTS (entries in instance tables) and process THOOOOSE in ANY way.
//e.g, update would be the following:
//process notifications -> integrate physics -> update entities through attachment table query -> encode frame rendering specification
//rendering thread will interpolate through these specifications to render at an arbitrary framerate/vsync
//while main thread manages task threads like loading and other interfaces

//as a network client, the update thread will predominantly be keeping up with the network
//and entity properties, likely instantiating enemies but not updating them locally.
//the attachment must find a way to heavily compress physics information for network speed

//Implement Resource Groups from here and continue working on the Graphics system.
//Next up might be Shaders. They're really important.
//  Addendum: Implement shader families. This will not be easy.

// Shader - Vertex Format Support
    // Generate simple vertex shaders based on vertex format specifications?
        // will likely need GLSL type reflection
    // Materials can be "configured" for generic vertex formats by compiling extra programs...

//From there, focus on geometry manipulation
//Definitely not using BGFX. I forgot how much of a pain in the ass the shader/texture workflow is.

class TestState : public vlg::ApplicationState{
public:
    TestState(){
        
    }
    
    ~TestState(){}
};

class TestApp : public vlg::Application{
public:
    TestState state;
    vlg::SoundEffect* effect;
    vlg::AudioVoice* voice = nullptr;
    
    TestApp() : vlg::Application("Test Application"){}
    ~TestApp(){}
    
    vlg::ApplicationState& onRun(const vlg::Configuration& config){
        vlg::VLGEngine& engine = vlg::VLGEngine::getInstance();
        
        vlg::ResourceCenter& resCenter = engine.getResourceCenter();
        resCenter.registerPackage("data/core");
        effect = resCenter.getResource<vlg::SoundEffect>("music://core/night");
        resCenter.load(*effect);
        
        engine.getNotificationCenter().addObserver(
                Poco::Observer(*this, &TestApp::onClick)
                );
        
        return state;
    }
    
    void onShutdown(){
        
    }
    
    void onClick(vlg::input::MouseButtonNotification* note){
        if(note->action){
            vlg::VLGEngine& engine = vlg::VLGEngine::getInstance();
            vlg::ResourceCenter& resCenter = engine.getResourceCenter();
            engine.getAudioSystem().play(*effect);
        }
    }
};

int main(int argc, const char** argv) {
    TestApp app;
    
    vlg::RunOptions runOptions;
    runOptions.argc = argc;
    runOptions.argv = argv;
    return vlg::VLGEngine::getInstance().run(runOptions, &app);
}