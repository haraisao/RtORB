
set(program CosNaming)
set(TARGET NameService)

cmake_minimum_required(VERSION 2.6)

IF("${OS_NAME}" MATCHES "FreeBSD")
add_definitions(-D${OS_NAME} -DUSE_UUID)
ELSEIF("${OS_NAME}" MATCHES "Linux")
add_definitions(-D${OS_NAME})
ENDIF()

set(OUTPUT_SOURCES 
    ${CMAKE_SOURCE_DIR}/CosName/CosNaming-common.c 
    ${CMAKE_SOURCE_DIR}/CosName/CosNaming-stubs.c
    ${CMAKE_SOURCE_DIR}/CosName/CosNaming.h 
    ${CMAKE_SOURCE_DIR}/CosName/CosNaming-skels.c 
    ${CMAKE_SOURCE_DIR}/CosName/CosNaming-skelimpl.c)

set(IDL_EXE ${CMAKE_SOURCE_DIR}/idl-compiler/rtorb-idl)
set(IDL_SOURCES ${CMAKE_SOURCE_DIR}/idl/CosNaming.idl)

set(SOURCES NameService.c)


link_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/RtORB/ /usr/local/lib/) 
include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/include)


#add_custom_command( OUTPUT ${OUTPUT_SOURCES}
#  COMMAND ${IDL_EXE} ${IDL_SOURCES} 
#  COMMAND ${IDL_EXE}  --skeleton-impl ${IDL_SOURCES} 
#  DEPENDS NameService.c
  #DEPENDS ${CMAKE_SOURCE_DIR}/lib/iop.c
#  )


add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} pthread RtORB )



install(PROGRAMS  ${PROJECT_SOURCE_DIR}/CosName/NameService DESTINATION  ${PROJECT_SOURCE_DIR}/bin/)


