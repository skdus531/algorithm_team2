typedef struct Node {			// 날짜별로 시간, 분, 초를 포함
	int hour, minute, second;
	struct Node *next;
} Node;

void setDepartureTime(int* t);
void setNode(Node* table, int* t);

void printTable(Node* table);