#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RBtree.h"
#include "path.h"

void RB_INSERT1(Node* z);
void RB_DELETE1(Node* x);
void Transplant(Node* u, Node* v);
Node* minimum(Node* x);

Node* root;
Node* NILL;
static int nodeNum;

int getNodeNum() {
	return nodeNum;
}

int isExist(int rsv_num) {
	Node* temp = root;
	int diff;
	while (temp != NILL) {
		diff = rsv_num - temp->rsv_num;
		if (diff > 0)
			temp = temp->rightChild;
		else if (diff < 0)
			temp = temp->leftChild;
		else
			return 1;
	}
	return 0;
}

void LR(Node* x) {
	Node* y;

	y = x->rightChild;
	x->rightChild = y->leftChild;
	if (y->leftChild != NILL)  y->leftChild->parent = x;

	y->parent = x->parent;
	if (y->parent == NILL) root = y;
	else if (x == x->parent->leftChild) x->parent->leftChild = y;
	else x->parent->rightChild = y;

	y->leftChild = x;
	x->parent = y;
}

void RR(Node* x) {
	Node* y;

	y = x->leftChild;
	x->leftChild = y->rightChild;
	if (y->rightChild != NILL)  y->rightChild->parent = x;


	y->parent = x->parent;
	if (y->parent == NILL)  root = y;
	else if (x == x->parent->leftChild) x->parent->leftChild = y;
	else  x->parent->rightChild = y;

	y->rightChild = x;
	x->parent = y;
}

void RB_INSERT(int rsv_num, char* name, int s, int d, int date, int seatLv) {
	Node* z, *x, *y;
	z = (Node*)malloc(sizeof(Node));

	z->rsv_num = rsv_num;
	z->name = name;
	z->s = s;
	z->d = d;
	z->date = date;
	z->seatLv = seatLv;
	z->color = 'R';
	z->leftChild = NILL;
	z->rightChild = NILL;

	x = root;
	y = NILL;

	nodeNum++;
	
	while (x != NILL) {
		y = x;
		if (z->rsv_num <= x->rsv_num) x = x->leftChild;
		else x = x->rightChild;
	}

	if (y == NILL) root = z;
	else if (z->rsv_num <= y->rsv_num) y->leftChild = z;
	else y->rightChild = z;

	z->parent = y;

	RB_INSERT1(z);
}

void RB_INSERT1(Node* z) {
	while (z->parent->color == 'R') {

		if (z->parent == z->parent->parent->leftChild) {

			if (z->parent->parent->rightChild->color == 'R') {
				z->parent->color = 'B';
				z->parent->parent->rightChild->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;
			}

			else {

				if (z == z->parent->rightChild) {
					z = z->parent;
					LR(z);
				}

				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				RR(z->parent->parent);
			}
		}

		else {
			if (z->parent->parent->leftChild->color == 'R') {
				z->parent->color = 'B';
				z->parent->parent->leftChild->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;
			}

			else {
				if (z == z->parent->leftChild) {
					z = z->parent;
					RR(z);
				}

				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				LR(z->parent->parent);
			}
		}
	}

	root->color = 'B';
}

void RB_DELETE(int rsv_num) {
	Node* y, *x, *z;
	z = root;
	while (z != NILL && rsv_num != z->rsv_num) {
		if (z->rsv_num > rsv_num)
			z = z->leftChild;
		else
			z = z->rightChild;
	}

	char yColor1;
	y = z;
	yColor1 = y->color;

	if (isExist(rsv_num)) {
		printf("\n [System]: Your reservation has been cancelled.\n");
		nodeNum--;
	}
	else {
		printf("\n [System]: We can't find your reservation. Please check your reservation number again.\n");
		return;
	}

	if (z->leftChild == NILL) {
		x = z->rightChild;
		Transplant(z, z->rightChild);
	}
	else if (z->rightChild == NILL) {
		x = z->leftChild;
		Transplant(z, z->leftChild);
	}
	else {
		y = minimum(z->rightChild);
		yColor1 = y->color;
		x = y->rightChild;

		if (y->parent == z) x->parent = y;
		else {
			Transplant(y, y->rightChild);
			y->rightChild = z->rightChild;
			y->rightChild->parent = y;
		}

		Transplant(z, y);
		y->leftChild = z->leftChild;
		y->leftChild->parent = y;
		y->color = z->color;
	}

	if (yColor1 == 'B') {
		RB_DELETE1(x);
	}
}

