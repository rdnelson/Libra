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

Memory::~Memory() {
	delete mMem;
}

Memory::MemoryOffset& Memory::MemoryOffset::operator=(const Memory::MemoryOffset& obj) {
	if(&obj == this) {
		return *this;
	}
	const_cast<Memory&>(mParent) = obj.mParent;
	mOffset = obj.mOffset;
	return *this;
}

const unsigned char& Memory::operator[](size_t idx) const {

	//update index to be within bounds and offset properly
	idx = idx % mSize;

	//update all callbacks about access
	for(size_t i = 0; i < mReadCallbacks.size(); i++)
		mReadCallbacks[i](idx);

	return mMem[idx];
}

Memory::MemoryOffset Memory::MemoryOffset::operator+(size_t offset) const {
	return MemoryOffset(mParent, (mOffset + offset) % mParent.mSize);
}

Memory::MemoryOffset& Memory::MemoryOffset::operator+=(size_t offset) {
	mOffset = (mOffset + offset) % mParent.mSize;
	return *this;
}

Memory::MemoryOffset& Memory::MemoryOffset::operator++() {
	mOffset = (mOffset + 1) % mParent.mSize;
	return *this;
}

Memory::MemoryOffset Memory::MemoryOffset::operator++(int) {
	MemoryOffset tmp = *this;
	mOffset = (mOffset + 1) % mParent.mSize;
	return tmp;
}

int Memory::RegisterReadCallback(AccessCallback callback) {
	if(callback) {
		mReadCallbacks.push_back(callback);
		return mReadCallbacks.size() - 1;
	}
	return -1;
}

void Memory::MemoryOffset::read(void* dst, size_t size) {
	if(mOffset + size > mParent.mSize) { //need to wrap memory access
		memcpy(dst, mParent.mMem + mOffset, mParent.mSize - mOffset);
		memcpy(dst, mParent.mMem, size -  (mParent.mSize - mOffset));
	} else {
		memcpy(dst, mParent.mMem + mOffset, size);
	}
}

void Memory::MemoryOffset::write(void* src, size_t size) {
	if(mOffset + size > mParent.mSize) { //need to wrap memory access
		memcpy(mParent.mMem + mOffset, src, mParent.mSize - mOffset);
		memcpy(mParent.mMem, src, size -  (mParent.mSize - mOffset));
	} else {
		memcpy(mParent.mMem + mOffset, src, size);

	}
}

Memory::MemoryOffset Memory::MemoryOffset::getNewOffset(size_t offset) {
	return MemoryOffset(mParent, offset);
}
