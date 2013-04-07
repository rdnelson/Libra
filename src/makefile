QMAKE=qmake
MKDIR=mkdir -p
CP=cp
RM=rm -rf
QPROJ=libra-qt.pro

.PHONY: release all debug clean

all: debug release

debug:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ debug
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) libra-debug ../bin/

release: | debug
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile
	$(MKDIR) ../bin
	$(CP) libra ../bin/

clean:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ debug
	$(MAKE) -f ./Makefile clean
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile clean
	$(RM) ./Makefile
	$(RM) ../bin
