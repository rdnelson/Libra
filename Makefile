LIBDIR=lib
GTESTDIR=$(LIBDIR)/gtest-1.6.0

.PHONY: all gtest

all: tests

tests: gtest libra
	$(MAKE) -C tests/ all

libra:
	$(MAKE) -C src/ all

gtest: $(GTESTDIR)/config.status
	$(MAKE) -C $(GTESTDIR) all

$(GTESTDIR)/config.status:
	cd $(GTESTDIR); ./configure
