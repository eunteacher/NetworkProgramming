#include <iostream>
#include <thread>
#include <Windows.h>
using namespace std;

// �����带 �Ҵ� �޾� �����ϴ� �Լ�
// ���� ������ʹ� ������ ���ο� �������̹Ƿ� ���ķ� ����
// �Լ��� ������ �Ͱ� ���ÿ� ������� ����
DWORD __stdcall ThreadRunner(LPVOID IpParameter)
{
	int* arg;
	int count; // �ݺ����� ������ Ƚ��

	count = 3;
	arg = (int*)IpParameter;

	// ���� ������ ���� �۾��� �����Ѵ�. �� �Լ��� ���ο� �������̹Ƿ� ���� ������ ���� �����忡 ������ ��ġ�� �ʴ´�.
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
		// ������ ����
		// ���ķ� �۾��� �����带 5�� �����ϰ�, �Ű������� arr�� �Է��Ѵ�.
		// 5�� �����尡 ��� ThreadRunner �Լ��� �����Ǿ�����, 5���� ���� �ٸ� ThreadRunner �Լ��� ȣ��ȴٰ� �����ϸ� �ȴ�.
		handleThread[i] = CreateThread(0, 0, ThreadRunner, &arr[i], 0, &threadId); // ������ ����
	}

	for (int i = 0; i < 5; i++)
		cout << "Main Thread Called " << '\n';
	// ������ ��� �����尡 ����Ǳ⸦ ��ٸ���.
	WaitForMultipleObjects(5, handleThread, TRUE, INFINITE);

	return 0;
}