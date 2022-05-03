#include "Packet.h"

Packet::Packet()
	:dataField(0), readPosition(0), writePosition(0), receivedSize(0)
{
	Clear();
}
// 생성할 때 프로토콜 ID 입력
Packet::Packet(unsigned short idValue)
	: dataField(0), readPosition(0), writePosition(0), receivedSize(0)
{
	Clear();
	Id(idValue);
}
// 복사 생성자 
Packet::Packet(const Packet& source)
	: dataField(0), readPosition(0), writePosition(0), receivedSize(0)
{
	Clear();
	CopyMemory(packetBuffer, source.packetBuffer, PACKETBUFFERSIZE);
	receivedSize = source.receivedSize;

	DWORD offset;
	offset = (DWORD)source.readPosition - (DWORD)source.dataField;
	readPosition += offset;

	offset = (DWORD)source.writePosition - (DWORD)source.dataField;
	writePosition += offset;
}

Packet::~Packet()
{
}

bool Packet::IsValidHeader()
{
	// 수신된 패킷의 크기가 최소 4바이트 이상일 때
	if (GetPacketSize() >= PACKETHEADERSIZE) return true;
	return false;
}
// 프로토콜 ID 변경
Packet& Packet::Id(unsigned short id)
{
	*packetHeader.protocolID = id;
	return *this;
}

unsigned short Packet::Id()
{
	return *packetHeader.protocolID;
}
// 초기화
void Packet::Clear()
{
	ZeroMemory(packetBuffer, 0, PACKETBUFFERSIZE);

	packetHeader.dataSize = (unsigned short*)(packetBuffer + 0); // 0~1번째
	packetHeader.protocolID = (unsigned short*)((char*)packetBuffer + 2); // 1~2번째

	dataField = &packetBuffer[4]; // 3~마지막까지
	readPosition = dataField;
	writePosition = dataField;
	endOfDataField = &dataField[PACKETBUFFERSIZE - 1];

	Id(0);

	receivedSize = 0;
}

unsigned short Packet::GetDataFieldSize()
{
	return *packetHeader.dataSize;
}

void Packet::CopyToBuffer(char* buffer, int size)
{
	Clear();
	CopyMemory(packetBuffer, buffer, size);
	receivedSize += size;
}
// 패킷 데이터 읽기
void Packet::ReadData(void* buffer, int size)
{
	if (readPosition + size > dataField + GetDataFieldSize() || readPosition + size > endOfDataField) return;
	CopyMemory(buffer, readPosition, size);
	readPosition += size;
}
// 패킷 데이터 쓰기
void Packet::WriteData(void* buffer, int size)
{
	if (writePosition + size > endOfDataField) return;
	CopyMemory(writePosition, buffer, size);
	writePosition += size;
	receivedSize += size;

	*packetHeader.dataSize += size;
}
