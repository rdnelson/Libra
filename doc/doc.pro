OBJDIR = ../obj/doc
DESTDIR = ../bin

CONFIG += console
SOURCE = conf.py
SPHINXFILES = conf.py

macx {
	CONFIG -= app_bundle
}



sphinx.input = SPHINXFILES
unix {
	sphinx.commands = sphinx-build -b man -d $$OBJDIR/doctrees $$PWD $$DESTDIR/man
	sphinx.output = $$DESTDIR/man/Libra.1
} else {
	sphinx.commands = sphinx-build -b html -d $$OBJDIR/doctrees $$PWD $$DESTDIR/Docs
	sphinx.output = $$DESTDIR/Docs/index.html
}
sphinx.name = Sphinx Documentation Compiler
sphinx.depends = FORCE

QMAKE_EXTRA_COMPILERS += sphinx
QMAKE_CC = echo
QMAKE_CXX = echo
QMAKE_LINK = echo
