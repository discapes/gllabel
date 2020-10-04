option(GLAD_PREFIX "GLAD prefix path" /usr)

find_path(GLAD_INCLUDE_PATH
    NAMES glad/glad.h
    PATHS ${GLAD_PREFIX}
    PATH_SUFFIXES include)

find_library(GLAD_LIBRARIES
    NAMES glad.a glad.lib
    PATHS ${GLAD_PREFIX}
    PATH_SUFFIXES lib)
    
 set(GLAD_FOUND GLAD_INCLUDE_PATH AND GLAD_LIBRARIES)
