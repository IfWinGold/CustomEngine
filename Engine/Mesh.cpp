#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	// GPU의 공간을 어떤 용도로 사용을 할 것이냐?
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 버퍼를 버퍼 사이즈만큼 할당을 받고싶다.
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// GPU에 버퍼를 저장할 공간을 _vertexBuffer에 할당받음 
	// DEVICE에 대해서 이루어지는 일들은 커맨드큐가 아닌 당장 이루어지는 ..
	// Resource를 만들는 단계
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer)); //엄밀히 말하면 Ram이 아닌 GPU의 공간을 가르킴(VRAM?)

	// Resource에 데이터를 복사를 하려면 막 복사하는것이 아닌 다음과 같은 단계를 거쳐야함.
	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // vertexDataBuffer 와 _vertexBuffer를 매핑(뚜껑 열기?) 
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // vertexDataBuffer에 데이터를 밀어넣음
	_vertexBuffer->Unmap(0, nullptr); // Unmap으로 매핑 종료(뚜껑 닫기?)



	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	//TODO
	// 1) Buffer에다가 데이터 세팅
	// 2) TableDescHealp에다가 CBV 전달
	// 3) 모두 세팅이 끝났으면 TableDescHeap 커밋
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}

	GEngine->GetTableDescHeap()->CommitTable();

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}