#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _node {
	char color;
	char* name;
	int rsv_num;
	int s, d, date;
	struct _node *parent;
	struct _node *leftChild;
	struct _node *rightChild;
}Node;

void RB_INSERT1(Node* z);
void RB_DELETE1(Node* x);
void Transplant(Node* u, Node* v);

int printFlag = 0;
Node* RBT_root;
Node* NILL;

int isExist(int rsv_num) {
	Node* temp = RBT_root;
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
	if (y->parent == NILL) RBT_root = y;
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
	if (y->parent == NILL)  RBT_root = y;
	else if (x == x->parent->leftChild) x->parent->leftChild = y;
	else  x->parent->rightChild = y;

	y->rightChild = x;
	x->parent = y;
}

void RB_INSERT(int rsv_num,char* name, int s, int d, int date) {
	Node* z, *x, *y;
	z = (Node*)malloc(sizeof(Node));

	z->rsv_num = rsv_num;
	z->name = name;
	z->s = s;
	z->d = d;
	z->date = date;
	z->color = 'R';
	z->leftChild = NILL;
	z->rightChild = NILL;

	x = RBT_root;
	y = NILL;

	if (isExist(rsv_num) == 1 && printFlag) {
		printf("%d exists in the tree. Insertion ignored!", rsv_num);
		return;
	}
	else if (isExist(rsv_num) == 0 && printFlag)
		printf("insertion of %d succeed! \n", rsv_num);

	while (x != NILL) {
		y = x;
		if (z->rsv_num <= x->rsv_num) x = x->leftChild;
		else x = x->rightChild;

	}

	if (y == NILL) RBT_root = z;
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

	RBT_root->color = 'B';
}

void RB_DELETE(int rsv_num) {
	Node* y, *x, *z;
	z = RBT_root;
	while (z != NILL && rsv_num != z->rsv_num) {
		if (z->rsv_num > rsv_num)
			z = z->leftChild;
		else
			z = z->rightChild;
	}

	char yColor1;
	y = z;
	yColor1 = y->color;

	if (isExist(rsv_num) == 0 && printFlag) {
		printf("%d is not in the tree. Deletion ignored.", rsv_num);
		return;
	}
	else if (isExist(rsv_num) == 1 && printFlag)
		printf("deletion of %d is succeed!\n", rsv_num);

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

	while (x != RBT_root && x->color == 'B') {
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
				x = RBT_root;
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
				x = RBT_root;
			}
		}
	}
	x->color = 'B';
}

void PRINT_RBT(Node* x) {
	if (x != NILL) {
		PRINT_RBT(x->leftChild);
		printf("%s's reservation number is %d\n",x->name, x->rsv_num);
		PRINT_RBT(x->rightChild);
	}
}

void Transplant(Node* u, Node* v) {
	if (u->parent == NILL) RBT_root = v;
	else if (u == u->parent->leftChild) u->parent->leftChild = v;
	else u->parent->rightChild = v;

	v->parent = u->parent;
}


Node* Search(int rsv_num) {
	Node* x;

	x = RBT_root;
	while (x != NILL && x->rsv_num != rsv_num) {
		if (rsv_num < x->rsv_num) x = x->leftChild;
		else x = x->rightChild;
	}
	return x;
}



