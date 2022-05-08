#include <iostream>
#include <thread>
using namespace std;

static int total = 0; // 정적 변수

/*
 * 입력으로 받은 num까지 total을 증가시킨다.
 */
void Sum(int num)
{
	for (int i = 0; i < num + 1; i++)
		total += 1;
}

int main()
{
	cout << "하나의 자원에 동시 접근 경우 " << '\n';

	thread t[10]; // 쓰레드 10개 생성
	// 쓰레드 초기화
	for (int i = 0; i < 10; i++)
		t[i] = thread(Sum, 100000);

	// 실행
	for (int i = 0; i < 10; i++)
		t[i].join();

	cout << total;
	return 0;
}
