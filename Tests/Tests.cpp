#include "pch.h"
#include "CppUnitTest.h"
#include "../lab3/Tree.h"
#include "TestClasses.cpp"
#include <sstream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		//создание дерева и его обход
		TEST_METHOD(BlockTest1) {
			Tree<int, 4> a("[0]{[1]{[5],[6],[7]},[2]{[8],[9],[10]},[3]{[11],[12]},[4]{[13],[14],[15]}}", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
			std::string traverse1 = "6 7 8 2 9 10 11 3 12 13 4 14 15 16 5 1";
			std::string traverse2 = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16";
			std::string traverse3 = "10 3 9 11 1 7 2 6 8 15 5 14 16 13 4 12";
			std::string traverse4 = "1 5 3 2 4 15 14 16 10 9 11 7 6 8 13 12";
			Assert::AreEqual((std::string)a, traverse1, L"Block #1 Test #1 failed: Traversal doesn't work as it must");
			a.set_traversal("B(1)(2)(3)(4)");
			Assert::AreEqual((std::string)a, traverse2, L"Block #1 Test #2 failed: Traversal doesn't work as it must");
			a.set_traversal("D(2)(R)(1)(4)(3)");
			Assert::AreEqual((std::string)a, traverse3, L"Block #1 Test #3 failed: Traversal doesn't work as it must");
			a.set_traversal("B(4)(2)(1)(3)");
			Assert::AreEqual((std::string)a, traverse4, L"Block #1 Test #4 failed: Traversal doesn't work as it must");
			Tree<std::string, 1> c("[5]{[5]}", { "","","","", "", "Hello", "", "", "" });
			std::string traverse5 = "Hello Hello";
			Assert::AreEqual(traverse5, (std::string)c, L"Block #1 Test #5 failed: traverse doen't work as it must");
		}
		//добавление, удаление элементов, foreach циклы
		TEST_METHOD(BlockTest2) {
			Tree<int, 4> a("[0]{[1]{[5],[6],[7]},[2]{[8],[9],[10]},[3]{[11],[12]},[4]{[13],[14],[15]}}", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			a.add(0, { 2, 1 });
			DynamicArray<int> traverse1 = { 6, 7, 8, 2, 0, 9, 10, 11, 3, 12, 13, 4, 14, 15, 16, 5, 1 };
			DynamicArray<int> traverse2 = {-1, 6, 7, 8, 2, 0, 9, 10, 11, 3, 12, 13, 4, 14, 15, 16, 5, 1 };
			DynamicArray<int> traverse3 = { -1, 6, 7, 8, 0, 9, 10, 11, 3, 12, 13, 4, 14, 15, 16, 5, 1 };
			DynamicArray<int> traverse4 = { -1, 6, 7, 8, 0, 9, 10, 11, 3, 12, 13, 14, 15, 16, 5, 1 };
			DynamicArray<int> traverse5 = { -1, 6, 7, 8, 12, 13, 14, 15, 16, 5, 1 };
			size_t ind = 0;
			for (auto i : a) {
				Assert::AreEqual(i, traverse1[ind], L"Block #2 Test #1 failed: Traversal doen't work as it must");
				ind++;
			}
			a.add(-1, a.begin());
			ind = 0;
			for (auto i : a) {
				Assert::AreEqual(i, traverse2[ind], L"Block #2 Test #2 failed: Adding an element by absolute path doen't work as it must");
				ind++;
			}
			a.erase({ 1 });
			ind = 0;
			for (auto i : a) {
				Assert::AreEqual(i, traverse3[ind], L"Block #2 Test #3 failed: Erasing an element by absolute path doen't work as it must");
				ind++;
			}
			a.erase(a.find(4));
			ind = 0;
			for (auto i : a) {
				Assert::AreEqual(i, traverse4[ind], L"Block #2 Test #4 failed: Erasing an element by absolute path doen't work as it must");
				ind++;
			}
			a.remove({ 2 });
			ind = 0;
			for (auto i : a) {
				Assert::AreEqual(i, traverse5[ind], L"Block #2 Test #5 failed: Erasing an element by absolute path doen't work as it must");
				ind++;
			}
			//в вершине дерева 'a', потомки: 'b', 'c', 'd', 'e', 'f', 'g', 'h'. У 'b' потомки следующие 10 букв алфавита, у 'f' оставшиеся 9. У каждого потомка 'b' один потомок, который образует поддерево, являющееся ранее описанным деревом
			//размер "основного" дерева - 26(количество бук в латинице) + 10*26 у каждого потомка 'b'. Итого 11*26=286
			Tree<char, 10> b("[0]{[1]{[8]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[9]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[10]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[11]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[12]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[13]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[14]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[15]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[16]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}},[17]{[0]{[1]{[8],[9],[10],[11],[12],[13],[14],[15],[16],[17]},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}}},[2],[3],[4],[5]{[18],[19],[20],[21],[22],[23],[24],[25]},[6],[7]}", "abcdefghijklmnopqrstuvwxyz");
			Assert::AreEqual(b.size(), (size_t)286, L"Block #1 Test #5 failed: wrong size");
			b.set_traversal("B(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)");
			std::string traverse6 = "a b c d e f g h i j k l m n o p q r s t u v w x y z a a a a a a a a a a b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h b c d e f g h i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z i j k l m n o p q r s t u v w x y z";
			Assert::AreEqual(traverse6, (std::string)b, L"Block #1 Test #5 failed: Traversal doesn't work as it must");
		}
		//map, reduce, where
		TEST_METHOD(BlockTest3) {
			Tree<int, 4> a("[0]{[1]{[5],[6],[7]},[2]{[8],[9],[10]},[3]{[11],[12]},[4]{[13],[14],[15]}}", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			Tree<int, 4> expected_result_1("[0]{[1]{[5],[6],[7]},[2]{[8],[9],[10]},[3]{[11],[12]},[4]{[13],[14],[15]}}", { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256 });
			auto b = a.map([](int value) {return value * value; });
			Assert::IsTrue(expected_result_1 == b, L"Block #3 Test #1 failed: map() doesn't work as it must");
			Tree<Student, 2> c("[0]{[1]{[3]{[7],[8]},[4]},[2]{[5],[6]}}", {
				Student("Klinov", 18, 514),
				Student("Kostyaev", 18, 514),
				Student("Matyashov", 18, 514),
				Student("Grozniy", 18, 503),
				Student("Danilin", 18, 514),
				Student("Voronin", 19, 525),
				Student("Baranov", 19, 514),
				Student("Popkov", 18, 514),
				Student("Ryzhov", 19, 514)
			});
			Tree<Student, 2> expected_result_2("[0]{[1]{[3],[4]},[2]}", {
				Student("Klinov", 18, 514),
				Student("Kostyaev", 18, 514),
				Student("Matyashov", 18, 514),
				Student("Popkov", 18, 514),
				Student("Danilin", 18, 514),
			});
			Assert::IsTrue(c.where([](Student st) {return st.age() == 18 && st.group() == 514; }) == expected_result_2, L"Block #3 Test #2 failed: where(0 doesn't work as it must");
			Assert::IsTrue(b.reduce([](int a, int b) {return a + b; }, 0) == 1496, L"Block #3 Test #3 failed: reduce() doesn't work as it must");
		}
		//subtree, merge, format string
		TEST_METHOD(BlockTest4) {
			Tree <double, 3> a("[0]{[1]{[4]{[2],[4],[6]},[5]{[8],[10]},[1]{[1],[3],[4]}},[2]{[3]{[11]},[2],[6]},[3]{[7],[8],[9]}}", { 5.3, -1.31, 59.1, 3.02, -7.9, 52.1, -0.7, 45.46, -135.34, 90.71, 0, 86.72 });
			Tree<double, 3> b = a.subtree({ 1, 2 });
			Assert::IsTrue(Tree<double, 3>("[5]{[8],[10]}", { 5.3, -1.31, 59.1, 3.02, -7.9, 52.1, -0.7, 45.46, -135.34, 90.71, 0 }) == b, L"Block #4 Test #1 failed: subtree() doesn't work as it must");
			Assert::IsTrue(a.is_subtree(b) != a.end(), L"Block #4 Test #2 failed: is_subtree() doesn't work as it must");
			Assert::IsTrue(a.is_subtree(a) != a.end(), L"Block #4 Test #3 failed: is_subtree() doesn't work as it must");
			Tree<double, 3> c("[0]{[1]{[2],[3],[4]},[4]{[5],[6],[7]},[7]{[7],[8],[9]}}", { 3.14, 4.3, -54.432, 4.13, -9.43, 5.2, -0.9, 31.1, 43.5, 93.1 });
			Tree <double, 3> d("[14]{[0]{[1]{[4]{[2],[4],[6]},[5]{[8],[10]},[1]{[1],[3],[4]}},[2]{[3]{[11]},[2],[6]},[3]{[7],[8],[9]}},[12]{[13]{[15],[16]},[16]{[17],[18],[19]},[19]{[19],[20],[21]}}}", { 5.3, -1.31, 59.1, 3.02, -7.9, 52.1, -0.7, 45.46, -135.34, 90.71, 0, 86.72, 3.14, 4.3, -54.432, 4.13, -9.43, 5.2, -0.9, 31.1, 43.5, 93.1 });
			a += c;
			Assert::IsTrue(d == a, L"Block #4 Test #4 failed: merge doesn't work as it must");
			std::string f = "[0]{[1]{[6]{[21],[22]},[7]{[23],[24]},[8],[9]},[2]{[10],[11],[12],[13]},[3]{[14],[15],[16],[17]},[4]{[18],[19]},[5]{[20]}}";
			Tree<int, 5> e(f, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 });
			std::stringstream ss;
			ss << e;
			std::string res;
			ss >> res;
			Assert::AreEqual(f, res, L"Block #4 Test #5 failed: operator<< doesn't work as it must");
		}
		//searchtree
		TEST_METHOD(BlockTest5) {
			SearchTree<int> a = { 5, 2, 1, 5, 1, 4, 6, 8, -2 };
			DynamicArray<int> expected_result1 = { -2, 1, 1, 2, 4, 5, 5, 6, 8 };
			int ind = 0;
			for (auto i : a) {
				Assert::AreEqual(expected_result1[ind], i, L"Block #5 Test #1 failed: there's no order in SearchTree");
				ind++;
			}
			Assert::IsTrue(a.find(7) == a.end(), L"Block #5 Test #2 failed: find() doesn't work as it must");
			Assert::IsTrue(*(a.find(5)) == 5, L"Block #5 Test #3 failed: find() doesn't work as it must");
		}
		//set
		TEST_METHOD(BlockTest6) {
			Set<int> a = { 2, 4, 3, -1, 2, 4, 0 };
			Set<int> b = { 3, 1, 5, 0, 9, 7, 7, 3 };
			Set<int> exp_intersection = { 0, 3 };
			Set<int> exp_union = { -1, 0, 1, 2, 3, 4, 5, 7, 9 };
			Set<int> exp_difference = { -1, 2, 4};
			DynamicArray<int> expected_result1 = { -1, 0, 2, 3, 4 }, expected_result2 = {0, 1, 3, 5, 7, 9};
			int ind = 0;
			for (auto i : a) {
				Assert::AreEqual(expected_result1[ind], i, L"Block #6 Test #1 failed: there's no order in Set");
				ind++;
			}
			ind = 0;
			for (auto i : b) {
				Assert::AreEqual(expected_result2[ind], i, L"Block #6 Test #2 failed: there's no order in Set");
				ind++;
			}
			Assert::IsTrue(a + b == exp_union, L"Block #6 Test #3 failed: operator + doesn't work as it must");
			Assert::IsTrue(a * b == exp_intersection, L"Block #6 Test #4 failed: operator * doesn't work as it must");
			Assert::IsTrue(a - b == exp_difference, L"Block #6 Test #5 failed: operator - doesn't work as it must");
		}
	};
}
