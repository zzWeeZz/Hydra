#pragma once

constexpr uint32_t g_FramesInFlight = 3;


#define HY_GET_INLINE [[nodiscard]] FORCEINLINE

template<typename T>
using PerFrameInFlight = std::array<T, g_FramesInFlight>;
