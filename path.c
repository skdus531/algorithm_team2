#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

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
	int s,d; //출발지, 도착지
} Edge;

void printGraph(Graph * graph);
void createEdge(Edge* edges);
void locationSetting(Graph * graph);
Graph* createGraph(Edge* edges);
void printMain();
void dijkstra(struct Graph* graph, int src);

int main() {
	Edge edges[100] = {0};
	createEdge(edges); //path 100개 랜덤 설정
	Graph * graph = createGraph(edges); //나라 위치 설정 & path 연결
	printGraph(graph); // 출발지 별 path 출력
	unsigned char c;
	dijkstra(graph, 0);
	dijkstra(graph, 1);
	dijkstra(graph, 2);

	/*
	while (1) {
		printMain();
		c = getch();
		if ((c == 0x00)||(c == 0xE0)) {
			printf("잘못된 입력입니다.\n");
			c = getch();
		}
		else {
			switch (c) {
			case '1':
				//printf("1\n");
				break;
			case '2':
				//printf("2\n");
				break;
			case '3':
				//printf("3\n");
				break;
			case '4':
				//printf("4\n");
				break;
			case '5':
				//printf("5 종료\n");
				exit(0);
				break;
			default:
				printf("잘못된 입력입니다.\n");
				break;
			}
		}
	}*/

	return 0;
}

Node* findNode(Graph* graph, int s, int d) {
	Node * ptr = graph->head[s];
	while (ptr != NULL) {
		if (ptr->dest == d) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

Graph* createGraph(Edge* edges) {//100개 edges 받아서
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	for(int i = 0; i < 26; i++){
		graph->head[i] = NULL;
	}

	locationSetting(graph); //나라 위치 랜덤 설정

	for (int i = 0; i < 100; i++) {
		int s = edges[i].s;
		int d = edges[i].d;
		int distance = (int)sqrt(pow(graph->location[s][0] - graph->location[d][0], 2) + pow(graph->location[s][1] - graph->location[d][1], 2));
		
		//path마다 양방향으로 Node 2개씩 만들기
		struct Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->dest = d;
		newNode->distance = distance;
		//departureTime은 나중에 매일 랜덤 설정하기
		newNode->next = graph->head[s];
		graph->head[s] = newNode;
		
		struct Node* newNode1 = (Node*)malloc(sizeof(Node));
		newNode1->dest = s;
		newNode1->distance = distance;
		//departureTime은 나중에 매일 랜덤 설정하기
		newNode1->next = graph->head[d];
		graph->head[d] = newNode1;
	}
	return graph;
}

void locationSetting(Graph * graph) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < 26; i++) {
		int x = rand() % 6001 - 3000; //너무 가깝지 않나? 50단위로 설정? 
		int y = rand() % 6001 - 3000;
		graph->location[i][0] = x;
		graph->location[i][1] = y;
		for (int j = 0; j < i; j++) {
			if ((graph->location[j][0] == x) && (graph->location[j][1] == y)) {
				i--;
				break;
			}
		}
	}
}

void createEdge(Edge* edges) {
	int i = 0;
	int check[26][26] = { 0 };
	int x = 0;
	srand((unsigned)time(NULL));

	while (i < 100) {
		int y;
		do {
			x = rand() % 26;
			y = rand() % 26;
		} while (x == y || (check[x][y] != 0));
		check[x][y]++;
		check[y][x]++;
		edges[i].s = x;
		edges[i].d = y;
		i++;
	}
}

void printGraph(Graph * graph) {
	int i = 0;
	for (i = 0; i < 26; i++) {
		Node * ptr = graph->head[i];
		printf("[%c]", i + 'a');
		while (ptr != NULL) {
			printf("-> %c(%dkm) ", ptr->dest+'a',ptr->distance);
			ptr = ptr->next;
		}
		printf("\n");
	}
}

void printMain() {
	printf("\n"); 
	printf("1. 예약"); 
	printf("2. 예약 취소");
	printf("3. 예약 확인");
	printf("4. 항공 시간표");
	printf("5. 종료");
	printf("\n");
	return;
}

void printArr(int dist[], int n, int s){
	printf("Src: %c\n", s + 'a');
	printf("Dest  Flight distance\n");
	for (int i = 0; i < n; i++){
			printf("%c \t %d\n", i + 'a', dist[i]);
	}
}

void dijkstra(Graph* graph, int s){	
	int path[26];
	int dist[26]; // 비행 거리
	int check[26]; // 방문했는지 체크
	
	
	for (int i = 0; i < 26; i++) {
		dist[i] = INT_MAX;
		check[i] = 0;
		path[i] = -1;
	}
	dist[s] = 0;
	path[s] = -2;

	for (int i = 0; i < 25; i++){
		int min = INT_MAX, index;
		for (int i = 0; i < 26; i++) {
			if (check[i] == 0 && min > dist[i]) {
				index = i;
				min = dist[i];
			}
		}
		int u = index;

		for (int v = 0; v < 26; v++) {
			Node* ptr = findNode(graph, u, v);
				if (ptr != NULL) {
					if (check[v] == 0 && dist[u] != INT_MAX && dist[v] > dist[u] + ptr->distance){
						dist[v] = dist[u] + ptr->distance;
						if (u == s) {
							path[v] = -2;
						}
						else {
							path[v] = u;
						}
					}
				}
			}
		check[u] = 1;
	}

	int k;
	printf("\n");
	Node* ptr = NULL;
	for (int i = 0; i < 26; i++) {
		printf("[%c]: ", i + 'a');
		if (path[i] == -1) {
			dist[i] = -1;
		}
		else{
			k = i;
			while (path[k] != -2) {
				ptr = findNode(graph, path[k], k);
				printf("<- %c(%d) ", path[k] + 'a', ptr->distance);
				k = path[k];
			}
			ptr = findNode(graph, s, k);
			if (ptr != NULL) {
				printf("<- %c(%d)", s + 'a', ptr->distance);
			}
		}
	printf("\n");

	}
	printf("\n");

	printArr(dist, 26,s);
}