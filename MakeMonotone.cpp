/*
Brian Pinson
U91813366
Project 2
10/23/2017

*/

#include "MakeMonotone.h"


int main()
{
	std::cout << "Computational Geometry Project 2\nInput file name:\n" << std::endl;


	MakeMonotone simplePolygon;

	return 0;
}




MakeMonotone::MakeMonotone() // constructor
{
	buildVertexList(); // builds doubly linked list of vertices
	setHighest();
	vertexTypeFinder(); // determines the type of vertex
	buildEdgeList();
	Make_Monotone();
	printList();
	std::cout << "finished";
	while (1);

}

MakeMonotone::~MakeMonotone() // destructor
{

}


void MakeMonotone::buildVertexList()
{
	std::string textfile;
	std::getline(std::cin, textfile);
	std::fstream file;
	file.open(textfile);
	while (!file.is_open()) // if file is not open repeat until file can be opened
	{
		std::cout << "\nError file not found" << std::endl;
		std::cout << "Input name of text file or press \'enter\' if filename is: input.txt" << std::endl;
		std::getline(std::cin, textfile);
		file.open(textfile);
	}
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); //skips to next line
		int vertices = std::stoi(line); //number of vertices
		if (vertices < 4)
		{
			std::cout << "Only " << vertices << " found, cannot triangulate" << std::endl;
			exit(1);
		}
		for (int i = 0; i < vertices; i++) //loop scans through each line of textfile and creates a linked list;
		{
			std::getline(file, line); //skips to next line
			addVertex(line);
		}

		for (vertex *temp = topV; temp != NULL; temp = temp->next)
		{
			if (temp->next == NULL)
			{
				topV->prev = temp;
				topV->prev->next = topV;
				break;
			}
		}

		file.close();
	}


}

void MakeMonotone::addVertex(std::string line)
{
	int x = 0;
	int y = 0;

	int start = 0; // marks the first char in the line that contains a number or decimal point
	int i = 0; // loop control

	int size = line.length();

	for (; i < size; i++) //scans for end of numerical value and calculates x
	{
		if (!its_a_number_exp(line[i]))
		{
			x = std::stoi(line.substr(start, i));
			break;
		}
	}

	i++;
	start = i;

	for (; i < size; i++) //scans for end of numerical value and calculates y
	{

		if (i + 1 == size)
		{
			y = std::stoi(line.substr(start, i));
			break;
		}
	}


	vertex *newV = new vertex;
	newV->x = x;
	newV->y = y;
	newV->next = NULL;
	newV->prev = NULL;


	if (topV == NULL) // puts in data for first vertex from file
	{
		topV = newV;
		newV = NULL;
	}
	else // adds in vertex data from input to doubly linked list
	{
		for (vertex *temp = topV; temp != NULL; temp = temp->next)
		{
			if (temp->next == NULL)
			{
				newV->prev = temp;
				newV->prev->next = newV;
				break;
			}
		}
	}
}

bool MakeMonotone::its_a_number_exp(char a) // returns true if a number, decimal, or minus char is present
{
	if (a == '0' || a == '1' || a == '2' || a == '3' || a == '4' || a == '5' ||
		a == '6' || a == '7' || a == '8' || a == '9' || a == '-')
		return true;
	else
		return false;
}

void MakeMonotone::printList()
{
	//
	//	prints out the vertices and their type
	//
	/*	std::cout << "Printed from vertices nodes" << std::endl;
	for (vertex *temp = topV;; temp = temp->next)
	{

	std::cout << "(" << temp->x << ", " << temp->y << ") ";
	if (temp->type == start) std::cout << "start" << std::endl;
	if (temp->type == end) std::cout << "end" << std::endl;
	if (temp->type == split) std::cout << "split" << std::endl;
	if (temp->type == merge) std::cout << "merge" << std::endl;
	if (temp->type == regular) std::cout << "regular" << std::endl;
	if (temp == NULL || temp->next == NULL || temp->prev == NULL) std::cout << "Critical error vertex node missing";


	if (temp->next == topV) break;
	}
	*/

	std::cout << "\n Diagonal list" << std::endl;
	int size = verticle.size();
	if (!verticle.empty())
		for (int i = 0; i < size; i++)
		{
			std::cout << "(" << verticle[i].xi << ", " << verticle[i].xi << ") (" << verticle[i].xNext << ", " << verticle[i].yNext << ") " << std::endl;
		}


}

void MakeMonotone::buildQueue()
{

	for (vertex *temp = topV;; temp = temp->next)
	{
		Q.push(*temp);
		if (temp->next == topV) break;
	}
}

void MakeMonotone::setHighest()
{
	vertex *highest = topV;
	for (vertex *temp = topV;; temp = temp->next) // finds the highest point, if multiple points have same height then left most
	{
		if (highest->y < temp->y)
		{
			highest = temp;
		}
		else if (highest->y == temp->y)
		{
			if (highest->x > temp->x)
			{
				highest = temp;
			}
		}

		if (temp->next == topV) break;
	}
	topV = highest;
}

