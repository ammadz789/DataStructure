#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include "QuadTree.h"
#include "strutils.h"

using namespace std;


int main()
{

	string filename = "cities.txt";  
	string cityName;
	ifstream input, in;
	input.open(filename.c_str());   //cities file is opened

	int x_max, y_max;
	int x_cord, y_cord;
	input>>x_max>>y_max;
	
	QuadTree tree;   //tree constructed
	while (!input.eof())
	{
		input>>cityName>>x_cord>>y_cord;
	
		//check if x and y cordinates within given range	
		if ( x_cord <= x_max && y_cord <= y_max){
			node *temp = new node(nullptr, nullptr, nullptr, nullptr, x_cord, y_cord, cityName); //new node constructed with given parameters, initially all nodes point to null
			tree.PT_INSERT(temp);	//function to insert nodes in tree is called
		}
	}

	node *r = tree.getRoot();
	tree.printTree(r);
	cout<<endl<<endl;

	string queryFile = "queries.txt";
	string line, x_str, y_str, radius_str;
	int x_Query, y_Query, radius;
	in.open(queryFile.c_str());    //query file opened
	while (getline(in,line))
	{
		stringstream ss(line);   //for parsing the line
		getline(ss, x_str, ',');
		getline(ss, y_str, ',');
		getline(ss, radius_str, ',');
		x_Query = atoi(x_str);   //strings converted to integer type
		y_Query = atoi(y_str);
		radius = atoi(radius_str);

		if ( x_Query > x_max || y_Query > y_max)
		{                                                          //check if query point coordinates within range
			cout<<"Query Coordinates out of range!"<<endl;
			return 0;
		}

		node *R=tree.getRoot();
		tree.searchTree(x_Query, y_Query, radius, R);   
		tree.printCityList(); //print the list of cities visited in search and cities following within the search radius
	
	}


	return 0;
}