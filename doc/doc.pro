OBJDIR = ../obj/doc
DESTDIR = ../bin

CONFIG += console
SOURCE = conf.py
SPHINXFILES = conf.py

macx {
	CONFIG -= app_bundle
}



sphinx.input = SPHINXFILES
sphinx.commands = sphinx-build -b html -d $$OBJDIR/doctrees $$PWD $$DESTDIR/Docs
sphinx.name = Sphinx Documentation Compiler
sphinx.depends = FORCE
sphinx.output = $$DESTDIR/Docs/index.html

QMAKE_EXTRA_COMPILERS += sphinx
QMAKE_CC = echo
QMAKE_CXX = echo
QMAKE_LINK = echo
