//
// Created by jesse on 2/27/21.
//

#pragma once

#include "resource_type.hpp"

#include <string>
#include <unordered_map>

#include <Poco/AutoPtr.h>
#include <Poco/DOM/Document.h>

namespace vlg{
    class ResourcePackage;
    
    struct ResourceInfo{
        bool exists;        //boolean indicating if the resource exists in the filesystem
        size_t size;        //the size of the resource, in bytes
        std::string fsPath; //the path in the virtual filesystem
        std::string alias;  //the alias assigned to the resource
        ResourceType type = ResourceType::Unknown;//the resource's type
        
        ResourcePackage* package = nullptr;
        Poco::AutoPtr<Poco::XML::Node> manifestNode = nullptr;
    };
    
    /**
     * The ResourcePackage class serves as the primary interface between the virtual
     * filesystem and collections of resources. Resource packages are simply a directory
     * (either a real directory, or an archive in one of many formats) that are mapped
     * in memory to specific aliases read from a manifest XML file.
     */
    class ResourcePackage{
    public:
        typedef std::unordered_map<std::string, ResourceInfo> ResourceMap;
        
        static constexpr char MANIFEST_FILENAME[]       = "manifest.xml";
        static constexpr char MANIFEST_KEY_TITLE[]      = "title";
        static constexpr char MANIFEST_KEY_NAME[]       = "name";
        static constexpr char MANIFEST_KEY_RESOURCES[]  = "resources";

        static constexpr char MANIFEST_NODE_RES[]      = "res";
        static constexpr char MANIFEST_PROP_TYPE[]      = "type";
        static constexpr char MANIFEST_PROP_ALIAS[]     = "alias";
        static constexpr char MANIFEST_PROP_PATH[]      = "path";
        
        static constexpr char TEMP_MOUNT_POINT[]      = "rpkgTemp";
        
        ResourcePackage(const std::string& path)
            : path(path){
            
            if(preLoad()){//if successful preload...
                if(load()){//if successful load...
                    postLoad();//clean up after the load process
                    valid = true;//and flip the validity boolean
                }
            }
        }
        
        ~ResourcePackage();
        
        /**
         * Returns a read-only pointer to the specified resource's information.
         * @param type 
         * @param alias 
         * @return 
         */
        const ResourceInfo& getResourceInfo(ResourceType type, const std::string& alias) const{
            return resources[type.getOrdinal()].at(alias);
        }
        
        /**
         * Returns a boolean indicating if a resource with the specified type and alias resides in this package.
         * @param type type of the resource
         * @param alias alias of the resource
         * @return a boolean indicating if the specified resource exists.
         */
        bool contains(ResourceType type, const std::string& alias) const{
            return resources[type.getOrdinal()].count(alias) > 0;
        }
        
        /**
         * Returns a resource mapping for the specified type of resource.
         * @param type the type of resource to query
         * @return the mapping associated with the specified type of resource.
         */
        const ResourceMap& getResources(ResourceType type){
            return resources[type.getOrdinal()];
        }
        
        /**
         * @return the formal title string of this resource package.
         */
        const std::string& getTitle() const{
            return pkgTitle;//the formal title
        }
        
        /**
         * @return the name of this package that is utilized in resource paths.
         */
        const std::string& getName() const{
            return pkgName;//the name used in resource paths
        }
        
        /**
         * @return a boolean indicating whether or not the resource package manifest was properly read.
         */
        bool isValid() const{
            return valid;
        }
        
        /**
         * @return the total number of resources in this package, of all types.
         */
        unsigned int getTotalResources() const{
            unsigned int j = 0;
            for(int i = 0; i < ResourceType::TOTAL; i++)
                j += resources[i].size();
            return j;
        }
    private:
        
        //pre-load; mounts the path on the virtual filesystem temporarily
        bool preLoad();
        
        //reads the manifest file that was mounted temporarily and gathers all its information.
        bool load();
        
        //post-load; unmounts the temporary path and re-mounts to a point associate with its name.
        void postLoad();
        
        bool valid = false;
        std::string path;       //path the package was constructed with
        std::string pkgTitle;   //the formal package title from the manifest
        std::string pkgName;    //the package name that is used in the filesystem and with resource paths
        
        Poco::AutoPtr<Poco::XML::Document> manifest;
        ResourceMap resources[ResourceType::TOTAL];
    };
}