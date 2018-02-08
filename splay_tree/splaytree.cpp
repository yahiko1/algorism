// splaytree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

struct splay
{
	int key;
	splay* lchild;
	splay* rchild;
};


splay* New_Node(int);
splay* LL_Rotate(splay*);
splay* RR_Rotate(splay*);
splay* Splay(splay*, int);
splay* Insert(int, splay*);
splay* Delete(int, splay*);
splay* Search(int, splay*);
void PreOrder(splay*);
splay* FindMin(splay*);
splay* FindMax(splay*);
splay* Merge(splay*, splay*);
void find(splay*, int, int&);
vector<splay> Split(int, splay*);



int main()
{
	int vector1[10] = { 9,8,7,6,5,4,3,2,1,0 };
	splay *root;
	root = nullptr;
	const int length = 10;
	
	for (int i = 0; i < length; i++)
		root = Insert(vector1[i], root);
	cout << "\nInsert 9,8,7,6,5,4,3,2,1,0\n PreOrder: \n";
	PreOrder(root);
	cout << endl;

	root = Search(9, root);
	cout << "\nSearch 9\n PreOrder: \n";
	PreOrder(root);
	cout << endl;

	root = Delete(6, root);
	cout << "\nDelete 6 \nPreOrder: \n";
	PreOrder(root);
	cout << endl;

	vector<splay> v;
	
	system("pause");
	return 0;
}




//LL(Y rotates to the left)
splay* LL_Rotate(splay* k2)
{
	splay* k1 = k2->rchild;
	k2->rchild = k1->lchild;
	k1->lchild = k2;
	return k1;
}


//RR(Y rotates to the right)
splay* RR_Rotate(splay* k2)
{
	splay* k1 = k2->lchild;
	k2->lchild = k1->rchild;
	k1->rchild = k2;
	return k1;
}


//An implementation of top-down splay tree
splay* Splay(splay* root, int key)
{
	// root is NULL or key is present at root
	if (root == nullptr || root->key == key)
		return root;
	if (root->key > key)
	{
		if (root->lchild == nullptr)
			return root;
		// Zig-Zig
		if (root->lchild->key > key)
		{
			root->lchild->lchild = Splay(root->lchild->lchild, key);
			root = RR_Rotate(root);
		}
		// Zig-Zag
		else if (root->lchild->key < key)
		{
			root->lchild->rchild = Splay(root->lchild->rchild, key);
			if (root->lchild->rchild != nullptr)
				root->lchild = LL_Rotate(root->lchild);
		}
		return (root->lchild == nullptr) ? root : RR_Rotate(root);
	}
	else
	{
		if (root->rchild == nullptr)
			return root;
		// Zag-Zig 
		if (root->rchild->key > key)
		{
			root->rchild->lchild = Splay(root->rchild->lchild, key);
			if (root->rchild->lchild != nullptr)
				root->rchild = RR_Rotate(root->rchild);
		}
		// Zag-Zag
		else if (root->rchild->key < key)
		{
			root->rchild->rchild = Splay(root->rchild->rchild, key);
			root = LL_Rotate(root);
		}
		return (root->rchild == nullptr) ? root : LL_Rotate(root);
	}
}


splay* New_Node(int key)
{
	splay* p_node = new splay;
	if (!p_node)
	{
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	p_node->key = key;
	p_node->lchild = p_node->rchild = nullptr;
	return p_node;
}


//Function to insert a new key k in splay tree with given root
splay* Insert(int key, splay* root)
{
	splay* p_node = nullptr;
	if (!p_node)
		p_node = New_Node(key);
	else
		p_node->key = key;
	if (!root)
	{
		root = p_node;
		p_node = nullptr;
		return root;
	}
	root = Splay(root, key);
	if (key < root->key)
	{
		p_node->lchild = root->lchild;
		p_node->rchild = root;
		root->lchild = nullptr;
		root = p_node;
	}
	else if (key > root->key)
	{
		p_node->rchild = root->rchild;
		p_node->lchild = root;
		root->rchild = nullptr;
		root = p_node;
	}
	else
		return root;
	p_node = nullptr;
	return root;
}


//The delete function for Splay tree.
splay* Delete(int key, splay* root)
{
	splay* temp;
	if (!root)
		return nullptr;
	root = Splay(root, key);
	if (key != root->key)
		return root;
	else
	{
		if (!root->lchild)
		{
			temp = root;
			root = root->rchild;
		}
		else
		{
			temp = root;
			root = Splay(root->lchild, key);
			root->rchild = temp->rchild;
		}
		free(temp);
		return root;
	}
}


splay* Search(int key, splay* root)
{
	return Splay(root, key);
}


void PreOrder(splay* root)
{
	if (root)
	{
		cout << "key: " << root->key;
		if (root->lchild)
			cout << " | left child: " << root->lchild->key;
		if (root->rchild)
			cout << " | right child: " << root->rchild->key;
		cout << "\n";
		PreOrder(root->lchild);
		/*cout << "key: " << root->key;
		if (root->lchild)
			cout << " | left child: " << root->lchild->key;
		if (root->rchild)
			cout << " | right child: " << root->rchild->key;
		cout << "\n";*/
		PreOrder(root->rchild);
	}
}


splay* FindMin(splay* root)
{
	splay* minimum = root;
	while (minimum != nullptr)
		minimum = minimum->lchild;
	return minimum;
}


splay* FindMax(splay* root)
{
	splay* maximum = root;
	while (maximum != nullptr)
		maximum = maximum->rchild;
	return maximum;
}


//Merge of two splay tree. All elements of first tree less than every element of second tree.
splay* Merge(splay* root1, splay* root2)
{
	int k;
	splay* max;
	max = FindMax(root1);
	k = max->key;
	root1 = Splay(root1, k);
	/*Note:  k == max element ot first splay tree,
	so after Splay(k, root1),
	the tree we get will have no right child tree.*/
	root1->rchild = root2;
	return root1;
}


//Split of splay tree
void find(splay* root, int k, int &m)
{
	if (k == root->key)
	{
		m = k;
		return;
	}
	if (root->key > k)
		if (k < m)
			m = k;
	find(root->rchild, k, m);
	find(root->lchild, k, m);
}
vector<splay> Split(int key, splay* root)
{
	vector<splay> v;
	int m = INT_MAX;
	find(root, key, m);
	if (m != INT_MAX)
	{
		Split(m, root);
		v.push_back(*root->lchild);
		v.push_back(*root->rchild);
	}
	return v;
}