#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>
#include <sstream>
#include <fstream>
#include <vector>
#include "strutils.h"

using namespace std;

//variables declared
unordered_map<string, int> row_map;    //key = rowname, data = row number
unordered_map<string, int> block_map;  //key = blockname, data = block number
unordered_map<string, string> customer_map;  //store customer names with thier respective reservations
vector<vector<tuple<int, string>>> row_matrix; //for accessing specific row heap e.g. C1 = row2 = row_matrix[2]
vector<vector<vector<tuple<string, string, string, int>>>> stadium;
vector<string> blockNames;
vector<string> rowNames;
ofstream output;
int numColoumns;

void heapsort(vector<tuple<int, string>> &a);  //prototype for heapsort

//print function, prints current stadium state
//print seats in order of input order of blocks and rows
//time complexity = O(B*R*C) as all seats are visited once
void print()
{
	output<<endl;
	for (int i=0; i < stadium.size(); i++)
	{
		output<<blockNames[i]<<endl;
		output<<"~~~~~~~"<<endl;
		for (int j=0; j <row_matrix.size() ;j++)
		{
			output<<rowNames[j]<<" : ";
			for (int k=0; k < numColoumns; k++)
			{
				string c_name = get<0>(stadium[i][j][k]);
				output<<c_name<<"  ";
			}
			output<<endl;
		}
		output<<"======="<<endl<<endl;
	}
}

//reserves the requested seat
//if requested seat is not available, prints error message
void reserve_seat(string customerName, string blockName, string rowName, int coloumnNumber)
{
	if (customer_map.find(customerName) != customer_map.end())   //to check if customer already has a reservation
	{
		output<<customerName<<" could not reserve a seat!"<<endl;
		return;
	}

	auto r_it = row_map.find(rowName);
	int rowNumber = r_it->second;
	auto b_it = block_map.find(blockName);
	int blockNumber = b_it->second;

	string c_Name = customerName.substr(0,3);
	int index;

	if (get<0>(stadium[blockNumber][rowNumber][coloumnNumber]) == "---")   //if requested seat is available
	{
		get<0>(stadium[blockNumber][rowNumber][coloumnNumber]) = c_Name;   //reserve the seat for customer
		string coloumn = itoa(coloumnNumber);
		customer_map[customerName] = blockName+"," + rowName+"," + coloumn;  //add customer's name in unordered map to avoid more
		                                                                     //than 1 reservation by the same customer
		output<<customerName<<" has reserved "<<blockName<<" "<<rowName<<"-"<<itoa(coloumnNumber)<<endl;

		for (int i = 0; i < row_matrix[rowNumber].size(); i++)  //complexity = O(B) since size of heap is equivelant to number of blocks 
		{
			if (get<1>(row_matrix[rowNumber][i] )== blockName){
				index = i;
				break;
			}
		}

		get<0>(row_matrix[rowNumber][index]) += 1; //update heap
		heapsort(row_matrix[rowNumber]);           //sort heap, complexity = O(logB)
	}

	else
	{
		output<<customerName<<" could not reserve a seat!"<<endl;
	}
}


