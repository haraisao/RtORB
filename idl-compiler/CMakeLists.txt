
cmake_minimum_required(VERSION 2.6)

set(TARGET rtorb-idl)
set(ORBIT_VERSION "0.1")
set(orbit_backends_dir "/usr/lib/orbit-2.0/idl-backend")
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/idl-compiler)

set(ORBIT_INCLUDE_DIR "/usr/local/include/libIDL-2.0" "/usr/include/libIDL-2.0" "/usr/include/glib-2.0" "/usr/lib/glib-2.0/include" "/usr/local/include/glib-2.0" "${PROJECT_INCLUDE_DIR}" "/usr/include/")


set(CMAKE_CXX_FLAGS -g -Wall -Wunused -Wmissing-prototypes -Wmissing-declarations)

set(D_OPTION "-DHAVE_CONFIG_H"  "-DORBIT2_INTERNAL_API" "-DG_DISABLE_DEPRECATED" "-DVERSION=\"${ORBIT_VERSION}\"" "-DORBIT_BACKENDS_DIR=\"${orbit_backends_dir}\"" )

set(IDL_SOURCE "orbit-idl-main.c" "orbit-idl-driver.c" "orbit-idl-backend.c" "orbit-idl-utils.c" "orbit-idl-c-backend.c" "orbit-idl-c-stubs.c" "orbit-idl-c-skels.c" "orbit-idl-c-headers.c" "orbit-idl-c-common.c" "orbit-idl-c-skelimpl.c" "orbit-idl-c-utils.c" "orbit-idl-c-typecode.c" "rtorb-idl-c-utils.c" "myOptions.c" "orbit-idl-cpp-headers.c" "rtorb-idl-c-decls.c")


set(LINK_LIB "gobject-2.0" "gthread-2.0" "glib-2.0" "gmodule-2.0" "IDL-2")

include_directories(${ORBIT_INCLUDE_DIR})


link_directories(${GLIB_LIBRARY_DIRS} ${IDL_LIBRARY_DIRS})
#link_directories(/usr/lib /usr/local/lib)

add_definitions(${D_OPTION})

add_executable(${TARGET} ${IDL_SOURCE})

target_link_libraries(${TARGET} ${LINK_LIB})

install(PROGRAMS ${PROJECT_SOURCE_DIR}/idl-compiler/rtorb-idl DESTINATION  ${PROJECT_SOURCE_DIR}/bin/)

