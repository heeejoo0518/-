#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

struct BTreeNode {
	int *key;
	BTreeNode **childPtr;
	int n;
};

BTreeNode* getNode();
void inorder(BTreeNode* Tree, int m);
void insert_tempNode(BTreeNode* newnode, int newKey);
void insertBT(BTreeNode* Tree, int m, int newKey);
void deleteBT(BTreeNode* Tree, int m, int oldKey);
bool underflow(int n, int m);
bool existAvailableSibling(BTreeNode* x, BTreeNode* y, int m);
BTreeNode* chooseBestSibling(BTreeNode* x, BTreeNode* y, int m);


BTreeNode* T; BTreeNode* y;

int main() {
	int insert[] = { 30, 20, 62, 110, 140, 15, 65, 136, 150, 120,
	   40, 132, 19, 128, 138, 100, 16, 145, 70, 42,
	   69, 43, 26, 60, 130, 50, 18, 7, 36, 58,
	   22, 41, 59, 57, 54, 33, 75, 124, 122, 123 };

	int deletion[] = { 40, 132, 19, 128, 138, 100, 16, 145, 70, 42,
		22, 41, 62, 110, 140, 15, 65, 124, 122, 123,
		30, 20, 59, 57, 54, 33, 75, 136, 150, 120,
		69, 43, 26, 60, 130, 50, 18, 7, 36, 58 };

	cout << "m=3" << endl;
	T = getNode();
	int m = 3;
	for (int i = 0; i < 40; i++) {
		insertBT(T, m, insert[i]);
		inorder(T, m);
		cout << endl;
	}

	for (int i = 0; i < 40; i++) {
		deleteBT(T, m, deletion[i]);
		inorder(T, m);
		cout << endl;
	}

	cout << "m=4" << endl;
	T = getNode(); y = getNode();//y도 초기화
	m = 4;
	for (int i = 0; i < 40; i++) {
		insertBT(T, m, insert[i]);
		inorder(T, m);
		cout << endl;
	}
	for (int i = 0; i < 40; i++) {
		deleteBT(T, m, deletion[i]);
		inorder(T, m);
		cout << endl;
	}
}

BTreeNode* getNode() {
	BTreeNode* T = new BTreeNode;
	//10까지 데이터 받을 수 있도록 설정
	T->key = new int[10]{};
	T->childPtr = new BTreeNode*[10]{};
	T->n = 0;
	//자식노드 초기화
	for (int i = 0; i < 10; i++) {
		T->childPtr[i] = NULL;
	}
	return T;
}

void inorder(BTreeNode* Tree, int m) {
	if (Tree == NULL) return;
	for (int i = 0; i < Tree->n; i++) {
		inorder(Tree->childPtr[i], m);
		//if (Tree->key[i] != 0) 
		cout << Tree->key[i] << " ";
	}
	inorder(Tree->childPtr[Tree->n], m);
}

void insertBT(BTreeNode* Tree, int m, int newKey) {
	//루트노드 생성
	if (T == NULL) {
		T = getNode();
		T->key[0] = newKey;
		T->n = 1;
		return;
	}

	//x노드의 주소를 스택에 저장하면서 newKey가 삽입될 위치를 탐색한다.
	BTreeNode* x = Tree;
	stack<BTreeNode*> BTreeStack;
	int i;
	do {
		i = 1;
		while (i <= x->n && newKey > x->key[i - 1]) i++;

		//삽입할 키 발견
		if (i <= x->n && newKey == x->key[i - 1]) {
			y = getNode();
			return;
		}

		BTreeStack.push(x);
		x = x->childPtr[i - 1];
		if (x == nullptr) break;
		if (x->n == 0) break;
	} while (true);

	//삽입할 키를 발견 못 한 상태. x=null임.
	//newKey를 B트리에 삽입한다.

	bool finish = false;
	x = BTreeStack.top();
	BTreeStack.pop();

	do {
		if (x->n < m - 1) {
			insert_tempNode(x, newKey);
			y = getNode();
			finish = true;
			break;
		}

		BTreeNode* tempNode = x;
		insert_tempNode(tempNode, newKey);

		//newKey 새로 설정
		newKey = tempNode->key[m / 2];
		y = getNode();
		x->key[m / 2] = 0;
		x->n--;

		//tempNode의 나머지 반절을 y에 삽입
		for (int i = m / 2 + 1; i < m; i++) {
			y->key[i - (m / 2 + 1)] = tempNode->key[i];
			y->childPtr[i - (m / 2 + 1)] = tempNode->childPtr[i];
			y->n++;
			x->key[i] = NULL;
			x->childPtr[i] = NULL;
			x->n--;
		}
		y->childPtr[y->n] = tempNode->childPtr[m];
		x->childPtr[m] = NULL;


		if (!BTreeStack.empty()) {
			x = BTreeStack.top();
			BTreeStack.pop();
		}
		else {//tree level 증가
			T = getNode();
			T->key[0] = newKey;
			T->childPtr[0] = x;
			T->childPtr[1] = y;
			T->n = 1;
			y = getNode();
			finish = true;
		}

	} while (!finish);
}

