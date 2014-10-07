QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp -fR
RM=rm -rf

.PHONY: all clean

all: libra.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) ..
	$(MAKE) -C build all
	$(MKDIR) bin
	$(CP) build/bin/* ./bin

clean:
	$(MAKE) -C build clean
	${RM} build bin
