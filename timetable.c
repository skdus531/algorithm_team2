#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct Node {         // ��¥���� �ð�, ��, �ʸ� ����
	int hour, minute, second;
	struct Node *next;
} Node;

void setDepartureTime(int* t);
void setNode(Node* table, int* t);
void printTable(Node* table);

int main() {
	Node table[31];         // 31���� ��¥
	int t[31];            // 31���� �ð�����
	setDepartureTime(t);
	setNode(table, t);
	printTable(table);

	return 0;
}

void setDepartureTime(int* t) {         // ��߽ð� 31�� ��������
	srand((unsigned)time(NULL));
	for (int i = 0; i < 31; i++) {
		int duplicate = 0;            // �ߺ�����(1�̸� �ߺ�)
		t[i] = rand() % 86400;         // 0h 0m 0s ~ 23h 59m 59s ��������
		for (int j = 0; j < i; j++) {
			if (t[j] == t[i]) {
				duplicate = 1;
				break;
			}
		}
		if (duplicate == 1) {         // �ߺ��Ǵ� ���̶�� �����
			i--;
		}
	}
}

void setNode(Node* table, int* t) {      // ���� ������ �ð��� ��,��,�ʷ� ������ ��忡 ����
	for (int i = 0; i < 31; i++) {
		int time_value = t[i];
		table[i].hour = time_value / 3600;
		time_value %= 3600;
		table[i].minute = time_value / 60;
		table[i].second = time_value % 60;
	}
}

void printTable(Node* table) {         // �ð�ǥ ���
	for (int j = 0; j < 5; j++) {
		int i = j * 7;
		for (int date = i; (date < i + 7) && (date<31); date++) {
			if (table[date].hour < 10) {
				printf("0");         // �� �ڸ� ���� ��ġ�� ���߱� ���� �տ� 0 ���Խ�Ű��
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