//reserves seat for customer in block with the least number of reservations for the requested row
//if customer already has reservation, prints error message
void reserve_seat_by_row(string customerName, string rowName)
{
	if (customer_map.find(customerName) != customer_map.end())   //to check if customer already has a reservation
	{
		output<<customerName<<" could not reserve a seat!"<<endl;
		return;
	}

	auto r_it = row_map.find(rowName);
	int rowNumber = r_it->second;
	tuple<int, string> first_tuple, second_tuple;
	first_tuple = row_matrix[rowNumber][0];     //root element of the given row's minheap, gives block with least no. of reservations
	second_tuple = row_matrix[rowNumber][1];    //next element to root in case of equal capacities for 2 blocks(decision will be made on hierarchy of blocks depending on thier input order) 
	
	int block_number_value;
	string b1, b2, blockNumber;
	bool flag = true; //true if block in root index of min heap is to be assigned the customer

	if (get<0>(first_tuple) == get<0>(second_tuple))  //check which block to assign seat in
	{
		b1 = get<1> (first_tuple);
		b2 = get<1> (second_tuple);
		auto b1_it = block_map.find(b1);
		auto b2_it = block_map.find(b2);
		
		if (b1_it->second > b2_it->second)
		{
			block_number_value = b2_it->second;
			blockNumber = b2;
			flag = false;
		}
		else
		{
			block_number_value = b1_it->second;
			blockNumber = b1;
		}

	}
	else
	{
		blockNumber = get<1>(first_tuple);
		auto b_it = block_map.find(blockNumber);
		block_number_value = b_it->second;
	}

	for (int i=0; i < numColoumns; i++)     //loop over coloumns of selected block and row to assign customer to leftmost empty seat
	{                                       //complexity = O(C)
		tuple<string, string, string, int> seatTuple;
		seatTuple =  stadium[block_number_value][rowNumber][i];
		if (get<0>(seatTuple) == "---")                          //if requested seat is available
		{
			string c_Name = customerName.substr(0,3);
			get<0>(stadium[block_number_value][rowNumber][i]) = c_Name;  //reserve the seat for customer
			string coloumn = itoa(i);
			output<<customerName<<" has reserved "<<blockNumber<<" "<<rowName<<"-"<<coloumn<<" by emptiest block"<<endl;			
			customer_map[customerName] = blockNumber+"," + rowName+"," + coloumn;  //update unordered map with customer name

			//update the respective heap
			if (flag == true)
			{
				get<0>(row_matrix[rowNumber][0]) += 1; 
			}
			else
			{
				get<0>(row_matrix[rowNumber][1]) += 1; 
			}

			heapsort(row_matrix[rowNumber]);   //sort heap, complexity = O(logB)

			return;
		}
	}
	output<<customerName<<" could not reserve a seat!"<<endl;
}

//prints the reserved seat details of the customer
//if no reservation for customer, prints error message
void get_seat(string customerName)
{
	if (customer_map.find(customerName) == customer_map.end())     //to check if customer already has a reservation
	{
		output<<"There is no reservation made for "<<customerName<<"!"<<endl;
	}

	else
	{
		auto it = customer_map.find(customerName);
		string result = it->second;
		string block, row, coloumn;
		
		stringstream ss(result);   //for parsing the line, extract the details of the customer's seat
		getline(ss, block, ',');
		getline(ss, row, ',');
		getline(ss, coloumn, ',');
		//print details
		output<<"Found that "<<customerName<<" has a reservation in "<<block<<" "<<row<<"-"<<coloumn<<endl;
	}
}

//cancel reservation of the customer, if it exists
void cancel_reservation(string customerName)
{	
	if (customer_map.find(customerName) == customer_map.end())     //to check if customer already has a reservation
	{
		output<<"Could not cancel the reservation for "<<customerName<<"; no reservation found!"<<endl;
	}

	else
	{	
		auto it = customer_map.find(customerName);
		string result = it->second;
		string block, row, coloumn;
		stringstream ss(result);   //for parsing the line
		getline(ss, block, ',');
		getline(ss, row, ',');
		getline(ss, coloumn, ',');
		auto r_it = row_map.find(row);
		int rowNumber = r_it->second;
		auto b_it = block_map.find(block);
		int blockNumber = b_it->second;



		get<0>(stadium[blockNumber][rowNumber][atoi(coloumn)]) = "---";  //cancelling reservation of the given seat
		customer_map.erase(customerName);                               //update unordered map(erase customer's name)
		output<<"Cancelled the reservation of "<<customerName<<endl;

		int index;
		for (int i = 0; i < row_matrix[rowNumber].size(); i++)
		{
			if (get<1>(row_matrix[rowNumber][i] )== block){
				index = i;
				break;
			}
		}

		get<0>(row_matrix[atoi(row)][index]) -= 1;   //update the respective row heap
		heapsort(row_matrix[atoi(row)]);             //sort the heap, complexity = O(logB)

	}
}



