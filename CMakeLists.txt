cmake_minimum_required(VERSION 2.6)


PROJECT(rtorb)


EXECUTE_PROCESS(COMMAND uname -s
                  RESULT_VARIABLE  result_val
                  OUTPUT_VARIABLE  output_val
                  ERROR_VARIABLE   error_val)
IF(result_val)
ELSE()
set(OS_NAME ${output_val})
execute_process(COMMAND echo "check os_name = ${OS_NAME}" )
ENDIF()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules/)
find_package(glib)
find_package(IDL)
IF(${OS_NAME} MATCHES "FreeBSD")
find_package(uuid)
ENDIF()


include_directories(/usr/include/)

add_subdirectory(idl-compiler)
add_subdirectory(lib)
add_subdirectory(CosName)
