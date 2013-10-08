/*-------------------------------------*\
|
|  File Name: CmdTree.hpp
|
|  Creation Date: 03-10-2013
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <string.h>
#include <vector>

class MemWnd;
class QAbstractSocket;

class CmdTree {

public:
	CmdTree() : mRoot('\0') {}
	typedef void (*Command)(QAbstractSocket*, std::vector<char*>);

	int AddCommand(char* cmd, Command handler);
	int DelCommand(char* cmd);

	Command GetCommand(char* cmd);
	Command operator[](char* cmd);

	int Count() { return mRoot.Count(); }

	static const int MSG_DEL_LEAF = 1;
	static const int ERR_INVALID_CMD = -1;
	static const int ERR_DUP_CMD = -2;

private:
	class Node {
	public:
		Node(char letter) : mLetter(letter), mWords(0), mCmd(0) { memset(mNodes, 0 , sizeof(mNodes)); }
		int GetWords() { return mWords; }
		Command GetCommand(char* word);
		int AddCommand(char* word, Command cmd);
		int DelCommand(char* word);
		int Count() { return mWords; }

	private:
		Node() {}
		bool isValidLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
		Command getLeafCommand();
		bool isLeaf();
		char mLetter;
		unsigned int mWords;
		Command mCmd;
		Node* mNodes[26];
	};

	Node mRoot;

};