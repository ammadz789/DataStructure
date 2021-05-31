#ifndef MPQ_h
#define MPQ_h

#include <vector>
#include <iostream>
using namespace std;

#pragma once
struct heapStruct  //struct for heap
{
	int label;
	int value;

};


class MPQ
{
public:
	MPQ(int size);    //constructor
	~MPQ();           //destructor
	void insert(int &value, int &label);  //insert in heap
	int remove(int &label);               //remove from heap
	int getMax();          //return max height of buildings in heap
	bool isEmpty();        //return true if heap contains no elements
	int getSize();         //return number of elements in heap
	


private:
	
	int currentSize;     //keeps track of number of elements currently in heap
	void percolateDown(int hole); //percolate down for deletion from heap
	vector<heapStruct> heap;      //vector of structs for storing building information in heap
	vector<int> locations;        //vector for keeping locations of elements in heap; streamlines insert/delete operations

};
#endif