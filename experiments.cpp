#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
struct segment {
	int a, b;
};

struct solution : vector<segment> {
	iterator next;
};

int main() {
	vector<int> a;
	a.resize(10);
	a[4] = 1;
	copy(cbegin(a), cend(a), ostream_iterator<int>(cout, " "));
	cout << endl;
	solution S;
	S.push_back({ 1, 2 });

	cout << S.size() << endl;

	return 0;
}