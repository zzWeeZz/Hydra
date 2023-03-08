#pragma once

constexpr uint32_t g_FramesInFlight = 3;

template<typename T>
using PerFrameInFlight = std::array<T, g_FramesInFlight>;
