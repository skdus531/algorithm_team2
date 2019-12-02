#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RBtree.h"
#include "path.h"

#define _CRT_SECURE_NO_WARNINGS

int calRsvNum(char s, char d, int date) {
	srand(time(NULL));
	s -= 'a'; d -= 'a';
	int ran = (rand() % 100000 )+100000 + date * 10000 + s* 100 + d;
	return ran;
}

void calRate(int *seatLv) {
	if (*seatLv == 1) *seatLv += 9;
	else if (*seatLv == 2) *seatLv += 3;
	else if (*seatLv == 3) *seatLv -= 2;
}

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
	
	int rsv[600] = { 0, };
	NILL = (Node*)malloc(sizeof(Node));
	NILL->color = 'B';
	root = NILL;

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
	
	
	//shortestPath  >  dijkstra(graph, src, dest, date, path)로 검색
	int path[10][2]; //경로 넘겨줄 배열
	
	dijkstra(graph, 0, 6, 20, path); //검색 a->g 20일 출발
	int flight = printPath(graph, path, 0); //비행시간(분 단위) 리턴
	printf("Flight time: %dh %dm\n", flight / 60, flight % 60);
	int level = 1; //좌석 레벨 (1: economy, 5: business, 10: prestige)
	long price = flight * 1200 * level;
	printf("Price: %dwon\n", price);
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
				char name[10] = { '\0' };
				int date, seatLv, rsv_num = 0;
				char s, d;
				printf("Please enter your name: ");  //이름 입력받기
				fgets(name, 10, stdin);
				name[strlen(name) - 1] = '\0';

				printf("Please enter the departure city(a~z): "); //source city 입력받기
				scanf("%c", &s); getchar();

				printf("Please enter the destination city(a~z): "); //destination 입력받기
				scanf("%c", &d); getchar();

				printf("Please enter the date of departure(1~31): "); //date 입력받기
				scanf("%d", &date); getchar();

				printf("Please enter the level of your seat(1~3)\n[1 is First class 2 is Business class 3 is Economy class] :  ");
				scanf("%d", &seatLv); getchar();  // seat level 입력 받기

				rsv_num = calRsvNum(s,d, date); 
				
				RB_INSERT(rsv_num, name, s, d, date, seatLv);
				PRINT_RBT(rsv_num);
				dijkstra(graph, s-'a', d-'a', date, path);
				flight = printPath(graph, path, 0);
				printf("- Flight time: %dh %dm\n", flight / 60, flight % 60);
				calRate(&seatLv);
				price = flight * 1200 * seatLv;
				printf("- Ticket price: %d won\n", price);

				printNode();
				break;
			}
			case '2': {
					int rsv_num = 0;
					printf("Please enter your reservation number : ");
					scanf("%d", &rsv_num); getchar();
					RB_DELETE(rsv_num);
					printNode();
					break;
			}
			case '3': {
				int rsv_num = 0;
				printf("Please enter your reservation number : ");
				scanf("%d", &rsv_num); getchar();
					
				if (isExist(rsv_num)) {
						PRINT_RBT(rsv_num);
						Node* temp = Search(rsv_num);
						dijkstra(graph, temp->s - 'a',temp->d - 'a', temp->date, path);
						flight = printPath(graph, path, 0);
						printf("- Flight time: %dh %dm\n", flight / 60, flight % 60);
						calRate(&(temp->seatLv));
						price = flight * 1200 * (temp->seatLv);
						printf("- Ticket price: %d won\n", price);
					}
				else printf("\n [System] : We can't find your reservation status. Please check your reservation again.");
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
