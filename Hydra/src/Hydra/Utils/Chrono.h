#pragma once
#include "Timer.h"

namespace Hydra
{
	class Chrono
	{
	public:
		static float Timestep();
		static float TotalTime();
		static float TimeStamp();
		static void UpdateTimeData();

	private:
		static Timer s_Timer;
	};
}


