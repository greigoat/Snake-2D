#pragma once

#include <memory>

// Pretty wrapper for weak_ptr
template<typename T>
class weakPtr
{
	private:

	public:

	weakPtr() {}
	weakPtr(const std::weak_ptr<T>& ptr) : ptr(ptr) {}
	weakPtr(const std::shared_ptr<T>& ptr) : ptr(ptr) {}
	weakPtr(std::shared_ptr<T>&& ptr) : ptr(ptr) {}

	std::weak_ptr<T> ptr;

	bool operator!()
	{
		return ptr.expired();
	}

	bool operator !=(std::nullptr_t)
	{
		return !ptr.expired();
	}

	bool operator ==(std::nullptr_t)
	{
		return ptr.expired();
	}

	T* operator->()
	{
		return ptr.lock().get();
	}
};