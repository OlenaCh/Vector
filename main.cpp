// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./main
// gcc cpparray.hpp main.cpp -o main -lstdc++ -Wall

#include "vector.hpp"

#include <string>
#include <iostream>
#include <cassert>

#define TESTSIZE 10

using namespace std;

static constexpr size_t S = TESTSIZE;

template<typename T>
Array<Array<T>> matrix_mult (const Array<Array<T>>& a, const Array<Array<T>>& b) {
	if (a.empty () || b.empty ()) return Array<Array<T>> ();

	assert (a.size () == b.front ().size ());

	size_t h = a.front ().size (), w = b.size (), d = a.size ();

	Array<Array<T>> res;
	res.resize (w);

	for (size_t i = 0; i < w; ++i)
		res[i].resize (h);

	for (size_t i = 0; i < w; ++i)
		for (size_t j = 0; j < h; ++j)
			for (size_t k = 0; k < d; ++k)
				res[i][j] += a[k][j] * b[i][k];

	return res;
}

int main2() {

	Array<int> b;
	b.reserve(3);
	b.push_back(3);
	b.push_back(4);
	b.push_back(5);

	Array<int> c;
	c.reserve(3);
	c.push_back(1);
	c.push_back(2);
	c.push_back(0);

	Array<int> d;
	d.reserve(3);
	d.push_back(10);
	d.push_back(20);
	d.push_back(9);

	d.reserve(15);

	Array<Array<int>> a;
	a.reserve(3);
	a.push_back(b);
	a.push_back(c);
	a.push_back(d);

	//for(auto& tr : a)
	//	cout << tr.size() << endl;

	Array<Array<int>> t(a);
	Array<Array<int>> k=a;

	k.append(t);


	for(size_t i=0;i<5;++i) {
		for(size_t j=0;j<3;++j) {
  		cout << k[i][j] << endl;
  	}
  }

	return 0;
}

int main () {
	Array<Array<float>> a, b, c;

	// first, generate some data
	a.reserve (S);
	b.reserve (S);

	for (size_t i = 0; i < S; ++i) {
		Array<float> col;
		col.reserve (S);

		for (size_t j = 0; j < S; ++j) {
			col.push_back (0.3 * i + 0.5 * j);
			col.back() /= S;
		}

		a.push_back (col);
		col.pop_back ();
		col.push_back (10 * i);
		b.push_back (col);
	}

	// permutations!
	a[5].swap (b[2]);
	b[3].swap (a[0]);

	// run the multiplication
	c = matrix_mult (a, b);

	// empty the matrices in various weird ways
	while (!a.empty ()) a.pop_back ();
	b.clear ();

	// print out the content of 'c'
	for (size_t i = 0; i < S; ++i) {
		for (size_t j = 0; j < S; ++j) cout << c[i][j] << '\t';
		cout << endl;
	}

	return 0;
}
