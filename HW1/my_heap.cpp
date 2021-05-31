#include <iostream>
using namespace std;
#include "my_heap.h"

//constructor, initializes the private data mebers
My_heap::My_heap (){
	heap_begin = nullptr;
	blk = nullptr;
	used_bytes = 0;
}

//add memory block to the end of My_heap	
memory_block* My_heap::bump_allocate(int num_bytes){

	memory_block *temp;
	if(heap_begin == nullptr)   //if linked list is empty
	{
		temp = new memory_block(nullptr, nullptr, true, num_bytes, 0);
		heap_begin = temp;
		blk = temp;                             //heap_begin = blk = temp
		used_bytes += num_bytes;               //because we only have one memory block in My_heap
		return temp;
	}
	

	if((used_bytes + num_bytes) > MAX_CAPACITY)     //if allocating the additional bytes exceeds the heap capacity
		return nullptr;

	temp = new memory_block(nullptr, blk, true, num_bytes, used_bytes);//+1
	blk->right = temp;
	blk = temp;	              // the new node is added to the end of the Linked list so it is pointed to by blk
	used_bytes += num_bytes;
	return temp;
}

//frees the memory block, taken by the function as parameter, from the heap
//joins adjacent free blocks
void My_heap::deallocate(memory_block *to_delete)
{

	if (heap_begin == nullptr)
		return;

	to_delete->used = false;            //memory block freed
	used_bytes -= to_delete->size;
	memory_block *leftBlock = to_delete->left;          //leftBlock points to previous memory block of the node we want to delete
	memory_block *rightBlock = to_delete->right;       //rightBlock points to next memory block of the node we want to delete   
	memory_block *temp;

	//various checks are implemented to locate and join adjacent memory blocks
	//if found, the nodes will be deleted and replaced by new memory block temp
	if (rightBlock == nullptr && leftBlock == nullptr)
		return;                                                   //to_delete is the only memory block

	else if (rightBlock == nullptr && leftBlock->used == false)   //to_delete is the last memory block
	{
		temp = new memory_block(nullptr, leftBlock->left, false, leftBlock->size + to_delete ->size, leftBlock->starting_address);
		leftBlock->left->right = temp;
		
		delete leftBlock;
		delete to_delete;
		leftBlock = nullptr;
		to_delete = nullptr;
		blk = temp;
		
	}


	else if (leftBlock == nullptr && rightBlock->used == false)         //to_delete is the first memory block
	{
		temp = new memory_block(rightBlock->right, nullptr, false, rightBlock->size + to_delete ->size, to_delete->starting_address);
		rightBlock->right->left = temp;
		
		delete rightBlock;
		delete to_delete;
		leftBlock = nullptr;
		to_delete = nullptr;	
		heap_begin = temp;
	}		
	
	
	else if(leftBlock != nullptr && leftBlock->used == false)
	{
		
		temp = new memory_block(to_delete->right, leftBlock->left, false, leftBlock->size + to_delete ->size, leftBlock->starting_address);
		leftBlock->left->right = temp;
		to_delete->right->left = temp;
		
		delete leftBlock;
		delete to_delete;
		leftBlock = nullptr;
		to_delete = nullptr;
		
	}
	else if(rightBlock != nullptr && rightBlock->used == false)
	{
		temp = new memory_block(rightBlock->right, to_delete->left, false, rightBlock->size + to_delete ->size, to_delete->starting_address);
		to_delete->left->right = temp;
		rightBlock->right->left = temp;
		
		delete rightBlock;
		delete to_delete;
		rightBlock = nullptr;
		to_delete = nullptr;
	}

}

//allocates bytes to the first free memory block of suitable size
//if no such block is found, allocate bytes by adding new memory block to the end of heap
memory_block *My_heap::first_fit_allocate(int num_bytes)
{
	if (heap_begin == nullptr)
		return nullptr;

	memory_block *temp;
	temp = heap_begin;
	//traversing the list from start to end so all nodes are checked
	while(temp != nullptr)   
	{
		if (temp->used != true && temp->size >= num_bytes)     
		{
			temp->used = true;               //status of the found memory block is changed from free to used
			used_bytes += temp->size;       //used_bytes incremented to contain bytes of the memory block
			return temp;
		}
		temp = temp->right;
	}


	return bump_allocate(num_bytes);   //bump allocate called if suitable memory block for allocation cannot be located in heap
}


//allocates bytes to the smallest free memory block which can accomodate num_bytes
//if no such block is found, allocate bytes by adding new memory block to the end of heap
memory_block *My_heap::best_fit_allocate(int num_bytes)
{
	if (heap_begin == nullptr)
		return nullptr;

	memory_block *temp;
	temp = heap_begin;
	int comparison = MAX_CAPACITY;
	memory_block *minBlock = nullptr;

	while (temp != nullptr)
	{
		if(temp->used == false)
		{
			if (comparison >= temp->size && temp->size >= num_bytes)
			{
				comparison = temp->size;     //if the memory block temp fits the criteria and is smaller than previous suitable blocks
				minBlock = temp;             //minblock will point to temp
			}
		}

		temp = temp->right;
	}

	if (minBlock != nullptr)         //if a suitable memory block is found
	{
		minBlock->used = true;
		used_bytes += minBlock->size;
		return minBlock;
	}

	return bump_allocate(num_bytes);     //bump allocate called if suitable memory block for allocation cannot be located in heap
}


