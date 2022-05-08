#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m; // ���ؽ� ����
static int total = 0; // ���� ����

/*
 * �Է����� ���� num���� total�� ������Ų��.
 */
void Sum(int num)
{
	for (int i = 0; i < num + 1; i++)
	{
		m.lock(); // lock�� �ɾ��ش�.
		total += 1;
		m.unlock(); // lock�� �����Ѵ�.
	}
}

int main()
{
	cout << "mutex�� �̿��Ͽ� �Ӱ� ������ ������ ���� ���� ����" << '\n';
	thread t[10]; // ������ 10�� ����
	// ������ �ʱ�ȭ
	for (int i = 0; i < 10; i++)
		t[i] = thread(Sum, 100000);

	// ����
	for (int i = 0; i < 10; i++)
		t[i].join();

	cout << total;
	return 0;
}
