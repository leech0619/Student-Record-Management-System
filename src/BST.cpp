#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "BST.h"


using namespace std;


BST::BST() {
	root = NULL;
	count = 0;
}


bool BST::empty() {
	if (count == 0) return true;
	return false;
}


int BST::size() {
	return count;
}


void BST::preOrderPrint() {
	if (root == NULL) return;// handle special case
	else preOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::preOrderPrint2(BTNode* cur) {

	if (cur == NULL) return;
	cur->item.print(cout);
	preOrderPrint2(cur->left);
	preOrderPrint2(cur->right);
}


void BST::inOrderPrint() {
	if (root == NULL) return;// handle special case
	else inOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::inOrderPrint2(BTNode* cur) {

	if (cur == NULL) return;

	inOrderPrint2(cur->left);
	cur->item.print(cout);
	inOrderPrint2(cur->right);
}


void BST::postOrderPrint() {
	if (root == NULL) return;// handle special case
	else postOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::postOrderPrint2(BTNode* cur) {
	if (cur == NULL) return;
	postOrderPrint2(cur->left);
	postOrderPrint2(cur->right);
	cur->item.print(cout);
}



int BST::countNode() {
	int	counter = 0;
	if (root == NULL) return 0;
	countNode2(root, counter);
	return counter;
}


void BST::countNode2(BTNode* cur, int& count) {
	if (cur == NULL) return;
	countNode2(cur->left, count);
	countNode2(cur->right, count);
	count++;
}


bool BST::findGrandsons(type grandFather) {
	if (root == NULL) return false;
	return (fGS2(grandFather, root));
}


bool BST::fGS2(type grandFather, BTNode* cur) {
	if (cur == NULL) return false;
	//if (cur->item == grandFather) {
	if (cur->item.compare2(grandFather)) {

		fGS3(cur, 0);// do another TT to find grandsons
		return true;
	}
	if (fGS2(grandFather, cur->left)) return true;
	return fGS2(grandFather, cur->right);
}


void BST::fGS3(BTNode* cur, int level) {
	if (cur == NULL) return;
	if (level == 2) {
		cur->item.print(cout);
		return;  // No need to search downward
	}
	fGS3(cur->left, level + 1);
	fGS3(cur->right, level + 1);
}



void BST::topDownLevelTraversal() {
	BTNode* cur;
	Queue		    q;


	if (empty()) return; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);
		if (cur != NULL) {
			cur->item.print(cout);

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
}

//insert for BST
bool BST::insert(type newItem) {
	BTNode* cur = new BTNode(newItem);
	if (!cur) return false;		// special case 1
	if (root == NULL) {
		root = cur;
		count++;
		return true; 			// special case 2
	}
	insert2(root, cur);			// normal
	count++;
	return true;
}


void BST::insert2(BTNode* cur, BTNode* newNode) {
	//if (cur->item > newNode->item) {
	if (cur->item.compare1(newNode->item)) {
		if (cur->left == NULL)
			cur->left = newNode;
		else
			insert2(cur->left, newNode);
	}
	else {
		if (cur->right == NULL)
			cur->right = newNode;
		else
			insert2(cur->right, newNode);
	}
}



bool BST::remove(type item) {
	if (root == NULL) return false; 		// special case 1: tree is empty
	return remove2(root, root, item); 		// normal case
}

bool BST::remove2(BTNode* pre, BTNode* cur, type item) {

	// Turn back when the search reaches the end of an external path
	if (cur == NULL) return false;

	// normal case: manage to find the item to be removed
	//if (cur->item == item) {
	if (cur->item.compare2(item)) {
		if (cur->left == NULL || cur->right == NULL)
			case2(pre, cur);	// case 2 and case 1: cur has less than 2 sons
		else
			case3(cur);		// case 3, cur has 2 sons
		count--;				// update the counter
		return true;
	}

	// Current node does NOT store the current item -> ask left sub-tree to check
	//if (cur->item > item)
	if (cur->item.compare1(item))
		return remove2(cur, cur->left, item);

	// Item is not in the left subtree, try the right sub-tree instead
	return remove2(cur, cur->right, item);
}


void BST::case2(BTNode* pre, BTNode* cur) {

	// special case: delete root node
	if (pre == cur) {
		if (cur->left != NULL)	// has left son?
			root = cur->left;
		else
			root = cur->right;

		free(cur);
		return;
	}

	if (pre->right == cur) {		// father is right son of grandfather? 
		if (cur->left == NULL)			// father has no left son?
			pre->right = cur->right;			// connect gfather/gson
		else
			pre->right = cur->left;
	}
	else {						// father is left son of grandfather?
		if (cur->left == NULL)			// father has no left son? 
			pre->left = cur->right;				// connect gfather/gson
		else
			pre->left = cur->left;
	}

	free(cur);					// remove item
}


void BST::case3(BTNode* cur) {
	BTNode* is, * isFather;

	// get the IS and IS_parent of current node
	is = isFather = cur->right;
	while (is->left != NULL) {
		isFather = is;
		is = is->left;
	}

	// copy IS node into current node
	cur->item = is->item;

	// Point IS_Father (grandfather) to IS_Child (grandson)
	if (is == isFather)
		cur->right = is->right;		// case 1: There is no IS_Father    
	else
		isFather->left = is->right;	// case 2: There is IS_Father

	// remove IS Node
	free(is);
}

//Assignment question (b)
//deepestNodes function
int BST::getHeight(BTNode* root) {
	int leftHeight, rightHeight;
	if (root == NULL) return 0;
	// Recursively calculate the height of the left and right subtrees.
	leftHeight = getHeight(root->left);
	rightHeight = getHeight(root->right);

	return max(leftHeight, rightHeight) + 1; //height is number of nodes plus 1 for current node
}

bool BST::deepestNodes() {
	if (empty()) return false;
	int max = getHeight(root); // Get the height of the tree.
	cout << "The deepest nodes of tree (Deepest Level: " << max << "): ";
	printDeepestNodes(root, 0, max - 1);
	return true;
}
// Recursive function to print the deepest nodes of the tree.
void BST::printDeepestNodes(BTNode* root, int curLevel, int deepestLevel) {
	if (root == NULL) return;
	if (curLevel == deepestLevel) {//if current level matches deepest level, print ID
		cout << root->item.id << " ";
		return;
	}
	// Recursively explore the left and right subtrees.
	printDeepestNodes(root->left, curLevel + 1, deepestLevel);
	printDeepestNodes(root->right, curLevel + 1, deepestLevel);
}

//Assignment question (c)
//display function
bool BST::display(int order, int source) {
	if (empty()) {
		cout << "\nEmpty tree!\n";
		return false;
	}
	ofstream outfile;
	outfile.open("student-info.txt", ios::trunc);
	if (outfile.fail()) {
		cout << "\nCould not open student-info.txt\n";
		return false;
	}
	// Check the user's choice for order and source and call the appropriate display function.
	if (order == 1 && source == 1) {
		displayAsc(root, cout);
	}
	else if (order == 2 && source == 1) {
		displayDesc(root, cout);
	}
	else if (order == 1 && source == 2) {
		displayAsc(root, outfile);
	}
	else if (order == 2 && source == 2) {
		displayDesc(root, outfile);
	}
	else {
		cout << "\nINVALID OPTION!\n";
		outfile.close();
		return false;
	}
	outfile.close();
	return true;
}
//display ID in ascending order
void BST::displayAsc(BTNode* cur, ostream& output) {
	if (cur == NULL) return;
	displayAsc(cur->left, output);
	cur->item.print(output);
	displayAsc(cur->right, output);
}
//display ID in descending order
void BST::displayDesc(BTNode* cur, ostream& output) {
	if (cur == NULL) return;
	displayDesc(cur->right, output);
	cur->item.print(output);
	displayDesc(cur->left, output);
}

//Assignment question (d)
//CloneSubtree function
bool BST::CloneSubtree(BST t1, type item) {
	if (t1.empty()) { // If the source tree 't1' is empty, cannot clone.
		return false;
	}
	// Find the node with the specified 'item' in tree 't1'.
	BTNode* nodeToClone = findNode(t1.root, item);
	// Clone the subtree rooted at 'nodeToClone' into the current BST.
	clonning(nodeToClone);

	if (empty()) { //subTree is still empty after cloning, return false
		return false;
	}
	cout << "\n **********Tree********** \n";
	t1.preOrderPrint();    // Print the contents of the original tree 't1'.
	cout << "\n **********Subtree********** \n";
	preOrderPrint(); // Print the contents of the cloned subtree in the current BST.
	return true;
}
//find a node with a specific 'item' in the tree.
BTNode* BST::findNode(BTNode* cur, type item) {
	if (cur == NULL) { //item not found
		return cur;
	}
	// Compare the 'item' with the data in the current node.
	if (cur->item.compare2(item)) {
		return cur; //Item found
	}
	else if (cur->item.compare1(item)) {
		// If 'item' is less than the current node's data, search in the left subtree.
		return findNode(cur->left, item);
	}
	else { //else (greater than current, search right subtree
		return findNode(cur->right, item);
	}
}
// Function to clone a subtree rooted at 'cur'.
void BST::clonning(BTNode* cur) {
	if (cur == NULL) return;
	// Insert the current node's item into subTree
	insert(cur->item);
	// Recursively clone the left and right subtrees.
	clonning(cur->left);
	clonning(cur->right);
}

void BST::clear() { //clear subtree's function
	clearRecursive(root); //main operation to clear the tree
	root = NULL;
	count = 0;
}

void BST::clearRecursive(BTNode* cur) {
	if (cur == NULL) {
		return;
	}
	clearRecursive(cur->left);    // Recursively clear the left subtree
	clearRecursive(cur->right);   // Recursively clear the right subtree
	delete cur;    // Delete the current node (cur) to free its memory
}
//Assignment question (e)
//printLevelNodes function
bool BST::printLevelNodes()
{
	if (empty()) return false; //empty tree

	BTNode* cur;
	Queue q;
	int level = 1; //root level = 1
	q.enqueue(root);

	while (!q.empty()) //process nodes while the queue is not empty
	{
		int count = q.size(); //no of nodes at current level
		cout << "Level " << level << " nodes: ";
		while (count > 0)
		{
			q.dequeue(cur);
			cout << cur->item.id << " ";
			if (cur->left != NULL)
			{
				q.enqueue(cur->left); //enqueue left son
			}
			if (cur->right != NULL)
			{
				q.enqueue(cur->right); //enqueue right son
			}
			count--;
		}
		cout << endl;
		level++;
	}
	return true;
}

//Assignment question (f)
//printPath function
bool BST::printPath() {
	string path;

	if (empty()) return false; //empty tree
	cout << "Below are all the external paths for the tree: \n";
	printPathHelper(root, path); // Start the recursive process to print paths from the root
	return true;
}

void BST::printPathHelper(BTNode* node, string path) {

	if (node == NULL) {
		return;
	}

	// Add the current node's id to the path
	path += to_string(node->item.id); //to_string convert an int to a string

	// If it's a leaf node, print the path
	if (node->left == NULL && node->right == NULL) {

		cout << path << endl;
		return;
	}

	// Otherwise, continue recursively for left and right subtrees
	path += " ";
	printPathHelper(node->left, path);
	printPathHelper(node->right, path);
}

