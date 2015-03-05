#
# $Revision: 1.1.1.1 $
# $Date: 2008/01/08 07:56:09 $
# $Id: Makefile,v 1.1.1.1 2008/01/08 07:56:09 yoshi Exp $
#

#TOP_DIR=/usr/local/src/RtORB
TOP_DIR=../..
#IDLC=$(TOP_DIR)/idl-compiler/rtorb-idl
IDLC=/usr/bin/omniidl
IDL_FLAGS= -bcxx #--skeleton-impl
IDL_SRC = echo
#IDL_SOURCE = $(TOP_DIR)/idl/$(IDL_SRC).idl
IDL_SOURCE = ../$(IDL_SRC).idl
#ECHO_SRCS = $(IDL_SRC).h $(IDL_SRC)-common.c $(IDL_SRC)-skels.c $(IDL_SRC)-stubs.c
ECHO_SRCS = $(IDL_SRC).hh $(IDL_SRC)SK.cc

S_SKELS = $(IDL_SRC)-common.c $(IDL_SRC)-skels.c
S_STUBS = $(IDL_SRC)-common.c $(IDL_SRC)-stubs.c
S_HEADERS = $(IDL_SRC).h

#COMMON_OBJS = $(IDL_SRC)-common.o 
COMMON_OBJS = $(IDL_SRC)SK.o 
#SKEL_OBJS = $(IDL_SRC)-skels.o $(IDL_SRC)-skelimpl.o 
#STUB_OBJS = $(IDL_SRC)-stubs.o
SKEL_OBJS =
STUB_OBJS =

SERVER_OBJS  =  Server.o $(COMMON_OBJS) $(SKEL_OBJS)
CLIENT_OBJS  =  Client.o  $(COMMON_OBJS) $(STUB_OBJS)
SAMPLE_OBJS =  Sample.o $(COMMON_OBJS) $(SKEL_OBJS) $(STUB_OBJS)

OBJS_DECODE=  decode-ior.o 


CC=g++
#CFLAGS= -g  -I. -I$(TOP_DIR)/include -DDEBUG # -DCYGWIN
CXXFLAGS= -g  -I. 
#LIBS= -lm -lpthread -L$(TOP_DIR)/lib -lrtorb -luuid
LIBS= -lomniORB4


TARGET = Server Client 

all: $(TARGET) 

$(IDL_SRC).hh: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) $(IDL_SOURCE)

$(IDL_SRC)SK.cc: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) $(IDL_SOURCE)

Server: $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LIBS)

Client: $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LIBS)

Sample: $(SAMPLE_OBJS)
	$(CC) -o $@ $(SAMPLE_OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe 

allclean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe $(ECHO_SRCS)

decode-ior: $(OBJS_DECODE)
	$(CC) -o $@ $(OBJS_DECODE) $(LIBS)

$(LIBORB): $(LIBOBJS)
	$(CC) -shared -o $@ $(LIBOBJS) 
