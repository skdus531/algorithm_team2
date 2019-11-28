#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct Node {         // 날짜별로 시간, 분, 초를 포함
	int hour, minute, second;
	struct Node *next;
} Node;

void setDepartureTime(int* t);
void setNode(Node* table, int* t);
void printTable(Node* table);

int main() {
	Node table[31];         // 31개의 날짜
	int t[31];            // 31개의 시간변수
	setDepartureTime(t);
	setNode(table, t);
	printTable(table);

	return 0;
}

void setDepartureTime(int* t) {         // 출발시각 31개 랜덤생성
	srand((unsigned)time(NULL));
	for (int i = 0; i < 31; i++) {
		int duplicate = 0;            // 중복변수(1이면 중복)
		t[i] = rand() % 86400;         // 0h 0m 0s ~ 23h 59m 59s 랜덤생성
		for (int j = 0; j < i; j++) {
			if (t[j] == t[i]) {
				duplicate = 1;
				break;
			}
		}
		if (duplicate == 1) {         // 중복되는 값이라면 재생성
			i--;
		}
	}
}

void setNode(Node* table, int* t) {      // 랜덤 생성한 시간을 시,분,초로 나누어 노드에 저장
	for (int i = 0; i < 31; i++) {
		int time_value = t[i];
		table[i].hour = time_value / 3600;
		time_value %= 3600;
		table[i].minute = time_value / 60;
		table[i].second = time_value % 60;
	}
}

void printTable(Node* table) {         // 시간표 출력
	for (int j = 0; j < 5; j++) {
		int i = j * 7;
		for (int date = i; (date < i + 7) && (date<31); date++) {
			if (table[date].hour < 10) {
				printf("0");         // 한 자리 수의 위치를 맞추기 위해 앞에 0 포함시키기
			}
			printf("%d h\t", table[date].hour);
		}
		printf("\n");
		for (int date = i; (date<i + 7) && (date<31); date++) {
			if (table[date].minute < 10) {
				printf("0");
			}
			printf("%d m\t", table[date].minute);
		}
		printf("\n");
		for (int date = i; (date < i + 7) && (date<31); date++) {
			if (table[date].second < 10) {
				printf("0");
			}
			printf("%d s\t", table[date].second);
		}
		printf("\n\n");
	}
}