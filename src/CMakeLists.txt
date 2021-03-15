set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES 
    ${SOURCE_FILES}
    ${SRCDIR}/main.cpp
    ${SRCDIR}/engine.cpp 
    ${SRCDIR}/display.cpp
    ${SRCDIR}/resource_type.cpp
    ${SRCDIR}/resource_pkg.cpp
    ${SRCDIR}/resource_center.cpp
    ${SRCDIR}/job.cpp

    PARENT_SCOPE)