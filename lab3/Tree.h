#pragma once
#include <exception>
#include <string>
#include <cmath>
#include "LinkedList.h"
#include "DynamicArray.h"
int atoi(std::string s) {
	size_t size = s.size();
	char* str = new char[size + 1];
	for (size_t i = 0; i < size; i++) str[i] = s[i];
	s[size] = '\0';
	int ans = atoi(str);
	delete[] str;
	return ans;
}
template <class T>
struct has_to_string {
private:
	static int detect(...);
	template <class U> static decltype(std::to_string(std::declval<U>())) detect(const U&);
public:
	static constexpr bool value = std::is_same<std::string, decltype(detect(std::declval<T>()))>::value;
};
template <class T>
struct has_cmp {
private:
	static int detect(...);
	template <class U> static decltype(std::declval<U>() > std::declval<U>()) detect(const U&);
public:
	static constexpr bool value = std::is_same<bool, decltype(detect(std::declval<T>()))>::value;
};
template <class T, size_t n>
class Tree {
private:
	T value;
	DynamicArray<Tree*>vertexes;
	Tree* parent;
	size_t size_;
	DynamicArray<int> traversal; //первый элемент - 0, если DFS, 1 - если BFS. Остальные элементы - порядок обхода, -1 - корень (только для DFS)
	//чтобы сто раз не прогонять алгоритм построения массива по строке был создан сеттер с массивом
	void set_traversal(DynamicArray<int> traversal) {
		if (vertexes.length()) {
			for (size_t i = 0; i < vertexes.length(); i++) vertexes[i]->set_traversal(traversal);
		}
		this->traversal = traversal;
	}
	//dfs проход - возвращает массив указателей на вершины, которые расположены по порядку прохода
	DynamicArray<Tree*> dfs(Tree* item) const {
		DynamicArray<Tree*> a(item->size_);
		if (a.length() == 1) {
			a[0] = item;
			return a;
		}
		size_t j = 0;
		for (size_t i = 1; i < n + 2; i++) {
			if (item->traversal[i] == -1) {
				a[j] = item;
				j++;
			}
			else if (item->vertexes.length() > traversal[i]) {
				DynamicArray<Tree*> b = dfs(item->vertexes[traversal[i]]);
				for (size_t k = 0; k < b.length(); k++) a[j + k] = b[k];
				j += b.length();
			}
		}
		return a;
	}
	Tree* set_value(const T& value) {
		Tree* vertex;
		try {
			vertex = new Tree;
		}
		catch (std::bad_alloc err) {
			throw TreeException(1);
		}
		vertex->value = value;
		vertex->size_ = 1;
		vertex->vertexes.reserve(n);
		return vertex;
	}
public:
	class Iterator {
		friend class Tree;
		Tree* it;
		bool isend;
		//находится в private, т.к. пользователь может поместить в конструктор совершенно любой указатель
		Iterator(Tree* const it) {
			this->it = it;
			isend = false;
		}
	public:
		Iterator() { it = nullptr; isend = false; }
		Iterator(const Iterator& other) {
			it = other.it;
			isend = other.isend;
		}
		bool isleaf() {
			return it->vertexes.length() == 0;
		}
		bool operator== (const Iterator& other) const {
			return it == other.it && isend == other.isend;
		}
		bool operator!= (const Iterator& other) const {
			return !(*this == other);
		}
		Iterator& operator++ () {
			if (it == nullptr) throw TreeException(4);
			if (isend) throw TreeException(6);
			if (it->traversal[0]) {
				Tree* root = it;
				bool imready = false;
				while (root->parent) root = root->parent;
				if (root == it) imready = true;
				LinkedList<Tree*> q;
				q.append(root);
				while (q.length()) {
					Tree* v = q.get_first();
					q.pop_front();
					for (size_t i = 1; i <= n; i++) {
						if (v->traversal[i] < v->vertexes.length()) {
							if (imready) {
								it = v->vertexes[it->traversal[i]];
								return *this;
							}
							if (v->vertexes[v->traversal[i]] == it) imready = true;
							q.append(v->vertexes[v->traversal[i]]);
						}
					}
				}
				isend = true;
				return *this;
			}
			else {
				Tree* old_it = it;
				while (it->parent) it = it->parent;
				DynamicArray <Tree*> a = it->dfs(it);
				for (size_t i = 0; i < it->size_; i++) {
					if (a[i] == old_it) {
						if (i == it->size_ - 1) {
							it = a[i];
							isend = true;
						}
						else it = a[i + 1];
					}
				}
			}
			return *this;
		}
		Iterator& operator-- () {
			if (it == nullptr) throw TreeException(4);
			if (isend) {
				isend = false;
				return *this;
			}
			if (it->traversal[0]) {
				Tree* root = it, *last;
				while (root->parent) root = root->parent;
				last = root;
				if (it == root) throw TreeException(7);
				LinkedList<Tree*> q;
				q.prepend(root);
				while (!q.length()) {
					Tree* v = q.get_first();
					q.pop_front();
					for (size_t i = 1; i <= n; i++) {
						if (v->traversal[i] < v->vertexes.length()) {
							if (v->vertexes[v->traversal[i]] == it) {
								it = last;
								return *this;
							}
							q.prepend(v->vertexes[v->traversal[i]]);
							last = v->vertexes[v->traversal[i]];
						}
					}
				}
			}
			else {
				Tree* old_it = it;
				while (it->parent) it = it->parent;
				DynamicArray <Tree*> a = it->dfs(it);
				for (size_t i = it->size_ - 1; i >= 0; i--) {
					if (a[i] == old_it) {
						if (i == 0) throw TreeException(7);
						else {
							it = a[i - 1];
							break;
						}
					}
				}
			}
			return *this;
		}
		Iterator& operator++ (int trash) {
			++(*this);
			return *this;
		}
		Iterator& operator-- (int trash) {
			--(*this);
			return *this;
		}
		T& operator* () {
			if (isend) throw TreeException(9);
			else if (it) return it->value;
			else throw TreeException(8);
		}
		//сдвигает итератор к потомку номера number, если number >= 0 и к корню, если number == -1
		void move(int number) {
			if (number >= it->vertexes.length()) throw TreeException(2);
			else if (number < -1) throw TreeException(2);
			if (number == -1) {
				if (it->parent) it = it->parent;
				else throw TreeException(2);
			}
			else it = it->vertexes[number];
		}
		size_t descendants() { return it->vertexes.length();  }
	};
	class TreeException {
		size_t id;
	public:
		TreeException(size_t id) {
			this->id = id;
		}
		const char* message() {
			if (id == 1) return "Error 1: Bad allocation of memory";
			else if (id == 2) return "Error 2: Incorrect path specified";
			else if (id == 3) return "Error 3: Incorrect format string specified";
			else if (id == 4) return "Error 4: operator++ is not applicable for empty iterator";
			else if (id == 5) return "Error 5: operator-- is not applicable for empty iterator";
			else if (id == 6) return "Error 6: operator++ is not applicable for end() of the tree";
			else if (id == 7) return "Error 7: operator-- is not applicable for begin() of the tree";
			else if (id == 8) return "Error 8: operator * is not applicable for empty iterator";
			else if (id == 9) return "Error 9: operator * is not applicable for end() of the tree";
			else if (id == 10) return "Error 10: The type used doesn't support output to a string (overload to_string())";
			else if (id == 11) return "Error 11: Incorrect traversal specified";
			else if (id == 12) return "Error 12: There's no head because the tree is empty";
			else if (id == 13) return "Error 13: operator [] isn't applicable for empty tree";
			else if (id == 14) return "Error 14: operator [] isn't applicable for end() of the tree";
			else if (id == 15) return "Error 15: this type isn't comparable";
			else if (id == 15) return "Error 16: lol, 0-ary tree? seriously?";
		}
	};
	Tree() {
		if (n == 0) throw TreeException(15);
		value = T();
		size_ = 0;
		parent = nullptr;
	}
	Tree(std::string traversal) : Tree() {
		if (traversal.length() < 4) throw TreeException(11);
		if (traversal[0] == 'B') {
			this->traversal.reserve(n + 1);
			this->traversal.resize(n + 1);
			this->traversal[0] = 1;
		}
		else if (traversal[0] == 'D') {
			this->traversal.reserve(n + 2);
			this->traversal.resize(n + 2);
			this->traversal[0] = 0;
		}
		else throw TreeException(11);
		size_t ind = 1;
		for (size_t i = 1; i < traversal.length(); i++) {
			if (ind >= this->traversal.length()) throw TreeException(11);
			if (traversal[i] != '(') throw TreeException(11);
			size_t closing_br = traversal.find(')', i + 1);
			if (closing_br == std::string::npos) throw TreeException(11);
			if (closing_br - i == 2 && traversal[i + 1] == 'R') {
				if (this->traversal[0]) throw TreeException(11);
				this->traversal[ind] = -1;
			}
			else {
				int number = atoi(traversal.substr(i + 1, closing_br - i - 1));
				if (number <= 0 && number > n) throw TreeException(11);
				this->traversal[ind] = number - 1;
			}
			ind++;
			i = closing_br;
		}
		//проверка на повторение индексов в обходе
		DynamicArray <int> checker(this->traversal.length() - 1);
		for (size_t i = 1; i < this->traversal.length(); i++) {
			if (checker[this->traversal[i]] == 0) checker[this->traversal[i]] = 1;
			else throw TreeException(11);
		}
	}
	Tree(const T& value) {
		if (n == 0) throw TreeException(15);
		size_ = 1;
		this->value = value;
		parent = nullptr;
		vertexes.reserve(n);
		traversal.reserve(n + 2);
		traversal.resize(n + 2);
		traversal[0] = 0;
		for (size_t i = 1; i <= n; i++) traversal[i] = i - 1;
		traversal[n + 1] = -1;
	}
	Tree(const T& value, std::string traversal) : Tree(traversal) {
		size_ = 1;
		this->value = value;
		parent = nullptr;
		vertexes.reserve(n);
	}
	Tree(const Tree& other) : Tree(other.value) {
		traversal = other.traversal;
		size_ = other.size_;
		vertexes.resize(other.vertexes.length());
		for (size_t i = 0; i < vertexes.length(); i++) {
			try {
				vertexes[i] = new Tree(*(other.vertexes[i]));
			}
			catch (std::bad_alloc err) {
				throw TreeException(1);
			}
			vertexes[i]->parent = this;
		}
	}
	Tree(Tree&& other) {
		traversal = other.traversal;
		size_ = other.size_;
		value = other.value;
		vertexes = other.vertexes;
		parent = other.parent;
		for (size_t i = 0; i < vertexes.length(); i++) {
			vertexes[i]->parent = this;
		}
		other.vertexes.clear();
	}
	Tree(std::string format_string, const std::initializer_list<T>& elements) {
		if (n == 0) throw TreeException(15);
		if (format_string.size() == 0) *this = Tree();
		else {
			if (format_string.size() < 3) throw TreeException(3);
			if (format_string[0] != '[') throw TreeException(3);
			int closing_br = format_string.find(']');
			if (closing_br == -1 || closing_br == 1) throw TreeException(3);
			int number = atoi(format_string.substr(1, (size_t)closing_br - 1));
			if (number < 0) throw TreeException(3);
			format_string = format_string.substr((size_t)closing_br + 1);
			if (number >= elements.size()) throw TreeException(3);
			traversal.reserve(n + 2);
			traversal.resize(n + 2);
			traversal[0] = 0;
			for (size_t i = 1; i <= n; i++) traversal[i] = i - 1;
			traversal[n + 1] = -1;
			size_ = 1;
			value = *(elements.begin() + number);
			vertexes.reserve(n);
			if (format_string != "") {
				if (format_string.size() < 3) throw TreeException(3);
				if (format_string[0] != '{' || format_string[format_string.size() - 1] != '}') throw TreeException(3);
				format_string = format_string.substr(1, format_string.size() - 2);
				int comma = 0, i = 0;
				while (comma != -1) {
					closing_br = format_string.find(']');
					if (closing_br == -1) throw TreeException(3);
					if (closing_br == format_string.size() - 1) comma = -1;
					else if (format_string[closing_br + 1] == ',') comma = closing_br + 1;
					else {
						if (format_string[closing_br + 1] != '{') throw TreeException(3);
						size_t brack_coeff = 1, j = closing_br + 2;
						for (j; brack_coeff != 0 && j < format_string.size(); j++) {
							if (format_string[j] == '{') brack_coeff++;
							else if (format_string[j] == '}') brack_coeff--;
						}
						comma = j;
						if (comma >= format_string.size()) comma = -1;
						else if (format_string[comma] != ',') throw TreeException(3);
					}
					try {
						vertexes.append(new Tree(format_string.substr(0, comma), elements));
						vertexes[i]->parent = this;
						size_ += vertexes[i]->size_;
					}
					catch (std::bad_alloc err) {
						throw TreeException(1);
					}
					catch (TreeException err) {
						clear();
						throw TreeException(3);
					}
					if (comma != -1) format_string = format_string.substr(comma + 1, format_string.size() - 1 - comma);
					else format_string = "";
					i++;
				}
			}
		}
	}
	template <size_t len>
	Tree(std::string format_string, const T(&elements)[len]) {
		if (n == 0) throw TreeException(15);
		if (len == 0) *this = Tree();
		else {
			if (format_string.size() < 3) throw TreeException(3);
			if (format_string[0] != '[') throw TreeException(3);
			int closing_br = format_string.find(']');
			if (closing_br == -1 || closing_br == 1) throw TreeException(3);
			int number = atoi(format_string.substr(1, (size_t)closing_br - 1));
			if (number < 0) throw TreeException(3);
			format_string = format_string.substr((size_t)closing_br + 1);
			if (number >= len) throw TreeException(3);
			if (format_string == "") *this = Tree(elements[number]);
			else {
				if (format_string.size() < 3) throw TreeException(3);
				if (format_string[0] != '{' || format_string[format_string.size() - 1] != '}') throw TreeException(3);
				*this = Tree(elements[number]);
				format_string = format_string.substr(1, format_string.size() - 2);
				int comma = 0, i = 0;
				while (comma != -1) {
					closing_br = format_string.find(']');
					if (closing_br == -1) throw TreeException(3);
					if (closing_br == format_string.size() - 1) comma = -1;
					else if (format_string[closing_br + 1] == ',') comma = closing_br + 1;
					else {
						if (format_string[closing_br + 1] != '{') throw TreeException(3);
						size_t brack_coeff = 1, j = closing_br + 2;
						for (j; brack_coeff != 0 && j < format_string.size(); j++) {
							if (format_string[j] == '{') brack_coeff++;
							else if (format_string[j] == '}') brack_coeff--;
						}
						comma = j;
						if (comma >= format_string.size()) comma = -1;
						else if (format_string[comma] != ',') throw TreeException(3);
					}
					try {
						vertexes.append(new Tree(format_string.substr(0, comma), elements));
						vertexes[i]->parent = this;
						size_ += vertexes[i]->size_;
					}
					catch (std::bad_alloc err) {
						throw TreeException(1);
					}
					catch (TreeException err) {
						clear();
						throw TreeException(3);
					}
					if (comma != -1) format_string = format_string.substr(comma + 1, format_string.size() - 1 - comma);
					else format_string = "";
					i++;
				}
			}
		}
	}
	~Tree() {
		if (vertexes.get_capacity()) {
			for (size_t i = 0; i < vertexes.length(); i++) {
				delete vertexes[i];
				vertexes[i] = nullptr;
			}
		}
	}
	bool operator== (const Tree& other) const {
		if (value != other.value || vertexes.length() != other.vertexes.length() || size_ != other.size_) return false;
		for (size_t i = 0; i < vertexes.length(); i++) {
			if (!(*(vertexes[i]) == *(other.vertexes[i]))) return false;
		}
		return true;
	}
	bool operator!= (const Tree& other) const {
		return !(*this == other);
	}
	Iterator begin() const {
		if (traversal[0]) return Iterator(const_cast<Tree*>(this));
		else if (size_ == 0) {
			Iterator ans(const_cast<Tree*>(this));
			ans.isend = true;
			return ans;
		}
		else {
			Tree* it = const_cast<Tree*>(this);
			while (it->vertexes.length() > traversal[1]) it = it->vertexes[traversal[1]];
			return Iterator(it);
		}
	}
	Iterator end() const {
		Iterator it = Iterator(const_cast<Tree*>(this));
		if (size_ == 0) {
			it.isend = true;
			return it;
		}
		if (traversal[0]) {
			size_t ind, max_height = 0;
			for (int i = n; i >= 0; i--) {
				if (traversal[i] < vertexes.length()) {
					size_t height = vertexes[traversal[i]]->height();
					if (height > max_height) {
						max_height = height;
						ind = traversal[i];
					}
				}
			}
			if (max_height > 1) it = vertexes[ind]->end();
			else it = Iterator(vertexes[ind]);
		}
		else if (traversal[n + 1] != -1) {
			while (true) {
				size_t ind = n + 1;
				while (traversal[ind] != -1 && traversal[ind] >= it.descendants()) ind--;
				if (traversal[ind] != -1) it.move(traversal[ind]);
				else break;
			}
		}
		it.isend = true;
		return it;
	}
	Iterator head() const {
		if (size_ == 0) throw TreeException(12);
		return Iterator(const_cast<Tree*>(this));
	}
	size_t size() const {
		return size_;
	}
	size_t height() const {
		if (vertexes.get_capacity() == 0) return 0;
		else {
			size_t max = 0;
			for (size_t i = 0; i < vertexes.length(); i++) {
				size_t res = vertexes[i]->height();
				if (max < res) max = res;
			}
			return max + 1;
		}
	}
	int index() const {
		if (parent == nullptr) return -1;
		else {
			for (size_t i = 0; i < parent->vertexes.length(); i++) {
				if (parent->vertexes[i] == this) return i;
			}
		}
	}
	size_t descendants() const {
		return vertexes.length();
	}
	void set_traversal(std::string traversal) {
		if (traversal.length() < 4) throw TreeException(11);
		if (traversal[0] == 'B') {
			this->traversal.reserve(n + 1);
			this->traversal.resize(n + 1);
			this->traversal[0] = 1;
		}
		else if (traversal[0] == 'D') {
			this->traversal.reserve(n + 2);
			this->traversal.resize(n + 2);
			this->traversal[0] = 0;
		}
		else throw TreeException(11);
		size_t ind = 1;
		for (size_t i = 1; i < traversal.length(); i++) {
			if (ind >= this->traversal.length()) throw TreeException(11);
			if (traversal[i] != '(') throw TreeException(11);
			size_t closing_br = traversal.find(')', i + 1);
			if (closing_br == std::string::npos) throw TreeException(11);
			if (closing_br - i == 2 && traversal[i + 1] == 'R') {
				if (this->traversal[0]) throw TreeException(11);
				this->traversal[ind] = -1;
			}
			else {
				int number = atoi(traversal.substr(i + 1, closing_br - i - 1));
				if (number <= 0 && number > n) throw TreeException(11);
				this->traversal[ind] = number - 1;
			}
			ind++;
			i = closing_br;
		}
		//проверка на повторение индексов в обходе
		DynamicArray <int> checker(this->traversal.length() - 1);
		for (size_t i = 1; i < this->traversal.length(); i++) {
			if (checker[this->traversal[i] + this->traversal.length() - n - 1] == 0) checker[this->traversal[i] + this->traversal.length() - n - 1] = 1;
			else throw TreeException(11);
		}
		set_traversal(this->traversal);
	}
	void add(const T& value, const std::initializer_list<size_t>& path = {}) {
		if (size_ == 0 && path.size() == 0) {
			size_ = 1;
			this->value = value;
			parent = nullptr;
			vertexes.reserve(n);
			return;
		}
		else if (size_ == 0 && path.size() != 0) throw TreeException(2);
		Tree* it = this;
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			else if (it->vertexes.get_capacity() == 0) throw TreeException(2);
			else if (i - 1 < it->vertexes.length()) it = it->vertexes[i - 1];
			else throw TreeException(2);
		}
		if (it->descendants() == n) throw TreeException(2);
		try {
			it->vertexes.append(new Tree(value));
		}
		catch (...) {
			throw TreeException(1);
		}
		it->vertexes[it->vertexes.length() - 1]->traversal = it->traversal;
		it->vertexes[it->vertexes.length() - 1]->parent = it;
		while (it->parent) {
			it->size_++;
			it = it->parent;
		}
		it->size_++;
	}
	void add(const T& value, Iterator to_where) {
		if (to_where.it->vertexes.get_capacity() == 0) {
			size_ = 1;
			this->value = value;
			parent = nullptr;
			vertexes.reserve(n);
			return;
		}
		if (to_where.descendants() == n) throw TreeException(2);
		try {
			Tree* temp = new Tree(value);
			to_where.it->vertexes.append(temp);
		}
		catch (...) {
			throw TreeException(1);
		}
		to_where.it->vertexes[to_where.descendants() - 1]->traversal = to_where.it->traversal;
		to_where.it->vertexes[to_where.descendants() - 1]->parent = to_where.it;
		Tree* it = to_where.it;
		while (it->parent) {
			it->size_++;
			it = it->parent;
		}
		it->size_++;
	}
	void remove(const std::initializer_list<size_t>& path = {}) {
		Tree* it = this;
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			else if (it->vertexes.get_capacity() == 0) throw TreeException(2);
			else if (it->vertexes[i - 1]) it = it->vertexes[i - 1];
			else throw TreeException(2);
		}
		if (it->parent) {
			size_t sz = it->size_;
			size_t ind = it->index();
			it = it->parent;
			it->vertexes[ind]->clear();
			it->vertexes.erase(ind);
			while (it) {
				it->size_ -= sz;
				it = it->parent;
			}
		}
		else it->clear();
	}
	void erase(const std::initializer_list<size_t>& path = {}) {
		Tree* it = this;
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			else if (it->vertexes.get_capacity() == 0) throw TreeException(2);
			else if (it->vertexes[i - 1]) it = it->vertexes[i - 1];
			else throw TreeException(2);
		}
		if (it->vertexes.length() == 0) {
			if (it->parent) it->parent->remove({(size_t)( it->index() + 1)});
			else it->clear();
		}
		else if (it->vertexes.length() == 1) {
			Tree* des = it->vertexes[0];
			it->vertexes.erase(0);
			Tree* parnt = it->parent;
			it->size_ = 1;
			if (parnt) {
				size_t ind = it->index();
				delete parnt->vertexes[ind];
				parnt->vertexes[ind] = des;
				parnt->vertexes[ind]->parent = parnt;
				it = parnt;
				while (it) {
					it->size_--;
					it = it->parent;
				}
			}
			else {
				it->remove();
				it->value = des->value;
				it->size_ = des->size_;
				it->vertexes = des->vertexes;
				it->parent = parnt;
			}
		}
		else {
			Tree* vert_to_delete = it;
			it = it->vertexes[it->vertexes.length()-1];
			while (it->vertexes.length()) it = it->vertexes[0];
			std::swap(vert_to_delete->value, it->value);
			it->erase();
		}
	}
	void erase(const Iterator& it) {
		if (it.it->parent) it.it->parent->erase({(size_t)(it.it->index() + 1)});
		else erase();
	}
	void clear() {
		for (size_t i = 0; i < vertexes.length(); i++) {
			vertexes[i]->clear();
			delete vertexes[i];
		}
		vertexes.resize(0);
		if (parent == nullptr) {
			vertexes.shrink_to_fit();
			size_ = 0;
		}
	}
	Tree map(T func(T)) const {
		Tree ans = *this;
		for (Iterator it = ans.begin(); it != ans.end(); it++) *it = func(*it);
		return ans;
	}
	Tree where(bool func(T)) const {
		Tree ans = *this;
		if (traversal[0]) {
			for (Iterator it = --ans.end(); it != ans.begin(); --it) {
				if (!func(*it)) {
					auto new_it = it;
					++new_it;
					ans.erase(it);
					if (new_it.isend) it = --ans.end();
					else it = new_it;
				}
			}
			if (!func(*(ans.begin()))) ans.erase(ans.begin());
		}
		else {
			for (auto it = ans.begin(); it != ans.end(); it++) {
				if (!func(*it)) {
					ans.erase(it);
					it = ans.begin();
				}
			}
		}
		return ans;
	}
	T reduce(T func(T, T), T start_value) {
		Iterator it = --end();
		T ans = func(*it, start_value);
		--it;
		for (it; it != begin(); --it) {
			ans = func(*it, ans);
		}
		return func(*(begin()), ans);
	}
	Tree subtree(const std::initializer_list<size_t>& path = {}) const {
		Tree* it = const_cast<Tree*>(this);
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			else if (it->vertexes.get_capacity() == 0) throw TreeException(2);
			else if (it->vertexes[i - 1]) it = it->vertexes[i - 1];
			else throw TreeException(2);
		}
		Tree ans = *it;
		return ans;
	}
	Tree subtree(const Iterator& it) {
		Tree ans(*(it.it));
		return ans;
	}
	Iterator find(const T& value) const {
		if (this->value == value) return Iterator(const_cast<Tree*>(this));
		Iterator ans;
		for (size_t i = 0; i < vertexes.length(); i++) {
			ans = vertexes[i]->find(value);
			if (ans.it->value == value) return ans;
		}
		return end();
	}
	Iterator is_subtree(const Tree& subtr) {
		if (*this == subtr) return Iterator(this);
		else {
			for (size_t i = 0; i < vertexes.length(); i++) {
				Iterator t = vertexes[i]->is_subtree(subtr);
				if (!t.isend) return t;
			}
			return end();
		}
	}
	Tree& operator= (const Tree& other) {
		clear();
		*this = Tree(other.value);
		traversal = other.traversal;
		size_ = other.size_;
		vertexes.reserve(n);
		vertexes.resize(other.vertexes.length());
		for (size_t i = 0; i < vertexes.length(); i++) {
			try {
				vertexes[i] = new Tree(*(other.vertexes[i]));
			}
			catch (std::bad_alloc err) {
				throw TreeException(1);
			}
			vertexes[i]->parent = this;
		}
	}
	Tree& operator= (Tree&& other) {
		size_ = other.size_;
		value = other.value;
		vertexes = other.vertexes;
		parent = other.parent;
		traversal = other.traversal;
		for (size_t i = 0; i < vertexes.length(); i++) vertexes[i]->parent = this;
		other.size_ = 0;
		other.value = T();
		other.vertexes.clear();
		return *this;
	}
	Tree& operator+= (const Tree& other) {
		if (n == 1) {
			Tree* it = this;
			while (it->vertexes.length()) it = it->vertexes[0];
			try {
				it->vertexes[0] = new Tree(other);
				it->vertexes[0]->parent = it;
			}
			catch (std::bad_alloc err) {
				throw TreeException(1);
			}
		}
		else {
			Tree* temp_head = new Tree(T());
			temp_head->vertexes.resize(2);
			try {
				temp_head->vertexes[0] = new Tree(*this);
				temp_head->vertexes[0]->parent = temp_head;
				temp_head->vertexes[1] = new Tree(other);
				temp_head->vertexes[1]->parent = temp_head;
			}
			catch (std::bad_alloc err) {
				throw TreeException(1);
			}
			temp_head->size_ = size_ + other.size_ + 1;
			temp_head->set_traversal(traversal);
			temp_head->erase();
			clear();
			*this = *temp_head;
			temp_head->clear();
		}
		return *this;
	}
	Tree operator+ (const Tree& other) const {
		Tree ans = *this;
		ans += other;
		return ans;
	}
	T& operator[] (const std::initializer_list<size_t>& path) const {
		Tree* it = *this;
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			if (it->vertexes[i]) throw TreeException(2);
			it = it->vertexes[i];
		}
		return it->value;
	}
	template <size_t len>
	T& operator[] (const size_t(&path)[len]) const {
		Tree* it = *this;
		for (size_t i = 0; i < len; i++) {
			if (path[i] == 0 || path[i] > n) throw TreeException(2);
			if (it->vertexes[path[i]]) throw TreeException(2);
			it = it->vertexes[i];
		}
		return it->value;
	}
	T& operator[] (const Iterator& it) {
		if (it.it == nullptr) throw TreeException(13);
		else if (it.isend) throw TreeException(14);
		return it.it->value;
	}
	operator std::string() const {
		if (has_to_string<T>::value) {
			if (vertexes.get_capacity() == 0) return std::string("");
			if (traversal[0]) {
				std::string str;
				const Tree* it = this;
				LinkedList<const Tree*> q;
				q.append(it);
				while (q.length()) {
					const Tree* v = q.get_first();
					q.pop_front();
					for (size_t i = 1; i < n + 1; i++) {
						if (traversal[i] < v->descendants()) q.append(v->vertexes[traversal[i]]);
					}
					if (q.length()) str += std::to_string(v->value) + " ";
					else str += std::to_string(v->value);
				}
				return str;
			}
			else {
				std::string str;
				DynamicArray<Tree*> trav = dfs(const_cast<Tree*>(this));
				for (size_t i = 0; i < trav.length(); i++) {
					str += std::to_string(trav[i]->value);
					if (i != trav.length() - 1) str += " ";
				}
				return str;
			}
		}
		else throw TreeException(10);
	}
	template <class T1, size_t n1>
	friend std::ostream& operator<< (std::ostream&, const Tree<T1, n1>&);
};
template<class T, size_t n>
std::ostream& operator<< (std::ostream& os, const Tree<T, n>& tree) {
	if (has_to_string<T>::value) {
		os << "[" << std::to_string(tree.value) << "]";
		if (tree.vertexes.length()) {
			os << "{";
			for (size_t i = 0; i < tree.vertexes.length(); i++) {
				os << *(tree.vertexes[i]);
				if (i != tree.vertexes.length() - 1) os << ',';
			}
			os << "}";
		}
	}
	else throw Tree<T, n>::TreeException(10);
	return os;
}
template <class T, bool ismulti = true>
class SearchTree {
	Tree<T, 2> tree;
	SearchTree(Tree<T, 2> tree) { this->tree = tree; }
public:
	class iterator {
		friend class SearchTree;
		typename Tree<T, 2>::Iterator it;
		iterator(typename Tree<T, 2>::Iterator it) {
			this->it = it;
		}
	public:
		iterator() = default;
		bool operator==(const iterator& other) const {
			return it == other.it;
		}
		bool operator!=(const iterator& other) {
			return it != other.it;
		}
		iterator& operator++() {
			++it;
			return *this;
		}
		iterator& operator--() {
			--it;
			return *this;
		}
		iterator& operator++ (int trash) {
			++it;
			return *this;
		}
		iterator& operator-- (int trash) {
			--it;
			return *this;
		}
		const T& operator* () {
			return *it;
		}
	};
	SearchTree() {
		tree = Tree<T, 2>();
		tree.set_traversal("D(1)(R)(2)");
	}
	template <size_t len>
	SearchTree(const T(&path)[len]) : SearchTree() {
		for (size_t i = 0; i < len; i++) insert(path[i]);
	}
	SearchTree(const std::initializer_list<T>& elements) : SearchTree() {
		for (auto i : elements) 
			insert(i);
	}
	bool operator== (const SearchTree& other) {
		auto it1 = begin(), it2 = other.begin(), end1 = end(), end2 = other.end();
		for (it1, it2; it1 != end1 && it2 != end2; it1++, it2++) {
			if (*it1 != *it2) return false;
		}
		return it1 == end1 && it2 == end2;
	}
	bool operator!= (const SearchTree& other) {
		return !(*this == other);
	}
	size_t size() const { return tree.size(); }
	void insert(const T& value) {
		if (has_cmp<T>::value) {
			if (tree.size() == 0) {
				tree.add(value);
				return;
			}
			auto it = tree.head();
			while (!it.isleaf()) {
				if (value == *it && !ismulti) return;
				else if (value < *it) it.move(0);
				else if (it.descendants() == 2) it.move(1);
				else {
					tree.add(value, it);
					return;
				}
			}
			if (value < *it) tree.add(value, it);
			else if ((!ismulti && value > *it) || ismulti) {
				T swap_value = *it;
				tree[it] = value;
				tree.add(swap_value, it);
			}
		}
		else throw Tree<T, 2>::TreeException(15);
	}
	void clear() {
		tree.clear();
	}
	iterator begin() const {
		return iterator(tree.begin());
	}
	iterator end() const {
		return iterator(tree.end());
	}
	iterator head() const {
		return iterator(tree.head());
	}
	iterator find(const T& value) const {
		iterator it = tree.head();
		while (!it.it.isleaf()) {
			if (value < *it) it.it.move(0);
			else if (value > *it) {
				if (it.it.descendants() == 2) it.it.move(1);
				else return end();
			}
			else return it;
		}
		if (value == *it) return it;
		else return end();
	}
	SearchTree map(T func(T)) {
		SearchTree ans;
		for (auto i : tree) {
			ans.insert(func(i));
		}
		return ans;
	}
	SearchTree where(bool func(T)) {
		SearchTree ans;
		for (auto i : tree) {
			if (func(i)) ans.insert(i);
		}
		return ans;
	}
	T reduce(T func(T, T), const T& start_value) {
		return tree.reduce(func, start_value);
	}
	SearchTree subtree(const std::initializer_list<int>& path = {}) {
		return SearchTree(tree.subtree(path));
	}
	SearchTree subtree(const iterator& it) {
		return SearchTree(tree.subtree(it.it));
	}
	iterator is_subtree(const SearchTree& subtr) {
		return iterator(tree.is_subtree(subtr.tree));
	}
	void erase(const iterator& it) {
		tree.erase(it.it);
	}
	operator std::string() const {
		return (std::string)tree;
	}
};
template <class T>
class Set {
	SearchTree<T, false> tree;
	Set(SearchTree<T, false> tree) {
		this->tree = tree;
	}
public:
	class iterator {
		friend class Set;
		typename SearchTree<T, false>::iterator it;
		iterator(typename SearchTree<T, false>::iterator it) {
			this->it = it;
		}
	public:
		bool operator==(const iterator& other) const {
			return it == other.it;
		}
		bool operator!=(const iterator& other) {
			return it != other.it;
		}
		iterator& operator++() {
			++it;
			return *this;
		}
		iterator& operator--() {
			--it;
			return *this;
		}
		iterator& operator++ (int trash) {
			++it;
			return *this;
		}
		iterator& operator-- (int trash) {
			--it;
			return *this;
		}
		const T& operator* () {
			return *it;
		}
	};
	Set() = default;
	template <size_t len>
	Set(const T(&path)[len]) {
		tree = SearchTree<T, false>(path);
	}
	Set(const std::initializer_list<T>& elements) {
		tree = elements;
	}
	bool operator== (const Set& other) { return tree == other.tree; }
	bool operator!= (const Set& other) { return tree != other.tree; }
	void clear() {
		tree.clear();
	}
	size_t size() const { return tree.size(); }
	void insert(const T& value) {
		tree.insert(value);
	}
	iterator find(const T& value) const {
		return iterator(tree.find(value));
	}
	void erase(const iterator& it) {
		tree.erase(it.it);
	}
	iterator begin() const {
		return iterator(tree.begin());
	}
	iterator end() const {
		return Set<T>::iterator(tree.end());
	}
	iterator head() const {
		return iterator(tree.head());
	}
	Set map(T func(T)) {
		return Set(tree.map(func));
	}
	Set where(bool func(T)) {
		return Set(tree.where(func));
	}
	T reduce(T func(T, T), const T& start_value) {
		return tree.reduce(func, start_value);
	}
	Set& operator+= (const Set& other) {
		for (auto i : other) {
			tree.insert(i);
		}
		return *this;
	}
	Set operator+ (const Set& other) const {
		Set ans = *this;
		ans += other;
		return ans;
	}
	Set& operator-= (const Set& other) {
		iterator last_it = begin(), it = last_it;
		while (it != end()) {
			if (other.find(*it) != other.end()) {
				erase(it);
				if (it == last_it) last_it = it = begin();
				else {
					it = last_it;
					it++;
				}
			}
			else {
				last_it = it;
				it++;
			}
		}
		return *this;
		return *this;
	}
	Set operator- (const Set& other) const {
		Set ans = *this;
		ans -= other;
		return ans;
	}
	Set& operator*= (const Set& other) {
		iterator last_it= begin(), it = last_it;
		while (it != end()) {
			if (other.find(*it) == other.end()) {
				erase(it);
				if (it == last_it) last_it = it = begin();
				else {
					it = last_it;
					it++;
				}
			}
			else {
				last_it = it;
				it++;
			}
		}
		return *this;
	}
	Set operator* (const Set& other) const {
		Set ans = *this;
		ans *= other;
		return ans;
	}
	bool is_subset(const Set& other) const {
		auto end_ = end();
		for (auto i : other) {
			if (find(i) == end_) return false;
		}
		return true;
	}
	operator std::string() const {
		return (std::string)tree;
	}
};