//newnode에 newkey를 넣고 정렬하는 함수
void insert_tempNode(BTreeNode* newnode, int newKey) {
	//newnode 맨 마지막자리에 newKey,pointer삽입
	newnode->key[newnode->n] = newKey;
	if (y != nullptr) newnode->childPtr[newnode->n + 1] = y;

	newnode->n++;
	int tmp; BTreeNode* ptr;
	for (int i = 0; i < newnode->n; i++) {
		for (int j = i; j < newnode->n; j++) {
			//초기화 과정에서 생긴 key=0를 뒤로 밀어냄
			if (newnode->key[i] == 0) {
				newnode->key[i] = newnode->key[j];
				newnode->key[j] = 0;
			}
			//newnode에 있는 key, pointer sort
			else if (newnode->key[i] > newnode->key[j]) {
				tmp = newnode->key[i];
				ptr = newnode->childPtr[i + 1];
				newnode->key[i] = newnode->key[j];
				newnode->childPtr[i + 1] = newnode->childPtr[j + 1];
				newnode->key[j] = tmp;
				newnode->childPtr[j + 1] = ptr;
			}
		}
	}
}

void deleteBT(BTreeNode* Tree, int m, int oldKey) {
	BTreeNode* x = Tree;
	stack<BTreeNode*> BTreeStack;
	int k;
	do {
		k = 1;
		while (k <= x->n && oldKey > x->key[k - 1]) k++;
		if (k <= x->n && oldKey == x->key[k - 1]) {//삭제할 키 발견
			break;
		}
		BTreeStack.push(x);
		x = x->childPtr[k - 1];
		if (x == nullptr) return;//종료
		if (x->n == 0) return;//종료
	} while (true);

	//자식 노드가 있는지(=내부노드인지 확인)
	bool isLeaf = true;
	for (int i = 0; i <= x->n; i++) {
		if (x->childPtr[i] == NULL) continue;
		else if (x->childPtr[i]->n == 0) continue;
		else if (x->childPtr[i]->key[0] == 0) continue;
		else {
			isLeaf = false;
			break;
		}
	}
	BTreeNode* internalNode = getNode();
	//키가 내부노드에서 발견됐다면(1)
	if (!isLeaf) {
		internalNode = x;
		BTreeStack.push(x);
		x = x->childPtr[k];
		do {

			BTreeStack.push(x);
			x = x->childPtr[0];
			if (x == nullptr) break;
			if (x->n == 0) break;

		} while (true);//x 최종값 = NULL
	}
	// 키가 내부노드에서 발견됐다면(2)
	if (x == nullptr || x->n == 0) {
		x = BTreeStack.top();
		BTreeStack.pop();
		//삭제할 키와 후행키 교환
		int temp = internalNode->key[k - 1];
		internalNode->key[k - 1] = x->key[0];
		x->key[0] = temp;//이제 x->key[0] == oldKey
	}

	bool finish = false;


	//delte oldkey in x node; x는 단말노드.
	//키가 내부노드에서 발견됐다면(3) = oldkey 위치가 교환된 상태.
	if (!isLeaf) {
		x->key[0] = 0; x->n--;
		for (int i = 0; i < x->n; i++) {
			int tmp = x->key[i];
			x->key[i] = x->key[i + 1];
			x->key[i + 1] = tmp;
		}

	}
	//x가 원래 단말노드였음. oldKey 위치 찾아서 sort
	else {
		for (int i = 0; i < x->n; i++) {
			if (x->key[i] == oldKey) {
				x->key[i] = 0; x->n--;
				for (int j = i; j < x->n; j++) {
					int tmp = x->key[j];
					x->key[j] = x->key[j + 1];
					x->key[j + 1] = tmp;
				}

			}
		}
	}


	if (!BTreeStack.empty()) {
		//y는 x의 부모노드
		y = BTreeStack.top();
		BTreeStack.pop();
	}

	do {
		//x가 root거나 underflow가 아닐 때
		if (T == x || !underflow(x->n, m)) {
			finish = true;
		}
		//bestSibling과 재분배
		else if (existAvailableSibling(x, y, m)) {
			BTreeNode* bestSibling = chooseBestSibling(x, y, m);

			//tempNode : 정상보다 50% 큰 Node
			BTreeNode* tempNode;
			BTreeNode* left = NULL;
			BTreeNode* right = NULL;

			int k;
			for (k = 0; k < m; k++) {
				//bestSibling이 왼쪽,flag==true
				if (y->childPtr[k] == bestSibling) {
					left = bestSibling;
					right = x;
					break;
				}
				//x가 왼쪽,flag==false
				else if (y->childPtr[k] == x) {
					left = x;
					right = bestSibling;
					break;
				}

			}


			//tempNode에 left 넣음
			tempNode = left;
			tempNode->key[tempNode->n] = y->key[k];
			tempNode->n++;

			for (int i = 0; i < right->n; i++) {
				tempNode->childPtr[tempNode->n] = right->childPtr[i];
				tempNode->key[tempNode->n] = right->key[i];
				tempNode->n++;
			}
			tempNode->childPtr[tempNode->n] = right->childPtr[right->n];


			//중간값을 y에 넣는다.
			int mid = tempNode->n;
			mid /= 2;
			y->key[k] = tempNode->key[mid];
			tempNode->key[mid] = 0;


			//left,right 노드 재분배
			int node_num = tempNode->n;
			tempNode->n--;
			right->n = 0;
			for (int i = mid + 1; i < node_num; i++) {
				right->key[i - (mid + 1)] = tempNode->key[i];
				right->childPtr[i - (mid + 1)] = tempNode->childPtr[i];
				right->n++;
			}
			right->childPtr[right->n] = tempNode->childPtr[node_num];

			left->n = 0;
			for (int i = 0; i < mid; i++) {
				left->key[i] = tempNode->key[i];
				left->childPtr[i] = tempNode->childPtr[i];
				left->n++;
			}
			left->childPtr[left->n] = tempNode->childPtr[mid];

			finish = true;
		}
		//bestSibling과 합병
		else {
			BTreeNode* bestSibling = chooseBestSibling(x, y, m);
			BTreeNode* left = NULL;
			BTreeNode* right = NULL;

			int k;
			for (k = 0; k < m; k++) {
				//bestSibling이 왼쪽,flag==true
				if (y->childPtr[k] == bestSibling) {
					left = bestSibling;
					right = x;
					break;
				}
				//x가 왼쪽,flag==false
				else if (y->childPtr[k] == x) {
					left = x;
					right = bestSibling;
					break;
				}
			}

			left->key[left->n] = y->key[k];
			y->key[k] = 0;
			left->n++;

			//y 새로 정렬
			for (int j = k; j < y->n; j++) {
				int tmpKey; BTreeNode* tmpNode;
				tmpKey = y->key[j];
				tmpNode = y->childPtr[j + 1];
				y->key[j] = y->key[j + 1];
				y->childPtr[j + 1] = y->childPtr[j + 2];
				y->key[j + 1] = tmpKey;
				y->childPtr[j + 2] = tmpNode;
			}
			y->childPtr[y->n + 1] = NULL;
			y->n--;

			//오른쪽 노드를 왼쪽 노드에 넣음
			for (int i = 0; i < right->n; i++) {
				left->childPtr[left->n] = right->childPtr[i];
				left->key[left->n] = right->key[i];
				left->n++;

			}
			left->childPtr[left->n] = right->childPtr[right->n];


			if (!BTreeStack.empty()) {
				x = y;
				y = BTreeStack.top();
				BTreeStack.pop();
			}
			else { //BTreeStack.empty() 
				finish = true;
				if (y->n == 0) {
					T = left;
				}
			}

		}
	} while (!finish);
}

