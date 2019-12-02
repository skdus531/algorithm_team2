#pragma once
#ifndef _node
typedef struct _node {
	char color;
	char* name;
	int rsv_num, s, d, date, seatLv;
	
	struct _node *parent;
	struct _node *leftChild;
	struct _node *rightChild;
}Node;


static int nodeNum;
Node* root;
Node* NILL;

int isExist(int value);
void printNode();
int RBTHeight(Node* x);

void LR(Node* x);
void RR(Node* x);

void RB_INSERT(int rsv_num, char* name, int s, int d, int date,int seatLv);
void RB_INSERT1(Node* z);
void RB_DELETE(int value);
void RB_DELETE1(Node* x);

void PRINT_RBT(int rsv_num);
void Transplant(Node* u, Node* v);
Node* Search(int value);
Node* minimum(Node* x);

#endif // _node


