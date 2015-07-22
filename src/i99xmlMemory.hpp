#ifndef I99XML_MEMORY_HPP
#define I99XML_MEMORY_HPP
#pragma once


#include <cstdint>
#include <sdk/plugin.h>
#include <memory>
#include <list>

namespace i99
{
	#ifndef I99XML_HEAP_SIZE 
	#define I99XML_HEAP_SIZE (64 * 1024 * 1024)
	#endif	
	
	#define I99XML_MAXHEAP_SIZE (1 * 1024 * 1024 * 1024)

	#ifndef I99XML_BLOCK_SIZE
	#define I99XML_BLOCK_SIZE (64 * 1024)
	#endif	

	
	struct Block { 
		char* pBlock;
		size_t size; //Size in blocks.
	};
	
	class Allocator { 
	public:
		Allocator(size_t blockSize, size_t heapSize);
		~Allocator();
		
		void* alloc(size_t size);
		void free(char* pointer);
		void clear();
		
		void* getPointer(intptr_t offset);
		intptr_t getPointerOffset(void* pointer);
		
	private:
		char* findFreeBlock(size_t size);
		void allocateBlock(char* pBlock, size_t size);
		
		void relocateNextBlockPtr();
		
		
		
	private:
		std::list<Block> mAllocList;		
		
		std::unique_ptr<char[]> mpHeap;
		char* mpEnd;
		char *mpNextBlock; //The next block to allocate if the free list is empty.
		
		
		size_t mBlockSize;
		size_t mHeapSize;		
	};
	
	
	
	

	//Interface to global heap object.
	void memPoolInit();
	void memPoolCleanup();
	
	void *memPoolAlloc(size_t size);
	void memPoolFree(void *pointer);	
	void memPoolClear();	
	
	void* cellToPtr(cell c); //Convert cell to physical pointer.	
	cell ptrToCell(void *pointer); //Convert physical pointer to cell.
}

#endif


