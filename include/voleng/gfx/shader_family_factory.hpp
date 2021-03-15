//
// Created by jesse on 3/6/21.
//

#pragma once

#include "../resource_factory.hpp"
#include "shader_family.hpp"

namespace vlg{
    namespace gfx{
        class GraphicsEngineSystem;
        
        class ShaderFamilyFactory : public ResourceFactory<ShaderFamily>{
        public:
            virtual ~ShaderFamilyFactory() = default;
            
            void load(Resource* res) const;
            
            void loadAsync(Resource* res) const;
            
            void unload(Resource* res) const;
            
            void unloadAsync(Resource* res) const;
            
            /**
             * @return returns a pointer to a ShaderFamily resource allocated with "new".
             */
            Resource* createInstance() const{
//                return new ShaderFamily(currentInfo);
                return nullptr;
            }
            
        private:
            friend class GraphicsEngineSystem;
            
            void parsePackage();
            
            ShaderFamilyFactory() = default;
        };
    }
}