#include <iostream>
#include <thread>
using namespace std;

static int total = 0; // ���� ����

/*
 * �Է����� ���� num���� total�� ������Ų��.
 */
void Sum(int num)
{
	for (int i = 0; i < num + 1; i++)
		total += 1;
}

int main()
{
	cout << "�ϳ��� �ڿ��� ���� ���� ��� " << '\n';

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
