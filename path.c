#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "path.h"


Dest* findNode(Graph* graph, int s, int d) { //s에서 d까지의 직항 경로가 있는지 찾는 함수
	Dest * ptr = graph->head[s];
	while (ptr != NULL) {
		if (ptr->dest == d) {
			return ptr; //해당 목적지 노드를 리턴
		}
		ptr = ptr->next;
	}
	return NULL; //경로 없는 경우 NULL 리턴
}

Graph* createGraph(Edge* edges, Date* date) { //100개의 edge와 200*31개의 출발 시간을 인풋으로 그래프 생성
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	for(int i = 0; i < 26; i++){ //초기화
		graph->head[i] = NULL;
	}

	locationSetting(graph); //나라 위치 랜덤 설정
	int k = 0, l = 0;
	for (int i = 0; i < 100; i++) { // 거리, 비행시간 계산, 목적지 노드 추가
		int s = edges[i].s;
		int d = edges[i].d;
		int distance = (int)sqrt(pow(graph->location[s][0] - graph->location[d][0], 2) + pow(graph->location[s][1] - graph->location[d][1], 2));
		int flight[2];
		
		flight[0] = distance / 500; //(시 단위)
		flight[1] = (distance % 500) * 60/500 ; //(분 단위)
		//path마다 양방향으로 Node 2개씩 만들기
		struct Dest* newNode = (Dest*)malloc(sizeof(Dest));
		struct Dest* newNode1 = (Dest*)malloc(sizeof(Dest));
		newNode->dest = d;
		newNode1->dest = s;
		newNode->distance = distance;
		newNode1->distance = distance;
		newNode->flightTime[0] = flight[0];
		newNode->flightTime[1] = flight[1];
		newNode1->flightTime[0] = flight[0];
		newNode1->flightTime[1] = flight[1];

		for (int j = 1; j < 32; j++) { // 각 경로마다 출발시간 날짜별 입력
			newNode->departureTime[j][0] = j;
			newNode->departureTime[j][1] = date[l].time[0][k];
			newNode->departureTime[j][2] = date[l].time[1][k++];
			newNode1->departureTime[j][0] = j;
			newNode1->departureTime[j][1] = date[l].time[0][k];
			newNode1->departureTime[j][2] = date[l].time[1][k++];
			if (k == 200) {
				l++; k = 0;
			}
		}

		int m, h;
		for (int j = 1; j < 32; j++) { //각 경로마다 도착시간 계산
			m = newNode->departureTime[j][2] + flight[1];
			newNode->arriveTime[j][2] = m % 60;
			h = newNode->departureTime[j][1] + flight[0];
			newNode->arriveTime[j][1] = (h + m / 60) % 24;
			newNode->arriveTime[j][0] = j + (h + m / 60)/24;

			m = newNode1->departureTime[j][2] + flight[1];
			newNode1->arriveTime[j][2] = m % 60;
			h = newNode1->departureTime[j][1] + flight[0];
			newNode1->arriveTime[j][1] = (h + m / 60) % 24;
			newNode1->arriveTime[j][0] = j + (h + m / 60) / 24;
		}

		newNode->next = graph->head[s];
		graph->head[s] = newNode;
		newNode1->next = graph->head[d];
		graph->head[d] = newNode1;
	}
	return graph; //그래프 리턴
}

void locationSetting(Graph * graph) { // 각 나라별 위치 랜덤 설정
	srand((unsigned)time(NULL));
	for (int i = 0; i < 26; i++) {
		int x = rand() % 6001 - 3000; 
		int y = rand() % 6001 - 3000;
		graph->location[i][0] = x;
		graph->location[i][1] = y;
		for (int j = 0; j < i; j++) {
			if ((graph->location[j][0] == x) && (graph->location[j][1] == y)) { //같은 위치 없도록 설정
				i--;
				break;
			}
		}
	}
}

void createEdge(Edge* edges) { //100개의 출발지->도착지 경로 설정
	int i = 0;
	int check[26][26] = { 0 };
	int x = 0;
	srand((unsigned)time(NULL));
	while (i < 100) {
		int y;
		do {
			x = rand() % 26;
			y = rand() % 26;
		} while (x == y || (check[x][y] != 0)); // 출발지 = 도착지인 경우, 중복인 경우 제외
		check[x][y]++;
		check[y][x]++;
		edges[i].s = x;
		edges[i].d = y;
		i++;
	}
}

