#define CATCH_CONFIG_MAIN
#include "LCS.h"
#include "catch.hpp"
using namespace std;


int LCS_recursive(const string& x, int i, const string& y, int j) {
	if (i == -1 || j == -1) return 0;
	if (x[i] == y[j]) return LCS_recursive(x, i - 1, y, j - 1) + 1;
	else return max(LCS_recursive(x, i, y, j - 1), LCS_recursive(x, i - 1, y, j));
}

int LCS(const string& x, const string& y) {
	return LCS_recursive(x, static_cast<int>(x.length()) - 1,
		y, static_cast<int>(y.length()) - 1);
}


//dynamic programming solution
int LCS_matrix(const std::string& x, const std::string& y) {
	size_t m = x.length();
	size_t n = y.length();

	vector<vector<int>> array(m + 1);

	// solve for LCS
	for (size_t i = 0; i <= m; i++) array[i] = vector<int>(n + 1, 0);
	//for (int j = 0; j <= n; j++) array[0][j] = 0;
	copy(cbegin(y), cend(y), ostream_iterator<char>(cout << "  ", " "));
	cout << endl;
	for (size_t i = 1; i <= m; i++) {
		cout << x[i-1] << ' ';
		for (size_t j = 1; j <= n; j++) {
			
			if (x[i - 1] == y[j - 1]) {
				array[i][j] = array[i - 1][j - 1] + 1;
				cout << array[i][j] << ' ';
			}
			else {
				array[i][j] = max(array[i][j - 1], array[i - 1][j]);
				cout << array[i][j] << ' ';
			}
		}
		cout << endl;
	}
	cout << endl;
	return array[m][n];
}


//memory optimized dynamic programming solution
int LCS_non_recursive(const std::string& x, const std::string& y) {
	size_t m = y.length();
	size_t n = x.length();

	vector<int> D(m + 1, 0);
	int diag = 0;

	copy(cbegin(y), cend(y), ostream_iterator<char>(cout << "    ", " ")) = '\n';

	copy(cbegin(D), cend(D), ostream_iterator<int>(cout << " ", " "));

	cout << endl;

	for (size_t i = 0; i < n; ++i) {
		diag = 0;
		for (size_t j = 1; j < m + 1; ++j)
			if (x[i] == y[j - 1]) {
				int temp = D[j];
				D[j] = diag + 1;
				diag = temp;
			}
			else {
				diag = D[j];
				D[j] = max(D[j], D[j - 1]);
			}
		cout << x[i] << ' ';
		copy(cbegin(D), cend(D), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
	cout << endl;
	return D[m];
}

#ifndef CATCH_CONFIG_MAIN
int main()
{
	std::string x, y;
	std::cin >> x >> y;

	std::cout << LCS_non_recursive(x, y) << std::endl;

	return 0;
}
#endif 


TEST_CASE("LCS non recursive", "[LCS]") {
	REQUIRE(LCS_non_recursive("gac", "agcat") == 2);
	REQUIRE(LCS_non_recursive("PuEHoAbCShj", "JSFmL") == 1);
	REQUIRE(LCS_non_recursive("sWwwELsnAj", "nPAjWLnjAW") == 4);
	REQUIRE(LCS_non_recursive("lWxRRruGGRlRdfrxRrxffuWfdflxxRdrGRlGffWGlldRlRrRlxlxfRGfWuWflfrlRrfGlrWlfruRlWdlfGfdRRdWfrfudrRfuGxl",
		"lGRRRffldRGWxfffGdrffdlfGfRuRlfdGrrudxflfRWxrffRuGdrGxurffRfxlfddGdGlufRrlxlxWuurWulldfuRdxfllRWlxRG") == 46);
}

//TEST_CASE("LCS recursive", "[LCS]") {
//	//std::cout << LCS(x, x.length()-1, y, y.length()-1) << std::endl;
//	REQUIRE(LCS("gac", "agcat") == 2);
//	REQUIRE(LCS("PuEHoAbCShj", "JSFmL") == 1);
//	REQUIRE(LCS("sWwwELsnAj", "nPAjWLnjAW") == 4);
//	//REQUIRE(LCSnonRecursive("lWxRRruGGRlRdfrxRrxffuWfdflxxRdrGRlGffWGlldRlRrRlxlxfRGfWuWflfrlRrfGlrWlfruRlWdlfGfdRRdWfrfudrRfuGxl",
//	//"lGRRRffldRGWxfffGdrffdlfGfRuRlfdGrrudxflfRWxrffRuGdrGxurffRfxlfddGdGlufRrlxlxWuurWulldfuRdxfllRWlxRG") == 46);
//}


TEST_CASE("LCS matrix", "[LCS]") {
	//std::cout << LCS(x, x.length()-1, y, y.length()-1) << std::endl;
	REQUIRE(LCS_matrix("gac", "agcat") == 2);
	REQUIRE(LCS_matrix("PuEHoAbCShj", "JSFmL") == 1);
	REQUIRE(LCS_matrix("sWwwELsnAj", "nPAjWLnjAW") == 4);
	//REQUIRE(LCSnonRecursive("lWxRRruGGRlRdfrxRrxffuWfdflxxRdrGRlGffWGlldRlRrRlxlxfRGfWuWflfrlRrfGlrWlfruRlWdlfGfdRRdWfrfudrRfuGxl",
	//"lGRRRffldRGWxfffGdrffdlfGfRuRlfdGrrudxflfRWxrffRuGdrGxurffRfxlfddGdGlufRrlxlxWuurWulldfuRdxfllRWlxRG") == 46);
}

