#pragma 

// 인력 사무소
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	// - ComPtr 은 COM 객체를 받아주는 일종의 스마트포인터
	ComPtr<ID3D12Debug>		_debugController; 
	ComPtr<IDXGIFactory>	_dxgi;				// 화면 관련 기능
	ComPtr<ID3D12Device>	_device;			// 각종 객체 생성
};

