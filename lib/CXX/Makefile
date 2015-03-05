#
# $Revision: 1.7 $
# $Date: 2008/06/06 08:04:06 $
# $Id: Makefile,v 1.7 2008/06/06 08:04:06 yoshi Exp $
#

UNAME := $(shell uname -s)

ifneq (,$(findstring CYGWIN, $(UNAME)))
	UNAME = Cygwin
endif

#TOP_DIR=/usr/local/src/RtORB
TOP_DIR=../..
IDLC=$(TOP_DIR)/idl-compiler/rtorb-idl
IDL_FLAGS= --skeleton-impl
IDL_SRC = echo
IDL_SOURCE = $(TOP_DIR)/idl/$(IDL_SRC).idl
ECHO_SRCS = $(IDL_SRC).h $(IDL_SRC)-common.c $(IDL_SRC)-skels.c $(IDL_SRC)-stubs.c
S_SKELS = $(IDL_SRC)-common.c $(IDL_SRC)-skels.c
S_STUBS = $(IDL_SRC)-common.c $(IDL_SRC)-stubs.c
S_HEADERS = $(IDL_SRC).h
COMMON_OBJS = $(IDL_SRC)-common.o 
SKEL_OBJS = $(IDL_SRC)-skels.o $(IDL_SRC)-skelimpl.o 
STUB_OBJS = $(IDL_SRC)-stubs.o

#NAMING_SRCS = ../CosName/CosNaming-common.c ../CosName/CosNaming-skels.c ../CosName/CosNaming-stubs.c ../CosName/CosNaming-skelimpl.c

NAMING_OBJS = $(NAMING_SRCS:.c=.o)

LIBOBJS = corba.o poa.o CosName.o corba-any.o corba-exception.o orb.o $(NAMING_OBJS) cdrStream.o

ifeq ($(UNAME), Cygwin)
LIBORB = RtORB_cpp.dll
LIBORB_A = 
CFLAGS=  -I. -I$(TOP_DIR)/include  -I../CosName $(DEFINE)
else
LIBORB = libRtORB_cpp.so
LIBORB_A = libRtORB_cpp.a
CFLAGS= -fpic -I. -I$(TOP_DIR)/include  -I../CosName $(DEFINE)
endif

#CFLAGS += -O2
CFLAGS += -O 
CFLAGS += -g
#CFLAGS += -DDEBUG_MALLOC
#CFLAGS += -DDEBUG
#CFLAGS += -D_REENTRANT
#CFLAGS += -D$(UNAME)

#CFLAGS= -fpic -I. -I$(TOP_DIR)/include  -I../CosName
CXXFLAGS= $(CFLAGS)


LIBS= -L$(TOP_DIR)/lib -lRtORB  -L. -lRtORB_cpp 

CC=g++

RANLIB=/usr/bin/ranlib

TARGET = $(LIBORB) $(LIBORB_A)

all: $(TARGET) 

CosName.o : CosNaming-stubs.cc

$(IDL_SRC).h: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) $(IDL_SOURCE)

$(IDL_SRC)-skelimpl.c: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) --skeleton-impl  $(IDL_SOURCE)

Server: $(S_HEADERS) $(S_SKELS) $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LIBS)

Client: $(S_HEADERS) $(S_SKELS) $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LIBS)

Sample: $(S_HEADERS) $(S_SKELS) $(SAMPLE_OBJS)
	$(CC) -o $@ $(SAMPLE_OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe 

allclean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe $(ECHO_SRCS)

decode-ior: $(OBJS_DECODE)
	$(CC) -o $@ $(OBJS_DECODE) $(LIBS)

$(LIBORB): $(LIBOBJS)
	$(CC) -shared -fPIC -o $@ $(LIBOBJS)  -L.. -lRtORB  -luuid

$(LIBORB_A): $(LIBOBJS)
	$(AR) rv $@ $(LIBOBJS) 
	$(RANLIB)  $@
