#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "path.h"
/*
typedef struct Date {			// 날짜별로 이름, 시간, 분을 포함
	int name;
	int time[2][200];			// time[0]이 시간, time[1]이 분
	struct Date *next;
} Date;

void setDepartureTime(int* t);
void setDate(Date* date, int* t);
void printDate(Date* date);

int main() {
	srand((unsigned)time(NULL));
	Date date[31];			// 31개의 날짜
	int t[200];				// 매일 200개의 출발시각
	setDate(date, t);
	//printDate(date);

	return 0;
}
*/
void setDepartureTime(int* t) {			// 출발시각 31개 랜덤생성
	for (int i = 0; i < 200; i++) {
		t[i] = rand() % 1440;			// 0h 0m 0s ~ 23h 59m 랜덤생성
	}
}

void setDate(Date* date, int* t) {		// 랜덤 생성한 시간을 시,분으로 나누어 노드에 저장
	for (int i = 0; i < 31; i++) {
		date[i].name = i;
		setDepartureTime(t);
		for (int j = 0; j < 200; j++) {
			date[i].time[0][j] = t[j] / 60;
			date[i].time[1][j] = t[j] % 60;
		}
	}
}

void printDate(Date* date) {			// 시간표 출력
	for(int i=0;i<31;i++) {
		printf("day ");
		if (date[i].name < 9) printf("0");
		printf("%d\n",date[i].name+1);
		for(int j=0;j<200;j++) {
			if (date[i].time[0][j] < 10) printf("0");
			printf("%d h ",date[i].time[0][j]);
			if (date[i].time[1][j] < 10) printf("0");
			printf("%d m\n", date[i].time[1][j]);
		}
		printf("\n");
	}
}
