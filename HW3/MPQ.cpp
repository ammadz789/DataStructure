#include <iostream>
#include <vector>
#include "MPQ.h"


//constructor
MPQ::MPQ(int size)
{	
	currentSize = 0;
	heapStruct temp;
	temp.label = 0;
	temp.value = 0;
	for (int i=0; i < size; i++)
	{
		heap.push_back(temp);          //initialize heap and locations vector; set size to total number of buildings 
		locations.push_back(0);
	}
}

//destructor
MPQ::~MPQ()
{
}

//Internal method to percolate down in the heap.
//hole is the index at which the percolate begins.
void MPQ::percolateDown(int hole)
{
	int child;
	heapStruct temp = heap[hole];

	for (; hole *2 <= currentSize; hole = child)
	{
		child = hole * 2;
		if (child != currentSize && heap[child+1].value > heap[child].value)
		{
			child++;
		}
		if (heap[child].value > temp.value)
		{
			heap[hole] = heap[child];
			locations[heap[hole].label] = hole;
		}
		else
			break;
	}		
	heap[hole] = temp;
	locations[heap[hole].label] = hole;
}

//function to check if heap empty
//returns true if heap contains no elements
bool MPQ::isEmpty()
{
	if (heap.size() == 0)
	{
		return true;
	}
	return false;
}

//function returns number of elements in heap
int MPQ::getSize()
{
	//int sizeValue = currentSize;
	return currentSize;
}

//inserts the height and label information of building in heap
//heap order property maintained
void MPQ::insert(int &value, int &label)
{
	int hole = ++currentSize;
	heapStruct temp;
	temp.label = label;
	temp.value = value;
	for(; hole > 1 && value > heap[hole/2].value; hole /=2) //maxheap is implemented
	{
		heap[hole] = heap[hole/2];
		locations[heap[hole].label] = hole;
	}
	heap[hole] = temp;
	locations[heap[hole].label] = hole;
}

//function returns max height of buildings in heap
//since max heap is implemented, root contains max element
int MPQ::getMax()
{
	if (!isEmpty())
	{		
		int maxVal = heap[1].value;
		return maxVal;
	}
	return 0;
}

//remove element from heap
int MPQ::remove(int &label)
{
	int toRemove_value = -1;
	if (!isEmpty())
	{
		heapStruct toDelete;
		toDelete = heap[locations[label]];
		toRemove_value = toDelete.value;
		//int currentSize = getSize();
		heap[locations[label]] = heap[currentSize];
		currentSize--;                   //size of heap decremented as element is removed from heap
		percolateDown(locations[label]); //to re-establish the heap order property
		locations[label] = 0;             //location index of removed element set as 0
	}
	return toRemove_value;
}

