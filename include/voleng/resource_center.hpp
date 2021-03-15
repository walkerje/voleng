//
// Created by jesse on 2/28/21.
//

#pragma once

#include "resource_pkg.hpp"
#include "resource_path.hpp"
#include "resource_factory.hpp"
#include "resource.hpp"

#include <memory>
#include <unordered_map>
#include <type_traits>

#include <Poco/Logger.h>
#include <Poco/DynamicFactory.h>

namespace vlg{
    class VLGEngine;
    
    /**
     * The ResourceCenter class sits between resource instances themselves and their respective packages
     * from which they originate. This class does not serve as a method of loading resources,
     * but rather as a form of accessing them. Memory management is done through a series of Manager objects
     * that pertain to each kind of resource.
     */
    class ResourceCenter{
    public:
        ~ResourceCenter(){}
        
        /**
         * Registers a resource package at the specified path.
         * Once packages have been registered, they CANNOT be un-registered.
         * @param path filesystem path to the package
         * @return pointer to the package, null on failure.
         */
        ResourcePackage* registerPackage(const std::string& path){
             ResourcePackage* package = new ResourcePackage(path);
             if(!package->isValid()){
                 delete package;
                 getLogger().warning("Failed to register resource package: \"%s\"", path);
                 return nullptr;
             }
             getLogger().information("Registered Package \"%s\".", package->getTitle());
             packages[package->getName()] = std::unique_ptr<ResourcePackage>(package);
             return package;
        }
        
        /**
         * Returns a read-write reference to a previously loaded package with the specified name.
         * @param name the name of the package, specified in its manifest
         * @return a read-write reference to the associated package.
         */
        ResourcePackage& getPackage(const std::string& name){
            return *packages[name];
        }
        
        /**
         * Sets the factory for the specified resource type.
         * The supplied pointer is owned by the resource center following this call.
         * @tparam T Language resource type
         * @param type resource type
         * @param instantiator pointer, now owned by the resource center.
         */
        template<typename T>
        void setFactory(ResourceType type, ResourceFactory<T>* instantiator){
            static_assert(std::is_base_of<Resource, T>::value, "Type T must be a child of the Resource class.");
            decltype(factory)::AbstractFactory* tempFactoryPtr = instantiator;
            
            factories[type.getOrdinal()] = instantiator;
            if(factory.isClass(type.getName()))
                factory.unregisterClass(type.getName());
            else getLogger().information("Registered factory for resource type \"%s\".", type.getName());
            
            factory.registerClass(type.getName(), tempFactoryPtr);
        }
        
        /**
         * Returns a pointer to the resource indicated by the specified path.
         * The invoker of this function does NOT own the returned pointer;
         * rather, the pointer is valid until the resource center is destructed.
         * Will return null if the resource does not exist.
         * @tparam T return type, default is Resource. MUST be derived from the Resource class.
         * @param path resource path
         * @return resource pointer
         */
        template<typename T = Resource>
        T* getResource(const ResourcePath& path){
            static_assert(std::is_base_of<Resource, T>::value, "Type T must be a child of the Resource class.");
            
            if(resources.count(path)){//resource is already mapped, just grab it (resource paths are pre-hashed).
                return ((T*)(resources[path].get()));
            }else{
                if(factories[path.getType().getOrdinal()] == nullptr)
                    return nullptr;//no factory for the resource type. :(
                    
                ResourcePackage& package = *packages[path.getPackageName()];
                if(!package.contains(path.getType(), path.getResourceAlias()))
                    return nullptr;//resource simply does not exist.
                
                const ResourceInfo& info = package.getResourceInfo(path.getType(), path.getResourceAlias());
                ResourceFactory<T>* factory = (ResourceFactory<T>*)factories[path.getType().getOrdinal()];
                factory->setInstanceInfo(info);//set the instantiation information
                //ideally, resource factories will get ALL of their information from the ResourceInfo
                //object because it's rather flexible due to the inclusion of a pointer to the underlying
                //manifest.
                
                Resource* resPtr = factory->createInstance();
                if(resPtr){//if a valid pointer is returned from the factory, add it to the resource map and return it.
                    resources[path] = std::unique_ptr<Resource>(resPtr);
                    return ((T*)resPtr);
                }else{
                    delete resPtr;
                    return nullptr;
                }
            }
        }
        
        /**
         * Returns a resource with the specified resource path.
         * @tparam T Resource language type
         * @param path resource path
         * @return resource pointer, null if invalid.
         * @sa ResourcePath
         * @sa getResource(ResourcePath)
         */
        template<typename T = Resource>
        inline T* getResource(const std::string& path){
            return getResource<T>(ResourcePath{path});
        }
        
        /**
         * Unloads the specified resource asynchronously.
         * Silently fails if the resource's state indicates it is not either idle or unloaded.
         * @param resource 
         */
        void load(Resource& resource){
            const Resource::State state = resource.getState();
            if(state != Resource::State::Idle && state != Resource::State::Unloaded)
                return;
            AbstractResourceFactory* factory = factories[resource.getType().getOrdinal()];
            factory->loadAsync(&resource);
        }
        
        /**
         * Unloads the specified resource asynchronously.
         * Silently fails if the resource's state indicates it cannot be unloaded.
         * @param resource 
         */
        void unload(Resource& resource){
            const Resource::State state = resource.getState();
            if(state != Resource::State::Loaded)
                return;
            AbstractResourceFactory* factory = factories[resource.getType().getOrdinal()];
            factory->unloadAsync(&resource);
        }
        
        /**
         * Attempts to unload all resources. Success depends on the state of all resources.
         * @return a boolean indicating if all resources are unloaded.
         */
        bool tryUnloadAll(){
            bool done = true;
            for(auto& resourcePair : resources){
                Resource& resource = *resourcePair.second;
                const Resource::State state = resource.getState();
                const bool pending = resource.isPending();
                //invalid, unloaded, and idle resources get SKIPPED.
                if(state == Resource::State::Invalid || state == Resource::State::Unloaded || state == Resource::State::Idle)
                    continue;//skip invalid resources...
                
                if(pending){
                    done = false;
                }else if(state == Resource::State::Loaded){
                    unload(resource);
                    done = false;
                }
            }
            return done;
        }
    private:
        
        friend class VLGEngine;
        
        ResourceCenter(){
            for(int i = 0; i < ResourceType::TOTAL; i++)
                factories[i] = nullptr;
        }
        
        /*A getter for the engine's logger.*/
        Poco::Logger& getLogger();

        AbstractResourceFactory* factories[ResourceType::TOTAL];
        Poco::DynamicFactory<Resource> factory;
        std::unordered_map<std::string, std::unique_ptr<ResourcePackage>> packages;
        std::unordered_map<ResourcePath, std::unique_ptr<Resource>>       resources;
    };
}