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
typedef std::pair<size_t, size_t> MemPair;

class Memory {

	public:
		Memory(size_t size);
		Memory(size_t size, unsigned char* mem);
		~Memory();

		const unsigned char& operator[](size_t idx);

		int RegisterReadCallback(AccessCallback callback);
		unsigned char* getPtr() const { return mMem; }
		size_t getSize() const { return mSize; }

		void DumpReadMemAccess();

		class MemoryOffset {
			public:
				MemoryOffset(const MemoryOffset& obj) : mParent(obj.mParent), mOffset(obj.mOffset), mEnabled(obj.mEnabled)  {}
				Memory::MemoryOffset& operator=(const Memory::MemoryOffset& obj);
				unsigned char operator[](size_t idx) {
					if(mEnabled) {
						mParent->mReadBuffer.push_back(MemPair(idx + mOffset, 1));
					}
					return mParent->mMem[(mOffset+idx) % mParent->mSize];
				}
				friend class Memory;

				void read(void* dst, size_t size);
				void write(void* src, size_t size);

				MemoryOffset operator+(size_t) const;
				MemoryOffset& operator+=(size_t);
				unsigned char operator*() {
					if(mEnabled) {
						mParent->mReadBuffer.push_back(MemPair(mOffset, 1));
					}
					return *(mParent->mMem + mOffset);
				}
				MemoryOffset& operator++();
				MemoryOffset operator++(int);
				unsigned char* operator()() { return mParent->mMem + mOffset; }

				void DisableCallbacks() { mEnabled = false; }
				void EnableCallbacks() { mEnabled = true; }

				MemoryOffset getNewOffset(size_t offset);

				size_t getOffset() const { return mOffset; }

			protected:
				MemoryOffset(Memory* parent, size_t offset, bool enabled)
					: mParent(parent), mOffset(offset), mEnabled(enabled) {}


				Memory *mParent;
				size_t mOffset;
				bool mEnabled;
		};

		MemoryOffset getOffset(size_t offset) const { return MemoryOffset(const_cast<Memory*>(this), offset, true); }

	private:
		unsigned char*	mMem;
		size_t	mSize;
		bool	mOwnMem;

		//First item is address, second is access size
		std::vector<MemPair> mReadBuffer;
		std::vector<AccessCallback> mReadCallbacks;
};
