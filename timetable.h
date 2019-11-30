typedef struct Date {			// 날짜별로 이름, 시간, 분을 포함
	int name;
	int time[2][200];			// time[0]이 시간, time[1]이 분
	struct Date *next;
} Date;

void setDepartureTime(int* t);		// 출발시각 31개 랜덤생성
void setDate(Date* date, int* t);	// 랜덤 생성한 시간을 시,분으로 나누어 노드에 저장

void printDate(Date* date);
