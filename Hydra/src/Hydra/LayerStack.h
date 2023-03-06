#pragma once
#include "Hydra/Layer.h"
namespace Hydra
{
	class LayerStack
	{
	public:
		LayerStack();
		void Push(Layer* layer);
		void PushOverlay(Layer* layer);
		void Pop(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_LayerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayerStack.end(); }
		~LayerStack();
	private:
		std::vector<Layer*> m_LayerStack;
		std::vector<Layer*>::iterator m_LayerStackIterator;
	};
}

