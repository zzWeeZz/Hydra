#pragma once
#include <chrono>
#include <iostream>

namespace Hydra
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Update();
		float GetDeltaTime() const;
		double GetTotalTime() const;
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviusTime;
		std::chrono::duration<float> m_DeltaTime;
		std::chrono::duration<double> m_TotalTime;
	};
}