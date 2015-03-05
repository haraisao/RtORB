#! /bin/bash

if [ "foo"$1 = "fooomni" ]; then
	omni/Server -ORBInitRef NameService=corbaloc::$2/NameService
else 
  if [ "foo"$1 = "foortorb" ]; then
	rtorb/Server -ORBInitRef NameService=corbaloc::$2/NameService
  else
	rtorb-cpp/Server -ORBInitRef NameService=corbaloc::$2/NameService
  fi
fi
