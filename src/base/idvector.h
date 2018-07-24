#ifndef _IDVECTOR_H_
#define _IDVECTOR_H_

#include <vector>
#include <queue>

template<class T>
class IdVector
{
public:
	template<class U, class V=IdVector>
	class _iterator
	{
		friend class IdVector; 
	public:
		_iterator():index(-1), container(0){}

		_iterator<U, V>& operator++()
		{
			++index; 
			while (index < container->_end.index && !container->obj_use[index])
			{
				++index; 
			}
			return *this; 
		}
		const _iterator<U, V> operator++(int)
		{
			_iterator<U, V> t = *this; 
			operator++(); 
			return t; 
		}
		_iterator<U, V>& operator--()
		{
			--index; 
			while (index >= container->_begin.index && !container->obj_use[index])
			{
				--index; 
			}
			return *this; 
		}
		const _iterator<U, V> operator--(int)
		{
			_iterator<U, V> t = *this; 
			operator--(); 
			return t; 
		}

		operator U*() { return &container->obj_list[index]; }
		U* operator->() { return &container->obj_list[index]; }
		U& operator*() { return container->obj_list[index]; }
		bool operator==(const _iterator<U, V> &_right) const { return (index == _right.index && container == _right.container); }
		bool operator!=(const _iterator<U, V> &_right) const { return (index != _right.index || container != _right.container); }

	protected:
		unsigned int index; 
		V *container; 
		_iterator(unsigned int index, V *parent=0):index(index), container(parent){}
	}; 
	typedef _iterator<T> iterator; 
	typedef _iterator<const T, const IdVector<T> > const_iterator; 

public:
	IdVector(unsigned int size=4096):_begin(0), _end(0), _size(0)
	{
		_begin.container = this; 
		_end.container = this; 
		obj_list.reserve(size); 
		obj_use.reserve(size); 
	}

	IdVector(const IdVector<T> &id_vector):_begin(0), _end(0), _size(id_vector._size)
	{
		*this = id_vector; 
		_begin.container = this; 
		_end.container = this; 
	}

	unsigned int insert(const T &value)
	{
		unsigned int index = -1; 
		if (recovery_list.size() == 0)
		{
			index = (unsigned int)obj_list.size(); 
			obj_list.push_back(value); 
			obj_use.push_back(true); 
		}
		else
		{
			index = recovery_list.front(); 
			recovery_list.pop(); 
			new (&obj_list[index])T(value); 
			obj_use[index] = true; 
		}

		if (_begin.index == _end.index)
		{
			_end.index = (_begin.index = index) + 1; 
		}
		else
		{
			if (_begin.index > index) _begin.index = index; 
			if (_end.index <= index) _end.index = index + 1; 
		}

		++_size; 
		return index; 
	}

	void erase(unsigned int index)
	{
		if (index >= obj_use.size() || !obj_use[index]) return; 

		obj_list[index].~T(); 
		recovery_list.push(index); 
		obj_use[index] = false; 

		if (_begin.index == index)
		{
			unsigned int i = index; 
			while(i < obj_use.size() && !obj_use[i] && i != _end.index) ++i; 
			_begin.index = i; 
		}
		if (_end.index == index + 1 && _begin.index != _end.index)
		{
			unsigned int i = index; 
			while(i < obj_use.size() && !obj_use[i] && i != _begin.index) --i; 
			_end.index = i + 1; 
		}

		--_size; 
	}

	void clear()
	{
		for (unsigned int i = 0;  i < obj_list.size();  ++i)
		{
			if (obj_use[i])
			{
				obj_list[i].~T(); 
			}
		}

		obj_list.clear(); 
		obj_use.clear(); 

		while (recovery_list.size() != 0) recovery_list.pop(); 

		_begin.index = 0; 
		_end.index = 0; 
	}

	iterator find(unsigned int index)
	{
		if (index < obj_use.size() && obj_use[index])
		{
			return iterator(index, this); 
		}
		return _end; 
	}

	const_iterator find(unsigned int index) const
	{
		if (index < obj_use.size() && obj_use[index])
		{
			return const_iterator(index, this); 
		}
		return const_iterator(_end.index, this); 
	}

	bool exist(unsigned int index)
	{
		if (index < obj_use.size() && obj_use[index])
		{
			return true; 
		}
		return false; 
	}

	bool empty() const { return (0 == _size); }
	unsigned int size() const { return _size; }

	iterator begin() { return _begin; }
	iterator end() { return _end; }

	const_iterator begin() const { return const_iterator(_begin.index, this); }
	const_iterator end() const { return const_iterator(_end.index, this); }

	T& operator[](unsigned int i) { return obj_list[i]; }
	const T& operator[](unsigned int i) const { return obj_list[i]; }

protected:
	std::vector<T> obj_list; 
	std::vector<bool> obj_use; 
	std::queue<unsigned int> recovery_list; 

	iterator _begin; 
	iterator _end; 
	unsigned int _size; 
}; 

#endif /* _IDVECTOR_H_ */
