#include "HYpch.h"
#include "Chrono.h"

Hydra::Timer Hydra::Chrono::s_Timer;
float Hydra::Chrono::Timestep()
{
	return s_Timer.GetDeltaTime();
}

float Hydra::Chrono::TotalTime()
{
	return static_cast<float>(s_Timer.GetTotalTime());
}

float Hydra::Chrono::TimeStamp()
{
	return 0.0f;
}

void Hydra::Chrono::UpdateTimeData()
{
	s_Timer.Update();
}
