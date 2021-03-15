set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${SRCDIR}/glad.c
        ${SRCDIR}/graphics_engine_system.cpp
        ${SRCDIR}/texture_factory.cpp
        ${SRCDIR}/texture_load_job.cpp
        ${SRCDIR}/shader_family_factory.cpp
        
        PARENT_SCOPE)
