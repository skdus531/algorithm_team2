typedef struct Graph {
	struct Dest* head[26];
	int location[26][2]; // [][0]: x좌표, [][1]: y좌표 
} Graph;


typedef struct Dest {
	int dest;// 0~26 (a~z)
	int distance; //거리(km) (500으로 나누면 flightTime)
	int departureTime[32][3]; //매일 랜덤 설정, [0]: 날짜, [1]: 시, [2]: 분
	int flightTime[2];
	int arriveTime[32][3];
	struct Dest * next;
} Dest;

typedef struct Edge {
	int s, d; //출발지, 도착지
} Edge;
typedef struct Date {			// 날짜별로 이름, 시간, 분을 포함
	int name;
	int time[2][200];			// time[0]이 시간, time[1]이 분
	struct Date *next;
} Date;

void setDepartureTime(int* t);		// 출발시각 31개 랜덤생성
void setDate(Date* date, int* t);	// 랜덤 생성한 시간을 시,분으로 나누어 노드에 저장
void printDate(Date* date);

Dest* findNode(Graph* graph, int s, int d);

void locationSetting(Graph * graph);

Graph* createGraph(Edge* edges, Date* date);

void createEdge(Edge* edges);

void printGraph(Graph * graph);

void printMain();

void printArr(int dist[], int n, int s);

void dijkstra(Graph* graph, int s, int date);
void dijkstra1(Graph* graph, int s);

int compare(int a[3]);