bool underflow(int n, int m) {
	if (m / 2 != 0) { //홀수인 경우 m+1
		m++;
	}
	if (n < (m / 2 - 1)) return true; //underflow
	return false;
}
bool existAvailableSibling(BTreeNode* x, BTreeNode* y, int m) {
	bool result = false;
	int i = 0;
	for (i; i < m; i++) {
		if (y->childPtr[i] == x) break;
	}
	if (i - 1 > 0 && y->childPtr[i - 1] != NULL) {
		//n값에서 하나 빼고 underflow 확인
		if (!underflow(y->childPtr[i - 1]->n - 1, m)) result = true;
	}
	if (i + 1 <= m && y->childPtr[i + 1] != NULL) {
		//n값에서 하나 빼고 underflow 확인
		if (!underflow(y->childPtr[i + 1]->n - 1, m)) result = true;
	}
	return result;
}
BTreeNode* chooseBestSibling(BTreeNode* x, BTreeNode* y, int m) {
	int i = 0;
	BTreeNode* left = getNode();
	BTreeNode* right = getNode();
	bool checkUnderflow_left = true;
	bool checkUnderflow_right = true;

	for (i; i < m; i++) {
		if (y->childPtr[i] == x) break;
	}
	if (i - 1 >= 0 && y->childPtr[i - 1] != NULL) {
		//n값에서 하나 빼고 underflow 확인
		checkUnderflow_left = underflow(y->childPtr[i - 1]->n - 1, m);
		left = y->childPtr[i - 1];
	}
	if (i + 1 <= m && y->childPtr[i + 1] != NULL) {
		//n값에서 하나 빼고 underflow 확인
		checkUnderflow_right = underflow(y->childPtr[i + 1]->n - 1, m);
		right = y->childPtr[i + 1];
	}
	if (checkUnderflow_left == true && checkUnderflow_right == true) {
		//둘 다 underflow 발생. 노드 합병할 sibling 선택
		if (left->n == 0) return right;
		if (right->n == 0) return left;

		if (m / 2 == 0) {
			//노드의 수가 m/2인 쪽을 선택
			if (left->n / 2 == m / 2) return left;
			else return right;
		}
		return left;
	}
	else if (checkUnderflow_left == false && checkUnderflow_right == false) {
		if (right->n >= left->n) return right;
		else return left;
	}
	else if (checkUnderflow_left == false) {
		return left;
	}
	else { //(checkUnderflow_right == false) 
		return right;
	}
}