QMAKE=qmake
MKDIR=mkdir -p
CP=cp
RM=rm -rf
QPROJ=libra-qt.pro

.PHONY: release all debug clean

all: debug release

full:
	$(MAKE) dbg
	$(MAKE) rel

dbg:
	$(MAKE) debug
	$(MAKE) debug32

rel:
	$(MAKE) release
	$(MAKE) release32

debug:
	$(QMAKE) $(QPROJ) CONFIG+=dbg
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) librad ../bin/

release: | debug
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) libra ../bin/
debug32:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ 32bit
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) librad32 ../bin/

release32: | debug32
	$(QMAKE) $(QPROJ) CONFIG+=32bit
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) libra32 ../bin/

clean:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ 32bit
	$(MAKE) -f ./Makefile clean
	$(QMAKE) $(QPROJ) CONFIG+=32bit
	$(MAKE) -f ./Makefile clean
	$(QMAKE) $(QPROJ) CONFIG+=dbg
	$(MAKE) -f ./Makefile clean
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile clean
	$(RM) ./Makefile
	$(RM) ../bin
