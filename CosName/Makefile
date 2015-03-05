#
# $Revision: 1.4 $
# $Date: 2008/04/21 08:07:24 $
# $Id: Makefile,v 1.4 2008/04/21 08:07:24 yoshi Exp $
#

UNAME := $(shell uname -s)
ifneq (,$(findstring CYGWIN, $(UNAME)))
	UNAME = Cygwin
endif

#CC= gcc
#TOP_DIR=/usr/local/src/RtORB
TOP_DIR=..
IDLC=$(TOP_DIR)/idl-compiler/rtorb-idl
#IDL_FLAGS= --nocpp-defs #--skeleton-impl
IDL_FLAGS= # --nocpp-defs #
IDL_SRC = CosNaming
IDL_SOURCE = $(TOP_DIR)/idl/$(IDL_SRC).idl
ECHO_SRCS = $(IDL_SRC).h $(IDL_SRC)-common.c $(IDL_SRC)-skels.c $(IDL_SRC)-stubs.c
S_SKELS = $(IDL_SRC)-common.c $(IDL_SRC)-skels.c
S_STUBS = $(IDL_SRC)-common.c $(IDL_SRC)-stubs.c
S_HEADERS = $(IDL_SRC).h
COMMON_OBJS = $(IDL_SRC)-common.o 
SKELIMPL_OBJ = $(IDL_SRC)-skelimpl.o 
SKEL_OBJS = $(IDL_SRC)-skels.o $(SKELIMPL_OBJ)
STUB_OBJS = $(IDL_SRC)-stubs.o


#SERVER_OBJS  =  NameService.o $(SKELIMPL_OBJ)
SERVER_OBJS  =  NameService.o # NameService_impl.o
#SERVER_OBJS += $(COMMON_OBJS) $(SKEL_OBJS)
CLIENT_OBJS  =  Client.o #$(COMMON_OBJS) $(STUB_OBJS)
SAMPLE_OBJS =  Sample.o $(COMMON_OBJS) $(SKEL_OBJS) $(STUB_OBJS)

OBJS_DECODE=  decode-ior.o 

NAMING_OBJ = CosNaming-common.o CosNaming-skels.o CosNaming-stubs.o CosNaming-skelimpl.o
NAMING_LIB = libRtORB_CosNaming.so

CFLAGS = -g -Wall -I. -I$(TOP_DIR)/include #-DDEBUG # -DCYGWIN
CFLAGS = -g -Wall -I. -I$(TOP_DIR)/include -D$(UNAME)
CFLAGS += -DUSE_THREAD
LIBS= -lpthread -L$(TOP_DIR)/lib -lRtORB -luuid #-lRtORB_CosNaming


TARGET = $(IDL_SRC).h  #NameService

all: $(TARGET) $(SKELIMPL_OBJ:.o=.c) 

$(IDL_SRC).h: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) $(IDL_SOURCE)

$(IDL_SRC)-skelimpl.c: $(IDL_SOURCE)
	$(IDLC) $(IDL_FLAGS) --skeleton-impl  $(IDL_SOURCE)

$(NAMING_LIB): $(NAMING_OBJ)
	$(CC) -shared -o $@  $(NAMING_OBJ) 

NameService: $(S_HEADERS) $(S_SKELS) $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LIBS)

Client: $(S_HEADERS) $(S_SKELS) $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LIBS)

Sample: $(S_HEADERS) $(S_SKELS) $(SAMPLE_OBJS)
	$(CC) -o $@ $(SAMPLE_OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe 


allclean:
	rm -f $(TARGET) *.o *~ $(TARGET).exe $(ECHO_SRCS) NameService

decode-ior: $(OBJS_DECODE)
	$(CC) -o $@ $(OBJS_DECODE) $(LIBS)

backup:
	for i in $(ECHO_SRCS); \
	do ( cp -f $$i $$i.bak; ) done

$(LIBORB): $(LIBOBJS)
	$(CC) -shared -o $@ $(LIBOBJS) 
