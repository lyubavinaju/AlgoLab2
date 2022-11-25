#pragma once

#include <stdlib.h>
#include <stdexcept>

template <typename T>
class Array final
{
private:
	int _size;
	int _capacity;
	T* pItems;
	static constexpr int DEFAULT_INITIAL_CAPACITY = 16;
	static constexpr int INC_PARAM = 2;

	T* const begin() const {
		return pItems;
	}
	T* const end() const {
		return pItems + _size - 1;
	}
	void swap(Array& other) {
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
		std::swap(pItems, other.pItems);
	}
public:

	Array(int capacity) : _capacity(capacity), _size(0) {
		pItems = static_cast<T*>(malloc(_capacity * sizeof(T)));
	}

	Array() : Array(DEFAULT_INITIAL_CAPACITY) {
	}

	Array(const Array& other) : Array(other._capacity) {
		_size = other._size;
		for (int i = 0; i < _size; i++) {
			new (pItems + i) T(other.pItems[i]);
		}
	}

	Array(Array&& other) :_size(other._size), _capacity(other._capacity), pItems(other.pItems) {
		other.pItems = nullptr;
		other._size = 0;
		other._capacity = 0;
	}

	Array& operator=(const Array& other) {
		if (this != &other) {
			swap(Array(other));
		}
		return *this;
	}

	Array& operator=(Array&& other) {
		if (this != &other) {
			std::swap(_size, other._size);
			std::swap(_capacity, other._capacity);
			std::swap(pItems, other.pItems);
		}
		return *this;
	}

	~Array() {
		for (int i = 0; i < _size; i++) {
			pItems[i].~T();
		}
		free(pItems);
	}

	int size() const {
		return _size;
	}

	int insert(int index, const T& value) {
		if (_size == _capacity) {
			_capacity *= INC_PARAM;
			T* p = static_cast<T*>(malloc(_capacity * sizeof(T)));

			for (int i = 0; i < index; i++) {
				new (p + i) T(std::move(pItems[i]));
			}
			for (int i = index; i < _size; i++) {
				new (p + i + 1) T(std::move(pItems[i]));
			}
			for (int i = 0; i < _size; i++) {
				pItems[i].~T();
			}
			free(pItems);
			pItems = p;
		}
		else {
			if (index != _size) {
				new (pItems + _size) T(std::move(pItems[_size - 1]));
				for (int i = _size - 1; i > index; i--) {
					pItems[i] = std::move(pItems[i - 1]);
				}
				pItems[index].~T();
			}
		}
		new(pItems + index) T(value);
		_size++;
		return index;
	}

	int insert(const T& value) {
		return insert(_size, value);
	}

	void remove(int index) {
		for (int i = index; i < _size - 1; i++) {
			pItems[i] = std::move(pItems[i + 1]);
		}
		pItems[_size - 1].~T();
		_size--;
	}

	const T& operator[] (int index) const {
		return pItems[index];
	}

	T& operator[] (int index) {
		return pItems[index];
	}

	class Iterator {
	protected:
		T* const start;
		T* const end;
		T* cur;
	public:
		Iterator(T* const _start, T* const _end) : start(_start), end(_end) {
			cur = _start;
		}
		const T& get() const {
			return *cur;
		}
		void set(const T& value) {
			*cur = value;
		}
		void next() {
			if (start <= end) {
				cur++;
			}
			else {
				cur--;
			}
		}
		bool hasNext() const {
			if (start <= end) {
				return cur + 1 <= end;
			}
			else {
				return cur - 1 >= end;
			}
		}
	};

	class ConstIterator : public Iterator {
	public:
		ConstIterator(T* const _start, T* const _end) :Iterator(_start, _end) {

		}
		void set(const T& value) = delete;
	};

	Iterator iterator() {
		return Iterator(begin(), end());
	}
	ConstIterator iterator() const {
		return ConstIterator(begin(), end());
	}

	Iterator reverseIterator() {
		return Iterator(end(), begin());
	}
	ConstIterator reverseIterator() const {
		return ConstIterator(end(), begin());
	}
};
