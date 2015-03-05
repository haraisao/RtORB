
CC = g++
CXX = g++

TOP_DIR = ../../..

DEFINE = -DORB_IS_RTORB
INCLUDE = -I ./ -I $(TOP_DIR)/include
LDFLAGS = -L $(TOP_DIR)/lib -lRtORB -lRtORB_cpp -lefence

CFLAGS = -g $(DEFINE) $(INCLUDE)
CXXFLAGS = $(CFLAGS)

IDL_FLAGS = --skeleton-impl -DORBIT2_IDL
IDLC = $(TOP_DIR)/idl-compiler/rtorb-idl

TEST_NAME = Struct2Test
IDL_SRC = ../$(TEST_NAME).idl

ORB_HEADERS = $(TEST_NAME).h
ORB_SRCS = $(TEST_NAME)-common.c $(TEST_NAME)-skelimpl.c $(TEST_NAME)-skels.c $(TEST_NAME)-stubs.c
ORB_OBJS = $(ORB_SRCS:.c=.o)

SOBJS = CopyTest.o $(ORB_OBJS)

TARGETS = CopyTest

.SUFFIXES : .o .cc .c

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all:
	make CopyTest

CopyTest: $(SOBJS)
	$(CXX) -o $@ $(SOBJS) $(LIBS) $(LDFLAGS)


orbsrcs:
	$(IDLC) $(IDL_FLAGS) $(IDL_SRC)

clean: 
	$(RM) *.o $(TARGETS) *~

allclean: 
	$(RM) $(ORB_HEADERS) $(ORB_SRCS) *.o $(TARGETS) *~

backup:
	for i in $(ORB_HEADERS) $(ORB_SRCS); \
	do ( cp -f $$i $$i.bak ) done
