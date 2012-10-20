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
	   opcodes/IMul.cpp \
	   opcodes/Cmp.cpp \
	   opcodes/Sub.cpp \
	   opcodes/Push.cpp \
	   opcodes/Neg.cpp \
	   opcodes/Or.cpp \
	   opcodes/Not.cpp \
	   opcodes/Pop.cpp \
	   opcodes/Lahf.cpp \
	   opcodes/Mul.cpp \
	   opcodes/Sahf.cpp \
	   opcodes/Sxx.cpp \
	   opcodes/Lxs.cpp \
	   opcodes/Lea.cpp \
	   opcodes/Out.cpp \
	   opcodes/In.cpp
PERIPHERAL_SRC=peripherals/Screen.cpp
ALL_SRC=VM.cpp \
	main.cpp \
	Instruction.cpp \
	Processor.cpp \
	Prefix.cpp \
	ModrmOperand.cpp \
	RegisterOperand.cpp \
	AddressOperand.cpp \
	$(OPCODE_SRC) \
	$(PERIPHERAL_SRC)
ALL_OBJ=$(ALL_SRC:%.cpp=%.o)
CPPFLAGS=-Wall
LDFLAGS=
CC=g++
TARGET=libra

.PHONY: clean

debug: CPPFLAGS += -g -DDEBUG
debug: $(TARGET)

profile: CPPFLAGS += -g -pg
profile: LDFLAGS += -pg
profile: $(TARGET)

release: CPPFLAGS += -O2
release: $(TARGET)

$(TARGET): $(ALL_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJ)

clean:
	$(RM) *.o opcodes/*.o $(TARGET)
