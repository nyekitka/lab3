#include <iostream>
#include "../Tests/TestClasses.h"
#include "Tree.h"

using namespace std;
int main() {
	Set<int> a = { 2, 4, 3, -1, 2, 4, 0 };
	Set<int> b = { 3, 1, 5, 0, 9, 7, 7, 3 };
	Set<int> c = a-b;
	for (auto i : a) cout << i << ' ';
	cout << '\n';
	for (auto i : b) cout << i << ' ';
	cout << '\n';
	for (auto i : c) cout << i << ' ';
	cout << '\n';
	return 0;
}

