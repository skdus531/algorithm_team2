typedef struct Graph {
	struct Node* head[26];
	int location[26][2]; // [][0]: x��ǥ, [][1]: y��ǥ 
} Graph;

typedef struct Node {
	int dest;// 0~26 (a~z)
	int distance; //�Ÿ�(km) (500���� ������ flightTime)
	int departureTime[3]; //���� ���� ����, [0]: ��¥, [1]: ��, [2]: ��
	struct Node * next;
} Node;

typedef struct Edge {
	int s, d; //�����, ������
} Edge;

Node* findNode(Graph* graph, int s, int d);

void locationSetting(Graph * graph);
Graph* createGraph(Edge* edges);

void createEdge(Edge* edges);

void printGraph(Graph * graph);

void printMain();

void printArr(int dist[], int n, int s);

void dijkstra(Graph* graph, int s);