#!/bin/bash
export DYLD_LIBRARY_PATH=../../lib
./Server -ORBInitRef NameService=corbaloc::localhost:2809/NameService
#gdb ./Server
