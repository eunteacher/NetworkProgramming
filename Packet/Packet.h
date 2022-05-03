#pragma once

#ifndef __Packet_H
//#define  __Packet_H

#include <Windows.h>

#define PACKETBUFFERSIZE 8192 // 패킷의 크기
#define PACKETHEADERSIZE 4

class Packet
{
public:
	Packet();
	Packet(unsigned short idValue); // 패킷을 생성할 때 프로토콜 ID 입력 
	Packet(const Packet& source); 

	virtual ~Packet();

public:
	bool IsValidHeader();

	Packet& Id(unsigned short id); // 프토토콜 ID 변경
	unsigned short Id();

	void Clear();

	// packetBuffer의 데이터를 복사
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
