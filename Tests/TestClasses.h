#pragma once
#include <iostream>
#include <string>

//integer numbers (int)
//real numbers (double)
//complex numbers (complex)
class complex {
private:
	double re, im;
	class complex_exception {
		size_t id;
	public:
		complex_exception(size_t id) {
			this->id = id;
		}
		const char* message() {
			if (id == 0) return "Error: division by zero";
			else return "Error: wrong input of the complex number";
		}
	};
public:
	complex();
	complex(const double&, const double&);
	//operators
	bool operator== (const complex&) const;
	bool operator!= (const complex&) const;
	complex& operator+= (const complex&);
	complex& operator-= (const complex&);
	complex& operator*= (const complex&);
	complex& operator/= (const complex&);
	complex operator+ (const complex&) const;
	complex operator- (const complex&) const;
	complex operator- () const;
	complex operator* (const complex&) const;
	complex operator/ (const complex&) const;
	friend std::istream& operator>> (std::istream&, complex&);
	friend std::ostream& operator<< (std::ostream&, const complex&);
	//functions
	friend double Re(const complex&);
	friend double Im(const complex&);
	friend double abs(const complex&);
};
//strings (std::string) and chars(char)
//namespace std {
//	string to_string(const char& c) {
//		char ans[2];
//		ans[0] = c;
//		ans[1] = '\0';
//		return (string)ans;
//	}
//};
//functions (*())
//students (Student)
class Student {
private:
	std::string name_; 
	size_t age_; 
	size_t group_; 
public:
	Student();
	Student(const std::string&, const size_t&, const size_t&);
	//operators
	bool operator== (const Student&) const;
	bool operator!= (const Student&) const;
	//methods
	std::string name() const;
	size_t age() const;
	size_t group() const;
};
//teacher (Teacher)
class Teacher {
private:
	std::string name_;
	size_t age_;
	size_t faculty_;
public:
	Teacher();
	Teacher(const std::string&, const size_t&, const size_t&);
	//operators
	bool operator== (const Teacher&) const;
	bool operator!= (const Teacher&) const;
	//methods
	std::string name() const;
	size_t age() const;
	size_t faculty() const;
};
//pairs (std::pair<T1, T2>)