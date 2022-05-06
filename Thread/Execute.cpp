#include <iostream>
#include <thread>
#include <Windows.h>
using namespace std;

// 쓰레드를 할당 받아 진행하는 함수
// 메인 쓰레드와는 독립된 새로운 쓰레드이므로 병렬로 진행
// 함수가 끝나는 것과 동시에 쓰레드는 종료
DWORD __stdcall ThreadRunner(LPVOID IpParameter)
{
	int* arg;
	int count; // 반복문을 실행할 횟수

	count = 3;
	arg = (int*)IpParameter;

	// 무한 루프를 돌며 작업을 진행한다. 이 함수는 새로운 쓰레드이므로 무한 루프가 메인 쓰레드에 영향을 끼치지 않는다.
	while (count-- > 0)
		cout << *arg << " " << "ThreadRunner Called" << '\n';

	return 0;
}

int main()
{
	unsigned long threadId;
	HANDLE handleThread[5];
	int arr[5] = { 0, };
	for (int i = 0; i < 5; i++)
	{
		arr[i] = i;
		// 쓰레드 생성
		// 병렬로 작업할 쓰레드를 5개 생성하고, 매개변수로 arr를 입력한다.
		// 5개 쓰레드가 모두 ThreadRunner 함수로 지정되었지만, 5개의 서로 다른 ThreadRunner 함수가 호출된다고 생각하면 된다.
		handleThread[i] = CreateThread(0, 0, ThreadRunner, &arr[i], 0, &threadId); // 쓰레드 생성
	}

	for (int i = 0; i < 5; i++)
		cout << "Main Thread Called " << '\n';
	// 생성된 모든 쓰레드가 종료되기를 기다린다.
	WaitForMultipleObjects(5, handleThread, TRUE, INFINITE);

	return 0;
}