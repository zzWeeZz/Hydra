#pragma once
#include "Hydra/API/Resource.h"
namespace Hydra
{
	class Pipeline : public Resource
	{
	public:
		virtual ~Pipeline() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}