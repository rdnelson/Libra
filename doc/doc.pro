OBJDIR = ../obj/doc
Release:DESTDIR = ../bin
Debug:DESTDIR = ../bin_debug

SOURCE = conf.py
SPHINXFILES = conf.py

sphinx.input = SPHINXFILES
sphinx.commands = sphinx-build -b html -d $$OBJDIR/doctrees . $$DESTDIR/Docs
sphinx.name = Sphinx Documentation Compiler
sphinx.depends = FORCE
sphinx.output = $$DESTDIR/Docs/index.html

QMAKE_EXTRA_COMPILERS += sphinx
