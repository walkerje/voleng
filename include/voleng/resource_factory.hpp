//
// Created by jesse on 2/28/21.
//

#pragma once

#include "resource_pkg.hpp"
#include "resource.hpp"

#include <Poco/DynamicFactory.h>
#include <Poco/Instantiator.h>

namespace vlg{
    class AbstractResourceFactory : public Poco::DynamicFactory<Resource>::AbstractFactory {
    public:
        virtual ~AbstractResourceFactory() = default;

        /**
         * Performs a load operation for the specified resource pointer.
         * This is done on the invoking thread, and has no guarantee of safety.
         * It's generally safest to load resources with async API.
         * @param resource 
         */
        virtual void load(Resource* resource) const = 0;

        /**
         * Queues a job to load the specified texture resource.
         * Notifies the graphics system upon completion.
         * @param resource 
         */
        virtual void loadAsync(Resource* resource) const = 0;

        /**
         * Performs an unload operation for the specified resource pointer.
         * This is done on the invoking thread, and has no guarantee of safety.
         * It's generally safest to unload resources with the async API.
         * @param resource 
         */
        virtual void unload(Resource* resource) const = 0;

        /**
         * Notifies the graphics system that the specified texture resource can be unloaded.
         * @param resource 
         */
        virtual void unloadAsync(Resource* resource) const = 0;

        /**
         * Sets the information for the next created instance.
         * @param info 
         */
        virtual void setInstanceInfo(const ResourceInfo& info) = 0;

        /**
         * Creates a resource instance using the current instance information.
         * @return resource pointer. 
         */
        virtual Resource* createInstance() const = 0;
    protected:
        AbstractResourceFactory(){}
    };
    
    template<typename T>
    class ResourceFactory : public AbstractResourceFactory{
    public:
        static_assert(std::is_base_of<Resource, T>::value, "Type T must be a child of the Resource class.");
        ResourceFactory(){}
        
        virtual ~ResourceFactory() = default;
        
        /**
         * Performs a load operation for the specified resource pointer.
         * This is done on the invoking thread, and has no guarantee of safety.
         * It's generally safest to load resources with async API.
         * @param resource 
         */
        virtual void load(Resource* resource) const = 0;
        
        /**
         * Queues a job to load the specified texture resource.
         * Notifies the graphics system upon completion.
         * @param resource 
         */
        virtual void loadAsync(Resource* resource) const = 0;
        
        /**
         * Performs an unload operation for the specified resource pointer.
         * This is done on the invoking thread, and has no guarantee of safety.
         * It's generally safest to unload resources with the async API.
         * @param resource 
         */
        virtual void unload(Resource* resource) const = 0;
        
        /**
         * Notifies the graphics system that the specified texture resource can be unloaded.
         * @param resource 
         */
        virtual void unloadAsync(Resource* resource) const = 0;
        
        /**
         * Sets the information for the next created instance.
         * @param info 
         */
        void setInstanceInfo(const ResourceInfo& info){
            currentInfo = info;
        }
        
        /**
         * Creates a resource instance using the current instance information.
         * @return resource pointer. 
         */
        virtual Resource* createInstance() const = 0;
    protected:
        ResourceInfo currentInfo;//used to construct the resource instance...
    };
}