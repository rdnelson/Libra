QMAKE=qmake
QPROJ=libra-qt.pro

.PHONY: release all debug clean

all: debug release

debug:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ debug
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile

release: | debug
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile
	$(RM) ./Makefile

clean:
	$(QMAKE) $(QPROJ) CONFIG+=dbg\ debug
	$(MAKE) -f ./Makefile clean
	$(QMAKE) $(QPROJ)
	$(MAKE) -f ./Makefile clean
	$(RM) ./Makefile
