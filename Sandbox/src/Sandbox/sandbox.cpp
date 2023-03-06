#include "Hydra/Hydra.h"


class SandBox : public Hydra::Application
{
public:
	SandBox()
	{
		
	}

	~SandBox()
	{

	}
private:
};

Hydra::Application* Hydra::CreateApplication()
{
	return new SandBox();
}