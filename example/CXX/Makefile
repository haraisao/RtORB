#
# $Revision: 1.2 $
# $Date: 2008/03/12 10:52:15 $
# $Id: Makefile,v 1.2 2008/03/12 10:52:15 yoshi Exp $
#

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

LIBOBJS = corba.o poa.o CosName.o 
LIBORB = libRtORB_cpp.so


SERVER_OBJS =  Server.o  $(COMMON_OBJS) $(SKEL_OBJS)
CLIENT_OBJS =  Client.o  $(COMMON_OBJS) $(STUB_OBJS)
SAMPLE_OBJS =  Sample.o  $(COMMON_OBJS) $(SKEL_OBJS) $(STUB_OBJS)

#CXXFLAGS= -g -O -I. -I$(TOP_DIR)/include 
CXXFLAGS=-g -I. -I$(TOP_DIR)/include

CFLAGS= -g -I. -I$(TOP_DIR)/include 
LIBS= -L$(TOP_DIR)/lib -lRtORB  -L$(TOP_DIR)/lib/CXX -lRtORB_cpp -lpthread 
#LIBS += -efence

CC=g++


TARGET = Server Client Sample

all: $(TARGET) 

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
	$(CC) -shared -o $@ $(LIBOBJS) 
