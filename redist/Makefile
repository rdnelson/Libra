SRCDIR=../src/
BINDIR=../bin/
BINFILE_AMD64=$(BINDIR)/libra
BINFILE_I386=$(BINDIR)/libra32
DOCDIR=../doc/
MANDIR=_build/man/
MANFILE=libra.1
MANBIN=$(DOCDIR)$(MANDIR)$(MANFILE)
TARGETDIR=debian/usr/bin/
TARGETMANDIR=debian/usr/share/man/man1/
TARGET_AMD64=libra-1.0.0_amd64.deb
TARGET_I386=libra-1.0.0_i386.deb
AMD64DIR=amd64/
I386DIR=i386/
MKDIR=mkdir -p
CP=cp
MV=mv
GZIP=gzip --best -f

.PHONY: all deb clean cleandoc cleanbin

all: deb

deb: $(TARGET_I386) $(TARGET_AMD64)

$(TARGET_AMD64): $(BINFILE_AMD64) $(MANFILE)
	$(MKDIR) $(AMD64DIR)$(TARGETDIR)
	$(CP) $(BINFILE_AMD64) $(AMD64DIR)$(TARGETDIR)
	$(MKDIR) $(AMD64DIR)$(TARGETMANDIR)
	$(CP) $(MANBIN) $(AMD64DIR)$(TARGETMANDIR)
	$(GZIP) $(AMD64DIR)$(TARGETMANDIR)$(MANFILE)
	dpkg-deb --build $(AMD64DIR)debian $(TARGET_AMD64)

$(TARGET_I386): $(BINFILE_I386) $(MANFILE)
	$(MKDIR) $(I386DIR)$(TARGETDIR)
	$(CP) $(BINFILE_I386) $(I386DIR)$(TARGETDIR)
	$(MKDIR) $(I386DIR)$(TARGETMANDIR)
	$(CP) $(MANBIN) $(I386DIR)$(TARGETMANDIR)
	$(GZIP) $(I386DIR)$(TARGETMANDIR)$(MANFILE)
	dpkg-deb --build $(I386DIR)debian $(TARGET_I386)

$(BINFILE_AMD64):
	$(MAKE) -C $(SRCDIR) release

$(BINFILE_I386):
	$(MAKE) -C $(SRCDIR) release32

clean: cleandoc cleanbin

cleanbin:
	$(MAKE) -C $(SRCDIR) clean

cleandoc:
	$(MAKE) -C $(DOCDIR) clean

$(MANFILE): cleandoc
	$(MAKE) -C $(DOCDIR) man
