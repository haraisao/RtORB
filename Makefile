#
#
#


UNAME := $(shell uname -s)

MAKE_OPT=""

ifneq (,$(findstring CYGWIN, $(UNAME)))
	UNAME = Cygwin
endif


all: config_rtorb.h link idl_compiler CosName-srcs libs NameService linkbin

link:
	@for i in include lib; \
	do \
		if [ ! -L $$i/CosName ]; then \
			echo \<$$i\> Create Symbolic link to \'CosName\'; \
			cd $$i; ln -s ../CosName ./; cd ..;\
		fi \
	done

idl_compiler:
	cd idl-compiler && $(MAKE)

libs:
	cd lib && $(MAKE) "DEFINE=$(DEFINE)"

CosName-srcs:
	cd CosName && $(MAKE) 

NameService:
	cd CosName && $(MAKE) NameService

# create configuration file 
config_rtorb.h: 
	tools/gen_config $(UNAME) $(MAKE_OPT)

linkbin:
	if [ ! -d bin ]; then \
		mkdir bin; \
	fi;\
	if cd bin; then \
		if [ ! -L rtorb-idl ]; then \
			ln -s ../idl-compiler/rtorb-idl ./; \
		fi; \
		if [ ! -L NameService ]; then \
			ln -s ../CosName/NameService ./; \
		fi \
	fi

distclean:
	/bin/rm -f *~
	cd idl-compiler && $(MAKE) clean
	cd lib && $(MAKE) allclean
	rm include/RtORB/config_rtorb.h

clean:
	/bin/rm -f *~
	cd idl-compiler && $(MAKE) clean
	cd lib && $(MAKE) allclean
	rm include/RtORB/config_rtorb.h
