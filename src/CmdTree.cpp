/*-------------------------------------*\
|
|  File Name: CmdTree.hpp
|
|  Creation Date: 03-10-2013
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "CmdTree.hpp"

// Convert letter to index (DOES NOT WORK FOR NON ALPHABETIC CHARS)
#define toind(a) (((a & 0x1F) - 1) % 26)
#define tolow(a) ((a >= 'A' && a <= 'Z') ? (a | 0x20) : a)

int CmdTree::AddCommand(char* cmd, Command handler) {
	return mRoot.AddCommand(cmd, handler);
}

int CmdTree::DelCommand(char* cmd) {
	return mRoot.DelCommand(cmd);
}

CmdTree::Command CmdTree::GetCommand(char* cmd) {
	return mRoot.GetCommand(cmd);
}

CmdTree::Command CmdTree::operator[](char* cmd) {
	return this->GetCommand(cmd);
}

CmdTree::Command CmdTree::Node::GetCommand(char* word) {
	// word should never be empty
	if (word[0] == '\0') {
		return 0;
	}

	// is this the root node?
	if (mLetter != '\0') {

		// should always be true, better safe though
		if(tolow(word[0]) == mLetter) {
			if (word[1] == '\0') {
				if (mCmd != 0)
					return mCmd;

				return mWords == 1 ? getLeafCommand() : 0;
			}
		} else {
			// should never be hit.
			return 0;
		}

		word++;
	} 

	// check the children.
	if (isValidLetter(word[0])) {
		return mNodes[toind(word[0])] == 0 ? 0 : mNodes[toind(word[0])]->GetCommand(word);
	}

	return 0;
}

CmdTree::Command CmdTree::Node::getLeafCommand() {
	// only called when there's a single path to follow
	for(int i = 0; i < 26; i++) {
		if (mNodes[i]) {
			return mNodes[i]->getLeafCommand();
		}
	}

	// only the leaf has no children
	return mCmd;
}

int CmdTree::Node::AddCommand(char* word, Command cmd) {
	// empty word or command
	if (mLetter == '\0' && word[0] == '\0' || cmd == 0) {
		return CmdTree::ERR_INVALID_CMD;
	}

	// this is the end of the word
	if (tolow(word[0]) == mLetter && word[1] == '\0') {

		// word already registered
		if (mCmd != 0) {
			return CmdTree::ERR_DUP_CMD;
		}

		// register command
		mWords++;
		mCmd = cmd;
		return 0;
	}

	if (mLetter != '\0') {
		word++;
	}

	// recurse into the tree
	if (isValidLetter(word[0])) {
		int index = toind(word[0]);

		// create a new node if necessary
		if(mNodes[index] == 0) {
			mNodes[index] = new CmdTree::Node(index + 'a');
		}

		// find out if the addition succeeded
		int retVal = mNodes[index]->AddCommand(word, cmd);

		// only increase word count if the word was added
		if(retVal == 0) {
			mWords++;
			return 0;
		}

		return retVal;
	}

	return CmdTree::ERR_INVALID_CMD;
}

int CmdTree::Node::DelCommand(char* word) {
	// can't delete empty string
	if (mLetter == '\0' && word[0] == '\0') {
		return CmdTree::ERR_INVALID_CMD;
	}

	// this is the owner
	if (mLetter == tolow(word[0]) && word[1] == '\0') {
		mWords--;
		mCmd = 0;

		// if it's a leaf, tell the owner to delete us.
		return isLeaf() ? CmdTree::MSG_DEL_LEAF : 0;
	}

	if (mLetter != '\0') {
		word++;
	}

	int index = toind(word[0]);
	if (isValidLetter(word[0]) && mNodes[index] != 0) {
		int retVal = mNodes[index]->DelCommand(word);
		if (retVal == CmdTree::MSG_DEL_LEAF) {
			delete mNodes[index];
			mNodes[index] = 0;
			mWords--;

			return isLeaf() ? CmdTree::MSG_DEL_LEAF : 0;
		} else if (retVal == 0) {
			mWords--;
			return 0;
		}
	}

	return CmdTree::ERR_INVALID_CMD;
}

bool CmdTree::Node::isLeaf() {
	// the root isn't a leaf even if it has no children
	if (mLetter == '\0')
		return false;

	// it's not a leaf if there are children
	for(int i = 0; i < 26; i++) {
		if (mNodes[i] != 0) {
			return false;
		}
	}
	return true;
}