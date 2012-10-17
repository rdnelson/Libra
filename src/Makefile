RM=rm -rf
OPCODE_SRC=opcodes/Add.cpp \
	   opcodes/Mov.cpp \
	   opcodes/Jcc.cpp \
	   opcodes/Test.cpp \
	   opcodes/Call.cpp \
	   opcodes/Ret.cpp \
	   opcodes/Aam.cpp \
	   opcodes/Xor.cpp \
	   opcodes/Aaa.cpp \
	   opcodes/CLSTX.cpp \
	   opcodes/Aad.cpp \
	   opcodes/Aas.cpp \
	   opcodes/And.cpp \
	   opcodes/Adc.cpp \
	   opcodes/Cbw.cpp \
	   opcodes/CmpsX.cpp \
	   opcodes/Cwd.cpp \
	   opcodes/IncDec.cpp \
	   opcodes/Div.cpp \
	   opcodes/Jmp.cpp \
	   opcodes/IDiv.cpp \
	   opcodes/IMul.cpp
ALL_SRC=VM.cpp \
	main.cpp \
	Instruction.cpp \
	Processor.cpp \
	Prefix.cpp \
	Operand.cpp \
	ModrmOperand.cpp \
	RegisterOperand.cpp \
	AddressOperand.cpp \
	$(OPCODE_SRC)
ALL_OBJ=$(ALL_SRC:%.cpp=%.o)
CPPFLAGS=-g -Wall
LDFLAGS=
CC=g++
TARGET=libra

.PHONY: clean

$(TARGET): $(ALL_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJ)

clean:
	$(RM) *.o opcodes/*.o $(TARGET)
