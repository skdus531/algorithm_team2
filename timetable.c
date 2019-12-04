#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "path.h"

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
