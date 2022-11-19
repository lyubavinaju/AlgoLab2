#include "Array.h"
#include <gtest/gtest.h>
#include <string>


TEST(Array, DefaultConstructor) {
	Array<int> a;
	ASSERT_EQ(a.size(), 0);
}

TEST(Array, CapacityConstructor) {
	Array<int> a(100);
	ASSERT_EQ(a.size(), 0);
}

TEST(Array, CopyConstructor) {
	Array<int> a;
	a.insert(1);
	Array<int> b(a);
	ASSERT_EQ(b.size(), 1);
}

TEST(Array, MoveConstructor) {
	Array<int> a;
	a.insert(1);
	Array<int> b(std::move(a));
	ASSERT_EQ(a.size(), 0);
	a.~Array();
	ASSERT_EQ(b.size(), 1);
}

TEST(Array, Insert) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	ASSERT_EQ(a.size(), testSize);
	for (int i = 0; i < testSize; ++i) {
		ASSERT_EQ(a[i], i + 1);
	}
}

TEST(Array, Change) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	for (int i = 0; i < a.size(); ++i) {
		a[i] *= 2;
	}
	for (int i = 0; i < testSize; ++i) {
		ASSERT_EQ(a[i], 2 * (i + 1));
	}
}

TEST(Array, Remove) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	a.remove(3);
	ASSERT_EQ(a[3], 5);
}

TEST(Array, Iterator) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	int i = 0;
	for (Array<int>::Iterator it = a.iterator(); it.hasNext(); it.next()) {
		ASSERT_EQ(it.get(), i + 1);
		i++;
	}
}

TEST(Array, ReverseIterator) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	int i = testSize - 1;
	for (Array<int>::Iterator it = a.reverseIterator(); it.hasNext(); it.next()) {
		ASSERT_EQ(it.get(), i + 1);
		i--;
	}
}

TEST(Array, SetIterator) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}

	Array<int>::Iterator it = a.iterator();
	it.next();
	it.next();
	ASSERT_EQ(it.get(), 3);
	it.set(-100);
	ASSERT_EQ(it.get(), -100);
}

TEST(Array, ComplexType) {
	Array<Array<int>> a;
	Array<int> ai;
	ai.insert(-100);
	a.insert(ai);
	ASSERT_EQ(a[0][0], -100);
}

TEST(Array, ExpandTest) {
	int testSize = 20;
	Array<int> a(testSize);
	for (int i = 0; i < 100 * testSize; ++i) {
		a.insert(i + 1);
	}
	ASSERT_EQ(a.size(), 100 * testSize);
}

TEST(Array, StringTest) {
	int testSize = 20;
	Array<std::string> a(testSize);
	for (int i = 0; i < 100 * testSize; ++i) {
		a.insert(std::to_string(i + 1));
	}
	ASSERT_EQ(a.size(), 100 * testSize);
	for (int i = 0; i < 100 * testSize; ++i) {
		ASSERT_EQ(a[i], std::to_string(i + 1));
	}
}

TEST(Array, ConstTest) {
	Array<int> a;
	int testSize = 10;
	for (int i = 0; i < testSize; ++i) {
		a.insert(i + 1);
	}
	Array<int> const b(a);
	int i = 0;
	for (Array<int>::ConstIterator it = b.iterator(); it.hasNext(); it.next()) {
		ASSERT_EQ(it.get(), i + 1);
		i++;
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}