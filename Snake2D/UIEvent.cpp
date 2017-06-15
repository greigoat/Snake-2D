#include "UIEvent.h"

// TODO: Make sure listeners array does not have dublcates
void UIEvent::AddListener(std::function<void()> listener)
{
	listeners.push_back(listener);
}

void UIEvent::Invoke()
{
	for (auto& listener : listeners)
	{
		listener();
	}
}