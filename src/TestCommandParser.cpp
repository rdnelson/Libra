#include "TestCommandParser.hpp"
#include <QAbstractSocket>
#include "MemWnd.hpp"
#include "QMemModel.hpp"

MemWnd* TestCommandParser::mWnd;

#define ADDCMD(a) mCmdTree.AddCommand(#a, _##a)
#define CMD(a) void TestCommandParser::_##a(QAbstractSocket* sock, std::vector<char*> args)
#define tolow(a) (a >= 'A' && a <= 'Z' ? (a | 0x20) : a)
#define REGCASE(a,b) (a << 8) + b

void TestCommandParser::ExecCommand(QAbstractSocket* sock, char* cmdName, std::vector<char*> args) {
	Command cmd = mCmdTree[cmdName];
	if(cmd) {
		cmd(sock, args);
	}
}
	
void TestCommandParser::Initialize() {
	ADDCMD(reg);
	ADDCMD(mem);
}

CMD(reg) {
	char resp[7];
	for(int i = 0; i < args.size(); i++) {
		if(strlen(args[0]) != 2) {
			sock->write("Invalid register\n");
			break;
		}

		int regCode = (tolow(args[0][0]) << 8) + tolow(args[0][1]); // TODO INCOMPLETE
		eRegisters reg;
		bool bit8 = false;

		switch(regCode) {
		case REGCASE('a' , 'x'):
			reg = REG_AX;
			break;
		case REGCASE('a' , 'l'):
			reg = REG_AL;
			bit8 = true;
			break;
		case REGCASE('a' , 'h'):
			reg = REG_AH;
			bit8 = true;
			break;
		case REGCASE('b' , 'x'):
			reg = REG_BX;
			break;
		case REGCASE('b' , 'l'):
			reg = REG_BL;
			bit8 = true;
			break;
		case REGCASE('b' , 'h'):
			reg = REG_BH;
			bit8 = true;
			break;
		case REGCASE('b' , 'p'):
			reg = REG_BP;
			break;
		case REGCASE('c' , 'x'):
			reg = REG_CX;
			break;
		case REGCASE('c' , 'l'):
			reg = REG_CL;
			bit8 = true;
			break;
		case REGCASE('c' , 'h'):
			reg = REG_CH;
			bit8 = true;
			break;
		case REGCASE('d' , 'x'):
			reg = REG_DX;
			break;
		case REGCASE('d' , 'l'):
			reg = REG_DL;
			bit8 = true;
			break;
		case REGCASE('d' , 'h'):
			reg = REG_DH;
			bit8 = true;
			break;
		case REGCASE('s' , 'p'):
			reg = REG_SP;
			break;
		case REGCASE('s' , 'i'):
			reg = REG_SI;
			break;
		case REGCASE('d' , 'i'):
			reg = REG_DI;
			break;
		case REGCASE('i' , 'p'):
			reg = REG_IP;
			break;
		case REGCASE('f' , 'l'):
			reg = REG_FLAGS;
			break;
		default:
			reg = NumRegisters;
		}
		if(reg != NumRegisters) {
			if(bit8)
				sprintf(resp, "0x%02X\n", mWnd->mVM.GetProc().GetRegister(reg));
			else
				sprintf(resp, "0x%04X\n", mWnd->mVM.GetProc().GetRegister(reg));
			sock->write(resp);
		} else {
			sock->write("Invalid register\n");
			break;
		}
	}
}

CMD(mem) {
	char resp[5];
	if (args.size() != 1) {
		sock->write("Invalid number of arguments.");
	}

	if(args[0][0] == '0' && args[0][1] == 'x') {
		// It's hex
		if (QMemModel::_validHexText(args[0] + 2, 4)) {
			// and it's valid
			int addr = QMemModel::_htoi(args[0] + 2);
			sprintf(resp, "0x%02X\n", mWnd->mVM.GetMemory(addr));
			sock->write(resp);
		} else {
			sock->write("Invalid address\n");
		}
	} else {
		char* ret;
		long addr = strtol(args[0], &ret, 10);
		if (addr == 0 && ret == args[0]) {
			sock->write("Invalid address\n");
		}
		sprintf(resp, "0x%02X\n", mWnd->mVM.GetMemory(addr));
		sock->write(resp);
	}
}