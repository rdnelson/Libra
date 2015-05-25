OBJDIR = ../obj/doc

CONFIG(debug, debug|release) {
	DESTDIR = ../bin_debug
}
CONFIG(release, debug|release) {
	DESTDIR = ../bin
}

CONFIG += console
SPHINXFILES = timer.rst \
		index.rst \
		changelog.rst \
		intro.rst \
		screen.rst \
		peripherals.rst \
		keyboard.rst \
		using.rst

macx {
	CONFIG -= app_bundle
}



sphinx.input = SPHINXFILES
sphinx.CONFIG += combine
win32|macx {
	sphinx.commands = sphinx-build -b html -d $$OBJDIR/doctrees $$PWD $$DESTDIR/Docs ${QMAKE_FILE_NAME}
	sphinx.output = $$DESTDIR/Docs/index.html
} else {
	sphinx.commands = sphinx-build -b man -d $$OBJDIR/doctrees $$PWD $$DESTDIR/man ${QMAKE_FILE_NAME}
	sphinx.output = $$DESTDIR/man/Libra.1
	sphinx.variable_out = OBJECTS

	TARGET = man/libra.1

	QMAKE_POST_LINK += $(COPY_FILE) $^ $@

	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	BINDIR = $$PREFIX/share/man

	target.files = $$DESTDIR/man/*
	target.path = $$BINDIR

	INSTALLS += target
}
sphinx.name = Sphinx Documentation Compiler

QMAKE_EXTRA_COMPILERS += sphinx
QMAKE_CC = echo
QMAKE_CXX = echo
QMAKE_LINK = echo
