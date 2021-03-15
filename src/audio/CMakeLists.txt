set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${SRCDIR}/audio_engine_system.cpp
        ${SRCDIR}/sound_effect_factory.cpp
        ${SRCDIR}/sound_effect_load_job.cpp
        ${SRCDIR}/music_factory.cpp
        ${SRCDIR}/music_load_job.cpp
        ${SRCDIR}/voice.cpp

        PARENT_SCOPE)