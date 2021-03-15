//
// Created by jesse on 3/1/21.
//

#pragma once

#include <glm/vec2.hpp>
#include "../resource.hpp"

namespace vlg{
    namespace gfx{
        
        class TextureFactory;
        
        /**
         * The Texture class is a very simple representation of an OpenGL texture.
         * As an interface to the Resource subsystem, almost the entirety of its
         * 
         */
        class Texture : public Resource{
        public:
            typedef glm::ivec2 Extent;
            typedef unsigned int Handle;
            
            static constexpr Handle INVALID_HANDLE = 0;
            
            ~Texture(){}
            
            /**
             * @return the size of the texture, in pixels.
             */
            const Extent& getExtent() const{
                return size;
            }
            
            /**
             * @return the underlying texture handle corresponding with this texture resource.
             */
            Handle getHandle() const{
                return handle;
            }
        private:
            friend class TextureFactory;

            Texture(const ResourceInfo& info)
                    : Resource(info), size({0, 0}), handle(INVALID_HANDLE){}
            
            Extent size;
            Handle handle;
        };
    }
    
    using gfx::Texture;
}