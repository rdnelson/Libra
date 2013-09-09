LIBDIR=lib
GTESTDIR=$(LIBDIR)/gtest-1.6.0

.PHONY: gtest all libra all_notests clean

all: ./test/bin/libra_test
	./test/bin/libra_test --gtest_output=xml

clean:
	$(MAKE) -C $(GTESTDIR) clean
	$(MAKE) -C test/ clean
	$(MAKE) -C src/ clean

all_notests: libra

./test/bin/libra_test: gtest libra
	$(MAKE) -C test/ all

libra:
	$(MAKE) -C src/ all

gtest: $(GTESTDIR)/config.status
	$(MAKE) -C $(GTESTDIR) all

$(GTESTDIR)/config.status:
	cd $(GTESTDIR); ./configure

deb:
	$(MAKE) -C redist/ all
