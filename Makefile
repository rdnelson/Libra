LIBDIR=lib
GTESTDIR=$(LIBDIR)/gtest-1.6.0

.PHONY: gtest

gtest: $(GTESTDIR)/config.status
	$(MAKE) -C $(GTESTDIR) all

$(GTESTDIR)/config.status:
	cd $(GTESTDIR); ./configure
