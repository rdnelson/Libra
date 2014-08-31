#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T17:47:20
#
#-------------------------------------------------

system(touch src/Version.h)
QT       += core gui
CONFIG   += qt thread debug_and_release
TARGET = libra
INCLUDEPATH += src
DESTDIR = bin

Release:OBJECTS_DIR = build/release/obj
Release:MOC_DIR = build/release/moc
Release:RCC_DIR = build/release/rcc
Release:UI_DIR = build/release/ui

Debug:OBJECTS_DIR = build/debug/obj
Debug:MOC_DIR = build/debug/moc
Debug:RCC_DIR = build/debug/rcc
Debug:UI_DIR = build/debug/ui
Debug:TARGET = librad

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
        src/MemWnd.cpp \
	src/VM.cpp \
	src/RegisterOperand.cpp \
	src/Processor.cpp \
	src/Prefix.cpp \
	src/ModrmOperand.cpp \
	src/main.cpp \
	src/Instruction.cpp \
	src/AddressOperand.cpp \
	src/VMWorker.cpp \
	src/Breakpoint.cpp \
	src/Memory.cpp \
	src/opcodes/Xor.cpp \
	src/opcodes/Test.cpp \
	src/opcodes/Sxx.cpp \
	src/opcodes/Sub.cpp \
	src/opcodes/Sahf.cpp \
	src/opcodes/Ret.cpp \
	src/opcodes/Push.cpp \
	src/opcodes/Pop.cpp \
	src/opcodes/Out.cpp \
	src/opcodes/Or.cpp \
	src/opcodes/Not.cpp \
	src/opcodes/Neg.cpp \
	src/opcodes/Mul.cpp \
	src/opcodes/Mov.cpp \
	src/opcodes/Lxs.cpp \
	src/opcodes/Lea.cpp \
	src/opcodes/Lahf.cpp \
	src/opcodes/Jmp.cpp \
	src/opcodes/Jcc.cpp \
	src/opcodes/IncDec.cpp \
	src/opcodes/In.cpp \
	src/opcodes/IMul.cpp \
	src/opcodes/IDiv.cpp \
	src/opcodes/Div.cpp \
	src/opcodes/Cwd.cpp \
	src/opcodes/CmpsX.cpp \
	src/opcodes/Cmp.cpp \
	src/opcodes/CLSTX.cpp \
	src/opcodes/Cbw.cpp \
	src/opcodes/Call.cpp \
	src/opcodes/And.cpp \
	src/opcodes/Add.cpp \
	src/opcodes/Adc.cpp \
	src/opcodes/Aas.cpp \
	src/opcodes/Aam.cpp \
	src/opcodes/Aad.cpp \
	src/opcodes/Aaa.cpp \
	src/opcodes/Rot.cpp \
	src/opcodes/Int.cpp \
	src/opcodes/Iret.cpp \
	src/opcodes/Hlt.cpp \
	src/opcodes/StiCli.cpp \
	src/opcodes/Nop.cpp \
	src/opcodes/Xlat.cpp \
	src/opcodes/Loop.cpp \
	src/opcodes/Stos.cpp \
	src/opcodes/Scas.cpp \
	src/opcodes/Sbb.cpp \
	src/opcodes/Lods.cpp \
	src/opcodes/Movs.cpp \
	src/opcodes/Xchg.cpp \
	src/opcodes/Daa.cpp \
	src/opcodes/Das.cpp \
	src/peripherals/Screen.cpp \
	src/peripherals/Keyboard.cpp \
	src/peripherals/Timer.cpp \
	src/QMemModel.cpp \
	src/QKbdFilter.cpp \
	src/QInstructionList.cpp \
	src/InstTable.cpp

HEADERS  += src/MemWnd.hpp \
	src/VM.hpp \
	src/RegisterOperand.hpp \
	src/Register.hpp \
	src/Processor.hpp \
	src/Prefix.hpp \
	src/IPeripheral.hpp \
	src/Operand.hpp \
	src/ModrmOperand.hpp \
	src/Instruction.hpp \
	src/ImmediateOperand.hpp \
	src/AddressOperand.hpp \
	src/Breakpoint.hpp \
	src/VMWorker.hpp \
	src/Memory.hpp \
	src/opcodes/Xor.hpp \
	src/opcodes/Test.hpp \
	src/opcodes/Sxx.hpp \
	src/opcodes/Sub.hpp \
	src/opcodes/Sahf.hpp \
	src/opcodes/Ret.hpp \
	src/opcodes/Push.hpp \
	src/opcodes/Pop.hpp \
	src/opcodes/Out.hpp \
	src/opcodes/Or.hpp \
	src/opcodes/Not.hpp \
	src/opcodes/Neg.hpp \
	src/opcodes/Mul.hpp \
	src/opcodes/Mov.hpp \
	src/opcodes/Lxs.hpp \
	src/opcodes/Lea.hpp \
	src/opcodes/Lahf.hpp \
	src/opcodes/Jmp.hpp \
	src/opcodes/Jcc.hpp \
	src/opcodes/IncDec.hpp \
	src/opcodes/In.hpp \
	src/opcodes/IMul.hpp \
	src/opcodes/IDiv.hpp \
	src/opcodes/Div.hpp \
	src/opcodes/Cwd.hpp \
	src/opcodes/CmpsX.hpp \
	src/opcodes/Cmp.hpp \
	src/opcodes/CLSTX.hpp \
	src/opcodes/Cbw.hpp \
	src/opcodes/Call.hpp \
	src/opcodes/And.hpp \
	src/opcodes/AllOpcodes.hpp \
	src/opcodes/Add.hpp \
	src/opcodes/Adc.hpp \
	src/opcodes/Aas.hpp \
	src/opcodes/Aam.hpp \
	src/opcodes/Aad.hpp \
	src/opcodes/Aaa.hpp \
	src/opcodes/Rot.hpp \
	src/opcodes/Int.hpp \
	src/opcodes/Iret.hpp \
	src/opcodes/Hlt.hpp \
	src/opcodes/StiCli.hpp \
	src/opcodes/Nop.hpp \
	src/opcodes/Xlat.hpp \
	src/opcodes/Loop.hpp \
	src/opcodes/Stos.hpp \
	src/opcodes/Scas.hpp \
	src/opcodes/Sbb.hpp \
	src/opcodes/Lods.hpp \
	src/opcodes/Movs.hpp \
	src/opcodes/Xchg.hpp \
	src/opcodes/Daa.hpp \
	src/opcodes/Das.hpp \
	src/peripherals/Screen.hpp \
	src/peripherals/AllPeripherals.hpp \
	src/peripherals/Keyboard.hpp \
	src/peripherals/Timer.hpp \
	src/VirgoInstruction.hpp \
	src/QMemModel.hpp \
	src/QKbdFilter.hpp \
	src/QInstructionList.hpp \
	src/Version.h

FORMS    += src/MemWnd.ui

RESOURCES += \
	res/Resources.qrc

versionTarget.target = src/Version.h
versionTarget.depends = FORCE
versionTarget.commands = sed -e "s/@GIT_VERSION@/$$system(git describe --tags --match v*)-$$system(git rev-parse --abbrev-ref HEAD)/" src/Version.h.in | sed -e "s/@HOST_NAME@/$$QMAKE_HOST.name/" > $$versionTarget.target
PRE_TARGETDEPS += src/Version.h
QMAKE_EXTRA_TARGETS += versionTarget
DEPENDPATH = src
