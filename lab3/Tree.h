#pragma once
#include <exception>
#include <string>

template <class T, size_t n>
class Tree {
private:
	T value;
	Tree** vertexes;
	size_t size;
public:
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
		}
	};
	Tree() {
		value = T();
		vertexes = nullptr;
		size = 0;
	}
	Tree(const T& value) {
		size = 1;
		this->value = value;
		try {
			vertexes = new Tree*[n];
		}
		catch (std::bad_alloc err) {
			throw TreeException(1);
		}
		for (size_t i = 0; i < n; i++) vertexes[i] = nullptr;
	}
	Tree(const Tree& other) : Tree(other.value) {
		size = other.size;
		for (size_t i = 0; i < n; i++) {
			if (other.vertexes[i]) {
				try {
					vertexes[i] = new Tree;
				}
				catch (std::bad_alloc err) {
					throw TreeException(1);
				}
				*(vertexes[i]) = Tree(other.vertexes[i]);
			}
			else vertexes[i] = nullptr;
		}
	}
	Tree(const std::string& format_string, const std::initializer_list<T>& elements) {
		if (format_string.size() == 0) *this = Tree();
		else {
			if (format_string.size() < 3) throw TreeException(3);
			if (format_string[0] != '[') throw TreeException(3);
			int closing_br = format_string.find(']');
			if (closing_br == -1 || closing_br == 2) throw TreeException(3);
			int number = atoi(format_string.substr(1, (size_t)closing_br - 1));
			if (number < 0) throw TreeException(3);
			format_string = format_string.substr((size_t)closing_br + 1);
			if (number >= elements.size()) throw TreeException(3);
			if (format_string == "") *this = Tree(elements[number]);
			else {
				if (format_string.size() < 3) throw TreeException(3);
				if (format_string[0] != '{' || format_string[format_string.size() - 1] != '}') throw TreeException(3);
				*this = Tree(elements[number]);
				format_string = format_string.substr(1, format_string.size() - 2);
				int comma = 0, i = 0;
				while (comma != -1) {
					comma = format_string.find(',');
					try {
						vertexes[i] = new Tree*;
					}
					catch (std::bad_alloc err) {
						throw TreeException(1);
					}
					try {
						*(vertexes[i]) = Tree(format_string.substr(0, comma), elements);
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

	void add(const T& value, const std::initializer_list<size_t>& path = {}) {
		Tree* it = this, index = 0;
		for (auto i : path) {
			if (i == 0 || i > n) throw TreeException(2);
			else if (it->vertexes[i - 1]) it = it->vertexes[i - 1];
			else {
				index = i;
				break;
			}
		}
		try {
			it->vertexes[i] = new Tree;
		}
		catch (std::bad_alloc) {
			throw TreeException(1);
		}
		*(it->vertexes[i]) = Tree(value);
		size++;
	}
	~Tree() {
		clear();
	}
	void clear() {
		for (size_t i = 0; i < n; i++) {
			if (vertexes[i]) vertexes[i]->clear();
		}
	}
};

/*if (format_string[0] != '[') throw TreeException(3);
			int closing_br = format_string.find(']');
			if (closing_br == -1) throw TreeException(3);
			size_t number = std::stoi(format_string.substr(1, (size_t)closing_br - 1));
			if (number > elements.size() || number == 0) throw TreeException(3);
			T value = *(elements.begin() - 1 + number);
			*this = Tree(value);
			if (format_string.size() > closing_br + 1) {
				if (format_string[closing_br + 1] != '{') throw TreeException(3);
				if (format_string[format_string.size() - 1] != '}') throw TreeException(3);
				format_string = format_string.substr(closing_br + 2, format_string.size() - closing_br - 3);
				int begin = 0, end = format_string.find(','), i = 0
				while (end != -1) {

					if (end - begin == 1) {

					}
					try {
						vertexes[i] = new Tree;
					}
					catch (std::bad_alloc err) {
						throw TreeException(1);
					}
					*(vertexes[i]) = Tree(format_string.substr(closing_br + 2, end - closing_br - 2), elements);
				}
			}*/