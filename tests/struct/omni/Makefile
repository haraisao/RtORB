#
#  Makefile for StructTest with omniorb
#
CFLAG_OS = -DLinux 

CFLAG_DEBUG = -g -Wall #-DDEBUG
#CFLAG_DEBUG += -DDEBUG_MALLOC 
#CFLAGS += -DUSE_THREAD

LIBS = -lomniORB4

IDL_NAME = StructTest
IDL_SRC = ../$(IDL_NAME).idl
IDL_CC = /usr/bin/omniidl
IDL_FLAGS = -bcxx

ORB_SRC = $(IDL_NAME)SK.cc $(IDL_NAME).hh

SOBJS  = Server.o $(IDL_NAME)SK.o
COBJS  = Client.o $(IDL_NAME)SK.o

CXXFLAGS =  -D__OMNIORB4__ -D_REENTRANT  -D__OSVERSION__=2 -D__linux__ -D__x86__ -I. $(CFLAG_DEBUG) $(CFLAG_OS)
CXXFLAGS = -g -I. $(CFLAG_DEBUG) $(CFLAG_OS)

CC=g++

TARGETS = Server Client

all: $(IDL_NAME).hh $(TARGETS)

$(IDL_NAME).hh: 
	$(IDL_CC) -bcxx $(IDL_SRC)

$(IDL_NAME).cc:
	$(IDL_CC) -bcxx $(IDL_SRC)

Server: $(ORB_SRC) $(SOBJS)
	$(CC) -o $@ $(SOBJS)  $(LIBS)

Client: $(ORB_SRC) $(COBJS)
	$(CC) -o $@ $(COBJS)  $(LIBS)

clean:
	rm -f $(TARGETS) *.o *~

allclean:
	rm -f $(TARGETS) *.o *~  $(ORB_SRC)

.PHONY: proxy

proxy:
	$(IDL_CC) $(IDL_FLAGS) $(IDL_SRC)

