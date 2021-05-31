#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MPQ.h"


using namespace std;

//struct for buildings
struct buildingInformation
{
	int label;
	int x_cord;
	int height;
	bool isLeft;
};

//insertion sort algorithim
//used as it is faster than quicksort for smaller array sizes
void insertionSort(vector<buildingInformation> & toSort)
{
	int j;
	for (int i = 0; i < toSort.size(); i++)
	{
		buildingInformation temp = toSort[i];

		for (j = i; j > 0 && temp.x_cord < toSort[j-1].x_cord; j--)
		{
			toSort[j] = toSort[j-1];			
		}
			
		toSort[j] = temp;
	}
}


int main()
{
	string filename = "input.txt";
	ifstream input;
	input.open(filename.c_str()); //file opened

	int numBuildings;
	int l_xcord, r_xcord;
	int height;
	int count = 0;
	
	input>>numBuildings; //the total number of buildings is read from first line of input
	vector<buildingInformation> building_array;  //vector of structs for storing building information

	while (!input.eof())
	{
		input>>l_xcord>>height>>r_xcord;
		buildingInformation b1, b2;
		b1.x_cord = l_xcord;
		b1.label = count;
		b1.height = height;
		b1.isLeft = true;
		b2.x_cord = r_xcord;
		b2.label = count;
		b2.height = height;
		b2.isLeft = false;

		//since each building has 2 x_cordinates, left and right x_cordinates are assigned to different structs
		building_array.push_back(b1);
		building_array.push_back(b2);
		count++;		//index incremented
	}
	  

	insertionSort(building_array); //to sort array of buildings


	MPQ myHeap(numBuildings); //heap object created

	if (building_array[0].x_cord != 0)   //check if first building starts from x=0
	{
		cout<<0<<"	"<<0<<endl;   
	}

	int max = 0;
	for (int i = 0; i < building_array.size(); i++)
	{

		if (building_array[i].isLeft == true)
		{
			myHeap.insert(building_array[i].height, building_array[i].label); //building details added to heap; building comes into view			
		}
		else if (building_array[i].isLeft == false)
		{
			myHeap.remove(building_array[i].label); //building removed from heap; building leaves view			
		}

		//check for max height of buildings currently in heap
		if (myHeap.getMax() != max)
		{
			max = myHeap.getMax();
			cout<<building_array[i].x_cord<<"	"<<max<<endl;  //if height changes owing to insertions/deletions, we print the current max height
		}

		//if no building left in heap
		if (myHeap.getSize() == 0)
		{
			if ((i+1) < building_array.size()) //if there are more buildings in building array
			{
				if (building_array[i].x_cord != building_array[i+1].x_cord)
				{
					cout<<building_array[i].x_cord<<"	"<<0<<endl;
				}
			}

			if (i == (building_array.size()-1)) //if there are no more buildings in array
			{
				cout<<building_array[i].x_cord<<"	"<<0<<endl;
			}			
		}
	}

	input.close();


	return 0;
}