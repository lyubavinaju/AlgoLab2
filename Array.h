#pragma once

#include <stdlib.h>
#include <stdexcept>

template <typename T>
class Array final
{
private:
	size_t _size;
	size_t _capacity;
	T* pItems;
	static constexpr size_t DEFAULT_INITIAL_CAPACITY = 16;
	static constexpr size_t INC_PARAM = 2;

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

	Array(size_t capacity) : _capacity(capacity), _size(0) {
		pItems = static_cast<T*>(malloc(_capacity * sizeof(T)));
	}

	Array() : Array(DEFAULT_INITIAL_CAPACITY) {
	}

	Array(const Array& other) : Array(other._capacity) {
		_size = other._size;
		for (size_t i = 0; i < _size; i++) {
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
			_size = other.size;
			_capacity = other._capacity;
			pItems = other._capacity;
			other.pItems = nullptr;
		}
		return *this;
	}

	~Array() {
		for (size_t i = 0; i < _size; i++) {
			pItems[i].~T();
		}
		free(pItems);
	}

	size_t size() const {
		return _size;
	}

	size_t insert(size_t index, const T& value) {
		if (_size == _capacity) {
			_capacity *= INC_PARAM;
			T* p = static_cast<T*>(malloc(_capacity * sizeof(T)));

			for (size_t i = 0; i < index; i++) {
				new (p + i) T(std::move(pItems[i]));
			}
			for (size_t i = index; i < _size; i++) {
				new (p + i + 1) T(std::move(pItems[i]));
			}
			for (size_t i = 0; i < _size; i++) {
				pItems[i].~T();
			}
			free(pItems);
			pItems = p;
		}
		else {
			for (size_t i = _size; i > index; i--) {
				new (pItems + i) T(std::move(pItems[i - 1]));
				pItems[i - 1].~T();
			}
		}

		new(pItems + index) T(value);
		_size++;
		return _size - 1;
	}

	size_t insert(const T& value) {
		return insert(_size, value);
	}

	void remove(size_t index) {
		pItems[index].~T();
		for (size_t i = index + 1; i < _size; i++) {
			new (pItems + i - 1) T(std::move(pItems[i]));
			pItems[i].~T();
		}
		_size--;
	}

	const T& operator[] (size_t index) const {
		return pItems[index];
	}

	T& operator[] (size_t index) {
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
