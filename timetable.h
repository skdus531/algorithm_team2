typedef struct Node {			// ��¥���� �ð�, ��, �ʸ� ����
	int hour, minute, second;
	struct Node *next;
} Node;

void setDepartureTime(int* t);
void setNode(Node* table, int* t);

void printTable(Node* table);