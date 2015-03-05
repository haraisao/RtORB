#
#
#
UNAME := $(shell uname -s)

ifneq (,$(findstring CYGWIN, $(UNAME)))
	UNAME = Cygwin
endif

INCS_PKGCONFIG := $(shell pkg-config --cflags libIDL-2.0)

CFLAG_OS = -D$(UNAME)

VERSION = -DVERSION=\"0.1\"
ORBIT_BACKENDS_DIR = -DORBIT_BACKENDS_DIR=\"/usr/lib/orbot-2.0/idl-backend\"

INCS_Linux = -I/usr/include/libIDL-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I.

INCS_Darwin = -I/opt/local/include/libIDL-2.0 -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include -I.

INCS_FreeBSD = -I/usr/local/include/libIDL-2.0 -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I.

#INCS_Cygwin = -I/usr/include/libIDL-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I.
INCS_Cygwin = $(INCS_PKGCONFIG) -I.


INCS = $(INCS_$(UNAME))
CFLAGS  = -DHAVE_CONFIG_H  -DORBIT2_INTERNAL_API -DG_DISABLE_DEPRECATED
CFLAGS += $(VERSION) $(ORBIT_BACKENDS_DIR)
CFLAGS += $(CFLAG_OS)
#CFLAGS += -Wall -Wunused -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -g -Wall -Wunused -Wmissing-prototypes -Wmissing-declarations
CFLAGS += $(INCS)
#CFLAGS +=  -DUSE_POPT

LIBPATH_Darwin = -L/opt/local/lib
LIBPATH_Linux = 
LIBPATH_FreeBSD = -L/usr/local/lib
LIBPATH_Cygwin = 
LIBPATH = $(LIBPATH_$(UNAME))

GLIBS = -lgobject-2.0 -lgthread-2.0 -lglib-2.0  -lgmodule-2.0
#LIBPOPT= -lpopt
LIBPOPT= 
LIBS = $(LIBPATH) -lIDL-2 $(LIBPOPT) $(GLIBS)


OBJS = \
       orbit-idl-main.o  \
       orbit-idl-driver.o              \
       orbit-idl-backend.o             \
       orbit-idl-utils.o               \
       orbit-idl-c-backend.o           \
       orbit-idl-c-stubs.o             \
       orbit-idl-c-skels.o             \
       orbit-idl-c-headers.o           \
       orbit-idl-c-common.o            \
       orbit-idl-c-skelimpl.o          \
       orbit-idl-c-utils.o             \
       orbit-idl-c-typecode.o          \
       rtorb-idl-c-utils.o		\
       orbit-idl-cpp-headers.o          \
       rtorb-idl-c-decls.o		\
       myOptions.o

#       orbit-idl-c-imodule.o           \
#       orbit-idl-c-deps.o              \
#       rtorb-idl-c-cppif.o		\

HEADERS =

PROGRAMS = rtorb-idl

all: $(PROGRAMS)

rtorb-idl: $(OBJS) $(HEADERS)
	$(CC) -o rtorb-idl $(OBJS) $(LIBS)


clean:
	rm -f *.o *~ $(PROGRAMS)
