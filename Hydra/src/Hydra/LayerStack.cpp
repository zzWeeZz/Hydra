#include "HYpch.h"
#include "LayerStack.h"

#include <sstream>
namespace Hydra
{
	LayerStack::LayerStack()
	{
		m_LayerStackIterator = m_LayerStack.begin();
	}

	void LayerStack::Push(Layer* layer)
	{
		m_LayerStackIterator = m_LayerStack.emplace(m_LayerStackIterator, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_LayerStack.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::Pop(Layer* layer)
	{
		const auto it = std::ranges::find(m_LayerStack, layer);
		if (it != m_LayerStack.end())
		{
			m_LayerStack.erase(it);
			layer->OnDetach();
			--m_LayerStackIterator;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		const auto it = std::ranges::find(m_LayerStack, layer);
		if (it != m_LayerStack.end())
		{
			m_LayerStack.erase(it);
			layer->OnDetach();
		}
	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_LayerStack)
		{
			delete layer;
			layer = nullptr;
		}
	}
}
