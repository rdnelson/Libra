LIBDIR=lib
GTESTDIR=$(LIBDIR)/gtest-1.6.0

.PHONY: gtest all libra all_notests clean coverage

all: ./test/bin/libra_test
	./test/bin/libra_test --gtest_output=xml

coverage: all
	./test/gcovr -x -r src/ -e ".*qrc_.*" -e ".*ui_.*" > coverage_report.xml

clean:
	$(MAKE) -C $(GTESTDIR) clean
	$(MAKE) -C test/ clean
	$(MAKE) -C src/ clean

all_notests: libra

./test/bin/libra_test: libra
	$(MAKE) -C test/ all

libra:
	$(MAKE) -C src/ all

gtest: $(GTESTDIR)/config.status
	$(MAKE) -C $(GTESTDIR) all

$(GTESTDIR)/config.status:
	cd $(GTESTDIR); ./configure
