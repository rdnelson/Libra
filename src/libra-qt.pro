#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T17:47:20
#
#-------------------------------------------------

QT       += core gui
CONFIG   += debug static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = libra-qt
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
	peripherals/Screen.cpp \
	peripherals/Keyboard.cpp \
	QMemModel.cpp \
	QKbdFilter.cpp

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
	peripherals/Screen.hpp \
	peripherals/AllPeripherals.hpp \
	peripherals/Keyboard.hpp \
	VirgoInstruction.hpp \
	QMemModel.hpp \
	QKbdFilter.hpp

FORMS    += MemWnd.ui

RESOURCES += \
	Resources.qrc
