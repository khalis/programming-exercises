#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//recursive sum:
int dumb_sum(int a) {
	if (a == 0) return 0;
	return a + dumb_sum(a - 1);
}

int main() {
	
	cout << dumb_sum(3) << endl;
	return 0;
}