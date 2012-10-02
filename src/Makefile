OPCODE_SRC=opcodes/Add.cpp
ALL_SRC=VM.cpp \
	main.cpp \
	Instruction.cpp \
	Processor.cpp \
	Prefix.cpp \
	Operand.cpp \
	AddressOperand.cpp \
	RegisterOperand.cpp \
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
	$(RM) *.o $(TARGET)
