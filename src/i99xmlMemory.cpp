#include "i99xmlMemory.hpp"
#include "logprintf.hpp"
#include <new>
#include <list>
#include <algorithm>
#include <cstring>
#include <cstddef>
#include <stdexcept>

namespace i99{
	
	//GLOBALS
	static Allocator* gp_allocator = nullptr;
	
	//FUNCTIONS
	template <typename T>
	bool isPow2(T val) {
		int counter = 0;
		while(counter <= 1 && val != 0) { 
			if(val & 1 == 1) 
				counter++;
			val  = val >> 1;
		}
		
		return counter >= 1;
	}
	
	//Return the exponent n where 2^n = po2.
	//Result is undefined if po2 is not a power of two.
	int getBase2Exponent(int po2)
	{
		if(po2 == 0 || po2 == 1)
			return 0;
			
		int counter = 0;
		while(po2 > 1) {
			po2 = po2 >> 1;
			counter++;
		}
		
		return counter;
	}

	
	/******************/
	/*   ALLOCATOR    */
	/******************/
	
	Allocator::Allocator(size_t blockSize, size_t heapSize)
		: mpNextBlock(nullptr), mBlockSize(0), mHeapSize(0)
	{ 
		//Allocate memory for the heap
		mpHeap.reset( new char[heapSize] );
		mpEnd = mpHeap.get() + heapSize;
		
		//Set the next block pointer to the beginning.
		mpNextBlock = mpHeap.get();
	}
	
	Allocator::~Allocator() {}
	
	void* Allocator::alloc(size_t size) { 
	
		//Find a free block of memory.
		char* pBlock = findFreeBlock(size);
		if(!pBlock)
			throw std::bad_alloc();
		
		//Allocate the block
		allocateBlock(pBlock, size);
		
		return (void*)pBlock;
	}
	
	char* Allocator::findFreeBlock(size_t size) { 
		if(mpNextBlock + size > mpEnd)
			return nullptr;
			
		return mpNextBlock;
	}
	
	void Allocator::allocateBlock(char* block, size_t size) { 
	
		//Add to the allocated list.
		Block allocBlock;
		allocBlock.pBlock = block;
		allocBlock.size = size;
		mAllocList.push_back(allocBlock);
		
		//Sort the allocated list.
		mAllocList.sort( [&](Block b1, Block b2) {
			return b1.pBlock < b2.pBlock;
		});
		
		//Advance the next block pointer.
		relocateNextBlockPtr();
	}
	
	void Allocator::relocateNextBlockPtr() {
		if(mAllocList.empty())
			mpNextBlock = mpHeap.get();
		else
			mpNextBlock = mAllocList.back().pBlock + mAllocList.back().size;
	}
	
	void Allocator::free(char* pointer) {
	
		//Remove from the alloc list.
		mAllocList.remove_if( [=](Block const& block) { 
			return block.pBlock == pointer;
		});
		
		relocateNextBlockPtr();
	
	}
	
	void Allocator::clear() {
		mAllocList.clear();
		relocateNextBlockPtr();
	}
	
	void* Allocator::getPointer(intptr_t offset) { 
		if(offset < 0) 
			return nullptr;
			
		return (void*)(mpHeap.get() + offset);		
	}
	
	intptr_t Allocator::getPointerOffset(void* pointer) {
		if(!pointer)
			return -1;
			
		return (char*)pointer - mpHeap.get();		
	}
	

	//FUNCTION DEFINITIONS
	void memPoolInit()	{		
		gp_allocator = new Allocator(I99XML_BLOCK_SIZE, I99XML_HEAP_SIZE);
	}
	
	void memPoolCleanup() {
		if(gp_allocator)
			gp_allocator->clear();
			
		delete gp_allocator;
	}

	void *memPoolAlloc(std::size_t size) {		
		void *ptr = (void*)gp_allocator->alloc(size);
		if(!ptr)
			throw std::bad_alloc();
		
		return ptr;
	}

	void memPoolFree(void *pointer) {
		gp_allocator->free((char*)pointer);
	}
	

	void memPoolClear() {
		gp_allocator->clear();
	}

	//Convert cell to physical pointer.
	//Makes the address relative to the memory pool. Suitable to be held in a 32 bit cell.
	void* cellToPtr(cell c) 		{
		return gp_allocator->getPointer(c);
	}
	
	//Convert physical pointer to cell.
	cell ptrToCell(void *pointer) 	{
		return gp_allocator->getPointerOffset(pointer);
	}		
}
