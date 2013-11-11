Libra - An educational 8086 emulator
====================================

[![Build Status](https://jenkins.rnelson.ca/job/Libra_cmake/badge/icon)](https://jenkins.rnelson.ca/job/Libra_cmake/)

Libra is an 8086 emulator with a focus on educational features. This
includes features such as:
* instruction list
* operand highlighting
* IDE like stepping functions
* simple to use peripherals
    * Screen
    * Keyboard
    * Timer

Building
========

Tool Dependencies
-----------------

For building just the libra binaries:
* Qt 4.8.3

For building the documentation:
* [sphinx] (http://sphinx-doc.org)

Windows
-------

Visual Studio 2010 project files are included in the msvc10 folder.

Libra.vcxproj has four different build targets, a debug and release
target for dynamic linking (Debug, Release), and a debug and release
target for static linking (Debug_Static, Release_Static.) These only
refer to whether Qt is statically or dynamically linked. The CRT is
always dynamically linked (Qt requires a dynamically linked CRT for
proper memory management.)

In order to build either of the Static builds, Qt must have been
compiled for static linking. See the [Qt wiki] (http://qt-project.org/wiki/Build_Standalone_Qt_Application_for_Windows)
for instructions on how to compile Qt in static mode.

The dynamic builds are dependent on qtgui4.dll, qtcore4.dll,
msvcr100.dll and msvcp100.dll.

The static builds are dependent on msvcr100.dll and msvcp100.dll.

Linux
-----

The following commands can be run to build Libra on linux.

	$ cd src
	$ qmake libra-qt.pro
	$ make

Mac OS X
--------

An Xcode project is available in the xcode folder.

Running
=======

Most features have to be accessed from the GUI itself, but libra
can be run with one argument, being the file to open on startup

	$ libra-qt [path_to_obj_file]
