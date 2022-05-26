#include "TestClasses.h"
//complex
namespace std {
	string to_string(string s) {
		return s;
	}
	string to_string(...) {
		return "";
	}
	string to_string(const char& c) {
		char ans[2];
		ans[0] = c;
		ans[1] = '\0';
		return (string)ans;
	}
};
complex::complex(const double& re, const double& im = 0) {
	this->re = re;
	this->im = im;
}
complex::complex() : complex(0, 0) {}
bool complex::operator== (const complex& other) const {
	return re == other.re && im == other.im;
}
bool complex::operator!= (const complex& other) const {
	return re != other.re || im != other.im;
}
complex& complex::operator+= (const complex& other) {
	re += other.re;
	im += other.im;
	return *this;
}
complex& complex::operator-= (const complex& other) {
	re -= other.re;
	im -= other.im;
	return *this;
}
complex& complex::operator*= (const complex& other) {
	re = re * other.re - im * other.im;
	im = re * other.im + other.re * im;
	return *this;
}
complex& complex::operator/= (const complex& other) {
	if (other == 0) throw complex::complex_exception(0);
	else {
		re = (re * other.re + im * other.im) / (other.re * other.re + other.im * other.im);
		im = (im * other.re - re * other.im) / (other.re * other.re + other.im * other.im);
		return *this;
	}
}
complex complex::operator+ (const complex& other) const {
	complex ans = *this;
	return ans += other;
}
complex complex::operator- (const complex& other) const {
	complex ans = *this;
	return ans -= other;
}
complex complex::operator- () const {
	complex ans(-re, -im);
	return ans;
}
complex complex::operator* (const complex& other) const {
	complex ans = *this;
	return ans *= other;
}
complex complex::operator/ (const complex& other) const {
	complex ans = *this;
	return ans /= other;
}
std::istream& operator>> (std::istream& is, complex& number) {
	char c;
	is >> number.re >> number.im >> c;
	if (c != 'i') throw complex::complex_exception(1);
	else return is;
};
std::ostream& operator<< (std::ostream& os, const complex& number) {
	if (number.re != 0) os << number.re;
	if (number.im > 0 && number.re != 0) {
		if (number.im == 1) os << " + i";
		else os << " + " << number.im << " * i";
	}
	else if (number.im < 0 && number.re != 0) {
		if (number.im == -1) os << " - i";
		else os << " - " << -number.im << " * i";
	}
	else if (number.re == 0) {
		if (number.im != 0) {
			if (number.re == 1) os << 'i';
			else if (number.re == -1) os << "-i";
			else os << number.im << " * i";
		}
		else os << "0";
	}
	return os;
}
double Re(const complex& number) {
	return number.re;
}
double Im(const complex& number) {
	return number.im;
}
double abs(const complex& number) {
	return number.re*number.re + number.im*number.im;
}
//student
Student::Student() = default;
Student::Student(const std::string& name, const size_t& age, const size_t& group) {
	name_ = name;
	age_ = age;
	group_ = group;
}
bool Student::operator== (const Student& other) const {
	return name_ == other.name_ && age_ == other.age_ && group_ == other.group_;
}
bool Student::operator!= (const Student& other) const {
	return !(*this == other);
}
std::string Student::name() const {
	return this->name_;
}
size_t Student::age() const {
	return this->age_;
}
size_t Student::group() const {
	return this->group_;
}
//teacher
Teacher::Teacher() = default;
Teacher::Teacher(const std::string& name, const size_t& age, const size_t& faculty) {
	faculty_ = faculty;
	name_ = name;
	age_ = age;
}
bool Teacher::operator==(const Teacher& other) const {
	return name_ == other.name_ && faculty_ == other.faculty_ && age_ == other.age_;
}
bool Teacher::operator!= (const Teacher& other) const {
	return !(*this == other);
}
std::string Teacher::name() const {
	return name_;
}
size_t Teacher::age() const {
	return age_;
}
size_t Teacher::faculty() const {
	return faculty_;
}