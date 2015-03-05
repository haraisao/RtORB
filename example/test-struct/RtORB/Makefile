#
# $Revision: 1.1.1.1 $
# $Date: 2008/01/08 07:56:08 $
# $Id: Makefile,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
#

TOP_DIR = ../../..
IDLC=$(TOP_DIR)/idl-compiler/rtorb-idl

IDL_FLAGS = # --skeleton-impl
IDL_TARGET = echo
IDL_SOURCE = ../$(IDL_TARGET).idl
IDL_SRCS = $(IDL_TARGET).h $(IDL_TARGET)-common.c $(IDL_TARGET)-skels.c $(IDL_TARGET)-stubs.c

S_COMMONS = $(IDL_TARGET)-common.c 
S_SKELS = $(IDL_TARGET)-skelimp.c $(IDL_TARGET)-skels.c
S_STUBS = $(IDL_TARGET)-stubs.c
S_HEADERS = $(IDL_TARGET).h

COMMON_OBJS = $(IDL_TARGET)-common.o 
SKEL_OBJS =  $(IDL_TARGET)-skels.o $(IDL_TARGET)-skelimpl.o 
STUB_OBJS =  $(IDL_TARGET)-stubs.o 

SAMPLE_OBJS = Sample.o $(COMMON_OBJS) $(SKEL_OBJS) $(STUB_OBJS)

SRV_OBJS = Server.o $(COMMON_OBJS) $(SKEL_OBJS)  
CLT_OBJS = Client.o $(COMMON_OBJS) $(STUB_OBJS) 

CFLAGS= -g  -I. -I$(TOP_DIR)/include # -DDEBUG 
#LIBS= -L$(TOP_DIR)/lib -lRtORB
LIBS= -L$(TOP_DIR)/lib -lRtORB -lpthread

TARGET = Server Client

all: $(TARGET) 

$(IDL_TARGET).h: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) $(IDL_SOURCE)

$(IDL_TARGET)-skelimpl.c: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) --skeleton-impl $(IDL_SOURCE)

Sample : $(IDL_SRCS) $(SAMPLE_OBJS)
	$(CC) -o $@ $(SAMPLE_OBJS) $(LIBS)

Server : $(IDL_SRCS) $(SRV_OBJS)
	$(CC) -o $@ $(SRV_OBJS) $(LIBS)

Client : $(IDL_SRCS) $(CLT_OBJS)
	$(CC) -o $@ $(CLT_OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe 

allclean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe $(IDL_SRCS)

$(LIBORB): $(LIBOBJS)
	$(CC) -shared -o $@ $(LIBOBJS) 
