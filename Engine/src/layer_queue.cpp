#include "corepch.h"
#include "layer_queue.h"

namespace zeus
{
	LayerQueue::LayerQueue()
	{
	}

	void LayerQueue::PushBack(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerQueue::PushFront(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin(), layer);
	}

	void LayerQueue::PopBack()
	{
		m_Layers.pop_back();
	}

	void LayerQueue::PopFront()
	{
		m_Layers.erase(m_Layers.begin());
	}
}
