#pragma once
#include<iostream>
using namespace std;


class G_StackEmpty {};

class Resetka {

	char **matrica;

public:
	Resetka() {
		matrica = new char*[5];
		for (int i = 0; i < 5; i++) {
			matrica[i] = new char[5];
		}
		cout << endl << "Elementi matrice? " << endl;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cin >> matrica[i][j];
			}
		}
	}

	Resetka(const Resetka& r) {
		matrica = new char*[5];
		for (int i = 0; i < 5; i++) {
			matrica[i] = new char[5];
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				matrica[i][j] = r.matrica[i][j];
			}
		}
	}

	~Resetka() { deleteR(); }

	void deleteR() {
		for (int i = 0; i < 5; i++) {
			delete[] matrica[i];
		}
		delete[] matrica;
	}


	Resetka* ubaci(char c, int j) {
		for (int i = 4; i >= 0; i--) {
			if (matrica[i][j] != 'P' && matrica[i][j] != 'C') {
				matrica[i][j] = c;
				return this;
			}
		}

		return this;
	}

	int slobodno(int j) {
		for (int i = 0; i < 5; i++) {
			if (matrica[i][j] != 'P' && matrica[i][j] != 'C') {
				return 1;
			}
		}
		return 0;
	}

	char naPotezu() {
		int k = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (matrica[i][j] == 'P' || matrica[i][j] == 'C') k++;
			}
		}
		if (k % 2 == 0) return 'P';
		else return 'C';
	}

	int jelKraj() {
		int k = 0;

		//Po horizontali
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				if (matrica[i][j] == matrica[i][j + 1] && matrica[i][j] == matrica[i][j + 2] && matrica[i][j + 1] == matrica[i][j + 2]) {
					if (matrica[i][j] == 'P') return 1;
					else if (matrica[i][j] == 'C') return 2;
				}
			}
		}

		//Po vertikali
		for (int j = 0; j < 5; j++) {
			for (int i = 0; i < 3; i++) {
				if (matrica[i][j] == matrica[i + 1][j] && matrica[i][j] == matrica[i + 2][j] && matrica[i + 1][j] == matrica[i + 2][j]) {
					if (matrica[i][j] == 'P') return 1;
					else if (matrica[i][j] == 'C') return 2;
				}
			}
		}
		//Po dijagonali:
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (matrica[i][j] == matrica[i + 1][j + 1] && matrica[i][j] == matrica[i + 2][j + 2] && matrica[i + 1][j + 1] == matrica[i + 2][j + 2]) {
					if (matrica[i][j] == 'P') return 1;
					else if (matrica[i][j] == 'C') return 2;
				}
			}
		}
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
				if (matrica[i][j] == matrica[i + 1][j + 1] == matrica[i + 2][j + 2]) {
					if (matrica[i][j] == 'P') return 1;
					else if (matrica[i][j] == 'C') return 2;
				}
			}
		}
		//nereseno:
		int s = 0;
		for (int j = 0; j < 5; j++) {
			if (!slobodno(j)) s++;
		}
		if (s == 5)return 3;

		return 0;

	}

	void ispisi() {
		cout << endl;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cout << matrica[i][j] << " ";
			}
			cout << endl;
		}
	}

};

class Node {

	int info;
	Resetka *stanjeIgre;
	Node* parent;
	Node **children;

	Node() {}
	Node(const Node& n) {}
	Node(Node&& n) {}

public:


	Node(int info, int n, Resetka* s) {
		this->info = info;
		children = new Node*[n];
		stanjeIgre = s;
		for (int i = 0; i < n; i++) {
			children[i] = nullptr;
		}
		this->parent = nullptr;
	}
	Node(int info, int n, Node* parent, Resetka* s) {
		this->info = info;
		children = new Node*[n];
		stanjeIgre = s;
		for (int i = 0; i < n; i++) {
			children[i] = nullptr;
		}
		this->parent = parent;
	}

	~Node() { deleteNode(); }			// brise samo taj cvor ,ne i njegovu decu 

	Resetka* getResetka() { return stanjeIgre; }
	Node** getChildren() { return children; }
	int getInfo() { return info; }
	Node *getParent() { return parent; }

	void setInfo(int n) { info = n; }

	//char getState() { return stanje_igre; }

	void printNode() { stanjeIgre->ispisi(); cout << "info: " << info << endl; }

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





class stabloIgre {

	int m;
	Node* root;
	int brCvorova;



public:
	stabloIgre() { m = 5; brCvorova = 0; }

	~stabloIgre() { deleteTree(); }

	int getBrCvorova() { return brCvorova; }

	Node* getRoot() { return root; }

	void kreiraj(Resetka* state);

	void ispisiStablo();

