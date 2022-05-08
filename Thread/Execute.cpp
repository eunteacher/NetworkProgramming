#include <iostream>
#include <thread>
using namespace std;

void Fun1()
{
	for (int i = 0; i < 10; i++)
		cout << "Fun1 " << i << "��° ȣ��" << '\n';
}

void Fun2()
{
	for (int i = 0; i < 10; i++)
		cout << "Fun2 " << i << "��° ȣ��" << '\n';
}

void ParameterFun1(int a)
{
	for (int i = 0; i < 5; i++)
		cout << "ParameterFun1 " << a + i << '\n';
}

void ParameterFun2(int a, int b)
{
	for (int i = 0; i < 5; i++)
		cout << "ParameterFun2 " << a + b + i << '\n';
}

int main()
{
	thread t1(Fun1);
	thread t2(Fun2);
	// join() : �����尡 ����Ǹ� ����
	t1.join();
	t2.join();
	thread t3(Fun1);
	thread t4(Fun2);
	// detach() : ���� ������� ���������� ����
	t3.detach();
	t4.detach();

	// ������ �Ű����� ����
	// �ִ� 4������ ���� ����
	thread t5(ParameterFun1, 1);
	t5.join();
	thread t6(ParameterFun2, 1, 2);
	t6.join();

	return 0;
}