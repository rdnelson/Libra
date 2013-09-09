#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T17:47:20
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qt thread
OBJECTS_DIR = obj/release
TARGET = libra

CONFIG(dbg) {
	message(Debug Mode)
	OBJECTS_DIR = obj/debug
	TARGET = librad
}

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
	Resources.qrc
