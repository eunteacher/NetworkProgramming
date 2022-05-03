#pragma once

#ifndef __Packet_H
//#define  __Packet_H

#include <Windows.h>

#define PACKETBUFFERSIZE 8192 // ��Ŷ�� ũ��
#define PACKETHEADERSIZE 4

class Packet
{
public:
	Packet();
	Packet(unsigned short idValue); // ��Ŷ�� ������ �� �������� ID �Է� 
	Packet(const Packet& source); 

	virtual ~Packet();

public:
	bool IsValidHeader();

	Packet& Id(unsigned short id); // �������� ID ����
	unsigned short Id();

	void Clear();

	// packetBuffer�� �����͸� ����
	unsigned short GetDataFieldSize();
	int GetPacketSize() { return GetDataFieldSize() + PACKETHEADERSIZE; }
	int GetReceivedSize() { return receivedSize; }
	char* GetPacketBuffer() { return packetBuffer; }
	void CopyToBuffer(char* buffer, int size);

	void ReadData(void* buffer, int size);
	void WriteData(void* buffer, int size);

protected:
	struct HEADER
	{
		unsigned short* dataSize;
		unsigned short* protocolID;
	};

	HEADER packetHeader;

private:
	char packetBuffer[PACKETBUFFERSIZE];
	char* dataField;
	char* readPosition, *writePosition;
	char* endOfDataField;
	int receivedSize;
};
#endif
