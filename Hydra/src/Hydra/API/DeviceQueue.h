#pragma once
#include "Hydra/Utils/EnumUtils.h"
namespace Hydra
{
	enum class QueueType : size_t
	{
		Graphics = BIT(0),
		Compute = BIT(1),
		Transfer = BIT(2),
		Count = BIT(3)
	};

	class DeviceQueue
	{
	public:
		virtual QueueType GetType() const { return m_QueueType; }

		

	protected:
		QueueType m_QueueType;
	};
}