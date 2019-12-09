#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "RBtree.h"
#include "path.h"

#define _CRT_SECURE_NO_WARNINGS

int calRsvNum(char s, char d, int date) {  //예약번호 계산 함수
	srand(GetTickCount());
	s -= 'a'; d -= 'a';
	int ran = (rand() % 100000 )+100000 + date * 10000 + s* 100 + d;  //예약번호 만들기
	while (isExist(ran)) {  //예약 번호 중복 피하기
		ran++;
	}
	return ran;
}

void calRate(int *seatLv) {  //좌석 클래스에 따라 비행기표 가격 곱비율 계산
	if (*seatLv == 1) *seatLv += 9;
	else if (*seatLv == 2) *seatLv += 3;
	else if (*seatLv == 3) *seatLv -= 2;
}

void reservation(Graph* graph) { //500개 랜덤 예약 함수
	char name[10] = { '\0' };
	int date[500], seatLv[500], rsv_num = 0;
	char s[500], d[500];
	int c[500], i = 0; //값하나씩 생성해서 바로 넣으려고 했더니 가끔 똑같은 값들이 연속적으로 나와서 배열에 저장
	int path[10][2];
	srand(GetTickCount());
	for (int k = 0; k < 500; k++) {
		c[k] = rand() % 9 + 1;
		date[k] = rand() % 31 + 1;
		seatLv[k] = rand() % 3 + 1;
		date[k] = rand() % 31 + 1;
		do {
			s[k] = rand() % 26 + 'a';
			d[k] = rand() % 26 + 'a';
			dijkstra(graph, s[k] - 'a', d[k] - 'a', date[k], path);
		} while (d[k] == s[k] || path[1][0] == -1); //출발지랑 도착지 같거나 경로 없는 경우 제외
	}
	srand(GetTickCount());
	for (int k = 0; k < 500; k++) {
		i = 0;
		//srand(GetTickCount());
		while (i < c[k]) {
			name[i++] = rand() % 26 + 'a';
		}
		name[i] = '\0';
		rsv_num = calRsvNum(s[k], d[k], date[k]);
		RB_INSERT(rsv_num, name, s[k], d[k], date[k], seatLv[k]);
	}
	return;
}

int main() {
	Date date[31];			// 31개의 날짜
	int t[200];				// 매일 200개의 출발시각
	setDate(date, t);
	Edge edges[100] = { 0 };
	createEdge(edges); //path 100개 랜덤 설정
	Graph* graph = createGraph(edges, date); //나라 위치 설정 & path 연결
	unsigned char c;
	
	NILL = (Node*)malloc(sizeof(Node));
	NILL->color = 'B';
	root = NILL;

	printf(" ********************************************** \n");
	printf("          Airline Reservation Service           \n");
	printf("                  - Team 2 -                   \n");
	printf(" ********************************************** \n");
	
	reservation(graph); //500개 예약
	
	int path[10][2]; //경로 넘겨줄 배열
	int flight;
	int level;
	long price;

	while (1) {
		printMain();
		c = _getch();
		if ((c == 0x00) || (c == 0xE0)) {
			printf(" [System] : Wrong Input.\n");
			c = _getch();
		}
		else {
			switch (c) {
			case '1': {
				printf(" [Reservation]\n");
				printf("===============================================\n");
				
				char name[10] = { '\0' };
				int date, seatLv, rsv_num = 0;
				char s, d;
				printf(" Please enter your name: ");  //이름 입력받기
				fgets(name, 10, stdin);
				name[strlen(name) - 1] = '\0';

				printf(" Please enter the departure city (a~z): "); //source city 입력받기
				scanf("%c", &s); getchar();

				printf(" Please enter the destination city (a~z): "); //destination 입력받기
				scanf("%c", &d); getchar();

				printf(" Please enter the date of departure (1~31): "); //date 입력받기
				scanf("%d", &date); getchar();

				printf(" Please enter the level of your seat(1~3)\n [1: First class  2: Business class  3: Economy class] :  ");
				scanf("%d", &seatLv); getchar();  // seat level 입력 받기

				rsv_num = calRsvNum(s,d, date); 
				int height = RBTHeight(root);
				int nodes = getNodeNum();
			
				dijkstra(graph, s - 'a', d - 'a', date, path);
				if (path[1][0] == -1) { //경로 없거나 출발지=도착지인 
					printf("\n [System] : Sorry, NO path for %c to %c.\n", s, d);
					break;
				}
				printf("\n [System] : Your reservation has been successfully completed!\n");
				RB_INSERT(rsv_num, name, s, d, date, seatLv);
				PRINT_RBT(rsv_num);
				
				flight = printPath(graph, path, 0);
				printf("- Flight time: %dh %dm\n", flight / 60, flight % 60);
				calRate(&seatLv);
				price = flight * 1200 * seatLv;
				printf("- Ticket price: %d won\n", price);

				printNode(height, nodes);
				break;
			}
			case '2': {
				printf(" [Cancel Reservation]\n");
				printf("===============================================\n");
				
					int rsv_num = 0;
					printf(" Please enter your reservation number : ");
					scanf("%d", &rsv_num); getchar();
					int height = RBTHeight(root);
					int nodes = getNodeNum();
					RB_DELETE(rsv_num);
					printNode(height,nodes);
					break;
			}
			case '3': {
				printf(" [Check Reservation]\n");
				printf("===============================================\n");
			
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
				else printf("\n [System] : We can't find your reservation status. Please check your reservation again.\n");
					break;
			}
			case '4':
				printDate(graph);
				break;
			case '5':
				printf(" [System] : Thank you for using our service.\n\n");
				exit(0);
				break;
			case '9': //관리자 모드
				printf(" [System] : Administer Mode.\n\n");
				char name1[20] = { '\0' };
				printf(" Password: ");  //비밀 번호 입력받기
				fgets(name1, 20, stdin);
				name1[strlen(name1) - 1] = '\0';
				if (!strcmp(name1, "qwerty")) { // 비밀번호 같으면 예약 번호 목록 출력
					int height = RBTHeight(root);
					printf("\n [Print Reservation Number]\n");
					printf("===============================================\n");
					printf("- Number of nodes : %d\n- Height of the tree: %d\n\n", getNodeNum(), height);
					printRsvNum(root);
					printf("\n");
				}
				else {
					printf(" [System] : Wrong Access!\n");
				}
				break;
			default:
				printf(" [System] : Wrong Input.\n");
				break;
			}
		}
	}
}
