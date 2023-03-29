#include "HYpch.h"
#include "Timer.h"

namespace Hydra
{
	Timer::Timer() : m_DeltaTime(0), m_TotalTime(0)
	{
		m_PreviusTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::Update()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsed = currentTime - m_PreviusTime;

		m_DeltaTime = elapsed;
		m_TotalTime += elapsed;
		m_PreviusTime = currentTime;
	}

	float Timer::GetDeltaTime() const
	{
		return m_DeltaTime.count();
	}

	double Timer::GetTotalTime() const
	{
		return m_TotalTime.count();
	}
}