void RB_DELETE1(Node* x) {
	Node* w;

	while (x != root && x->color == 'B') {
		if (x == x->parent->leftChild) {
			w = x->parent->rightChild;

			if (w->color == 'R') {
				w->color = 'B';
				x->parent->color = 'R';
				LR(x->parent);
				w = x->parent->rightChild;
			}

			if (w->leftChild->color == 'B' && w->rightChild->color == 'B') {
				w->color = 'R';
				x->parent->color = 'B';
				x = x->parent;
			}
			else {

				if (w->rightChild->color == 'B') {
					w->color = 'R';
					w->leftChild->color = 'B';
					RR(w);
					w = x->parent->rightChild;
				}

				w->color = x->parent->color;
				x->parent->color = 'B';
				w->rightChild->color = 'B';
				LR(x->parent);
				x = root;
			}

		}
		else {
			w = x->parent->leftChild;

			if (w->color == 'R') {
				w->color = 'B';
				x->parent->color = 'B';
				RR(x->parent);
				w = x->parent->leftChild;
			}

			if (w->leftChild->color == 'B' && w->rightChild->color == 'B') {
				w->color = 'R';
				x->parent->color = 'B';
				x = x->parent;
			}
			else {
				if (w->leftChild->color == 'B') {
					w->color = 'R';
					w->rightChild->color = 'B';
					LR(w);
					w = x->parent->leftChild;
				}

				w->color = x->parent->color;
				x->parent->color = 'B';
				w->leftChild->color = 'B';
				RR(x->parent);
				x = root;
			}
		}
	}
	x->color = 'B';
}

void PRINT_RBT(int rsv_num) {
	Node* x = Search(rsv_num);
	char* seatLv;
	printf("\n------------Reservation Information------------\n");
	printf("- Reserver's name: %s\n- Reservation number: %d\n", x->name, x->rsv_num);
	printf("- Departure city: %c\n- Destination city : %c\n", x->s, x->d);
	printf("- Date of departure: 2019/12/%d\n", x->date);
	if (x->seatLv == 1) seatLv = "First class";
	else if (x->seatLv == 2) seatLv = "Business class";
	else seatLv = "Economy class";
	printf("- Level of seat : %s\n", seatLv);
}

void Transplant(Node* u, Node* v) {
	if (u->parent == NILL) root = v;
	else if (u == u->parent->leftChild) u->parent->leftChild = v;
	else u->parent->rightChild = v;

	v->parent = u->parent;
}


Node* Search(int rsv_num) {
	Node* x;
	x = root;
	while (x != NILL && x->rsv_num != rsv_num) {
		if (rsv_num < x->rsv_num) x = x->leftChild;
		else x = x->rightChild;
	}

	return x;
}

Node* minimum(Node* x) {
	while (x->leftChild != NILL) {
		x = x->leftChild;
	}
	return x;
}

int RBTHeight(Node* x) {
	if (x == NILL)
		return 0;
	else {
		int left_side;
		int right_side;
		left_side = RBTHeight(x->leftChild);
		right_side = RBTHeight(x->rightChild);
		if (left_side > right_side)
			return left_side + 1;
		else
			return right_side + 1;
	}
}

void printNode(int prevHeight, int prevNodes) {
	int height = RBTHeight(root);
	printf("-----------------------------------------------\n");
	printf(" Before>\n- Number of nodes : %d\n- Height of the tree: %d\n\n", prevNodes, prevHeight);
	printf(" After>\n- Number of nodes : %d\n- Height of the tree: %d\n\n", nodeNum, height);
} 
