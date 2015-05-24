#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T17:47:20
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qt thread

CONFIG(release, debug|release) {
	OBJDIR = ../obj/release
	DESTDIR = ../bin
	TARGET = libra
}

CONFIG(debug, debug|release) {
	OBJDIR = ../obj/debug
	DESTDIR = ../bin_debug
	TARGET = librad
}



OBJECTS_DIR = $$OBJDIR/obj
MOC_DIR = $$OBJDIR/moc
RCC_DIR = $$OBJDIR/rcc
UI_DIR = $$OBJDIR/ui

INCLUDEPATH += $$OBJDIR

CONFIG(32bit) {
    message(32bit version)
    OBJECTS_DIR = $$JOIN(OBJECTS_DIR,,,32)
    QMAKE_CXXFLAGS += -m32
    QMAKE_LFLAGS += -m32
    LIBS += -L/usr/lib/i386-linux-gnu -L/lib/i386-linux-gnu
    TARGET = $$JOIN(TARGET,,,32)
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES +=\
        MemWnd.cpp \
	VM.cpp \
	RegisterOperand.cpp \
	Processor.cpp \
	Prefix.cpp \
	ModrmOperand.cpp \
	main.cpp \
	Instruction.cpp \
	AddressOperand.cpp \
	VMWorker.cpp \
	Breakpoint.cpp \
	Memory.cpp \
	opcodes/Xor.cpp \
	opcodes/Test.cpp \
	opcodes/Sxx.cpp \
	opcodes/Sub.cpp \
	opcodes/Sahf.cpp \
	opcodes/Ret.cpp \
	opcodes/Push.cpp \
	opcodes/Pop.cpp \
	opcodes/Out.cpp \
	opcodes/Or.cpp \
	opcodes/Not.cpp \
	opcodes/Neg.cpp \
	opcodes/Mul.cpp \
	opcodes/Mov.cpp \
	opcodes/Lxs.cpp \
	opcodes/Lea.cpp \
	opcodes/Lahf.cpp \
	opcodes/Jmp.cpp \
	opcodes/Jcc.cpp \
	opcodes/IncDec.cpp \
	opcodes/In.cpp \
	opcodes/IMul.cpp \
	opcodes/IDiv.cpp \
	opcodes/Div.cpp \
	opcodes/Cwd.cpp \
	opcodes/CmpsX.cpp \
	opcodes/Cmp.cpp \
	opcodes/CLSTX.cpp \
	opcodes/Cbw.cpp \
	opcodes/Call.cpp \
	opcodes/And.cpp \
	opcodes/Add.cpp \
	opcodes/Adc.cpp \
	opcodes/Aas.cpp \
	opcodes/Aam.cpp \
	opcodes/Aad.cpp \
	opcodes/Aaa.cpp \
	opcodes/Rot.cpp \
	opcodes/Int.cpp \
	opcodes/Iret.cpp \
	opcodes/Hlt.cpp \
	opcodes/StiCli.cpp \
	opcodes/Nop.cpp \
	opcodes/Xlat.cpp \
	opcodes/Loop.cpp \
	opcodes/Stos.cpp \
	opcodes/Scas.cpp \
	opcodes/Sbb.cpp \
	opcodes/Lods.cpp \
	opcodes/Movs.cpp \
	opcodes/Xchg.cpp \
	opcodes/Daa.cpp \
	opcodes/Das.cpp \
	peripherals/Screen.cpp \
	peripherals/Keyboard.cpp \
	peripherals/Timer.cpp \
	QMemModel.cpp \
	QKbdFilter.cpp \
	QInstructionList.cpp \
	InstTable.cpp

HEADERS  += MemWnd.hpp \
	VM.hpp \
	RegisterOperand.hpp \
	Register.hpp \
	Processor.hpp \
	Prefix.hpp \
	IPeripheral.hpp \
	Operand.hpp \
	ModrmOperand.hpp \
	Instruction.hpp \
	ImmediateOperand.hpp \
	AddressOperand.hpp \
	Breakpoint.hpp \
	VMWorker.hpp \
	Memory.hpp \
	opcodes/Xor.hpp \
	opcodes/Test.hpp \
	opcodes/Sxx.hpp \
	opcodes/Sub.hpp \
	opcodes/Sahf.hpp \
	opcodes/Ret.hpp \
	opcodes/Push.hpp \
	opcodes/Pop.hpp \
	opcodes/Out.hpp \
	opcodes/Or.hpp \
	opcodes/Not.hpp \
	opcodes/Neg.hpp \
	opcodes/Mul.hpp \
	opcodes/Mov.hpp \
	opcodes/Lxs.hpp \
	opcodes/Lea.hpp \
	opcodes/Lahf.hpp \
	opcodes/Jmp.hpp \
	opcodes/Jcc.hpp \
	opcodes/IncDec.hpp \
	opcodes/In.hpp \
	opcodes/IMul.hpp \
	opcodes/IDiv.hpp \
	opcodes/Div.hpp \
	opcodes/Cwd.hpp \
	opcodes/CmpsX.hpp \
	opcodes/Cmp.hpp \
	opcodes/CLSTX.hpp \
	opcodes/Cbw.hpp \
	opcodes/Call.hpp \
	opcodes/And.hpp \
	opcodes/AllOpcodes.hpp \
	opcodes/Add.hpp \
	opcodes/Adc.hpp \
	opcodes/Aas.hpp \
	opcodes/Aam.hpp \
	opcodes/Aad.hpp \
	opcodes/Aaa.hpp \
	opcodes/Rot.hpp \
	opcodes/Int.hpp \
	opcodes/Iret.hpp \
	opcodes/Hlt.hpp \
	opcodes/StiCli.hpp \
	opcodes/Nop.hpp \
	opcodes/Xlat.hpp \
	opcodes/Loop.hpp \
	opcodes/Stos.hpp \
	opcodes/Scas.hpp \
	opcodes/Sbb.hpp \
	opcodes/Lods.hpp \
	opcodes/Movs.hpp \
	opcodes/Xchg.hpp \
	opcodes/Daa.hpp \
	opcodes/Das.hpp \
	peripherals/Screen.hpp \
	peripherals/AllPeripherals.hpp \
	peripherals/Keyboard.hpp \
	peripherals/Timer.hpp \
	VirgoInstruction.hpp \
	QMemModel.hpp \
	QKbdFilter.hpp \
	QInstructionList.hpp

FORMS    += MemWnd.ui

RESOURCES += \
	../res/Resources.qrc

VERSIONFILE += Version.in


version_c.output = $$OBJDIR\\${QMAKE_FILE_BASE}.h
version_c.input = VERSIONFILE
win32-msvc* {
    version_c.commands = powershell -Command \"cat ${QMAKE_FILE_IN} | %%{$_ -replace \\\"@GIT_VERSION@\\\", ((git describe --tags --match v*) + \\\"-\\\" + (git rev-parse --abbrev-ref HEAD))} | %%{$_ -replace \\\"@HOST_NAME@\\\", (hostname)} > ${QMAKE_FILE_OUT}\"
} else {
    version_c.commands = sed -e "s/@GIT_VERSION@/$$system(git describe --tags --match v* 2>/dev/null)-$$system(git rev-parse --abbrev-ref HEAD 2>/dev/null)/" ${QMAKE_FILE_IN} | sed -e "s/\\-/$$system(cat ../VERSION 2>/dev/null)/" | sed -e "s/\\\"\\\"/\\\"Unknown\\\"/" | sed -e "s/@HOST_NAME@/$$QMAKE_HOST.name/" > ${QMAKE_FILE_OUT}
}
version_c.variable_out = GENERATED_FILES
version_c.name = Version Compiler
version_c.depends = FORCE

QMAKE_EXTRA_COMPILERS += version_c

win32 {
    debug:EXTRA_BINFILES += $$QMAKE_LIBDIR_QT/../bin/QtCored4.dll \
                            $$QMAKE_LIBDIR_QT/../bin/QtGuid4.dll \
                            $$QMAKE_LIBDIR_QT/../plugins/imageformats/qicod4.dll \
                            $$(WINDIR)/System32/msvcp100d.dll \
                            $$(WINDIR)/System32/msvcr100d.dll
    release:EXTRA_BINFILES += $$QMAKE_LIBDIR_QT/../bin/QtCore4.dll \
                              $$QMAKE_LIBDIR_QT/../bin/QtGui4.dll \
                              $$QMAKE_LIBDIR_QT/../plugins/imageformats/qico4.dll \
                              $$(WINDIR)/System32/msvcp100.dll \
                              $$(WINDIR)/System32/msvcr100.dll
    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
    EXTRA_BINFILES_WIN ~= s,/,\\,g
    DESTDIR_WIN = $$DESTDIR
    DESTDIR_WIN ~= s,/,\\,g
    for(FILE,EXTRA_BINFILES_WIN) {
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DESTDIR_WIN) $$escape_expand(\\n\\t)
    }
    QMAKE_POST_LINK += $$QMAKE_CHK_DIR_EXISTS $$quote($$DESTDIR_WIN\\imageformats) $$QMAKE_MKDIR $$quote($$DESTDIR_WIN\\imageformats) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MOVE $$quote($$DESTDIR_WIN\\qico*) $$quote($$DESTDIR_WIN\\imageformats) $$escape_expand(\\n\\t)

    RC_FILE += ../res/msvc10_resources.rc

    HEADERS += Version.in
}

macx {
    ICON = ../res/Libra.icns

    QMAKE_POST_LINK += $$QMAKE_MKDIR $$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats $$escape_expand(\\n\\t)
    debug:QMAKE_POST_LINK += $$QMAKE_COPY $$QMAKE_LIBDIR_QT/../plugins/imageformats/libqico.dylib $$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats $$escape_expand(\\n\\t)
    release:QMAKE_POST_LINK += $$QMAKE_COPY $$QMAKE_LIBDIR_QT/../plugins/imageformats/libqico.dylib $$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += macdeployqt $$DESTDIR/$${TARGET}.app -executable=$$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats/libqico.dylib $$escape_expand(\\n\\t)

    QMAKE_POST_LINK2 += install_name_tool -change @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats/libqico.dylib $$escape_expand(\\n\\t)
    QMAKE_POST_LINK2 += install_name_tool -change @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $$DESTDIR/$${TARGET}.app/Contents/PlugIns/imageformats/libqico.dylib $$escape_expand(\\n\\t)

}

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += target

    target.path = $$BINDIR
}