//allocates bytes to the first free memory block which can accomodate num_bytes
//split the block to free unused memory
//if no such block is found, allocate bytes by adding new memory block to the end of heap
memory_block *My_heap::first_fit_split_allocate(int num_bytes)
{
	if (heap_begin == nullptr)
		return nullptr;

	memory_block *temp;
	temp = heap_begin;
	memory_block *usedBlock = nullptr;
	memory_block *notUsedBlock = nullptr;


	while(temp != nullptr)
	{
		if (temp->used == false && temp->size >= num_bytes)
		{
			if (temp->size == num_bytes)   //we do not split block if its size equals num of bytes we want too allocate
			{
				temp->used = true;
				return temp;
			}
			else
			{
				if (temp == heap_begin)  //if suitable block for allocation is the first block in heap
				{
					usedBlock = new memory_block(nullptr, nullptr, true, num_bytes, temp->starting_address);
					notUsedBlock = new memory_block(temp->right, usedBlock, false, temp->size - num_bytes, temp->starting_address + num_bytes);
					usedBlock->right = notUsedBlock;
					temp->right->left = notUsedBlock;
					used_bytes += num_bytes;
					delete temp;
					heap_begin = usedBlock;
					return usedBlock;

				}
				else if (temp == blk)   ////if suitable block for allocation is the last block in heap
				{
					usedBlock = new memory_block(nullptr, temp->left, true, num_bytes, temp->starting_address);
					notUsedBlock = new memory_block(nullptr, usedBlock, false, temp->size - num_bytes, temp->starting_address + num_bytes);
					usedBlock->right = notUsedBlock;
					temp->left->right = usedBlock;
					used_bytes += num_bytes;
					delete temp;
					blk = notUsedBlock;
					return usedBlock;
				}
				else
				{
					usedBlock = new memory_block(nullptr, temp->left, true, num_bytes, temp->starting_address);
					notUsedBlock = new memory_block(temp->right, usedBlock, false, temp->size-num_bytes, temp->starting_address + num_bytes);
					usedBlock->right = notUsedBlock;
					temp->left->right = usedBlock;
					temp->right->left = notUsedBlock;
					used_bytes += num_bytes;
					delete temp;                         //original block is split into two new blocks and then deleted
					temp = nullptr;
					return usedBlock;
				}
				
			}
		}
		temp = temp->right;
	}
	return bump_allocate(num_bytes);    //bump allocate called if suitable memory block for allocation cannot be located in heap
}

//calculate and return fragmantation %
float My_heap::get_fragmantation()
{
	int free_memory, biggest_free_block, used_memory, total_size, comparison = 0, notUsedMemory = 0;
	float fragmantation = 0;
	int result;

	memory_block *temp = heap_begin;
	memory_block *maxBlock;


	while (temp != nullptr)
	{
		if (temp->used == false)
		{
			if (temp->size > comparison)             //to locate the largest free block in heap
			{
				comparison = temp->size;
				maxBlock = temp;
				
			}
			notUsedMemory += temp->size;
		}

		temp = temp->right;
	}
	
	free_memory = MAX_CAPACITY - used_bytes;
//	total_size = blk->starting_address + blk->size;

	//if-else statement determines whether a block within the linked list is largest free block or whether the free block after the last node is largest
	if (comparison > ( MAX_CAPACITY - used_bytes))
	{
		biggest_free_block = maxBlock->size;
	}
	else
	{
		biggest_free_block = MAX_CAPACITY - (used_bytes + notUsedMemory);//total_size;
	}
	result = (free_memory - biggest_free_block) ;
	fragmantation = (float)result /(float) (free_memory) ;  //typecasting int to float 
	return fragmantation*100;
}


//prints status of heap
void My_heap::print_heap()
{
	int numNodes = 0, usedBlock = 0, notUsedBlock = 0, usedMemory = 0;
	memory_block *temp = heap_begin;
	while (temp != nullptr)
	{
		if (temp->used == true)
		{
			usedBlock++;
			usedMemory += temp->size;
		}

		else
		{
			notUsedBlock++;
		}

		numNodes++;
		temp = temp->right;
	}

	//used_bytes = usedMemory;	
	cout<<"Maximum capacity of heap: "<<dec<<MAX_CAPACITY<<"B"<<endl;
	cout<<"Currently used memory (B): "<<dec<<used_bytes<<endl;
	cout<<"Total memory blocks: "<<dec<<numNodes<<endl;
	cout<<"Total used memory blocks: "<<dec<<usedBlock<<endl;
	cout<<"Total free memory blocks: "<<dec<<notUsedBlock<<endl;
	cout<<"Fragmentation: "<<get_fragmantation()<<"%"<<endl<<"------------------------------"<<endl;

	memory_block *ptr = heap_begin;
	int count = 0;
	while (ptr != nullptr)
	{	
		
		cout<<"Block "<<dec<<count<<"		"<<"Used: ";
		ptr->used == true ? cout<<"True" : cout<<"False";
		cout<<"	"<<"Size (B): "<<dec<<ptr->size<<"	"<<"Starting Address: 0x"; 
		cout<<hex<<ptr->starting_address<<endl;    //cout<<hex used to obtain hexadecimal equivelant of function
		count++;
		ptr = ptr->right;			
	}    
	cout<<"------------------------------"<<endl<<"------------------------------"<<endl;
}

//destructor
My_heap::~My_heap()
{
	memory_block *temp = heap_begin;
	cout<<"At destruction, the heap had a memory leak of "<<dec<<used_bytes<<" bytes"<<endl;
	while( temp != nullptr)
	{
		heap_begin = heap_begin->right; 
		delete temp;                        //deallocate(temp)
		temp = heap_begin;
	}
}




