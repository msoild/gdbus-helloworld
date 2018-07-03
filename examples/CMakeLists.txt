# At least this cmake version is needed
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Define a project name for the SW package
PROJECT (HiworldDbusExample)

# version
SET(HiworldDbusExample_VERSION_MAJOR 1)
SET(HiworldDbusExample_VERSION_MINOR 0)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)

ADD_SUBDIRECTORY(xml)

# The library has to be dynamically linked to the header
INCLUDE_DIRECTORIES(/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/generated)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)


link_directories(${PROJECT_BINARY_DIR}/)

set(SRCclient ${PROJECT_SOURCE_DIR}/src/libClient.c ${PROJECT_BINARY_DIR}/generated/Example-generated.c )
add_library(GDBusClient  ${SRCclient})


set(SRCserver  ${PROJECT_SOURCE_DIR}/src/libServer.c ${PROJECT_BINARY_DIR}/generated/Example-generated.c)
add_library(GDBusServer  ${SRCserver})

add_executable(SrvExample ${PROJECT_SOURCE_DIR}/src/SrvExample.c)
target_link_libraries(SrvExample ${PROJECT_BINARY_DIR}/libGDBusServer.a pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 rt glib-2.0)

add_executable(CltExample ${PROJECT_SOURCE_DIR}/src/CltExample.c)
target_link_libraries(CltExample ${PROJECT_BINARY_DIR}/libGDBusClient.a pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 rt glib-2.0)



