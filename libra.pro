QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
	error("Use qmake from Qt v4.4 or higher.")
}

TEMPLATE = subdirs
SUBDIRS = src doc
CONFIG += debug_and_release
