#include <voleng/resource_center.hpp>
#include <voleng/engine.hpp>

namespace vlg{
    Poco::Logger& ResourceCenter::getLogger() {
        return VLGEngine::getInstance().getLogger();
    }
}