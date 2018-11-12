#ifndef MAKEMONOTONE_H
#define MAKEMONOTONE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

enum Type { unassigned = 0, start, end, regular, split, merge }; // types of vertices




class MakeMonotone
{
public:
	explicit MakeMonotone(); // constructor
	~MakeMonotone(); // destructor
private:
	struct edge;
	struct vertex
	{
		int x = 0;
		int y = 0;
		Type type = unassigned;
		vertex *next = NULL;
		vertex *prev = NULL;
		edge *ei = NULL; // edge ei of vi

	};

	struct edge
	{
		vertex *vi = NULL;
		int xi = 0; // x coordinate of ei
		int yi = 0; // y coordinate of ei
		int xNext = 0; // x coordinate of ei+1
		int yNext = 0; // y coordinate of ei+1
		edge *next;
		edge *prev;
		vertex *helper = NULL;
	};

	std::vector <edge> verticle;

	vertex *topV = NULL; // first vector from input



	void buildVertexList();
	void buildEdgeList();
	void addVertex(std::string); // builds list of vertex from input text file
	bool its_a_number_exp(char a); // checks if text file has valid inputs
	void printList(); // prints list of vertices for testing
	void setHighest(); // sets topV to highest vertex
	void vertexTypeFinder(); // identifies and stores vertex type in vertex.type
	struct comparer {
		bool operator()(vertex const &a, vertex const &b)
		{
			if (a.y == b.y) return (a.x < b.x); else return a.y < b.y;
		}
	};
	std::priority_queue<vertex, std::vector <vertex>, comparer> Q; // the priority queue
	void buildQueue(); // builds priority queue
	void Make_Monotone();
	void Handle_Start(vertex &);
	void Handle_End(vertex);
	void Handle_Split(vertex);
	void Handle_Merge(vertex);
	void Handle_Regular(vertex);
	void setHelper(edge &);
	edge directlyLeft(vertex);
	std::vector<edge> tv;

	//////////// Binary Search Tree

	struct Tree // binary tree
	{
		edge ei;
		struct Tree *left = NULL;
		struct Tree *right = NULL;
	};

	struct Tree *Leaf(edge a) // adds a new node to the tree
	{
		struct Tree *temp = new Tree;
		temp->ei = a;
		temp->left = NULL;
		temp->right = NULL;
		return temp;
	};

	struct Tree *insert(struct Tree *leaf, edge a) // inserts new node into tree
	{
		if (leaf == NULL)
			return Leaf(a);
		else if (a.yi < leaf->ei.yi)
			leaf->left = insert(leaf->left, a);
		else if (a.yi > leaf->ei.yi)
			leaf->right = insert(leaf->right, a);

		return leaf;
	}

	Tree *T = NULL; // binary search tree used for log(n)

	edge minimum(Tree *root) // finds smallest node taking left branch if availiable
	{
		if (root == NULL)
		{
			return root->ei;
		}
		if (root->left != NULL)
		{
			return minimum(root->left);
		}
		return root->ei;
	}


	struct Tree *deleteLeaf(struct Tree *root, edge *deleteThis) // deletes a node from the BST
	{
		if (root == NULL)
			return NULL;
		if (deleteThis->yi < root->ei.yi)
			root->left = deleteLeaf(root->left, deleteThis);
		else if (deleteThis->yi > root->ei.yi)
			root->right = deleteLeaf(root->right, deleteThis);
		else
		{
			if (root->left == NULL && root->right == NULL)
			{
				delete(root);
				root = NULL;
			}
			else if (root->right == NULL)
			{
				struct Tree *temp = root;
				root = root->left;
				delete temp;
			}
			else if (root->left == NULL)
			{
				struct Tree *temp = root;
				root = root->right;
				delete temp;
			}
			else
			{
				edge temp = minimum(root->right);
				root->ei = temp;
				root->right = deleteLeaf(root->right, &temp);
			}
		}
		return root;
	}



};


#endif