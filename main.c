#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RBtree.h"
#include "path.h"
#include "timetable.h"


int main() {
	srand((unsigned)time(NULL));
	Date date[31];			// 31개의 날짜
	int t[200];				// 매일 200개의 출발시각
	setDate(date, t);
	//printDate(date);
	Edge edges[100] = { 0 };
	createEdge(edges); //path 100개 랜덤 설정
	Graph* graph = createGraph(edges, date); //나라 위치 설정 & path 연결
	//printGraph(graph); // 출발지 별 path 출력
	unsigned char c;

	//전체 경로의 출발시간 찾기
  /*
	int i = 0;
	for (i = 0; i < 26; i++) {
		Dest * ptr = graph->head[i];
		while (ptr != NULL) {
			printf("[%c] -> %c(%dkm) \n", i + 'a', ptr->dest + 'a', ptr->distance);
			for (int j = 1; j < 32; j++) {
				printf(" %d %dh %dm ", ptr->departureTime[j][0], ptr->departureTime[j][1], ptr->departureTime[j][2]);
			}
			printf("\n\n");
			ptr = ptr->next;
		}
		printf("\n");
	}
*/



	while (1) {
		printMain();
		c = getch();
		if ((c == 0x00) || (c == 0xE0)) {
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
}
