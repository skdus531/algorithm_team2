typedef struct Graph {
	struct Node* head[26];
	int location[26][2]; // [][0]: x좌표, [][1]: y좌표 
} Graph;

typedef struct Node {
	int dest;// 0~26 (a~z)
	int distance; //거리(km) (500으로 나누면 flightTime)
	int departureTime[3]; //매일 랜덤 설정, [0]: 날짜, [1]: 시, [2]: 분
	struct Node * next;
} Node;

typedef struct Edge {
	int s, d; //출발지, 도착지
} Edge;

Node* findNode(Graph* graph, int s, int d);

void locationSetting(Graph * graph);
Graph* createGraph(Edge* edges);

void createEdge(Edge* edges);

void printGraph(Graph * graph);

void printMain();

void printArr(int dist[], int n, int s);

void dijkstra(Graph* graph, int s);