/* Attempt at
* https://www.hackerrank.com/challenges/attending-workshops

*/

#define CATCH_CONFIG_MAIN
//#define logging
//#undef CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include "catch.hpp"

using namespace std;
//Define the structs Workshops and Available_Workshops.

struct Workshop {
	int start;
	int duration;
	int end;
};

ostream& operator << (ostream& stream, const Workshop& a) {
	return stream << "{" << a.start << ", " << a.end << "}";
}

struct Available_Workshops {
	size_t n;
	Workshop* workshops;
};

Workshop* begin(Available_Workshops& a) {
	return a.workshops;
}

Workshop* end(Available_Workshops& a) {
	return a.workshops + a.n;
}



struct solution : vector<Workshop> {
	Workshop* next = nullptr;
	explicit operator bool() const { return !empty(); }
};

size_t max_solution;
size_t zero_solutions;


Available_Workshops* initialize(int* start, int* duration, size_t n) {
	if (!n) return nullptr;
	Available_Workshops* res = new Available_Workshops;
	res->n = n;
	res->workshops = new Workshop[n];
	while (n--)
		res->workshops[n] = { start[n], duration[n], start[n] + duration[n] };
	return res;
}

bool is_extendable(Available_Workshops* sched, solution& c) {
	if (c.next == sched->workshops + sched->n) {
		c.clear();
		return false;
	}
	return true;
}

solution& extend_solution(solution& c) {
	c.push_back(*c.next);
	c.next++;
	return c;
}

bool overlap(const Workshop& a, const Workshop& b) {
	if (a.duration == 0 || b.duration == 0) return false;
	return (a.start < b.end) && (b.start < a.end);
}

bool reject(solution c) {
	if (!c) return false;
	return any_of(begin(c), prev(end(c)),
		[&a = as_const(c.back())](const auto& b) {
		return (a.start < b.end) && (b.start < a.end);
		//return (a.start - b.end) * (b.start - a.end) > 0;
		//overlap condition is valid only if start > end for a and b
	}
	);
}

solution root(Available_Workshops* sched) {
	solution c;
	c.next = find_if(sched->workshops, sched->workshops + sched->n,
		[](const Workshop& a) {
			return a.duration != 0;
		});
	zero_solutions = distance(sched->workshops, c.next);
	//c.next = sched->workshops;
	return c;
};

solution first(Available_Workshops* sched, solution c) {
	if (!is_extendable(sched, c)) return c;
	return extend_solution(c);
}

solution next(Available_Workshops* sched, solution c) {
	if (!is_extendable(sched, c)) return c;
	if (c) c.pop_back();
	return extend_solution(c);
};

auto bt(Available_Workshops* sched, solution c) {
#ifdef  logging
	cout << "[" << (reject(c) ? "        " : "solution") << "](" << max_solution << "): ";
	for (const auto& w : c)
		cout << "{" << w.start << "," << w.end << "} ";
	cout << endl;
#endif //  logging

	if (reject(c)) return;
	max_solution = max(max_solution, c.size()); // if accept(c) then output(c)
	if (max_solution == sched->n) return;
	auto s = first(sched, c);

	while (s) {
		bt(sched, s);
		s = next(sched, s);
	}
};

int CalculateMaxWorkshops1(Available_Workshops* sched) {
	if (sched->n == 0) return 0;
	max_solution = 0;
	//bool cmp(const Type1 & a, const Type2 & b);
	sort(sched->workshops, sched->workshops + sched->n, [](const Workshop& a, const Workshop& b) {
		return a.duration < b.duration;
		});
	bt(sched, root(sched));
#ifdef logging
	cout << endl;
#endif // logging

	return static_cast<int>(max_solution + zero_solutions);
}

int CalculateMaxWorkshops(Available_Workshops* sched) {
	if (sched->n == 0) return 0;
	sort(begin(*sched), end(*sched), [](const Workshop& a, const Workshop& b) {
		return a.start != b.start ? a.start < b.start : a.duration < b.duration;
		});

	auto active = sched->workshops[0];

	auto time = active.start;
	int solutions = active.duration == 0 ? 0 : 1;
	for (auto current : *sched) {
		if (current.duration == 0) {
			solutions++;
			continue;
		}
		if (time == current.start) continue;
		time = current.start;

		if (time >= active.end) {
			active = current;
			solutions++;
			continue;
		}
		if (active.end - time >= current.duration) active = current;
	}
	return solutions;
}

#ifndef CATCH_CONFIG_MAIN
int main(int argc, char* argv[]) {
	int n; // number of workshops
	cin >> n;
	// create arrays of unknown size n
	int* start_time = new int[n];
	int* duration = new int[n];

	for (int i = 0; i < n; i++) {
		cin >> start_time[i];
	}
	for (int i = 0; i < n; i++) {
		cin >> duration[i];
	}

	Available_Workshops* ptr;
	ptr = initialize(start_time, duration, n);
	cout << CalculateMaxWorkshops(ptr) << endl;
	delete[] ptr->workshops;
	delete ptr;
	return 0;
}
#endif

TEST_CASE("first case", "[first]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {1,1,2},{3,1,4},{0,6,6},{5,2,7},{5,4,9},{8,1,9} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 4);
	delete ptr;
}

TEST_CASE("first case shuffled seed 0", "[shuffled0]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {5,4,9}, {8,1,9}, {3,1,4}, {1,1,2}, {0,6,6}, {5,2,7} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 4);
	delete ptr;
}

TEST_CASE("first case shuffled", "[./workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {1,1,2},{3,1,4},{0,6,6},{5,2,7},{5,4,9},{8,1,9} };
	std::random_device rd;
	std::mt19937 g(rd());
	g.seed(1);

	std::shuffle(begin(input), end(input), g);
	//for (const auto& w : input)
	//	cout << "{" << w.start << "," << w.duration << "," << w.end << "}, ";
	//cout << endl;
	ptr->n = input.size();
	ptr->workshops = input.data();

	REQUIRE(CalculateMaxWorkshops(ptr) == 4);
	delete ptr;
}


TEST_CASE("empty case", "[workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{};
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 0);
	delete ptr;
}

TEST_CASE("one case", "[workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {0, 10, 10} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 1);
	delete ptr;
}

TEST_CASE("two copies of an event", "[workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {0, 10, 10}, {0, 10, 10} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 1);
	delete ptr;
}

TEST_CASE("tightly fitting events", "[workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {0, 10, 10}, {10, 10, 20} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 2);
	delete ptr;
}

TEST_CASE("overlap() zero length", "[overlap]") {
	////REQUIRE(overlap({ 10, 0, 10 }, { 0, 6, 6 }) == false);
	vector<Workshop> input{ {5,4,9}, {8,1,9}, {3,1,4}, {1,1,2}, {0,6,6}, {5,2,7}, {2, 0, 2} };
	for (const auto& w : input) {
		//cout << w.start << endl;
		REQUIRE(overlap({ 7, 0, 7 }, w) == false);
	}

}

TEST_CASE("zero length", "[workshops]") {
	Available_Workshops* ptr = new Available_Workshops;
	vector<Workshop> input{ {0, 0, 0}, {1, 2, 3}, {0, 0, 0}, {1, 2, 3}, {0, 0, 0}, {1, 2, 3} };
	ptr->n = input.size();
	ptr->workshops = input.data();
	REQUIRE(CalculateMaxWorkshops(ptr) == 4);
	delete ptr;
}

