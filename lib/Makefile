#
#  Makefile for librtorb.so
#  Copyright(C) 2006, Isao Hara,AIST
#  $Revision: 1.10 $
#  $Date: 2008/06/06 10:14:21 $
#  $Id: Makefile,v 1.10 2008/06/06 10:14:21 yoshi Exp $
#
#

UNAME := $(shell uname -s)

ifneq (,$(findstring CYGWIN, $(UNAME)))
	UNAME = Cygwin
endif

#CFLAG_OS = -D$(UNAME)

CFLAG_DEBUG = -Wall
CFLAG_DEBUG += -g
#CFLAG_DEBUG += -DDEBUG
#CFLAG_DEBUG += -DDEBUG_MALLOC 
#CFLAG_DEBUG += -DHASH_DEBUG
#CFLAG_DEBUG += -DDEBUG_GIOP 

CFLAG_THREAD =
#CFLAG_THREAD = -DUSE_THREAD -D_REENTRANT

#CFLAGS_UUID =
#CFLAGS_UUID = -DUSE_UUID

ifeq ($(UNAME),Cygwin)
CFLAGS = $(INCLUDE) $(CFLAG_DEBUG) $(CFLAG_THREAD) $(CFLAGS_UUID)
UUID_LIB = -L/usr/lib/e2fsprogs -luuid
else
CFLAGS = -fpic $(INCLUDE) $(CFLAG_DEBUG) $(CFLAG_OS) $(CFLAG_THREAD) $(CFLAG_UUID)
endif
CFLAGS += -O2
#CFLAGS += -ansi

TOP_DIR=..
IDL_SRC = CosNaming
IDL_SOURCE = $(TOP_DIR)/idl/$(IDL_SRC).idl

INCLUDE = -I. -I../include -I./CosName 

NAMING_SRCS = CosName/CosNaming-common.c CosName/CosNaming-skels.c CosName/CosNaming-stubs.c CosName/NameService_impl.c
NAMING_OBJ = $(NAMING_SRCS:.c=.o)
NAMING_HEADER = CosName/CosNaming.h

#LIBOBJS  = client.o sockserv.o
LIBOBJS  = socket.o hashtable.o list.o array.o
LIBOBJS += util.o endian.o allocater.o
LIBOBJS += iop.o giop.o giop-msg.o giop-marshal.o
LIBOBJS += orb.o 
LIBOBJS += corba.o corba-typecode.o corba-exception.o corba-sequence.o corba-any.o
LIBOBJS += poa.o
LIBOBJS += rtorb.o connection.o server_loop.o
LIBOBJS += $(NAMING_OBJ)

ifdef CFLAG_THREAD
LIBOBJS += pthread.o
endif 

ifeq ($(UNAME),Cygwin)
LIBORB = RtORB.dll
LIBORB_A = libRtORB.a
LIBORB_CPP = RtORB_cpp.dll
NAMING_LIB = 
else
LIBORB = libRtORB.so
LIBORB_A = libRtORB.a
LIBORB_CPP = libRtORB_cpp.so
NAMING_LIB = libRtORB_CosNaming.so
endif


RANLIB=/usr/bin/ranlib

all: 
	$(MAKE) CosNaming
	$(MAKE) $(LIBORB)
	$(MAKE) libCXX
	$(MAKE) $(LIBORB_A)

CosName:
	if [ ! -L CosName ]; then \
		ln -s ../CosName ./; \
	fi;
	
CosNaming: CosName $(IDL_SOURCE)
	cd CosName && $(MAKE) 

libCXX: 
	cd CXX && $(MAKE) "DEFINE=$(DEFINE)" "CC=$(CXX)" "CXX=$(CXX)"
	if [ ! -L $(LIBORB_CPP) ]; then \
		ln -s CXX/$(LIBORB_CPP) ./; \
	fi

$(LIBORB): $(LIBOBJS) $(NAMING_HEADER) $(NAMING_SRCS)
	$(CC) -shared -o $@ $(LIBOBJS)  $(UUID_LIB)

$(LIBORB_A): $(LIBOBJS) $(NAMING_HEADER) $(NAMING_SRCS)
	$(AR) rv $@ $(LIBOBJS)  # $(UUID_LIB)
	$(RANLIB)  $@

clean:
	$(MAKE) -C CXX clean 
	rm -f *~  $(LIBOBJS)  $(NAMING_HEADER)

allclean:
	rm -f *.o *~ $(LIBORB)
	cd CosName && $(MAKE) allclean
	cd CXX && $(MAKE) allclean