int main()
{
	string filename = "inputs.txt";
	ifstream input;
	input.open(filename.c_str()); //file opened
	
	string c_line, r_line, str;	

	getline(input, c_line);
	istringstream c_in(c_line);
	while(c_in>>str)
	{
		blockNames.push_back(str);  // storing block names in an array 
	}

	getline(input, r_line);
	istringstream r_in(r_line);
	while(r_in>>str)
	{
		rowNames.push_back(str);   // storing row names in an array 
	}

	input>>numColoumns;
	

	for (int i = 0; i < rowNames.size(); i++)
	{
		string currentBlockName;
		vector<tuple<int, string>> row_heap; //heap for single row

		for (int j = 0; j <blockNames.size(); j++)
		{
			tuple<int, string> row_tuple;
			currentBlockName = blockNames[j];
			get<0>(row_tuple) = 0;            //no. of reserved seats in row
			get<1>(row_tuple) = currentBlockName;
			row_heap.push_back(row_tuple);

		}
		row_matrix.push_back(row_heap);		//for storing all the row heaps
	}

	

	for (int i = 0; i < blockNames.size(); i++)
	{
		string currentBlockName = blockNames[i];
		block_map[currentBlockName] = i;		
		//create block matrix with size (C*R)
		vector<vector<tuple<string, string, string, int>>> block_matrix(rowNames.size(), vector<tuple<string, string, string, int>>(numColoumns)); 

		for (int j = 0; j <rowNames.size(); j++)
		{
			string currentRowName = rowNames[j];
			row_map[currentRowName] = j;			

			for (int k = 0; k < numColoumns; k++)    //seat tuples are initialized and inserted to block matrix
			{
				string customerName = "---";
				tuple<string, string, string, int> seat_tuple;
				seat_tuple = make_tuple(customerName, currentBlockName, currentRowName, k);
				block_matrix[j][k] = seat_tuple;
			}
		}
		stadium.push_back(block_matrix); //stadium contains all the seats in the stadium
	}



	string outFile = "output.txt";  //output file
	output.open(outFile.c_str());

	string line, temp_str;
	while (getline(input, line))
	{
		if (line != "")     //if not empty line
		{
			istringstream ss(line);  //the line is parsed
			ss>>temp_str;
			//check which function to implement depending upon the instructions in the input line
			//line parsed and relevent information such as customername is extracted
			if (temp_str == "print")
			{
				print();
			}
			else if (temp_str == "reserve_seat")
			{
				ss>>temp_str;
				string c_name = temp_str;
				ss>>temp_str;
				string b_name = temp_str;
				ss>>temp_str;
				string r_name = temp_str;
				ss>>temp_str;
				string col_name = temp_str;

				reserve_seat(c_name, b_name, r_name, atoi(col_name));

			}

			else if (temp_str == "reserve_seat_by_row")
			{
				ss>>temp_str;
				string c_name = temp_str;
				ss>>temp_str;
				string r_name = temp_str;

				reserve_seat_by_row(c_name, r_name);

			}

			else if (temp_str == "get_seat")
			{
				ss>>temp_str;
				string c_name = temp_str;

				get_seat(c_name);
			}

			else if (temp_str == "cancel_reservation")
			{
				ss>>temp_str;
				string c_name = temp_str;

				cancel_reservation(c_name);
			}
		}
	}


	//files are closed
	input.close();
	output.close();
	return 0;
}

/*
---------------------------------------------------------------------------
||Heap sort algorithim used in this program is copied from lecture slides||
---------------------------------------------------------------------------
*/

int leftChild(int i)
{
	return 2*i + 1;
}

void percolateDown(vector<tuple<int, string>> &a, int i, int n)
{
	int child;
	tuple<int, string> temp;

	for (temp = a[i]; leftChild(i) < n; i =child)
	{
		child = leftChild(i);
		if (child != n-1 && a[child] < a[child+1]) 
		{
			child++;
		}
		if (get<0>(a[child]) > get<0>(temp))
		{
			a[i] = a[child];
		}
		else
			break;
	}
	a[i] = temp;

}



void heapsort(vector<tuple<int, string>> &a)
{
	for (int i = a.size()/2; i >= 0; i--)
	{
		percolateDown(a, i, a.size());
	}

	for (int j = a.size()-1; j>0; j--)
	{
		tuple<int, string> temp;
		temp = a[0];
		a[0] = a[j];
		a[j] = temp;

		percolateDown(a, 0, j);
	}
}