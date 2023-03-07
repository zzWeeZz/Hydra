#pragma once
namespace Hydra
{
	enum class QueueType
	{
		Graphics,
		Compute,
		Transfer,
		Count
	};

	class DeviceQueue
	{
	public:
		virtual QueueType GetType() const { return m_QueueType; }

	protected:
		QueueType m_QueueType;
	};
}