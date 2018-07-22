#ifndef _IDVECTOR_H_
#define _IDVECTOR_H_

#include <vector>
#include <queue>

template<class T>
class IdVector
{
public:
	unsigned int insert(const T& value)
	{
		unsigned int index = 0; 
		if (!recovery_list.empty())
		{
			index = recovery_list.front(); 
			recovery_list.pop(); 
			obj_list[index] = value; 
		}
		else
		{
			index = obj_list.size(); 
			obj_list.push_back(value); 
		}

		++_size; 

		return index; 
	}

	void remove(unsigned int index)
	{
		recovery_list.push(index); 
		--_size; 
	}

	unsigned int size(){return _size; }

	T& operator[](unsigned int index){return obj_list[index]; }

private:
	int _size; 
	std::vector<T> obj_list; 
	std::queue<unsigned int> recovery_list; 
}; 

#endif /* _IDVECTOR_H_ */
