#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m; // 뮤텍스 선언
static int total = 0; // 정적 변수

/*
 * 입력으로 받은 num까지 total을 증가시킨다.
 */
void Sum(int num)
{
	for (int i = 0; i < num + 1; i++)
	{
		m.lock(); // lock을 걸어준다.
		total += 1;
		m.unlock(); // lock을 해제한다.
	}
}

int main()
{
	cout << "mutex를 이용하여 임계 영역을 생성해 동시 접근 방지" << '\n';
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
