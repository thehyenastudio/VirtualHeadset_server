#pragma once
#include <wx/thread.h>

#include <queue>

template <typename T>
class Queue
{
public:
	explicit Queue(std::size_t maxSize = 10) : cond(mutex), maxSize(maxSize) {}

	void Push(T&& item)
	{
		wxMutexLocker lock(mutex);
		while (queue.size() >= maxSize)
		{
			queue.pop(); // Drop oldest
		}

		queue.push(std::move(item));
		cond.Signal();
	}

	bool WaitAndPop(T& item, wxThread* thread)
	{
		wxMutexLocker lock(mutex);
		while (queue.empty())
		{
			if (thread->TestDestroy())
			{
				return false;
			}
			cond.Wait();
		}

		item = std::move(queue.front());
		queue.pop();
		cond.Signal();
		return true;
	}

	void NotifyAll()
	{
		wxMutexLocker lock(mutex);
		cond.Broadcast();
	}

private:
	std::queue<T> queue;
	mutable wxMutex mutex;
	wxCondition cond;
	std::size_t maxSize;
};