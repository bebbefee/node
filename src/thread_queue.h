#ifndef _THREAD_QUEUE_H_
#define _THREAD_QUEUE_H_

#include <mutex>
#include <deque>

template<typename T>
class ThreadQueue
{
public:
	void push(T && item) 
	{
		std::lock_guard<std::mutex> g(mutex);
		deque.push_back(std::forward<T>(item));		
	}

	void push(T const & item) 
	{
		std::lock_guard<std::mutex> g(mutex);
		deque.push_back(item);
	}

	void push_back(T const & item)
	{
		std::lock_guard<std::mutex> g(mutex);
		deque.push_back(item);
	}	

	bool pop(T & outVal) 
	{ 
		std::lock_guard<std::mutex> g(mutex);
		if (deque.empty()) return false;
		outVal = deque.front();
		deque.pop_front();
		return true;
	}

	bool pop_front()
	{
		std::lock_guard<std::mutex> g(mutex);
		if (deque.empty()) return false;
		deque.pop_front();
		return true;
	}	

	T& front()
	{
		return deque.front(); 
	}

	bool empty() 
	{
		std::lock_guard<std::mutex> g(mutex);
		return deque.empty();
	}

	void clear() 
	{
		std::lock_guard<std::mutex> g(mutex);
		deque.clear();
	}

	void clear(std::function< void(T &) > f) 
	{
		std::lock_guard<std::mutex> g(mutex);
		for (auto &o : deque) f(o);
		deque.clear();
	}

private:
	std::mutex      mutex;
	std::deque<T>   deque;
};

#endif /* !_THREAD_QUEUE_H_ */