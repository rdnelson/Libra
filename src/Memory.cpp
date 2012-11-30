/*-------------------------------------*\
|
|  File Name: Memory.cpp
|
|  Creation Date: 30-11-2012
|
|  Last Modified: Fri Nov 3, 2012  2:06:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Memory.hpp"

Memory::Memory(size_t size) : mMem(0) {
	mMem = new unsigned char[size];
	mSize = size;
}

Memory::Memory(size_t size, unsigned char* mem, size_t offset) : mMem(mem), mSize(size), mOffset(offset) {
}

Memory::~Memory() {
	delete mMem;
}

const unsigned char& Memory::operator[](size_t idx) const {

	//update index to be within bounds and offset properly
	idx = (idx + mOffset) % mSize;

	//update all callbacks about access
	for(size_t i = 0; i < mReadCallbacks.size(); i++)
		mReadCallbacks[i](idx);

	return mMem[idx];
}

Memory Memory::operator+(size_t offset) const {
	return Memory(mSize, mMem, offset);
}

Memory& Memory::operator+=(size_t offset) {
	mOffset = (mOffset + offset) % mSize;
}

Memory& Memory::operator++() {
	mOffset = (mOffset + 1) % mSize;
	return *this;
}

Memory Memory::operator++(int) {
	Memory tmp = *this;
	mOffset = (mOffset + 1) % mSize;
	return tmp;
}

int Memory::RegisterReadCallback(AccessCallback callback) {
	if(callback) {
		mReadCallbacks.push_back(callback);
		return mReadCallbacks.size() - 1;
	}
	return -1;
}

