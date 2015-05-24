QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp -fR
RM=rm -rf
MV=mv

.PHONY: target debug release all clean

release: TARGET=release
release: target
	$(RM) ./bin/
	$(MKDIR) bin
	$(CP) build/bin/* ./bin
ifeq (Darwin, $(shell uname -s))
	$(MV) ./bin/Docs ./bin/libra.app/Contents/Docs
endif
	

debug: TARGET=debug
debug: target
	$(RM) ./bin_debug/
	$(MKDIR) bin_debug
	$(CP) build/bin_debug/* ./bin_debug
ifeq (Darwin, $(shell uname -s))
	$(MV) ./bin_debug/Docs ./bin_debug/librad.app/Contents/Docs
endif

all: TARGET=debug_and_release
all: target
	$(RM) ./bin/
	$(RM) ./bin_debug/
	$(MKDIR) bin
	$(MKDIR) bin_debug
	$(CP) build/bin/* ./bin
	$(CP) build/bin_debug/* ./bin_debug
ifeq (Darwin, $(shell uname -s))
	$(MV) ./bin/Docs ./bin/libra.app/Contents/Docs
	$(MV) ./bin_debug/Docs ./bin_debug/librad.app/Contents/Docs
endif

target: libra.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) .. CONFIG+=$(TARGET)
	$(MAKE) -C build all

clean:
	$(MAKE) -C build clean
	${RM} build bin
