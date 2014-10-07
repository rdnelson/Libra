QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp -fR
RM=rm -rf
MV=mv

.PHONY: all clean

all: libra.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) ..
	$(MAKE) -C build all
	$(RM) ./bin/
	$(MKDIR) bin
	$(CP) build/bin/* ./bin
ifeq (Darwin, $(shell uname -s))
	$(MV) ./bin/Docs ./bin/libra.app/Content/
endif

clean:
	$(MAKE) -C build clean
	${RM} build bin
