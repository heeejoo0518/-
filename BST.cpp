#include<iostream>

using namespace std;

struct node { 
	int key; 
	node* left; 
	node* right;
};

void insertBST(node* tree, int newKey);
void deleteBST(node* tree, int deleteKey);
void inorder(node* tree);

node* getNode();
int height(node* tree);
int noNodes(node* tree);
node* maxNode(node* tree);
node* minNode(node* tree);
node* Tree;
int main() {
	int* key = new int[16]{ 8,18,11,5,15,4,9,1,7,17,6,14,10,3,19,20 }; 
	Tree = new node{ 0,NULL,NULL };  
	inorder(Tree); 
	cout << endl; 
	//삽입 
	for (int i = 0; i < 16; i++) {  
		insertBST(Tree, key[i]);  
		inorder(Tree);  cout << endl; 
	} 
	//삭제(삽입순서와 동일) 
	for (int i = 0; i < 16; i++) {  
		deleteBST(Tree, key[i]);  
		inorder(Tree);  cout << endl;
	} 
	//삽입2 
	for (int i = 0; i < 16; i++) {  
		insertBST(Tree, key[i]);  
		inorder(Tree);  
		cout << endl; 
	} 
	//삭제2(역순) 
	for (int i = 15; i > -1; i--) {  
		deleteBST(Tree, key[i]);  
		inorder(Tree);  
		cout << endl; 
	} 
	return 0;
}
	node* getNode() { 
		node* newNode; 
		newNode = new node; 
		return newNode; 
	}
	int height(node* tree) { 
		if (tree == NULL) return -1; 
		int left_h = height(tree->left) + 1; 
		int right_h = height(tree->right) + 1; 
		if (left_h > right_h) return left_h;
		else return right_h; 
	}
	int noNodes(node* tree) { 
		if (tree == NULL) return 0;
		return noNodes(tree->left) + noNodes(tree->right) + 1; 
	}
	node* maxNode(node* tree) { 
		if (tree->right == NULL) return tree; 
		return maxNode(tree->right); 
	}
	node* minNode(node* tree) { 
		if (tree->left == NULL) return tree; 
		return minNode(tree->left);
	}
	void inorder(node* tree) { 
		if (tree == NULL) return; 
		inorder(tree->left); 
		if (tree->key != 0) cout << tree->key << " "; 
		inorder(tree->right);
	}
	void insertBST(node* tree, int newKey) { 
		node* p = tree;
		node* q = NULL;
		while (p != NULL) {
			if (newKey == p->key) return; 
			q = p; 
			if (newKey < p->key) p = p->left;  
			else p = p->right; 
		}
		node* newNode = getNode(); 
		newNode->key = newKey; 
		newNode->left = NULL; 
		newNode->right = NULL; 
		if (tree == NULL) {
			tree = newNode;
		} else if (newKey < q->key) q->left = newNode; 
		else q->right = newNode; return;
	}
	
	void deleteBST(node* tree, int deleteKey) {
		node* q = NULL; 
		node* p = Tree; 
		while (p != NULL) { 
			if (deleteKey == p->key) break; 
			q = p;  
			if (deleteKey < p->key) p = p->left; 
			else p = p->right; 
		} if (p == NULL) return; 
		if (q == NULL && p->left == NULL && p->right == NULL) { 
			p->key = NULL; 
			return;
		}
		if (p->left == NULL && p->right == NULL) { 
			if (q->left == p) q->left = NULL;  
			else q->right = NULL; 
		}
		else if (p->left == NULL || p->right == NULL) { 
			if (p->left != NULL) { 
				if (q == NULL) { 
					p->key = p->left->key;    
					p->left = NULL; 
				} else if (q->left == p) q->left = p->left;   
				else q->right = p->left; 
			} else { 
				if (q == NULL) { 
					p->key = p->right->key;    
					p->right = NULL; 
				} else if (q->left == p) q->left = p->right;   
				else q->right = p->right; } }
		else {
			node* r;  bool flag;  
			int left_h = height(p->left);  
			int right_h = height(p->right);
			if (left_h > right_h) { 
				r = maxNode(p->left);   
				flag = true; }
			else if (left_h < right_h) { 
				r = minNode(p->right);  
				flag = false;
			}
			else {
				int left_no = noNodes(p->left);   
				int right_no = noNodes(p->right);   
				if (left_no >= right_no) { r = maxNode(p->left);    
				flag = true;
				} else { 
					r = minNode(p->right);   
					flag = false; 
				}
			}    
			if (flag) deleteBST(p->left, r->key);  
			else deleteBST(p->right, r->key);  p->key = r->key;
		}
	}