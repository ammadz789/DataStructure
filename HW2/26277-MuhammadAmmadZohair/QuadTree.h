#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;

//the node structure has seven field
struct node{
	
	//4 pointers denoting quadrants
	node *NE;
	node *NW;
	node *SE;
	node *SW;

	int X_cord;
	int Y_cord;
	string city_name;
	

	node(node *ne, node *nw, node *se, node *sw, int x, int y, string str)  //constructor
	{
		NE = ne;
		NW = nw;
		SE = se;
		SW = sw;
		X_cord = x;
		Y_cord = y;
		city_name = str;
	}
};


class QuadTree
{
private:
	node *root;      //the root node

public:
	QuadTree()
	{
		root = nullptr;    // root is initialized as null in the constructor
	}
	~QuadTree() //destructor for deallocating memory
	{
		deleteTree(root);
	}

	void deleteTree(node*Root)
	{
		if (Root != nullptr)
		{
			deleteTree(Root->SE);
			deleteTree(Root->SW);
			deleteTree(Root->NE);
			deleteTree(Root->NW);
		}
		delete Root;
	}


	int indicator;

	//insert inserts the new node in the suitable location
	void PT_INSERT(node *toInsert)    
	{
		if (root == nullptr)
		{
			root = toInsert;
			return;
		}
		else
		{
			node *temp = root;
			if (temp != nullptr)
			{

				node *insertHere = PT_COMPARE(toInsert, root);
				if (indicator == 1)
				{
					insertHere->SW = toInsert;
				}
				else if (indicator == 2)
				{
					insertHere->NW = toInsert;
				}
				else if (indicator == 3)
				{
					insertHere->SE = toInsert;
				}
				else if (indicator == 4)
				{
					insertHere->NE = toInsert;
				}
				
			}		
		}
	}

	//compare function recursively compares the node to add with already existent nodes in the tree inorder to determine a suitable
	//location to insert the new node with respect to other nodes
	node *PT_COMPARE(node *toSearch, node *Root)
	{
		if (Root == nullptr)
			return Root;
		indicator = 0;
		if (toSearch->X_cord < Root->X_cord)
		{
			if (toSearch->Y_cord < Root->Y_cord)
			{
				if (Root->SW == nullptr)
				{
					indicator = 1;
					return Root; 
				}
				return PT_COMPARE(toSearch, Root->SW);
			}
			else
			{
				if (Root->NW == nullptr)
				{
					indicator = 2;
					return Root; 
				}
				return PT_COMPARE(toSearch, Root->NW);
			}
		}
		else if (toSearch->Y_cord < Root->Y_cord)
		{
			
			if (Root->SE == nullptr)
				{
					indicator = 3;
					return Root; 
				}
			return PT_COMPARE(toSearch, Root->SE);
		}
		
		else if (toSearch->X_cord == Root->X_cord)   //if new node lies on a quadrant line of another node
		{
			if (toSearch->Y_cord < Root->Y_cord)
			{
				if (Root->SE == nullptr)
				{
					indicator =3;
					return Root;
				}
				return PT_COMPARE(toSearch, Root->SE);
			}
			else
			{
				if (Root->NE == nullptr)
				{
					indicator = 4;
					return Root;
				}
				return PT_COMPARE(toSearch, Root->NE);
			}
		}
		
		else if (toSearch->Y_cord == Root->Y_cord)    //if new node lies on a quadrant line of another node
		{
			if (toSearch->X_cord < Root->X_cord)
			{
				if (Root->NW == nullptr)
				{
					indicator = 2;
					return Root;
				}
				return PT_COMPARE(toSearch, Root->NW);
			}
			else
			{
				if (Root->NE == nullptr)
				{
					indicator = 4;
					return Root;
				}
				return PT_COMPARE(toSearch, Root->NE);
			}
		}
		
		else 
		{
			if (Root->NE == nullptr)
				{
					indicator = 4;
					return Root; 
				}			
			return PT_COMPARE(toSearch, Root->NE);
		}	
	}


	//returns the root node of tree
	node *getRoot()
	{
		return root;
	}

	//pretty print the tree recursively
	void printTree(node *Root)
	{
		if (Root != nullptr)
		{
			cout<<Root->city_name<<endl;
			printTree(Root->SE);
			printTree(Root->SW);
			printTree(Root->NE);
			printTree(Root->NW);
		}
		
	}
	
	vector <string> visitedCity;  //list of cities visited in search function
	vector <string> resultCity;   //list of cities within proximity of search point
	


//prints list of cities visited while searching and the list of cities within proximity of the search coordinates	
void printCityList()
	{
		if (resultCity.size() == 0)
		{
			cout<<"<None>";
		}
		else
		{
			for (int i = 0; i < resultCity.size(); i++)
			{
				if ( i == 0)
				{
					cout<<resultCity[i];
				}
				else
				{
					cout<<", "<<resultCity[i];
				}
			}
		}
		cout<<endl;
		for (int i = 0; i < visitedCity.size(); i++)
		{
			if ( i == 0)
			{
				cout<<visitedCity[i];
			}
			else
			{
				cout<<", "<<visitedCity[i];
			}
		}
		cout<<endl<<endl;
		//vectors cleared for next search
		visitedCity.clear();
		resultCity.clear();
	}

	
	//returns true if node being searched lies within range of search point
	bool checkProximity(double radius, double distBetweenPoints)
	{
		if (distBetweenPoints > radius)
			return false;
		else
			return true;
	}

