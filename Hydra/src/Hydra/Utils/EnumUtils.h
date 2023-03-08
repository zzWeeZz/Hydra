#pragma once

#define BIT(x) (1 << x)

namespace Hydra
{

	template<typename T>
	T operator|(const T& lhs, const T& rhs)
	{
		return static_cast<T>(static_cast<size_t>(lhs) | static_cast<size_t>(rhs));
	}

	template<typename T>
	T operator|=(T& lhs, const T& rhs)
	{
		lhs = static_cast<T>(static_cast<size_t>(lhs) | static_cast<size_t>(rhs));
		return lhs;
	}

	template<typename T>
	bool operator&(const T& lhs, const T& rhs)
	{
		return static_cast<bool>(static_cast<size_t>(lhs) & static_cast<size_t>(rhs));
	}
}

//template<typename T>
//T operator|(const T& lhs, const T& rhs)
//{
//	return static_cast<size_t>(lhs) | static_cast<size_t>(rhs);
//}
//
//template<typename T>
//T operator|(const T& lhs, const T& rhs)
//{
//	return static_cast<size_t>(lhs) | static_cast<size_t>(rhs);
//}