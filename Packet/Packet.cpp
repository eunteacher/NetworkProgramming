#include "Packet.h"

Packet::Packet()
	:dataField(0), readPosition(0), writePosition(0), receivedSize(0)
{
	Clear();
}
// ������ �� �������� ID �Է�
Packet::Packet(unsigned short idValue)
	: dataField(0), readPosition(0), writePosition(0), receivedSize(0)
{
	Clear();
	Id(idValue);
}
// ���� ������ 
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
	// ���ŵ� ��Ŷ�� ũ�Ⱑ �ּ� 4����Ʈ �̻��� ��
	if (GetPacketSize() >= PACKETHEADERSIZE) return true;
	return false;
}
// �������� ID ����
Packet& Packet::Id(unsigned short id)
{
	*packetHeader.protocolID = id;
	return *this;
}

unsigned short Packet::Id()
{
	return *packetHeader.protocolID;
}
// �ʱ�ȭ
void Packet::Clear()
{
	ZeroMemory(packetBuffer, 0, PACKETBUFFERSIZE);

	packetHeader.dataSize = (unsigned short*)(packetBuffer + 0); // 0~1��°
	packetHeader.protocolID = (unsigned short*)((char*)packetBuffer + 2); // 1~2��°

	dataField = &packetBuffer[4]; // 3~����������
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
// ��Ŷ ������ �б�
void Packet::ReadData(void* buffer, int size)
{
	if (readPosition + size > dataField + GetDataFieldSize() || readPosition + size > endOfDataField) return;
	CopyMemory(buffer, readPosition, size);
	readPosition += size;
}
// ��Ŷ ������ ����
void Packet::WriteData(void* buffer, int size)
{
	if (writePosition + size > endOfDataField) return;
	CopyMemory(writePosition, buffer, size);
	writePosition += size;
	receivedSize += size;

	*packetHeader.dataSize += size;
}
