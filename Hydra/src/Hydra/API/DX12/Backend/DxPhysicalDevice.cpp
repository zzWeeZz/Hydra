#include "HYpch.h"
#include "DxPhysicalDevice.h"
namespace Hydra
{
	DxPhysicalDevice::DxPhysicalDevice(const PhysicalDeviceSpecifications& specs) : PhysicalDevice(specs)
	{
		Create();
	}
	void DxPhysicalDevice::Create()
	{
		
		HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(m_Factory.GetAddressOf()));

		bool adapterfound = false;

		while (m_Factory->EnumAdapters1(m_AdapterIndex, m_Adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// we dont want a software device
				m_AdapterIndex++; // add this line here. Its not currently in the downloadable project
				continue;
			}

			// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
			hr = D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
			{
				adapterfound = true;
				HY_CORE_INFO("DX12: Physical Device found!");
				break;
			}

			m_AdapterIndex++;
		}
	}
}