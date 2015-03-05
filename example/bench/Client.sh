#! /bin/bash

#echo " ****** omniORB4 ******"
#omni/Client -ORBInitRef NameService=corbaloc::$1/NameService  $2
#echo "****** RtORB ******"
#rtorb/Client -ORBInitRef NameService=corbaloc::$1/NameService  $2
echo "****** RtORB C++ ******"
rtorb-cpp/Client -ORBInitRef NameService=corbaloc::$1/NameService  $2