void MakeMonotone::buildEdgeList()
{
	for (vertex *temp = topV;; temp = temp->next) // scans through all the vertices to create edge list
	{
		edge *newEdge = new edge;
		newEdge->vi = temp;
		newEdge->xi = temp->x;
		newEdge->yi = temp->y;
		newEdge->xNext = temp->next->x;
		newEdge->yNext = temp->next->y;

		temp->ei = newEdge;

		if (temp->next == topV) break; // terminates the loop when the top vertex is found
	}

	for (vertex *temp = topV;; temp = temp->next) // scans through all the vertices to connect edge list
	{
		temp->ei->next = temp->next->ei;
		temp->ei->prev = temp->prev->ei;

		if (temp->next == topV) break; // terminates the loop when the top vertex is found
	}

}





void MakeMonotone::vertexTypeFinder() // responsible for identifying the vertex type;
{
	for (vertex *temp = topV;; temp = temp->next) // scans through all the vertices to determine its type
	{

		if (temp->y >= temp->next->y && temp->y >= temp->prev->y)
		{
			int passed = 0;
			for (vertex *vert = temp->next;; vert = vert->next) // scans through edges to find out interior angle
			{
				if (temp->y < vert->y && temp->y > vert->next->y && temp->x < vert->x || temp->y > vert->y && temp->y < vert->next->y && temp->x < vert->x)
					passed++;
				if (vert->next == temp) break;
			}

			if (passed % 2 == 0)
				temp->type = start; // angle was less than pi
			else
				temp->type = split; // angle was greater than pi
		}
		else if (temp->y <= temp->next->y && temp->y <= temp->prev->y)
		{
			int passed = 0;
			for (vertex *vert = temp->next;; vert = vert->next) // scans through edges to find out interior angle
			{
				if (temp->y < vert->y && temp->y > vert->next->y && temp->x < vert->x || temp->y > vert->y && temp->y < vert->next->y && temp->x < vert->x)
					passed++;
				if (vert->next == temp) break;
			}

			if (passed % 2 == 0)
				temp->type = end; // angle was less than pi
			else
				temp->type = merge; // angle was greater than pi
		}
		else
		{
			temp->type = regular;
		}
		if (temp->next == topV) break; // terminates the for loop when the next vertex is the first
	}
}











void MakeMonotone::Make_Monotone()
{
	buildQueue();
	Tree *T = NULL;

	while (!Q.empty())
	{
		vertex v = Q.top();
		Q.pop();
		if (v.type == start)
			Handle_Start(v);
		else if (v.type == end)
			Handle_End(v);
		else if (v.type == split)
			Handle_Split(v);
		else if (v.type == merge)
			Handle_Merge(v);
		else if (v.type == regular)
			Handle_Regular(v);
	}
}

void MakeMonotone::Handle_Start(vertex &v)
{

	insert(T, *v.ei);
	v.ei->helper = v.ei->vi;
}


void MakeMonotone::Handle_End(vertex v)
{
	return; // disabled rest of code, crashes below this

	edge *e = v.ei->prev; // edge e is the prev edge of the vertex v*
	if (e->helper == NULL) // if edge e has no helper
	{
		setHelper(*e); // function will set e->helper
	}


	if (v.ei->prev->helper->type == merge) // if edge ei-1 helper is a merge type add to diagonal list
	{
		edge vert;
		vert.xi = v.x;
		vert.yi = v.y;
		vert.xNext = v.ei->prev->helper->x;
		vert.yNext = v.ei->prev->helper->y;
		verticle.push_back(vert);
	}
	delete(T, e->prev); // deletes edge ei-1 from BST T


}


void MakeMonotone::Handle_Split(vertex v)
{
	edge vert;
	vert.xi = v.x;
	vert.yi = v.y;
	vert.xNext = Q.top().x;
	vert.yNext = Q.top().y;
	verticle.push_back(vert);

	return; // disabled rest of code, crashes below this


	edge left = directlyLeft(v); // finds edge directly left of split vertex v
								 //	edge vert;
	vert.xi = v.x;
	vert.yi = v.y;
	vert.xNext = left.xi;
	vert.yNext = left.yi;

	verticle.push_back(vert); // adds (v, vj) to diagonal list

	left.helper = &v;
	v.ei->helper = &v;
	insert(T, *v.ei);


}


void MakeMonotone::Handle_Merge(vertex v)
{
	edge vert;
	vert.xi = v.x;
	vert.yi = v.y;
	vert.xNext = Q.top().x;
	vert.yNext = Q.top().y;
	verticle.push_back(vert);


	return; // disabled rest of code, crashes below this

	edge *e = v.ei;
	if (e->helper->type == merge) // if helper of edge e is a merge type add to diagonal
	{
		edge diag;
		diag.xi = v.x;
		diag.yi = v.y;
		diag.xNext = e->helper->x;
		diag.yNext = e->helper->y;
		verticle.push_back(diag);
	}

	delete(T, e->prev); // deletes ei-1 from T


	edge ej = directlyLeft(v); // ej is directly left of v
	if (ej.helper->type == merge) // if ej is a merge type
	{
		edge diag;
		diag.xi = v.x;
		diag.yi = v.y;
		diag.xNext = ej.helper->x;
		diag.yNext = ej.helper->y;
		verticle.push_back(diag);
	}
	ej.helper = &v; // sets ej->helper to v

}


