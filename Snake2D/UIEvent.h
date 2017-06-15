#pragma once

#include <functional>
#include <vector>


/// <summary> 
/// Represents UI Event
/// </summary>
struct UIEvent
{
	void AddListener(std::function<void()> listener);
	void Invoke();

	private:
	std::vector<std::function<void()>> listeners;
};