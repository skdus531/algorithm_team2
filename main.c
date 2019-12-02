#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RBtree.h"
#include "path.h"

#define _CRT_SECURE_NO_WARNINGS

static int rsvNum = 1;

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
	
	NILL = (Node*)malloc(sizeof(Node));
	NILL->color = 'B';
	root = NILL;

	//전체 경로의 출발시간 찾기
  
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

	//shortestPath  >  dijkstra(graph, src, dest, date, path)로 검색
	int path[10];
	int * path1 = dijkstra(graph, 0, 6, 20, path);
	i = 0;
	while(path1[i] != -1){
		printf("%c <- ", path1[i++]+'a');
	}
	printf("\n");


	while (1) {
		printMain();
		c = _getch();
		if ((c == 0x00) || (c == 0xE0)) {
			printf("Wrong Input.\n");
			c = _getch();
		}
		else {
			switch (c) {
			case '1': {
					//사용자에게 이름, s, d, date 입력 받아 insert 하기
				//예약번호는 지금 static 변수 rsvNum++해서 쓰기
				break;
			}
			case '2': {
					int rsv_num = 0;
					printf("Please enter your reservation number : ");
					scanf("%d", &rsv_num);
					RB_DELETE(rsv_num);
					break;
			}
			case '3': {
				int rsv_num = 0;
				printf("Please enter your reservation number : ");
				scanf("%d", &rsv_num);
					
				if (isExist(rsv_num)) {
						Node* temp = (Node*)malloc(sizeof(Node));
						temp = Search(rsv_num);
						PRINT_RBT(temp);
					}
				else printf("We can't find your reservation status. Please check your reservation again.");
					break;
			}
			case '4':
				//printf("4\n");
				break;
			case '5':
				//printf("5 종료\n");
				exit(0);
				break;
			default:
				printf("Wrong Input.\n");
				break;
			}
		}
	}
}
