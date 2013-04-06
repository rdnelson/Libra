LIBDIR=lib
GTESTDIR=$(LIBDIR)/gtest-1.6.0

.PHONY: gtest all libra all_notests clean

all: test_detail.xml

clean:
	$(MAKE) -C $(GTESTDIR) clean
	$(MAKE) -C test/ clean
	$(MAKE) -C src/ clean

all_notests: libra

./test/bin/libra_test: gtest libra
	$(MAKE) -C test/ all

test_detail.xml: ./test/bin/libra_test
	./test/bin/libra_test --gtest_output=xml

libra:
	$(MAKE) -C src/ all

gtest: $(GTESTDIR)/config.status
	$(MAKE) -C $(GTESTDIR) all

$(GTESTDIR)/config.status:
	cd $(GTESTDIR); ./configure
