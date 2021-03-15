//
// Created by jesse on 2/28/21.
//

#include <voleng/resource_pkg.hpp>
#include <physfs.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/File.h>

namespace vlg{
    ResourcePackage::~ResourcePackage() {
        //unmount on destruct if valid. Yay!
        if(valid)
            PHYSFS_unmount(path.c_str());
    }
    
    bool ResourcePackage::preLoad() {
        if(!Poco::File(path).exists())
            return false;
        
        PHYSFS_mount(path.c_str(), TEMP_MOUNT_POINT, 1);
        
        const std::string manifestPath = std::string(TEMP_MOUNT_POINT) + "/" + MANIFEST_FILENAME;
        
        if(!PHYSFS_exists(manifestPath.c_str())){
            PHYSFS_unmount(path.c_str());
            return false;
        }

        //stat the manifest file to get its size...
        PHYSFS_Stat manifestStat; PHYSFS_stat(manifestPath.c_str(), &manifestStat);

        //Slurp the manifest file contents...
        char* manifestSource = new char[manifestStat.filesize];
        
        PHYSFS_File* manifestFile = PHYSFS_openRead(manifestPath.c_str());
        PHYSFS_readBytes(manifestFile, manifestSource, manifestStat.filesize);
        PHYSFS_close(manifestFile);
        
        try{//catch parsing errors and return false...
            Poco::XML::DOMParser domParser;//parse the manifest into a DOM
            manifest = domParser.parseMemory(manifestSource, manifestStat.filesize);
            delete[] manifestSource;
            return true;
        }catch(...){
            PHYSFS_unmount(path.c_str());
            delete[] manifestSource;
            return false;
        }
    }
    
    bool ResourcePackage::load() {
        try{
            PHYSFS_Stat stat;//used for collecting file sizes...
            
            Poco::XML::NodeIterator it(manifest, Poco::XML::NodeFilter::SHOW_ALL);
            Poco::XML::Node* node = it.nextNode();
            
            while(node){
                if(node->nodeName() == MANIFEST_KEY_TITLE){//read the title
                    node = it.nextNode();//get to the next node, which is the value
                    pkgTitle = node->nodeValue();
                }else if(node->nodeName() == MANIFEST_KEY_NAME){//read the name
                    node = it.nextNode();//again, get to the next value node
                    pkgName = node->nodeValue();
                }else if(node->nodeName() == MANIFEST_KEY_RESOURCES){//resources node...
                    if(node->hasChildNodes()){//if the <resources> tag has children (which it SHOULD)

                        Poco::XML::NodeIterator resIt(node, Poco::XML::NodeFilter::SHOW_ALL);
                        Poco::XML::Node* resNode = resIt.nextNode();
                        
                        while(resNode){//for each resource node...
                            if(resNode->nodeName() == MANIFEST_NODE_RES){
                                if(!resNode->hasAttributes()){//skip resources with no attributes.
                                    resNode = resIt.nextNode();
                                    continue;
                                }
                                
                                Poco::XML::NamedNodeMap* resAttribs = resNode->attributes();

                                const std::string path = resAttribs->getNamedItem(MANIFEST_PROP_PATH)->getNodeValue();

                                ResourceInfo info;
                                info.alias = resAttribs->getNamedItem(MANIFEST_PROP_ALIAS)->getNodeValue();
                                info.type = ResourceType::value(resAttribs->getNamedItem(MANIFEST_PROP_TYPE)->getNodeValue());
                                info.fsPath = pkgName + "/" + path;
                                info.package = this;
                                info.manifestNode = resNode;

                                //temporary path used to stat the file and get its size
                                //the temp path is also used to check for the corresponding file's existence
                                const std::string tempPath = std::string(TEMP_MOUNT_POINT) + "/" + path;

                                info.exists = PHYSFS_exists(tempPath.c_str());

                                if(info.exists){
                                    PHYSFS_stat(tempPath.c_str(), &stat);
                                    info.size = stat.filesize;
                                }else info.size = 0;

                                resources[info.type.getOrdinal()][info.alias] = info;
                            }
                            
                            resNode = resIt.nextNode();
                        }
                        
                    }else break;
                }
                
                node = it.nextNode();
            }
            
            return true;
        }catch(...){
            PHYSFS_unmount(path.c_str());//unmount on failure for ANY reason.
            return false;
        }
    }
    
    void ResourcePackage::postLoad() {
        PHYSFS_unmount(path.c_str());//unmount and remount at the package's name as the root directory
        PHYSFS_mount(path.c_str(), pkgName.c_str(), 0);
    }
}