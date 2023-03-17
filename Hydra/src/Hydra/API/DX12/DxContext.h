#pragma once
#include "Hydra/API/Context.h"

namespace Hydra
{
	class DxContext : public Context
	{
	public:
		void Initalize(const ContextSpecification& specs) override;
		void Shutdown() override;
		void WaitForIdle() override;
	private:

	};
}
