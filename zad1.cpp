#pragma once
#include <iostream>
using namespace std;

class G_StackEmpty {};

class Node {

	int info;
	Node* parent;
	Node **children;

	Node() {}
	Node(const Node& n) {}
	Node(Node&& n) {}

public:

	Node(int info, int n) {
		this->info = info;
		children = new Node*[n];
		for (int i = 0; i < n; i++) {
			children[i] = nullptr;
		}
		this->parent = nullptr;
	}

	Node(int info, int n, Node* root) {
		this->info = info;
		children = new Node*[n];
		for (int i = 0; i < n; i++) {
			children[i] = nullptr;
		}
		this->parent = root;
	}

	~Node() { deleteNode(); }			// brise samo taj cvor ,ne i njegovu decu 


	Node** getChildren() { return children; }

	int getInfo() { return info; }

	void setRoot(Node* n) { parent = n; }

	void printNode() { cout << info << " "; }

	void deleteNode() {
		parent = nullptr;
		delete parent;

		delete[] children;
		children = nullptr;
	}
};

class Stack {

	struct Elem {
		Node *info;
		Elem *next;

		Elem(Node* info) {
			this->info = info;
			next = nullptr;
		}
	};

	Elem *bottom, *top;

	Stack(const Stack&) {}
	Stack(Stack&&) {}

public:

	Stack() { bottom = top = nullptr; }
	~Stack() { delete[] top; top = bottom = nullptr; }


	void PUSH(Node* node) {
		Elem *p = new Elem(node);
		if (top == nullptr) {
			top = bottom = p;
		}
		else {
			p->next = top;
			top = p;
		}
	}

	Node* POP() {
		if (top == nullptr) {
			throw new G_StackEmpty();
		}
		else {
			Elem* e = top;
			top = top->next;
			e->next = nullptr;
			Node* x = e->info;
			delete e;
			return x;
		}
	}

	int STACK_EMPTY() {
		if (top == nullptr) return 1;
		else return 0;
	}


};



class intStack {

	struct Elem {
		int num;
		Elem *next;
		Elem(int n) {
			num = n;
			next = nullptr;
		}
	};

	Elem *bottom, *top;

public:
	intStack() {
		bottom = top = nullptr;
	}

	~intStack() { delete[] top; top = bottom = nullptr; }


	void PUSH(int n) {
		Elem *p = new Elem(n);
		if (top == nullptr) {
			top = bottom = p;
		}
		else {
			p->next = top;
			top = p;
		}
	}

	int POP() {
		if (top == nullptr) {
			throw new G_StackEmpty();
		}
		else {
			Elem* e = top;
			top = top->next;
			e->next = nullptr;
			int x = e->num;
			delete e;
			return x;
		}
	}

	int STACK_EMPTY() {
		if (top == nullptr) return 1;
		else return 0;
	}




};







class Tree {

	int m;
	Node *root;

	Tree(const Tree& s) {}
	Tree(const Tree&& s) {}

	int treeDepth();


public:

	Tree(int m) {
		this->m = m;
		root = nullptr;

	}
	~Tree() { deleteTree(); }

	Node* getRoot() { return root; }
	int getM() { return m; }
	void putNode(int);
	void deleteTree();
	void printTree();
	int treeWidth();


};




void Tree::deleteTree() {
	Stack *s = new Stack();
	s->PUSH(root);
	while (!s->STACK_EMPTY()) {
		Node* next = s->POP(); //Treba da se proveri da li je stek prazan ali se to nikad ne desava
		while (next != nullptr) {
			for (int i = m - 1; i > 0; i--) {
				if (next->getChildren()[i] != nullptr) {
					s->PUSH(next->getChildren()[i]);
				}
			}
			if (next->getChildren()[0] == nullptr) {
				next->deleteNode();
				next = nullptr;
			}
			else {
				Node *p = next;
				next = next->getChildren()[0];
				p->deleteNode();
			}
		}
	}
	delete s;
}

void Tree::putNode(int info) {

	if (root == nullptr) {
		root = new Node(info, m);
		return;
	}
	int depth = treeDepth();
	intStack *s1 = new intStack();
	Stack *s = new Stack();
	int curr_level = 0;
	Node* next = nullptr;

	s->PUSH(root);
	s1->PUSH(0);
	while (!s->STACK_EMPTY()) {
		next = s->POP();
		curr_level = s1->POP();

		while (next != nullptr) {
			if (next == root) {
				for (int i = 0; i < m; i++) {
					if (next->getChildren()[i] == nullptr) {
						next->getChildren()[i] = new Node(info, m, next);
						return;
					}
				}
			}

			if (curr_level == depth - 1) {
				for (int i = 0; i < m; i++) {
					if (next->getChildren()[i] == nullptr) {
						next->getChildren()[i] = new Node(info, m, next);
						return;
					}
				}
				break;
			}
			else {
				for (int i = m - 1; i > 0; i--) {
					if (next->getChildren()[i] != nullptr) {
						s->PUSH(next->getChildren()[i]);
						s1->PUSH(curr_level + 1);
					}
				}
				next = next->getChildren()[0];
				curr_level++;
			}
		}
	}
	next = root;
	while (next->getChildren()[0] != nullptr) next = next->getChildren()[0];
	next->getChildren()[0] = new Node(info, m, next);
}

