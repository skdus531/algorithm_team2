#pragma once
typedef struct _node {
	char color;
	char* name;
	int rsv_num;
	int s, d, date;
	struct _node *parent;
	struct _node *leftChild;
	struct _node *rightChild;
}Node;
int printFlag;
int isExist(int rsv_num);

void LR(Node* x);
void RR(Node* x);

void RB_INSERT(int rsv_num, char* name, int s, int d, int date);
void RB_INSERT1(Node* z);
void RB_DELETE(int rsv_num);
void RB_DELETE1(Node* x);

void PRINT_RBT(Node* x);
void Transplant(Node* u, Node* v);
Node* Search(int rsv_num);
