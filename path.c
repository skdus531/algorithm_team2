#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "path.h"


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
		//int flight = distance * 60 / 500; //분 단위
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

		int m, h;
		for (int j = 1; j < 32; j++) {
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
	return graph;
}

void locationSetting(Graph * graph) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < 26; i++) {
		int x = rand() % 6001 - 3000; 
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

void printMain() {
	printf("[Main]\n"); 
	printf("1. reservation\n"); 
	printf("2. cancel reservation\n");
	printf("3. check reservation\n");
	printf("4. time table\n");
	printf("5. exit\n");
	printf("\n");
	return;
}


void dijkstra(Graph* graph, int s, int d, int date,int shortestPath[10][2]) {
	int path[26][2];
	int check[26]; // 방문했는지 체크
	int arrT[26][2]; // [0]는 날짜, [1]은 컴페어함수 리턴값?

	for (int i = 0; i < 26; i++) {
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

	for (int i = 0; i < 25; i++) {
		int min = INT_MAX, index;

		for (int i = 0; i < 26; i++) {
			if (check[i] == 0 && min > arrT[i][1]){
				index = i;
				min = arrT[i][1];
			}
		}
		int u = index;

		for (int v = 0; v < 26; v++) {
			Dest* ptr = findNode(graph, u, v);
			if (ptr != NULL) { //(날짜보다 뒤에여야 함!) v가 방문한적 없고 src->u까지의 길이 있으며, src->u의 도착시간이 u->v의 출발시간보다 빨라야하고, src->v의 도착시간보다 src->u -> v까지의 도착시간이 작으면 업데이트
				if (check[v] == 0 && arrT[u][1] != INT_MAX) {
					int a = 0;

					while (arrT[u][0] + a < 32 && arrT[u][1] >= compare(ptr->departureTime[arrT[u][0] + a])) {
						a++;
					}

					if (arrT[u][0] + a < 32 && compare(ptr->arriveTime[arrT[u][0] + a]) < arrT[v][1]) {
						arrT[v][1] = compare(ptr->arriveTime[arrT[u][0] + a]);
						arrT[v][0] = ptr->arriveTime[arrT[u][0] + a][0];
							
						if (u == s) {
								path[v][0] = s;
								path[v][1] = date;
						}
						else {
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
	
	if (path[d][0] == -1) {
		shortestPath = NULL;
	}
	else {
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

/*	
	int k;
	printf("\n");
	Dest* ptr = NULL;
	
	for (int i = 0; i < 26; i++) {
		printf("[%c]: ", i + 'a');
		if (path[i][0] == -1) {
			//dist[i] = -1;
		}
		else {
			k = i;
			while (path[k][0] != s) {
				ptr = findNode(graph, path[k][0], k);
				printf("<- %c(%d %dh %dm ~ %d %dh %dm) ", path[k][0] + 'a', path[k][1], ptr->departureTime[path[k][1]][1], ptr->departureTime[path[k][1]][2], ptr->arriveTime[path[k][1]][0], ptr->arriveTime[path[k][1]][1], ptr->arriveTime[path[k][1]][2]);
				k = path[k][0];
			}
			ptr = findNode(graph, s, k);
			if (ptr != NULL) {
				printf("<- %c(%d %dh %dm ~ %d %dh %dm) ", s + 'a', path[k][1], ptr->departureTime[path[k][1]][1], ptr->departureTime[path[k][1]][2], ptr->arriveTime[path[k][1]][0], ptr->arriveTime[path[k][1]][1], ptr->arriveTime[path[k][1]][2]);
			}
		}
		printf("\n");

	}
	printf("\n");
*/
}

int compare(int a[3]) {
	return a[0] * 10000 + a[1] * 100 + a[2];
}

void printPath(Graph* graph, int path[10][2], int i) {
	int s, d, ti;
	Dest* ptr;

	if (path[i + 1][0] == -1){
		return;
	}
	else {
		printPath(graph, path, i + 1);
		s = path[i + 1][0];  //path[i+1][0] = 출발지
		d = path[i][0];      //path[i][0] = 도착지
		ti = path[i + 1][1]; //path[i][1] = path[i+1][0]에서의 출발 날짜
		ptr = findNode(graph, path[i + 1][0], path[i][0]);
		printf("%c -> %c (%d %dh %dm ~ %d %dh %dm)\n",  s + 'a', d + 'a', ptr->departureTime[ti][0], ptr->departureTime[ti][1], ptr->departureTime[ti][2], ptr->arriveTime[ti][0], ptr->arriveTime[ti][1], ptr->arriveTime[ti][2]);
	}
}
