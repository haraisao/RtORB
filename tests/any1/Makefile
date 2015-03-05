RM = rm -f
CC = g++
LD = g++

TOP_DIR = ../..

IDL_FLAGS = --skeleton-impl -DORBIT2_IDL
IDLC = $(TOP_DIR)/idl-compiler/rtorb-idl

IDL_NAME = AnyTest
IDL_SRC = $(IDL_NAME).idl

IDL_FLAGS = --skeleton-impl -DORBIT2_IDL
IDLC = $(TOP_DIR)/idl-compiler/rtorb-idl

TEST_NAME = $(IDL_NAME)

ORB_HEADERS = $(TEST_NAME).h
ORB_SRCS = $(TEST_NAME)-common.c $(TEST_NAME)-skelimpl.c $(TEST_NAME)-skels.c $(TEST_NAME)-stubs.c

CXXFLAGS=-g -I$(TOP_DIR)/include -I.

LIBS= -L$(TOP_DIR)/lib -lRtORB  -L$(TOP_DIR)/lib/CXX -lRtORB_cpp -lpthread 

.SUFFIXES :  .o .cc .c 

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<


#SRCS = char.cc octet.cc string.cc boolean.cc
SRCS = any-test.cc $(ORB_SRCS)

OBJS = any-test.o $(ORB_SRCS:.c=.o)

TARGETS = anytest


all: anytest

anytest: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

orbsrcs:
	$(IDLC) $(IDL_FLAGS) $(IDL_SRC)

clean:
	$(RM) $(TARGETS) *.o

