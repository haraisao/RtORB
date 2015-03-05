#
#  Makefile for AnyTest with omniorb
#
CFLAG_OS = -DLinux 

CFLAG_DEBUG = -g -Wall #-DDEBUG
#CFLAG_DEBUG += -DDEBUG_MALLOC 
#CFLAGS += -DUSE_THREAD

TEST_NAME = AnyTest

TOP_DIR = ../../..

IDL_NAME = AnyTest
IDL_SRC = ../$(IDL_NAME).idl

IDL_FLAGS = --skeleton-impl -DORBIT2_IDL
IDLC = $(TOP_DIR)/idl-compiler/rtorb-idl

ORB_HEADERS = $(TEST_NAME).h
ORB_SRCS = $(TEST_NAME)-common.c $(TEST_NAME)-skelimpl.c $(TEST_NAME)-skels.c $(TEST_NAME)-stubs.c

ORB_OBJS = $(ORB_SRCS:.c=.o)

SOBJS  = Server.o $(ORB_OBJS)
COBJS  = Client.o $(ORB_OBJS)

DEFINE = -DORB_IS_RTORB

#CXXFLAGS = -g -I. $(CFLAG_DEBUG) $(CFLAG_OS)
INCLUDE = -I ./ -I $(TOP_DIR)/include
LDFLAGS = -L $(TOP_DIR)/lib -lRtORB -lRtORB_cpp -lefence
CFLAGS = -g $(DEFINE) $(INCLUDE)
CXXFLAGS = $(CFLAGS)

CXX=g++
CC = g++

TARGETS = Server Client

.SUFFIXES : .o .cc .c

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<


all: $(TARGETS)

#$(IDL_NAME).hh: 
#	$(IDL_CC) -bcxx $(IDL_SRC)

#$(IDL_NAME).cc:
#	$(IDL_CC) -bcxx $(IDL_SRC)

Server: $(ORB_SRC) $(SOBJS)
	$(CC) -o $@ $(SOBJS)  $(LDFLAGS)

Client: $(ORB_SRC) $(COBJS)
	$(CC) -o $@ $(COBJS)  $(LDFLAGS)


orbsrcs:
	$(IDLC) $(IDL_FLAGS) $(IDL_SRC)

link:
	@for i in $(TARGETS:=.cc); \
	do \
		if ! [ -L $$i ]; then \
			echo Creating Symbolik link $$i; \
			ln -s ../omni/$$i ./; \
		fi \
	done

clean:
	rm -f $(TARGETS) *.o *~

allclean:
	rm -f $(TARGETS) *.o *~  $(ORB_SRC)

.PHONY: proxy

proxy:
	$(IDL_CC) $(IDL_FLAGS) $(IDL_SRC)

