//
// Created by jesse on 2/28/21.
//

#pragma once

#include "resource_type.hpp"
#include <string>
#include <Poco/URI.h>

namespace vlg{
    /**
     * The ResourcePath class is akin to a URI specifically for engine resources.
     * These paths are often in the following form:    type://pkgname/alias
     * This can be extended in the future to support specific queries and fragments.
     */
    class ResourcePath{
    public:
        ResourcePath(const std::string& path)
            : path(path), uri(path){
            hash = std::hash<std::string>{}(path);
            auto preTrimAlias = uri.getPath();
            //trim off the leading forward slash from the path to get the proper alias...
            alias = preTrimAlias.substr(1, preTrimAlias.length() - 1);
        }
        
        ~ResourcePath(){}
        
        bool operator==(const ResourcePath& other) const{
            return hash == other.hash;
        }
        
        /**
         * @return the type of resource this path represents.
         */
        ResourceType getType() const{
            return ResourceType::value(uri.getScheme());
        }
        
        /**
         * @return the name of the package the resource this path refers to resides in.
         */
        const std::string& getPackageName() const{
            return uri.getHost();
        }
        
        /**
         * @return the alias assigned to the corresponding resource of this path.
         */
        const std::string& getResourceAlias() const{
            return alias;
        }
        
        /**
         * @return the fragment alongside this path. Usually empty.
         */
        const std::string& getFragment() const{
            return uri.getFragment();
        }

        /**
         * @return the pre-calculated hash of this path.
         */
        size_t getHash() const{
            return hash;
        }
        
        /**
         * @return the path's string.
         */
        const std::string& get() const{
            return path;
        }
    private:
        std::string path;
        std::string alias;
        Poco::URI uri;
        size_t hash;
    };
}

namespace std{
    template<>
    struct hash<vlg::ResourcePath>{
        size_t operator()(const vlg::ResourcePath& path) const noexcept{
            return path.getHash();
        }
    };
}