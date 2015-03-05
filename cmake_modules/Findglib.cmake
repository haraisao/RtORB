#hogehogehoge

if(UNIX)
  if(NOT GLIB_LIBRARY_DIRS)
    find_library(
      GLIB_LIBRARY  glib-2.0
      PATHS /usr/lib/ /home/lib/ /usr/local/lib/ /lib/
      NO_DEFAULT_PATH)

   if(NOT GLIB_LIBRARY)
      find_library(GLIB_LIBRARY glib)
     MESSAGE(FATAL_ERROR "STOP CMAKE ---> Install libglib package...")
   endif()
 
   if(GLIB_LIBRARY)
    get_filename_component(GLIB_LIBRARY_DIRS ${GLIB_LIBRARY} PATH)
    message(STATUS "detected ${GLIB_LIBRARY}")
   else()
    message(STATUS "NOT detected ${GLIB_LIBRARY}")
   endif()
  endif()

endif(UNIX)




