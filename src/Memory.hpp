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

typedef void (*AccessCallback)(size_t offset, size_t size);

class Memory {

	public:
		Memory(size_t size);
		Memory(size_t size, unsigned char* mem);
		~Memory();

		const unsigned char& operator[](size_t idx) const;

		int RegisterReadCallback(AccessCallback callback);
		unsigned char* getPtr() const { return mMem; }
		size_t getSize() const { return mSize; }

		class MemoryOffset {
			public:
				MemoryOffset(const MemoryOffset& obj) : mParent(obj.mParent), mOffset(obj.mOffset) {}
				Memory::MemoryOffset& operator=(const Memory::MemoryOffset& obj);
				unsigned char operator[](size_t idx) const { return mParent.mMem[(mOffset+idx) % mParent.mSize]; }
				friend class Memory;

				void read(void* dst, size_t size);
				void write(void* src, size_t size);

				MemoryOffset operator+(size_t) const;
				MemoryOffset& operator+=(size_t);
				unsigned char operator*() const { return *(mParent.mMem + mOffset); }
				MemoryOffset& operator++();
				MemoryOffset operator++(int);
				unsigned char* operator()() { return mParent.mMem + mOffset; }

				MemoryOffset getNewOffset(size_t offset);

				size_t getOffset() const { return mOffset; }

			protected:
				MemoryOffset(const Memory& parent, size_t offset)
					: mParent(parent), mOffset(offset) {}


				const Memory& mParent;
				size_t mOffset;
		};

		MemoryOffset getOffset(size_t offset) const { return MemoryOffset(*this, offset); }

	private:
		unsigned char*	mMem;
		size_t	mSize;
		bool	mOwnMem;
		std::vector<AccessCallback> mReadCallbacks;
};
