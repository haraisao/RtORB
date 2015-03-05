#!/bin/bash
export DYLD_LIBRARY_PATH=../../lib
./Client -ORBInitRef NameService=corbaloc::localhost:2809/NameService
#gdb ./Client
