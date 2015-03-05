#hogehogehoge

if(UNIX)
  if(NOT UUID_LIBRARY_DIRS)
    find_library(
      UUID_LIBRARY uuid
      PATHS PATHS /usr/lib/ /home/lib/ /usr/local/lib/ /lib/
      NO_DEFAULT_PATH)

   if(NOT UUID_LIBRARY)
     MESSAGE(FATAL_ERROR "STOP CMAKE ---> Install libuuid package.......")
      find_library(UUID_LIBRARY uuid)
   endif()
 
   if(UUID_LIBRARY)
    get_filename_component(UUID_LIBRARY_DIRS ${UUID_LIBRARY} PATH)
    message(STATUS "detected ${UUID_LIBRARY}")
   else()
      message(STATUS "NOT detected ${UUID_LIBRARY}")
   endif()
  endif()

endif(UNIX)




