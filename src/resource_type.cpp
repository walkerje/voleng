#include <voleng/resource_type.hpp>

#include <unordered_map>

namespace vlg{
    const ResourceType ResourceType::Unknown("unknown", -1);
    
    const ResourceType ResourceType::Texture        ("texture", 0);
    const ResourceType ResourceType::Material       ("material", 1);
    const ResourceType ResourceType::Shader         ("shader", 2);
    const ResourceType ResourceType::Script         ("script", 3);
    const ResourceType ResourceType::Text           ("text", 4);
    const ResourceType ResourceType::SoundEffect    ("sfx", 5);
    const ResourceType ResourceType::Music          ("music", 6);
    const ResourceType ResourceType::Font           ("font", 7);
    
    ResourceType ResourceType::value(int ordinal) {
        if(ordinal < 0 || ordinal >= TOTAL)
            return ResourceType::Unknown;
        
        static const ResourceType MAPPED_TYPES[] = {
            Texture,
            Material,
            Shader,
            Script,
            Text,
            SoundEffect,
            Music,
            Font
        };
        
        return MAPPED_TYPES[ordinal];
    }
    
    ResourceType ResourceType::value(const std::string& name) {
        static const std::unordered_map<std::string, ResourceType> MAPPED_TYPES = {
                {Texture.getName(),     Texture},
                {Material.getName(),    Material},
                {Shader.getName(),      Shader},
                {Script.getName(),      Script},
                {Text.getName(),        Text},
                {SoundEffect.getName(), SoundEffect},
                {Music.getName(),       Music},
                {Font.getName(),        Font}
        };
        
        try{
            return MAPPED_TYPES.at(name);
        }catch(...){
            return Unknown;
        }
    }
}