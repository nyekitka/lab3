#pragma once
#include <initializer_list>
#include <exception>
template <class T> class LinkedList {
	
private:
	template <class T1> class Node {
	public:
		Node<T1>* prev;
		T1 value;
		Node <T1>* next;
		Node() {
			prev = next = nullptr;
			value = T1();
		}
		bool operator== (const Node <T1> other) const {
			return prev == other->prev && next == other->next && value == other->value;
		}
	};
	Node<T> *head;
	Node<T> *tail;
	size_t size;

public:

	/*													exceptions													*/

	class LLException {
	private:
		size_t id;
	public:
		LLException(const size_t id) {
			this->id = id;
		}
		const char* message() {
			if (id == 1) return "Error 1: list subscript out of range.";
			else return "Error 2: bad allocation of dynamic memory.";
		}

	};

	/*													constructors												*/

	LinkedList() {
		size = 0;
		head = tail = nullptr;
	}
	template <size_t len>
	LinkedList(const T(&items)[len]) {
		size = len;
		if (size == 0) head = tail = nullptr;
		else {
			try {
				head = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			head->prev = nullptr;
			Node<T>* t = head;
			for (size_t i = 0; i < size; i++) {
				t->value = items[i];
				if (i != size - 1) {
					try {
						t->next = new Node<T>;
					}
					catch (std::bad_alloc err) {
						throw LLException(2);
					}
					t->next->prev = t;
					t = t->next;
				}
				else {
					t->next = nullptr;
					tail = t;
				}
			}
		}
	}
	LinkedList(const std::initializer_list<T> items) {
		size = items.size();
		if (size == 0) head = tail = nullptr;
		else {
			try {
				head = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			head->prev = nullptr;
			Node<T>* t = head;
			for (auto item : items) {
				t->value = item;
				try {
					t->next = new Node<T>;
				}
				catch (std::bad_alloc err) {
					throw LLException(2);
				}
				t->next->prev = t;
				t = t->next;
			}
			t = t->prev;
			delete t->next;
			t->next = nullptr;
			tail = t;
		}
	}
	LinkedList(const LinkedList<T>& other) {
		size = other.size;
		Node<T>* otherptr = other.head, * ptr;
		try {
			ptr = new Node<T>;
		}
		catch (std::bad_alloc err) {
			throw LLException(2);
		}
		ptr->prev = nullptr;
		if (size) head = ptr;
		for (size_t i = 0; i < size; i++) {
			ptr->value = otherptr->value;
			try {
				ptr->next = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			ptr->next->prev = ptr;
			ptr = ptr->next;
			otherptr = otherptr->next;
		}
		if (size) {
			ptr = ptr->prev;
			delete ptr->next;
			ptr->next = nullptr;
			tail = ptr;
		}
		else {
			delete ptr;
			head = nullptr;
			tail = nullptr;
		}
	}
	LinkedList(LinkedList<T>&& other) {
		size = other.size;
		head = other.head;
		tail = other.tail;
		other.head = other.tail = nullptr;
		other.size = 0;
	}

	/*													destructor													*/

	~LinkedList() {
		if (size > 1) {
			Node<T>* ptr = head->next;
			while (ptr) {
				delete ptr->prev;
				ptr = ptr->next;
			}
		}
		if (size) delete tail;
	}

	/*													methods														*/

	size_t length() const {
		return size;
	}
	T& get_first() const {
		if (size) return head->value;
		throw LLException(1);
	}
	T& get_last() const {
		if (size) return tail->value;
		throw LLException(1);
	}
	LinkedList<T> sublist(const size_t begin, const size_t end) const {
		if (begin > end || end >= size) throw LLException(1);
		LinkedList<T> ans;
		
		size_t head_ind = 0, tail_ind = size - 1;
		Node <T>* newhead = head, * newtail = tail;
		while (head_ind < begin || tail_ind > end) {
			if (head_ind < begin) {
				newhead = newhead->next;
				head_ind++;
			}
			if (tail_ind > end) {
				newtail = newtail->prev;
				tail_ind--;
			}
		}
		if (begin == end) {
			try {
				ans.head = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			ans.head->value = newhead->value;
			ans.head->prev = nullptr;
			ans.head->next = nullptr;
			ans.tail = ans.head;
			ans.size = 1;
			return ans;
		}
		try {
			ans.head = new Node<T>;
		}
		catch (std::bad_alloc err) {
			throw LLException(2);
		}
		Node<T>* ptr = newhead, *ansptr = ans.head;
		ans.head->prev = nullptr;
		ans.head->value = newhead->value;
		do {
			ptr = ptr->next;
			
			try {
				ansptr->next = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			ansptr->next->prev = ansptr;
			ansptr->next->value = ptr->value;
			ansptr = ansptr->next;
			
		} while (ptr != newtail);
		ansptr->next = nullptr;
		ans.tail = ansptr;
		ans.size = end - begin + 1;
		return ans;
	}
	int find(const T value) const {
		Node<T>* ptr1 = head, * ptr2 = tail;
		if (size == 0) return -1;
		for (size_t i = 0; i <= size / 2; i++) {
			if (ptr1->value == value) return i;
			if (ptr2->value == value) return size - 1 - i;
			ptr1 = ptr1->next;
			ptr2 = ptr2->prev;
		}
		return -1;
	}
	void clear() {
		if (size > 1) {
			Node<T>* ptr = head->next;
			while (ptr) {
				delete ptr->prev;
				ptr = ptr->next;
			}
		}
		if (size) delete tail;
		size = 0;
		head = tail = nullptr;
	}
	void insert(const T value, const size_t index) {
		if (index == 0) this->prepend(value);
		else if (index == size) this->append(value);
		else if (index > size) throw LLException(1);
		Node<T>* ptr = head, * element;
		if (index <= size / 2) {
			for (size_t i = 0; i < index; i++) ptr = ptr->next;
		}
		else {
			ptr = tail;
			for (size_t i = size - 1; i > index; i--) ptr = ptr->prev;
		}
		try {
			element = new Node<T>;
		}
		catch (std::bad_alloc err) {
			throw LLException(2);
		}
		element->value = value;
		element->prev = ptr->prev;
		element->next = ptr;
		ptr->prev->next = element;
		ptr->prev = element;
		size++;
	}
	void append(const T& value) {
		if (size == 0) {
			try {
				head = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			tail = head;
			head->value = value;
			size++;
			return;
		}
		try {
			tail->next = new Node<T>;
		}
		catch (std::bad_alloc err) {
			throw LLException(2);
		}
		size++;
		tail->next->prev = tail;
		tail = tail->next;
		tail->next = nullptr;
		tail->value = value;
	}
	void prepend(const T& value) {
		if (size == 0) {
			try {
				head = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			tail = head;
			head->value = value;
			size++;
			return;
		}
		try {
			head->prev = new Node<T>;
		}
		catch (std::bad_alloc err) {
			throw LLException(2);
		}
		size++;
		head->prev->next = head;
		head = head->prev;
		head->prev = nullptr;
		head->value = value;
	}
	void pop_back() {
		if (size == 0) throw LLException(1);
		if (size == 1) {
			delete head;
			head = tail = nullptr;
		}
		else {
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
		}
		size--;
	}
	void pop_front() {
		if (size == 0) throw LLException(1);
		if (size == 1) {
			delete head;
			head = tail = nullptr;
		}
		else {
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		}
		size--;
	}
	void erase(const size_t index) {
		if (index >= size) throw LLException(1);
		else if (index == 0) pop_front();
		else if (index == size - 1) pop_back();
		else {
			Node<T>* ptr;
			if (index > size / 2) {
				ptr = tail;
				for (size_t i = size - 1; i != index; i--) ptr = ptr->prev;
			}
			else {
				ptr = head;
				for (size_t i = 0; i != index; i++) ptr = ptr->next;
			}
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;
			size--;
			delete ptr;
		}
	}

	/*													operators													*/

	bool operator== (const LinkedList<T> other) const {
		Node<T>* ptr = head, *otherptr = other.head;
		while (ptr != nullptr) {
			if (otherptr == nullptr) return false;
			if (ptr->value != otherptr->value) return false;
			ptr = ptr->next;
			otherptr = otherptr->next;
		}
		if (otherptr != nullptr) return false;
		else return true;
	}
	bool operator!= (const LinkedList<T> other) const {
		return !(*this == other);
	}
	LinkedList<T>& operator= (const LinkedList<T>& other) {
		Node<T>* ptr = head, * otherptr = other.head;
		if (size >= other.size) {
			while (otherptr != nullptr) {
				ptr->value = otherptr->value;
				ptr = ptr->next;
				otherptr = otherptr->next;
			}
			if (ptr != nullptr) {
				while (tail != ptr) {
					tail = tail->prev;
					delete tail->next;
				}
				tail = tail->prev;
				delete tail->next;
				tail->next = nullptr;
			}
		}
		else {
			while (ptr != nullptr) {
				ptr->value = otherptr->value;
				ptr = ptr->next;
				otherptr = otherptr->next;
			}
			ptr = tail;
			if (ptr == nullptr) {
				try {
					head = new Node<T>;
				}
				catch (std::bad_alloc err) {
					throw LLException(2);
				}
				ptr = tail = head;
				tail->prev = nullptr;
				tail->value = otherptr->value;
				tail->next = nullptr;
				otherptr = otherptr->next;
			}
			while (otherptr != nullptr) {
				try {
					ptr->next = new Node<T>;
				}
				catch (std::bad_alloc err) {
					throw LLException(2);
				}
				ptr->next->prev = ptr;
				ptr->next->value = otherptr->value;
				ptr = ptr->next;
				otherptr = otherptr->next;
			}
			ptr->next = nullptr;
			tail = ptr;
		}
		size = other.size;
		return *this;
	}
	LinkedList<T>& operator= (LinkedList<T>&& other) {
		if (head != nullptr) {
			head = head->next;
			while (head != tail) {
				delete head->prev;
				head = head->next;
			}
			delete tail;
		}
		head = other.head;
		tail = other.tail;
		size = other.size;
		other.head = nullptr;
		other.tail = nullptr;
		other.size = 0;
		return *this;
	}
	T& operator[] (const size_t index) const {
		if (index >= size) throw LLException(1);
		if (index > size / 2) {
			Node<T>* ptr = tail;
			for (size_t i = size - 1; i > index; i--) ptr = ptr->prev;
			return ptr->value;
		}
		else {
			Node<T>* ptr = head;
			for (size_t i = 0; i < index; i++) ptr = ptr->next;
			return ptr->value;
		}

	}
	LinkedList<T>& operator+= (const LinkedList<T>& other) {
		Node <T>* ptr = tail,*otherptr = other.head;
		while (otherptr) {
			try {
				ptr->next = new Node<T>;
			}
			catch (std::bad_alloc err) {
				throw LLException(2);
			}
			ptr->next->prev = ptr;
			ptr->next->value = otherptr->value;
			ptr = ptr->next;
			otherptr = otherptr->next;
		}
		ptr->next = nullptr;
		size += other.size;
		tail = ptr;
		return *this;
	}
	LinkedList<T> operator+ (const LinkedList<T>& other) const {
		LinkedList<T> ans = *this;
		return ans += other;
	}
};