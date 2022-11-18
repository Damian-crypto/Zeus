#pragma once

#include "layer.h"

namespace zeus
{
	class LayerQueue
	{
	private:
		std::vector<Layer*> m_Layers;

	public:
		LayerQueue();

		void PushBack(Layer* layer);
		void PushFront(Layer* layer);
		void PopBack();
		void PopFront();

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	};
}