void Tree::printTree() {
	cout << endl << "Ispis stabla po preorder poretku: " << endl;
	Stack *s = new Stack();
	s->PUSH(root);
	while (!s->STACK_EMPTY()) {
		Node* next = s->POP();
		while (next != nullptr) {
			next->printNode();
			for (int i = m - 1; i > 0; i--) {
				if (next->getChildren()[i] != nullptr) {
					s->PUSH(next->getChildren()[i]);
				}
			}
			next = next->getChildren()[0];
		}
	}
}

int Tree::treeWidth() {
	int width = 0;
	if (root == nullptr) {
		return 0;
	}
	else {
		int depth = treeDepth();
		int curr_level = 0;
		intStack *s1 = new intStack();
		Stack *s = new Stack();
		s->PUSH(root);
		s1->PUSH(0);
		int cnt = 0;

		width = m ^ (depth - 1); //ovo mozda ne radi

		while (!s->STACK_EMPTY()) {
			Node* next = s->POP();
			curr_level = s1->POP();

			while (next != nullptr) {
				if (next == root) {
					for (int i = 0; i < m; i++) {
						if (next->getChildren()[i] == nullptr) break;
						cnt++;
					}
					if (cnt != m && cnt != 0) return cnt;
					else if (cnt == 0) return 1;
					cnt = 0;
				}

				if (curr_level == depth - 1) {
					for (int i = 0; i < m; i++) {
						if (next->getChildren()[i] != nullptr) {
							cnt++;
						}
						else {
							return cnt > width ? cnt : width;
						}
					}
					break;
				}
				else {
					for (int i = m - 1; i > 0; i--) {
						if (next->getChildren()[i] != nullptr) {
							s->PUSH(next->getChildren()[i]);
							s1->PUSH(curr_level + 1);
						}
					}
					next = next->getChildren()[0];
					curr_level++;
				}
			}
		}
		return cnt;
	}

}

int Tree::treeDepth() {
	int x = 0;
	if (root != nullptr) {
		Node *next = root->getChildren()[0];
		while (next) {
			x++;
			next = next->getChildren()[0];
		}
	}
	return x;
}



int main() {

	int uslov = 1;
	int brStabala = 0;
	Tree *stablo = nullptr;
	while (uslov) {
		cout << endl << "0.Kraj programa" << endl << "1.Kreiraj stablo" << endl << "2.Ubaci cvor" << endl << "3.Ispisi stablo" << endl << "4.Obrisi stablo" << endl << "5.Sirina stabla" << endl;
		int k;
		cin >> k;
		if (k < 0 || k > 5) {
			cout << endl << "***Pogresan izbor***" << endl;
			continue;
		}
		switch (k) {
		case 0:
			uslov = 0;
			break;

		case 1:
			if (brStabala == 1) { cout << "***Vec postoji stablo reda: " << stablo->getM() << " ***" << endl; break; }
			cout << "Red stabla? ";
			int m;
			cin >> m;
			if (m < 2) { cout << endl << "***Red stabla mora biti veci ili jednak od 2***" << endl; break; }
			stablo = new Tree(m);
			brStabala++;
			break;
		case 2: {
			if (stablo == nullptr) {
				cout << endl << "***Stablo ne postoji***" << endl;

			}
			else {
				cout << "Vrednost novog cvora? ";
				int n;
				cin >> n;
				stablo->putNode(n);
			}break;
		}
		case 3: {
			if (stablo == nullptr) { cout << endl << "***Stablo ne postoji***" << endl; break; }
			else {
				if (stablo->getRoot() == nullptr) {
					cout << endl << "***Stablo je prazno***" << endl; break;
				}
			}
			stablo->printTree();
			break;
		}
		case 4: {
			if (stablo == nullptr) { cout << endl << "***Stablo ne postoji***" << endl; break; }
			stablo->deleteTree();
			stablo = nullptr;
			brStabala--;
			break;
		}
		case 5: {
			if (stablo == nullptr) {
				cout << endl << "***Stablo ne postoji***" << endl;

			}
			else {
				if (stablo->getRoot() == nullptr) {
					cout << endl << "***Stablo je prazno***" << endl; break;
				}

				cout << endl << "Trenutna sirina stabla je: " << stablo->treeWidth() << endl;;
			}

		}
		}
	}
}
