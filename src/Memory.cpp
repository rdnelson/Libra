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
#include <cstring>
#include <iostream>

Memory::Memory(size_t size) : mMem(0), mOwnMem(true), mReadLogEnabled(true), mWriteLogEnabled(true) {
	mMem = new unsigned char[size];
	memset(mMem, 0xFF, size * sizeof(unsigned char));
	mSize = size;
}

Memory::Memory(size_t size, unsigned char* mem) : mMem(mem), mSize(size), mOwnMem(false), mReadLogEnabled(true), mWriteLogEnabled(true) {

}

Memory::~Memory() {
	if(mOwnMem) {
		delete mMem;
	}
}

void Memory::Clear() {
	if(mMem) {
		memset(mMem, 0xFF, mSize * sizeof(unsigned char));
	}
}

void Memory::notifyReadCallbacks() {

	if(mReadBuffer.size() == 0) {
		return;
	}

	/*size_t contIndex = mReadBuffer[0].first + mReadBuffer[0].second;

	for(int i = 1; i < mReadBuffer.size(); i++) {
		if(mReadBuffer[i].first == contIndex) {
			mReadBuffer[i-1].second += mReadBuffer[i].second;
			mReadBuffer.erase(mReadBuffer.begin() + i);
			contIndex = mReadBuffer[i-1].first + mReadBuffer[i-1].second;
		}
	}*/

	for(int i = 0; i < mReadCallbacks.size(); i++) {
		for(int j = 0; j < mReadBuffer.size(); j++) {
			mReadCallbacks[i](mReadBuffer[j].first, mReadBuffer[j].second);
		}
	}
	mReadBuffer.clear();
}

void Memory::notifyWriteCallbacks() {

	if(mWriteBuffer.size() == 0) {
		return;
	}

	for(int i = 0; i < mWriteCallbacks.size(); i++) {
		for(int j = 0; j < mWriteBuffer.size(); j++) {
			mWriteCallbacks[i](mWriteBuffer[j].first, mWriteBuffer[j].second);
		}
	}
	mWriteBuffer.clear();
}

Memory::MemoryOffset& Memory::MemoryOffset::operator=(const Memory::MemoryOffset& obj) {
	if(&obj == this) {
		return *this;
	}
	const_cast<Memory&>(mParent) = obj.mParent;
	mOffset = obj.mOffset;
	return *this;
}

const unsigned char& Memory::operator[](size_t idx) {

	//update index to be within bounds and offset properly
	idx = idx % mSize;

	if(mReadLogEnabled) {
		//update all callbacks about access (always 1 byte)
		mReadBuffer.push_back(MemPair(idx, 1));
	}

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

int Memory::RegisterWriteCallback(AccessCallback callback) {
	if(callback) {
		mWriteCallbacks.push_back(callback);
		return mWriteCallbacks.size() - 1;
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
	if(mParent.mReadLogEnabled)
		mParent.mReadBuffer.push_back(MemPair(mOffset, size));
}

void Memory::MemoryOffset::write(void* src, size_t size) {
	if(mOffset + size > mParent.mSize) { //need to wrap memory access
		memcpy(mParent.mMem + mOffset, src, mParent.mSize - mOffset);
		memcpy(mParent.mMem, src, size -  (mParent.mSize - mOffset));
	} else {
		memcpy(mParent.mMem + mOffset, src, size);
	}
	if(mParent.mWriteLogEnabled)
		mParent.mWriteBuffer.push_back(MemPair(mOffset, size));
}

Memory::MemoryOffset Memory::MemoryOffset::getNewOffset(size_t offset) {
	return MemoryOffset(mParent, offset);
}