	void deleteTree();



};

void stabloIgre::kreiraj(Resetka* state) {
	int k = 0;
	root = new Node(3, m, state);

	Stack *s1 = new Stack();
	Stack *s = new Stack();
	brCvorova++;
	Node *next = nullptr;
	s->PUSH(root);
	while (!s->STACK_EMPTY()) {
		next = s->POP();
		while (next != nullptr) {
			if (!(k = next->getResetka()->jelKraj())) {
				for (int i = 0; i < m; i++) {
					if (next->getResetka()->slobodno(i)) {
						Resetka *r = new Resetka(*next->getResetka());
						next->getChildren()[i] = new Node(3, m, next, r->ubaci(next->getResetka()->naPotezu(), i));
						brCvorova++;
					}
				}
				for (int i = m - 1; i > 0; i--) {
					if (next->getChildren()[i] != nullptr) {
						s->PUSH(next->getChildren()[i]);
					}
				}
				next = next->getChildren()[0];
			}
			else {
				switch (k) {
				case 1: next->setInfo(1); break;
				case 2: next->setInfo(-1); break;
				case 3: next->setInfo(0); break;
				}
				s1->PUSH(next);
				next = nullptr;
			}
		}
	}
	while (!s1->STACK_EMPTY()) {
		next = s1->POP();
		next = next->getParent();
		while (next != nullptr) {
			int q = 0;
			if (next->getResetka()->naPotezu() == 'P') {
				q = -1;
				for (int i = 0; i < m; i++) {
					if (next->getChildren()[i] && next->getChildren()[i]->getInfo() > q && next->getChildren()[i]->getInfo() < 3) q = next->getChildren()[i]->getInfo();
				}
			}
			else {
				q = 1;
				for (int i = 0; i < m; i++) {
					if (next->getChildren()[i] && next->getChildren()[i]->getInfo() < q && next->getChildren()[i]->getInfo() < 3) q = next->getChildren()[i]->getInfo();
				}
			}
			next->setInfo(q);
			next = next->getParent();
		}
	}
}

void stabloIgre::ispisiStablo() {
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

void stabloIgre::deleteTree() {
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
				Node* p = next;
				next = next->getChildren()[0];
				p->deleteNode();
			}
		}
	}
}


int main() {
	int uslov = 1;
	stabloIgre *stablo_igre = nullptr;
	Resetka *pocetnoStanje = nullptr;
	Node* trenutniCvor = nullptr;
	while (uslov) {
		cout << endl << "0.Kraj programa" << endl << "1.Zapocinjanje nove igre i formiranje stabla" << endl << "2.Ispis stabla igre" << endl << "3.Ispisi broj cvorova" << endl;// , duzinu i sirinu stabla"<<endl;
		int izbor;
		cin >> izbor;

		switch (izbor) {
		case 0:
			uslov = 0;
			break;
		case 1:
			if (stablo_igre != nullptr) {
				stablo_igre->deleteTree();
				stablo_igre = nullptr;
			}
			cout << "Matricu popuniti elementim 'P' ili 'C' (potez odigran od strane plavog,odnosno crvenog igraca) ili nekim drugim KARAKTEROM (predstavlja prazno mesto)" << endl;
			pocetnoStanje = new Resetka();
			cout << "Kreiranje stabla u toku..." << endl;
			stablo_igre = new stabloIgre();
			stablo_igre->kreiraj(pocetnoStanje);
			cout << "Kreiranje stabla je gotovo" << endl;
			switch (pocetnoStanje->naPotezu()) {
			case 'C': cout << "Prvi "; break;
			case 'P': cout << "Prvi "; break;
			}

			trenutniCvor = stablo_igre->getRoot();
			while (!trenutniCvor->getResetka()->jelKraj()) {
				switch (trenutniCvor->getResetka()->naPotezu()) {
				case 'C':cout << "igra crveni igrac" << endl << endl; break;
				case 'P':cout << "igra plavi igrac" << endl << endl; break;
				}
				cout << "Trenutno stanje igre:";
				trenutniCvor->printNode();
				cout << "Zeljena kolona? ";
				int j;
				cin >> j;
				while (j > 4 || j < 0) {
					cout << "***Ta kolona ne postoji***" << endl << "Probaj opet: ";
					cin >> j;
				}
				while (!trenutniCvor->getChildren()[j]) {
					cout << "***Ta kolona je popunjena***" << endl << "Probaj opet: ";
					cin >> j;
					while (j > 4 || j < 0) {
						cout << "***Ta kolona ne postoji***" << endl << "Probaj opet: ";
						cin >> j;
					}
				}
				trenutniCvor = trenutniCvor->getChildren()[j];
				cout << endl << endl;
			}
			switch (trenutniCvor->getResetka()->naPotezu()) {
			case 'C':cout << "Pobedio je plavi igrac" << endl; break;
			case 'P':cout << "Pobedio je crveni igrac" << endl; break;
			}

			break;
		case 2:
			if (stablo_igre == nullptr) { cout << "***Stablo nije formirano***"; break; }
			stablo_igre->ispisiStablo();
			break;
		case 3:
			if (stablo_igre == nullptr) { cout << "***Stablo nije formirano***"; break; }
			cout << endl << "Broj cvorova: " << stablo_igre->getBrCvorova() << endl;
			break;
		default:
			cout << endl << "***Pogresan izbor***" << endl;
			break;
		}

	}

}