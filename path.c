#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "timetable.h"
#include "path.h"

/*
int main() {
	Edge edges[100] = {0};
	createEdge(edges); //path 100개 랜덤 설정
	Graph * graph = createGraph(edges); //나라 위치 설정 & path 연결
	printGraph(graph); // 출발지 별 path 출력
	unsigned char c;
	dijkstra(graph, 0);
	dijkstra(graph, 1);
	dijkstra(graph, 2);

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
	}

	return 0;
}
*/
Dest* findNode(Graph* graph, int s, int d) {
	Dest * ptr = graph->head[s];
	while (ptr != NULL) {
		if (ptr->dest == d) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

Graph* createGraph(Edge* edges, Date* date) {//100개 edges 받아서
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	for(int i = 0; i < 26; i++){
		graph->head[i] = NULL;
	}

	locationSetting(graph); //나라 위치 랜덤 설정
	int k = 0, l = 0;
	for (int i = 0; i < 100; i++) {
		int s = edges[i].s;
		int d = edges[i].d;
		int distance = (int)sqrt(pow(graph->location[s][0] - graph->location[d][0], 2) + pow(graph->location[s][1] - graph->location[d][1], 2));
		int flight[2];
		flight[0] = distance / 500;
		flight[1] = (distance % 500) * 60/500 ;
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

		for (int j = 1; j < 32; j++) {
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

		for (int j = 1; j < 32; j++) {
			newNode->arriveTime[j][0] = j + (flight[0] / 24);
			newNode->arriveTime[j][1] = newNode->departureTime[j][1] + (flight[0] % 24);
			newNode->arriveTime[j][2] = newNode->departureTime[j][2] + flight[1];
			newNode1->arriveTime[j][0] = j + (flight[0] / 24);
			newNode1->arriveTime[j][1] = newNode1->departureTime[j][1] + (flight[0] % 24);
			newNode1->arriveTime[j][2] = newNode1->departureTime[j][2] + flight[1];
		}

		//departureTime은 나중에 매일 랜덤 설정하기
		newNode->next = graph->head[s];
		graph->head[s] = newNode;
		
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
		Dest * ptr = graph->head[i];
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
/*
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
			Dest* ptr = findNode(graph, u, v);
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
	Dest* ptr = NULL;
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
*/

void dijkstra(Graph* graph, int s, int date) {
	int path[26];
	int dist[26]; // 비행 거리
	int check[26]; // 방문했는지 체크
	//int depTime[26][31][2];
	//int arrTime[26][31][2];
	int arrT[26][2]; // [0]는 날짜, [1]은 컴페어함수 리턴값?

	//arrTime, depTime 설정하기


	for (int i = 0; i < 26; i++) {
		//dist[i] = INT_MAX;
		arrT[i][1] = INT_MAX;/*
		Dest* ptr = findNode(graph, s, i);
		if (ptr != NULL) {
			arrT[i][0] = ptr->arriveTime[date][0];
			arrT[i][1] = compare(ptr->arriveTime[date]);
		}*/
		check[i] = 0;
		path[i] = -1;
	}
//	dist[s] = 0;
	arrT[s][0] = date;
	arrT[s][1] = 0;
	path[s] = -2;

	for (int i = 0; i < 25; i++) {
		int min = INT_MAX, index;
		for (int i = 0; i < 26; i++) {
			if (check[i] == 0 && min > arrT[i][1]){//dist[i]) {
				index = i;
				min = arrT[i][1];// dist[i];
			}
		}
		int u = index;

		for (int v = 0; v < 26; v++) {
			Dest* ptr = findNode(graph, u, v);
			if (ptr != NULL) { //(날짜보다 뒤에여야 함!) v가 방문한적 없고 src->u까지의 길이 있으며, src->u의 도착시간이 u->v의 출발시간보다 빨라야하고, src->v의 도착시간보다 src->u -> v까지의 도착시간이 작으면 업데이트
				
				//만약 날짜가 더 빠르다면, 다음날꺼를 타는 방법도 생각..
				if (arrT[u][0] > date && check[v] == 0 && arrT[u][1] != INT_MAX && arrT[u][1] < compare(ptr->departureTime[arrT[u][0]]) && compare(ptr->arriveTime[arrT[u][0]]) < arrT[v][1]) {//dist[v] > dist[u] + ptr->distance) {
		//			if (check[v] == 0 && dist[u] != INT_MAX && compare(src->u의 arriveTime[날짜]) < compare(ptr->departureTime[날짜]) && compare(ptr->arriveTime[날짜]) < compare(현재 저장된 src->v도착시간)) {//dist[v] > dist[u] + ptr->distance) {
					//dist[v] = dist[u] + ptr->distance;
					arrT[v][1] = compare(ptr->arriveTime[arrT[u][0]]);
					arrT[v][0] = ptr->arriveTime[arrT[u][0]][0];

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
	Dest* ptr = NULL;
	for (int i = 0; i < 26; i++) {
		printf("[%c]: ", i + 'a');
		if (path[i] == -1) {
			dist[i] = -1;
		}
		else {
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

	printArr(dist, 26, s);

}

int compare(int a[3]) {
	return a[0] * 100 + a[1] * 100 + a[2];
}