	//calculates the distance between the node being searched and the search point using pythagoras theorem
	//updates the resultCity list if node(city) is within proximity of search point
	void calculateDistance(int x, int y, int radius, node *cityNode)
	{
		double distance = 0, x_distance = 0, y_distance = 0, base = 0, opposite = 0, hypotenuse = 0;

		x_distance = abs(x-(cityNode->X_cord));
		y_distance = abs(y-(cityNode->Y_cord));

		if (x_distance == 0)
		{
			if (checkProximity(radius, y_distance))
			{
				resultCity.push_back(cityNode->city_name);
			}
			return;
		}
		else if (y_distance == 0)
		{
			if (checkProximity(radius, x_distance))
			{
				resultCity.push_back(cityNode->city_name);
			}
			return;
		}

		base = pow(x_distance, 2);
		opposite = pow(y_distance, 2);
		hypotenuse = sqrt(base + opposite);

		if (checkProximity(radius, hypotenuse))
		{
			resultCity.push_back(cityNode->city_name);
		}
		return;
	}


	//function for searching the tree recursively
	//13 possible cases are given to traverse the quadrants in the required order
	void searchTree(int x, int y, int radius, node *Root)
	{
		if (Root == nullptr)
			return;

		bool flag = false;
		double distance = 0, x_distance = 0, y_distance = 0, base = 0, opposite = 0, hypotenuse = 0;
		x_distance = abs(x-(Root->X_cord));
		y_distance = abs(y-(Root->Y_cord));
		base = pow(x_distance, 2);
		opposite = pow(y_distance, 2);
		hypotenuse = sqrt(base + opposite);

		if (x_distance == 0)
		{
			if (checkProximity(radius, y_distance))
			{
				flag = true;
			}
		}
		else if (y_distance == 0)
		{
			if (checkProximity(radius, x_distance))
			{
				flag = true;
			}
		}	

		else if (checkProximity(radius, hypotenuse))
		{
			flag = true;
		}
		

		if(flag == true) //13 (if node(city) lies within the search radius, all quadrants searched)
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}
			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}


		else if ((radius + y) <= Root->Y_cord && abs(radius-x) >= Root->X_cord) //1
		{
			visitedCity.push_back(Root->city_name);  //name of city visited pushed into visitedCity vector to keep record of the nodes visited
			calculateDistance(x, y, radius, Root);  //check if city being searched lies within search radius
			if (Root->SE != nullptr)
				{
					searchTree(x, y, radius, Root->SE);  //recursive function call on South East quadrant of the node
				}
		}

		else if ((radius+y) <= Root->Y_cord && (radius+x) <= Root->X_cord)//3
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}
		}

		else if (abs(radius-y) >= Root->Y_cord && abs(radius-x) >= Root->X_cord)//6
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
		}

		else if (abs(radius-y) >= Root->Y_cord && (radius+x) <= Root->X_cord)//8
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}
				

		else if (((radius+y) <= Root->Y_cord) )//2
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}
		}

		else if (abs(radius-x) >= Root->X_cord)//4
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}
			if (Root->NE != nullptr)
			{				
				searchTree(x, y, radius, Root->NE);
			}
		}

		else if ((x+radius) <= Root->X_cord)//5
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}

		else if (abs(radius-y) >= Root->Y_cord)//7
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}
		

		else if (((y+radius) > Root->Y_cord) && (abs(x-radius) < Root->X_cord)  && (x > Root->X_cord) && (Root->Y_cord > y)) //9
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			

			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}

			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}

			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
					
		}

		else if (((radius+y) > Root->Y_cord) && ((radius+x) > Root->X_cord) && (x < Root->X_cord) && (Root->Y_cord > y)) //10
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			
			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}

			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}

		else if ((abs(y-radius) < Root->Y_cord) && (abs(x-radius) < Root->X_cord) && (x > Root->X_cord) && (Root->Y_cord < y)) //11
		{
			visitedCity.push_back(Root->city_name); 
			calculateDistance(x, y, radius, Root);
			
			if (Root->SE != nullptr)
			{
				searchTree(x, y, radius, Root->SE);
			}
			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}

		else if (abs(y-radius) < Root->Y_cord && (x+radius) > Root->X_cord && (x < Root->X_cord) && (Root->Y_cord < y)) //12
		{
			visitedCity.push_back(Root->city_name);
			calculateDistance(x, y, radius, Root);
			
			
			if (Root->SW != nullptr)
			{
				searchTree(x, y, radius, Root->SW);
			}
			if (Root->NE != nullptr)
			{
				searchTree(x, y, radius, Root->NE);
			}
			if (Root->NW != nullptr)
			{
				searchTree(x, y, radius, Root->NW);
			}
		}
		
						
	}
};