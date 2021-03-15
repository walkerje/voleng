//
// Created by jesse on 2/27/21.
//

#pragma once

#include <string>

namespace vlg{
    /**
     * All of the different types of resources available to the engine.
     * Covers just about everything.
     */
    class ResourceType{
    public:
        static const ResourceType Unknown;
        
        static const ResourceType Texture;
        static const ResourceType Material;
        static const ResourceType Shader;
        static const ResourceType Script;
        static const ResourceType Text;
        static const ResourceType SoundEffect;
        static const ResourceType Music;
        static const ResourceType Font;
        
        static constexpr int TOTAL = 8;
        
        bool operator==(const ResourceType& other) const{
            return ordinal == other.ordinal;
        }
        
        /**
         * @return the name string of this resource type.
         */
        const std::string& getName() const{
            return name;
        }
        
        /**
         * @return the integer ordinal of this resource type.
         */
        int getOrdinal() const{
            return ordinal;
        }
        
        /**
         * Converts a resource type name to its class value equivalent.
         * @param name Resource type name.
         * @return corresponding resource type.
         */
        static ResourceType value(const std::string& name);
        
        /**
         * Converts a resource type ordinal to its class value equivalent.
         * @param ordinal resource type ID
         * @return corresponding resource type.
         */
        static ResourceType value(int ordinal);
    private:
        std::string name;
        int ordinal;
        
        ResourceType(const std::string& name, int ordinal)
            : name(name), ordinal(ordinal){}
    };
}