void printDate(Graph* graph) {
	for (int i = 1; i < 32; i++) {
		printf("<day ");
		if (i < 10) printf("0");
		printf("%d>\n", i);
		for (int j = 0; j < 26; j++) {
			Dest* ptr = graph->head[j];
			printf("[%c] : \n", j + 'a');
			while (ptr != NULL) {
				if (ptr->departureTime[i][0]<10) printf("0");
				printf("%d day\t", ptr->departureTime[i][0]);
				if (ptr->departureTime[i][1]<10) printf("0");
				printf("%d h\t", ptr->departureTime[i][1]);
				if (ptr->departureTime[i][2]<10) printf("0");
				printf("%d s", ptr->departureTime[i][2]);
				ptr = ptr->next;
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}
}

void printMain() { //메인 메뉴 출력 함수
	printf("\n [Main Menu]\n");
	printf("===============================================\n");
	printf("  1. Reservation        2. Cancel Reservation  \n\n");
	printf("  3. Check Reservation  4. Time Table          \n\n");
	printf("  5. Exit                                      \n\n");
	return;
}


void dijkstra(Graph* graph, int s, int d, int date,int shortestPath[10][2]) { //최단 시간 경로 찾는 함수, dijkstra 알고리즘 이용
	int path[26][2]; // [0]: index 이전의 나라, [1]: 이전 나라에서 출발한 날짜
	int check[26]; // 방문했는지 체크
	int arrT[26][2]; // [0]는 도착 날짜, [1]은 compare 함수 리턴값

	for (int i = 0; i < 26; i++) { //초기화
			arrT[i][0] = date;
			arrT[i][1] = INT_MAX;
			check[i] = 0;
			path[i][0] = -1;
			path[i][1] = 0;
	}
	arrT[s][0] = date;
	arrT[s][1] = 0;
	path[s][0] = s;
	path[s][1] = date;

	for (int i = 0; i < 25; i++) { //dijkstra 알고리즘 응용
		int min = INT_MAX, index;

		for (int i = 0; i < 26; i++) { // 도착시간 최소값 찾기
			if (check[i] == 0 && min > arrT[i][1]){
				index = i;
				min = arrT[i][1];
			}
		}
		int u = index;

		for (int v = 0; v < 26; v++) {
			Dest* ptr = findNode(graph, u, v); //직항 경로 있는지 확인
			if (ptr != NULL) { // v가 방문한적 없고, src->u까지의 길이 있으며, src->u의 도착시간이 u->v의 출발시간보다 빨라야하고, src->v의 도착시간보다 src->u->v까지의 도착시간이 작으면 업데이트
				if (check[v] == 0 && arrT[u][1] != INT_MAX) {
					int a = 0;

					while (arrT[u][0] + a < 32 && arrT[u][1] >= compare(ptr->departureTime[arrT[u][0] + a])) { //출발 항공 있을 때까지 기다림
						a++;
					}

					if (arrT[u][0] + a < 32 && compare(ptr->arriveTime[arrT[u][0] + a]) < arrT[v][1]) { //업데이트
						arrT[v][1] = compare(ptr->arriveTime[arrT[u][0] + a]);
						arrT[v][0] = ptr->arriveTime[arrT[u][0] + a][0];
							
						if (u == s) { // 이전이 src인 경우, 직항 (최단 시간 경로)
								path[v][0] = s;
								path[v][1] = date;
						}
						else { //경유
							path[v][0] = u;
							path[v][1] = arrT[u][0] + a;
						}
					}
				}
			}
		}
		check[u] = 1; 
	}
	

	int t;
	int i = 0;
	Dest* ptr1 = NULL;
	
	shortestPath[i][0] = d;
	shortestPath[i++][1] = 0;
	
	if (path[d][0] != -1) { //shortestPath 배열에 최단 경로 입력
		t = d;
		while (path[t][0] != s) {
			ptr1 = findNode(graph, path[t][0], t);
			shortestPath[i][0] = path[t][0]; 
			shortestPath[i++][1] = path[t][1];
			t = path[t][0];
		}
		ptr1 = findNode(graph, s, t);
		if (ptr1 != NULL) {
			shortestPath[i][0] = s;
			shortestPath[i++][1] = date;
		}
	}
	shortestPath[i][0] = -1;
}

int compare(int a[3]) { // 시간 비교 함수
	return a[0] * 10000 + a[1] * 100 + a[2];
}

int printPath(Graph* graph, int path[10][2], int i) { //경로 출력 함수
	int s, d, ti;
	Dest* ptr;

	if (path[i + 1][0] == -1){
		return 0;
	}
	else {
		int f = printPath(graph, path, i + 1);

		s = path[i + 1][0];  //path[i+1][0] = 출발지
		d = path[i][0];      //path[i][0] = 도착지
		ti = path[i + 1][1]; //path[i][1] = path[i+1][0]에서의 출발 날짜
		ptr = findNode(graph, path[i + 1][0], path[i][0]);

		printf("- Flight path(Arrival time): %c -> %c ([Day%d] [Time %d : %d] ~ [Day%d] [Time %d : %d])\n",  s + 'a', d + 'a', ptr->departureTime[ti][0], ptr->departureTime[ti][1], ptr->departureTime[ti][2], ptr->arriveTime[ti][0], ptr->arriveTime[ti][1], ptr->arriveTime[ti][2]);
		return f + ptr->flightTime[0] * 60 + ptr->flightTime[1]; //총 비행 시간 (waiting time 제외)
	}
}
