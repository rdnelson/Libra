/*-------------------------------------*\
|
|  File Name: Memory.hpp
|
|  Creation Date: 30-11-2012
|
|  Last Modified: Fri Nov 3, 2012  2:06:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>
#include <cstring>

typedef void (*AccessCallback)(size_t);

class Memory {

	public:
		Memory(size_t size);
		Memory(size_t size, unsigned char* mem, size_t offset);
		~Memory();

		const unsigned char& operator[](size_t idx) const;
		Memory operator+(size_t) const;
		Memory& operator+=(size_t);
		unsigned char operator*() const { return mMem[mOffset % mSize]; }
		Memory& operator++();
		Memory operator++(int);

		int RegisterReadCallback(AccessCallback callback);
		unsigned char* getPtr() { return mMem; }
		unsigned char* getHead() { return mMem + (mOffset % mSize); }
		size_t getSize() const { return mSize; }
		size_t getOffset() const { return mOffset; }

	private:
		unsigned char*	mMem;
		size_t	mSize;
		size_t	mOffset;
		std::vector<AccessCallback> mReadCallbacks;
};
