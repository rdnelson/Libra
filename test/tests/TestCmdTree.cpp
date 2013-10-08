/*-------------------------------------*\
|
|  File Name: TestInstruction.cpp
|
|  Creation Date: 07-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  5:34:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "gtest/gtest.h"
#include "Common.hpp"

#include "CmdTree.hpp"

void _testCmd1(QAbstractSocket*, std::vector<char*>) { return; }
void _testCmd2(QAbstractSocket*, std::vector<char*>) { return; }
void _testCmd3(QAbstractSocket*, std::vector<char*>) { return; }
void _testCmd4(QAbstractSocket*, std::vector<char*>) { return; }

TEST(CmdTreeTest, AddCommand) {
	CmdTree cmds;
	CmdTree::Command testCmd1 = _testCmd1;
	CmdTree::Command testCmd2 = _testCmd2;
	CmdTree::Command testCmd3 = _testCmd3;
	CmdTree::Command testCmd4 = _testCmd4;

	ASSERT_EQ(0, cmds.AddCommand("start", testCmd1));
	ASSERT_EQ( 0,cmds.AddCommand("quit", testCmd2));
	ASSERT_EQ( 0,cmds.AddCommand("stop", testCmd3));
	ASSERT_EQ( CmdTree::ERR_INVALID_CMD,cmds.AddCommand("", testCmd4));
	ASSERT_EQ( CmdTree::ERR_DUP_CMD,cmds.AddCommand("start", testCmd2));
}

TEST(CmdTreeTest, GetCommand) {
	CmdTree cmds;
	CmdTree::Command emptyCmd = 0;
	CmdTree::Command testCmd1 = _testCmd1;
	CmdTree::Command testCmd2 = _testCmd2;
	CmdTree::Command testCmd3 = _testCmd3;
	CmdTree::Command testCmd4 = _testCmd4;

	// insert commands
	ASSERT_EQ(0, cmds.AddCommand("start", testCmd1));
	ASSERT_EQ(0, cmds.AddCommand("quit", testCmd2));
	ASSERT_EQ(0, cmds.AddCommand("stop", testCmd3));

	// test full commands
	ASSERT_EQ(testCmd1, cmds.GetCommand("start"));
	ASSERT_EQ(testCmd1, cmds.GetCommand("START"));
	ASSERT_EQ(testCmd1, cmds.GetCommand("StaRt"));
	ASSERT_EQ(testCmd2, cmds.GetCommand("quit"));
	ASSERT_EQ(testCmd2, cmds.GetCommand("QUIT"));
	ASSERT_EQ(testCmd2, cmds.GetCommand("QuIT"));
	ASSERT_EQ(testCmd3, cmds.GetCommand("stop"));
	ASSERT_EQ(emptyCmd, cmds.GetCommand(""));
	ASSERT_EQ(emptyCmd, cmds.GetCommand("invalid"));

	// test other access method
	ASSERT_EQ(testCmd1, cmds["start"]);
	ASSERT_EQ(testCmd2, cmds["quit"]);
	ASSERT_EQ(testCmd3, cmds["stop"]);
	ASSERT_EQ(emptyCmd, cmds[""]);
	ASSERT_EQ(emptyCmd, cmds["invalid"]);

	// test partial commands
	ASSERT_EQ(emptyCmd, cmds["st"]);
	ASSERT_EQ(testCmd1, cmds["sta"]);
	ASSERT_EQ(testCmd1, cmds["sTa"]);
	ASSERT_EQ(testCmd2, cmds["q"]);
	ASSERT_EQ(testCmd3, cmds["sto"]);
}

TEST(CmdTreeTest, DelCommand) {
	CmdTree cmds;
	CmdTree::Command emptyCmd = 0;
	CmdTree::Command testCmd1 = _testCmd1;
	CmdTree::Command testCmd2 = _testCmd2;
	CmdTree::Command testCmd3 = _testCmd3;
	CmdTree::Command testCmd4 = _testCmd4;

	// insert commands
	ASSERT_EQ(0, cmds.AddCommand("start", testCmd1));
	ASSERT_EQ(0, cmds.AddCommand("stop", testCmd2));

	// delete doesn't interfere with other words
	ASSERT_EQ(testCmd1, cmds["start"]);
	ASSERT_EQ(0, cmds.DelCommand("start"));
	ASSERT_EQ(emptyCmd, cmds["start"]);
	ASSERT_EQ(testCmd2, cmds["stop"]);

	// readding works
	ASSERT_EQ(0, cmds.AddCommand("start", testCmd3));
	ASSERT_EQ(testCmd3, cmds["start"]);

	ASSERT_EQ(0, cmds.DelCommand("start"));
	ASSERT_EQ(0, cmds.DelCommand("stop"));

	ASSERT_EQ(CmdTree::ERR_INVALID_CMD, cmds.DelCommand("invalid"));
	ASSERT_EQ(CmdTree::ERR_INVALID_CMD, cmds.DelCommand(""));
}

TEST(CmdTreeTest, Count) {
	CmdTree cmds;
	CmdTree::Command emptyCmd = 0;
	CmdTree::Command testCmd1 = _testCmd1;
	CmdTree::Command testCmd2 = _testCmd2;
	CmdTree::Command testCmd3 = _testCmd3;
	CmdTree::Command testCmd4 = _testCmd4;

	// insert commands
	ASSERT_EQ(0, cmds.AddCommand("start", testCmd1));
	ASSERT_EQ(1, cmds.Count());
	ASSERT_EQ(0, cmds.AddCommand("stop", testCmd2));
	ASSERT_EQ(2, cmds.Count());

	// delete reduces count
	ASSERT_EQ(0, cmds.DelCommand("start"));
	ASSERT_EQ(1, cmds.Count());

	// readding works
	ASSERT_EQ(0, cmds.AddCommand("start", testCmd3));
	ASSERT_EQ(2, cmds.Count());

	ASSERT_EQ(0, cmds.DelCommand("start"));
	ASSERT_EQ(1, cmds.Count());
	ASSERT_EQ(0, cmds.DelCommand("stop"));
	ASSERT_EQ(0, cmds.Count());
}
