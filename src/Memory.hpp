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
#include <iostream>

typedef void (*AccessCallback)(size_t offset, size_t size);
typedef std::pair<size_t, size_t> MemPair;

class Memory {

	public:
		Memory(size_t size);
		Memory(size_t size, unsigned char* mem);
		~Memory();

		const unsigned char& operator[](size_t idx);

		int RegisterReadCallback(AccessCallback callback);
		int RegisterWriteCallback(AccessCallback callback);
		unsigned char* getPtr() const { return mMem; }
		size_t getSize() const { return mSize; }
		void notifyReadCallbacks();
		void notifyWriteCallbacks();
		void clear() { memset(mMem, 0xFF, mSize); }

		class MemoryOffset {
			public:
				MemoryOffset(const MemoryOffset& obj) : mParent(obj.mParent), mOffset(obj.mOffset), mEnabled(true)  {}
				Memory::MemoryOffset& operator=(const Memory::MemoryOffset& obj);
				unsigned char operator[](size_t idx) {
					if(mParent.mReadLogEnabled) {
						mParent.mReadBuffer.push_back(MemPair(idx, 1));
						std::cout << "Array operator used" << std::endl;
					}
					return mParent.mMem[(mOffset+idx) % mParent.mSize];
				}
				friend class Memory;

				void read(void* dst, size_t size);
				void write(void* src, size_t size);

				MemoryOffset operator+(size_t) const;
				MemoryOffset& operator+=(size_t);
				unsigned char operator*() {
					if(mParent.mReadLogEnabled) {
						mParent.mReadBuffer.push_back(MemPair(mOffset, 1));
						std::cout << "Indirection operator used" << std::endl;
					}
					return *(mParent.mMem + mOffset);
				}
				MemoryOffset& operator++();
				MemoryOffset operator++(int);
				unsigned char* operator()() { return mParent.mMem + mOffset; }

				//void DisableCallbacks() { mEnabled = false; }
				//void EnableCallbacks() { mEnabled = true; }

				void DisableMemReadLog() { mParent.mReadLogEnabled = false; }
				void EnableMemReadLog() { mParent.mReadLogEnabled = true; }
				bool IsMemReadLogEnabled() { return mParent.mReadLogEnabled; }

				void DisableMemWriteLog() { mParent.mWriteLogEnabled = false; }
				void EnableMemWriteLog() { mParent.mWriteLogEnabled = true; }
				bool IsMemWriteLogEnabled() { return mParent.mWriteLogEnabled; }

				MemoryOffset getNewOffset(size_t offset);

				size_t getOffset() const { return mOffset; }

			protected:
				MemoryOffset(Memory& parent, size_t offset)
					: mParent(parent), mOffset(offset) {}


				Memory& mParent;
				size_t mOffset;
				bool mEnabled;
		};
		MemoryOffset getOffset(size_t offset) { return MemoryOffset(*this, offset); }

	private:
		unsigned char*	mMem;
		size_t	mSize;
		bool	mOwnMem;
		bool	mReadLogEnabled;
		bool	mWriteLogEnabled;

		//First item is address, second is access size
		std::vector<MemPair> mReadBuffer;
		std::vector<AccessCallback> mReadCallbacks;
		std::vector<MemPair> mWriteBuffer;
		std::vector<AccessCallback> mWriteCallbacks;
};
