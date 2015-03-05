#
#  Makefile for librtorb.so
#  Copyright(C) 2006, Isao Hara,AIST
#  $Revision: 1.1.1.1 $
#  $Date: 2008/01/08 07:56:08 $
#  $Id: Makefile,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
#
#
#CFLAG_OS = -DCYGWIN
CFLAG_OS = -DLinux 

CFLAG_DEBUG = -g -Wall #-DDEBUG
#CFLAG_DEBUG += -DDEBUG_MALLOC 

CFLAG_DEBUG = -O -Wall
LIBS = -lomniORB4

IDL_NAME = echo
IDL_SRC = ../$(IDL_NAME).idl
IDL_CC = /usr/bin/omniidl

ORB_SRC = $(IDL_NAME)SK.cc $(IDL_NAME).hh

SOBJS  = Server.o echoSK.o
COBJS  = Client.o echoSK.o

CXXFLAGS =  -D__OMNIORB4__ -D_REENTRANT  -D__OSVERSION__=2 -D__linux__ -D__x86__ -I. $(CFLAG_DEBUG) $(CFLAG_OS)
CXXFLAGS = -g -I. $(CFLAG_DEBUG) $(CFLAG_OS)
#CFLAGS += -DUSE_THREAD

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

