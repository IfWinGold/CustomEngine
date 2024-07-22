#pragma once

// [계약서 / 결재]

// CPU [   ]		GPU [    ] 
// 한국 [   ]		베트남 [    ]
// 정보들을  GPU쪽에 전달하는, 내가 GPU의 어느어느 공간을 사용할 것이다.
// 정확하게는 VRAM?


class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature> _signature;
};

