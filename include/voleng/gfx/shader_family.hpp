//
// Created by jesse on 3/4/21.
//

#pragma once

#include "../resource.hpp"

#include <memory>
#include <unordered_map>

namespace vlg{
    namespace gfx{
        class ShaderFamily;
        class GraphicsEngineSystem;
        
        class ShaderProgram{
        public:
           typedef unsigned int Handle;
           static constexpr Handle INVALID_HANDLE = 0;
           
           ~ShaderProgram() = default;
           
           /**
            * @return the handle of this shader program.
            */
           Handle getHandle() const{
               return handle;
           }
           
           /**
            * @return the shader family that owns this shader.
            */
           const ShaderFamily& getFamily() const{
               return *family;
           }
        private:
            friend class ShaderFamily;
            friend class GraphicsEngineSystem;
            
            ShaderProgram() = default;
            Handle handle = 0;
            ShaderFamily* family = nullptr;
        };
        
        class ShaderFamily : public Resource{
        public:
            ~ShaderFamily() = default;
            
            /**
             * Returns a read-only string containing the contents of the block associated with the specified key.
             * @param key 
             * @return 
             */
            const std::string& getBlock(const std::string key) const{
                return blocks.at(key);
            }
            
            /**
             * Returns a read-only reference to the shader program that corresponds with the specified alias.
             * @param alias the alias that corresponds with the returned shader program.
             * @return read-only reference to the shader program that corresponds with the specified alias.
             */
            const ShaderProgram& getProgram(const std::string& alias) const{
                return *programs.at(alias);
            }
        private:
            friend class GraphicsEngineSystem;
            
            std::unordered_map<std::string, std::string> blocks;
            std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> programs;
            
            ShaderFamily(const ResourceInfo& info)
                : Resource(info){}
        };
    }
}