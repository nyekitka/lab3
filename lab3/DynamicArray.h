#pragma once
#include <initializer_list>
#include <exception>
template <class T>
class DynamicArray {
private:
	T* array;
	size_t size;
	size_t capacity;
public:

	/*										exceptions										*/

	class DAException {
	private:
		size_t id;
	public:
		DAException(const size_t id) {
			this->id = id;
		}
		const char* message() const {
			if (id == 1) return "Error 1: array subscript out of range.";
			else return "Error 2: bad allocation of dynamic memory.";
		}
	};

	/*										constructors									*/
	DynamicArray() {
		array = nullptr;
		size = 0;
		capacity = 0;
	}
	template <size_t len>
	DynamicArray(const T(&items)[len]) {
		capacity = len * 2;
		size = len;
		try {
			array = new T[capacity];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		for (size_t i = 0; i < len; i++) array[i] = items[i];
	}
	DynamicArray(const std::initializer_list<T>& list) {
		size = list.size();
		capacity = 2 * size;
		try {
			array = new T[capacity];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		size_t i = 0;
		for (auto iter : list) {
			array[i] = iter;
			i++;
		}
	}
	DynamicArray(const size_t size, const T elem = T()) {
		this->size = size;
		capacity = 2 * size;
		try {
			array = new T[capacity];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		for (size_t i = 0; i < size; i++) array[i] = elem;
	}
	DynamicArray(const DynamicArray<T>& other) {
		size = other.size;
		capacity = other.capacity;
		try {
			array = new T[capacity];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		for (size_t i = 0; i < size; i++) array[i] = other.array[i];
	}
	DynamicArray(DynamicArray<T>&& other) {
		size = other.size;
		capacity = other.capacity;
		array = other.array;
		other.array = nullptr;
		other.size = other.capacity = 0;
	}
	/*										destructor										*/
	~DynamicArray() {
		if (array != nullptr) delete[] array;
	}
	/*										methods											*/
	size_t length() const {
		return size;
	}
	size_t get_capacity() const {
		return capacity;
	}
	void resize(const size_t newsize) {
		if (newsize <= size) size = newsize;
		else if (newsize <= capacity) {
			for (size_t i = size; i < newsize; i++) array[i] = T();
			size = newsize;
		}
		else {
			capacity = newsize * 2;
			T* newarr;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = 0; i < size; i++) newarr[i] = array[i];
			for (size_t i = size; i < newsize; i++) newarr[i] = T();
			size = newsize;
			delete[] array;
			array = newarr;
		}
	}
	void shrink_to_fit() {
		capacity = size;
		T* newarr;
		try {
			newarr = new T[size];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		for (size_t i = 0; i < size; i++) newarr[i] = array[i];
		delete[] array;
		array = newarr;
	}
	void reserve(const size_t newcap) {
		if (newcap > capacity) {
			capacity = newcap;
			T* newarr;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = 0; i < size; i++) newarr[i] = array[i];
			delete[] array;
			array = newarr;
		}
	}
	void clear() {
		if (array != nullptr) delete[] array;
		capacity = size = 0;
		array = nullptr;
	}
	void append(const T& value) {
		if (capacity >= size + 1) {
			array[size] = value;
			size++;
		}
		else {
			capacity = (size + 1) * 2;
			T* newarr;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = 0; i < size; i++) newarr[i] = array[i];
			newarr[size] = value;
			delete[] array;
			array = newarr;
			size++;
		}
	}
	void prepend(const T& value) {
		if (capacity >= size + 1) {
			size++;
			for (size_t i = size - 1; i > 0; i--) array[i] = array[i - 1];
			array[0] = value;
		}
		else {
			T* newarr;
			capacity *= 2;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = size; i > 0; i--) newarr[i] = array[i - 1];
			newarr[0] = value;
			delete[] array;
			array = newarr;
			size++;
		}
	}
	void insert(const T& value, const size_t index) {
		if (index >= size) throw DAException(1);
		if (capacity >= size + 1) {
			for (size_t i = size; i > index; i--) array[i] = array[i - 1];
			array[index] = value;
			size++;
		}
		else {
			T* newarr;
			capacity *= 2;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = 0; i < index; i++) newarr[i] = array[i];
			newarr[index] = value;
			for (size_t i = index + 1; i <= size; i++) newarr[i] = array[i - 1];
			size++;
			delete[] array;
			array = newarr;
		}
	}
	void erase(const size_t index) {
		if (index >= size) throw DAException(1);
		for (size_t i = index + 1; i < size; i++) array[i - 1] = array[i];
		size--;
	}
	int find(const T& value) const {
		for (size_t i = 0; i < size; i++) {
			if (array[i] == value) return i;
		}
		return -1;
	}
	int rfind(const T& value) const {
		for (size_t i = size - 1; i >= 0; i--) {
			if (array[i] == value) return i;
		}
		return -1;
	}
	DynamicArray<T> subarray(const size_t begin, const size_t end) const {
		if (end >= size || begin > end) throw DAException(1);
		DynamicArray<T> ans(end - begin + 1);
		for (size_t i = 0; i <= end - begin; i++) ans[i] = array[i + begin];
		return ans;
	}
	/*										operators										*/
	DynamicArray& operator= (const DynamicArray<T>& other) {
		T* newarr;
		size = other.size;
		capacity = other.capacity;
		try {
			newarr = new T[other.capacity];
		}
		catch (std::bad_alloc err) {
			throw DAException(2);
		}
		for (size_t i = 0; i < size; i++) newarr[i] = other.array[i];
		delete[] array;
		array = newarr;
		return *this;
	}
	DynamicArray& operator= (DynamicArray<T>&& other) {
		delete[] array;
		size = other.size;
		capacity = other.capacity;
		array = other.array;
		other.array = nullptr;
		return *this;
	}
	T& operator[] (const size_t index) const {
		if (index >= size) throw DAException(1);
		return array[index];
	}
	bool operator== (const DynamicArray& other) const {
		if (size != other.size) return false;
		else {
			for (size_t i = 0; i < size; i++) {
				if (array[i] != other.array[i]) return false;
			}
		}
		return true;
	}
	bool operator== (const std::initializer_list<T>& other) const {
		if (size != other.size()) return false;
		else {
			size_t i = 0;
			for (auto it : other) {
				if (it != array[i]) return false;
				i++;
			}
		}
		return true;
	}
	DynamicArray& operator+= (const DynamicArray& other) {
		if (capacity >= size + other.size) {
			for (size_t i = size; i < size + other.size; i++) array[i] = other.array[i - size];
			size += other.size;
		}
		else {
			T* newarr;
			size + other.size > 2 * capacity ? capacity = size + other.size : capacity *= 2;
			try {
				newarr = new T[capacity];
			}
			catch (std::bad_alloc err) {
				throw DAException(2);
			}
			for (size_t i = 0; i < size; i++) newarr[i] = array[i];
			for (size_t i = size; i < size + other.size; i++) newarr[i] = other.array[i - size];
			delete[] array;
			array = newarr;
			size += other.size;
		}
		return *this;
	}
	DynamicArray operator+ (const DynamicArray& other) const {
		DynamicArray<T> ans = *this;
		return ans += other;
	}
};



/*****************************************
*		DynamicArray Exceptions:
*		
*		1) array subscript out of range
*		2) bad allocation of memory

*****************************************/