void MakeMonotone::Handle_Regular(vertex v)
{
	edge vert;
	vert.xi = v.x;
	vert.yi = v.y;
	vert.xNext = Q.top().x;
	vert.yNext = Q.top().y;
	verticle.push_back(vert);

	return; // disabled rest of code, crashes below this

	int passed = 0;
	for (vertex *vert = topV;; vert = vert->next) // scans through edges to find out if interior or exterior
	{
		if (v.y < vert->y && v.y > vert->next->y && v.x < vert->x || v.y > vert->y && v.y < vert->next->y && v.x < vert->x)
		{
			passed++;
		}
		if (vert->next == topV) break;
	}

	if (passed % 2 != 0) // Regular is to the right
	{
		if (v.ei->prev->helper->type == merge) // if ei-1's helper is a merge type
		{
			edge diag;
			diag.xi = v.x;
			diag.yi = v.y;
			diag.xNext = v.ei->prev->helper->x;
			diag.yNext = v.ei->prev->helper->y;
			verticle.push_back(diag);
		}
		delete(T, v.ei->prev); // delete ei-1 from T
		insert(T, *v.ei); // insert ei into T
		v.ei->helper = &v; // set ei helper to v
	}
	else // regular is the left
	{
		edge ej = directlyLeft(v);
		if (ej.helper->type == merge) // if ej->helper is a merge type
		{
			edge diag;
			diag.xi = v.x;
			diag.yi = v.y;
			diag.xNext = ej.helper->x;
			diag.yNext = ej.helper->y;
			verticle.push_back(diag); // insert(v, vj) into diagonal list
		}
		ej.helper = &v;  // set ej->helper to v
	}

}




MakeMonotone::edge MakeMonotone::directlyLeft(vertex v) // returns edge directly left
{

	std::vector< edge> leftOf; // finds a list of all edges directly left of vertex v
	int size = tv.size();
	for (int i = 0; i < size; i++) // scans through list
	{
		edge temp = tv[i];
		if (temp.xi < v.x || temp.xNext < v.x) // if vertex is to the left of v
		{
			if ((v.x > temp.xi || v.x > temp.xNext) && (v.y < temp.yNext && v.y > temp.yi || v.y > temp.yNext && v.y < temp.yi)) // is the edge between vertex v
			{
				leftOf.push_back(temp);
			}
		}
	}

	size = leftOf.size();
	int xi = leftOf[0].xi;
	int xj = leftOf[0].xNext;
	int loc = 0;
	for (int i = 1; i < size; i++) // scans through list of edges left of v and finds the nearest one to v
	{
		if (xi < leftOf[i].xi && xj < leftOf[i].xNext) // if the edge is closer to x than the current edge
		{
			loc = i;
			int xi = leftOf[i].xi;
			int xj = leftOf[i].xNext;
		}
		else if (xi < leftOf[i].xi || xj < leftOf[i].xNext) // rare case where two edges have one end point closer to v than the other
		{
			if (xi - xj > leftOf[i].xi - leftOf[i].xNext)
			{
				loc = i;
				int xi = leftOf[i].xi;
				int xj = leftOf[i].xNext;
			}
		}
	}

	if (leftOf.empty())
	{
		edge leftOfVertex = leftOf[loc];
		return leftOfVertex;
	}

}


void MakeMonotone::setHelper(edge &e) // finds the helper of edge e and set e->Helper
{
	std::vector<vertex> candidates; // holds vertices that could be helper of e
	for (vertex *temp = topV;; temp = temp->next) // checks every vertex for possible helper
	{
		if ((temp->y < e.yi && temp->y > e.yNext) || (temp->y > e.yi && temp->y < e.yNext)) // if possible helper vertex is to the left or right of the edge e
		{
			candidates.push_back(*temp);
		}
		if (temp->next == topV) break;
	}


	std::vector<vertex> helper;
	int size = candidates.size();
	for (int i = 0; i < size; i++) // needs to check if line to helper is inside or outside
	{

		int passed = 0;
		vertex *temp = &candidates[i];
		for (vertex *vert = topV;; vert = vert->next) // scans through edges to find out if interior or exterior
		{
			if (temp->y < vert->y && temp->y > vert->next->y && temp->x < vert->x || temp->y > vert->y && temp->y < vert->next->y && temp->x < vert->x)
			{
				passed++;
			}
			if (vert->next == topV) break;
		}

		if (passed % 2 != 0) // the diagonal would be inside the polygon
		{
			helper.push_back(candidates[i]);
		}

	}

	candidates.clear(); // no longer needed



	size = helper.size();
	int lowest = helper[0].y;
	int loc = 0;
	for (int i = 0; i < size; i++) // scans through list of possible helpers and picks lowest vertex
	{

		if (helper[i].y < lowest)
		{
			lowest = helper[i].y;
			loc = i;
		}
	}

	if (!helper.empty())
	{
		vertex a = helper[loc];
		e.helper = &a;

	}
	else
	{
		e.helper = e.vi; // no vertex was found across from edge e
	}
}







