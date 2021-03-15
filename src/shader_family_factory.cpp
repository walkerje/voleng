#include <voleng/gfx/shader_family_factory.hpp>
#include <voleng/gfx/graphics_engine_system.hpp>

namespace vlg{
    namespace gfx{
        void ShaderFamilyFactory::load(Resource *res) const {
            //implement shader family parser
            //make this complex. give it the time it deserves.
            //definitely use poco's XML stuff but add ways to include metadata
            //and material "link points"... whatever they are.
            //also need to process the GLSL a little bit
            //to process include statements from resource URIs.
            
            //definitely use CDATA xml nodes to house shader source
            //this should be a shallow structure (one root, one layer of root child nodes, each with cdata code)
            //meta can be configured through node attributes and a "meta" tag?
        }
        
        void ShaderFamilyFactory::loadAsync(Resource *res) const {
            
        }
        
        void ShaderFamilyFactory::unload(Resource *res) const {
            
        }
        
        void ShaderFamilyFactory::unloadAsync(Resource *res) const {
            
        }
    }
}