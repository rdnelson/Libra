#pragma once

#include <vector>
#include "CmdTree.hpp"

#define CMD(a) static void _##a(QAbstractSocket* sock, std::vector<char*> args)

class MemWnd;
class QAbstractSocket;

class TestCommandParser {
public:
	TestCommandParser(MemWnd* wnd) { mWnd = wnd; Initialize(); } 

	void ExecCommand(QAbstractSocket* sock, char* cmd, std::vector<char*> args);
	void Initialize();

private:
	CMD(reg);
	CMD(mem);

	static MemWnd* mWnd;
	typedef void (*Command)(QAbstractSocket*, std::vector<char*>);
	CmdTree mCmdTree;
};

#undef CMD