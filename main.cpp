// main.cpp : Defines the entry point for the console application.
/*
written and tested on vs_2017, win7x64
may need stdafx depending on project settings

	C++ code that follows some good conventions/practices
	except separating classes/funcs into other files.
	upwards of ~2.7x faster than v2.6
	also utilizes STL

remove console color calls and windows.h to compile on linux
tested on eclipse CDT, ubuntu 18.04LTS
*/

#define MEASURE_TIME

///TODO:
// error handling
// lower coupling
// maybe dont slap inline everywhere


//#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
//#ifdef MEASURE_TIME
//#include <chrono>
#include <windows.h>
//#include <cstdlib>
//#endif

using namespace std;

// lol globals
random_device rando;
mt19937 gen(rando());

vector<char> v_ops;

int total;
int group_total;
int last_total;
bool times_negative1 = false;
bool twenty_side = false;

bool F_DOUBLES = false;
bool F_EXPLODES = true;
//bool F_VERB = 0x4;

string last_statement = "a20+7";
const string DELIMITERS = "+-*/";
const string OPERATION_TYPES = "acdvtACDVT";

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// console color alias
inline const auto ConColor(int c) {
	SetConsoleTextAttribute(hConsole, c);
}

enum OPERATION {
	ADV = 0, DISADV, NORMAL, CRIT, TRIPLE, NOOP
};

enum COLOR {
	GRAY = 8, DARKBLUE = 9, GREEN = 10, CYAN = 11, RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
};


void print();
void printStats();


inline bool isDelim(char &c) {
	return DELIMITERS.find(c) != string::npos;
}


inline bool isDice(string &c) {
	return OPERATION_TYPES.find_first_of(c) != string::npos;
}



// generic base class for vector
class unit {
public:
	unit(bool tf) : d(tf) {};
	virtual string show() { return ""; };
	virtual int calc() { return -9999; };
	bool d;
};



class modifier : public unit {
public:
	modifier(int i) : number(i), unit(false) {};

	string show() override {
		return "mod_t";
	}

	int calc() override {
		return number;
	}

private:
	int number;
};


class dice : public unit {
public:
	dice(string &input) : count(1), unit(true) {

		size_t found = input.find_first_of(OPERATION_TYPES);

		char c = input[found];
		switch (c) {
			// Advantage
		case 'a':
		case 'A':
			op = ADV; break;
			// nothing
		case 'd':
		case 'D':
			op = NORMAL; break;
			// disadVantage
		case 'v':
		case 'V':
			op = DISADV; break;
			// crit
		case 'c':
		case 'C':
			op = CRIT; break;
			//triple advantage, super speshiul case
		case 't':
		case 'T':
			op = TRIPLE; break;
		default: op = NORMAL;
		}

		if (input[0] != c)
			count = stoi(input.substr(0, found));
		sides = stoi(input.substr(found + 1));
	}

	inline int calc() override {
		/// we always return t1
		uniform_int_distribution<int> rnd(1, sides);
		int t1 = 0, t2 = 0, t3 = 0;

		switch (op) {
		case ADV:
			for (int i = 0; i < count; ++i) {
				t1 += rnd(gen);
				rnd.reset();
				t2 += rnd(gen);
				rnd.reset();
			}

			if (t1 > t2)
				break;
			t1 = t2;
			break;

		case NORMAL:
			for (int i = 0; i < count; ++i) {
				t1 += rnd(gen);
				rnd.reset();
			}
			break;

		case DISADV:
			for (int i = 0; i < count; ++i) {
				t1 += rnd(gen);
				rnd.reset();
				t2 += rnd(gen);
				rnd.reset();
			}


			if (t1 < t2)
				break;
			t1 = t2;
			break;

		case CRIT:
			if(F_DOUBLES) // double dice rolling
				count *= 2;
			else t1 = count*sides; // add max value
			if (F_EXPLODES) {
				if (sides == 1)
					return -1;
				int value(0);
				for (int i = 0; i < count; ++i) {
					value = rnd(gen);
					if (value == sides)
						--i;
					t1 += value;
					rnd.reset();
				}
			} // else no explosions
			else {
				for (int i = 0; i < count; ++i) {
					t1 += rnd(gen);
					rnd.reset();
				}
			}
			break;

		case TRIPLE:
			t1 = 0;
			t2 = 0;
			t3 = 0;
			for (int i = 0; i < count; ++i) {
				t1 += rnd(gen);
				rnd.reset();
				t2 += rnd(gen);
				rnd.reset();
				t3 += rnd(gen);
				rnd.reset();
			}

			if (t1 > t2 && t1 > t3)
				break;
			if (t2 > t1 && t2 > t3) {
				t1 = t2;
				break;
			}
			t1 = t3;
		}

		// returning t1, is it a 20?
		if (sides == 20) {
			twenty_side = true;
			if (t1 == 1)
				ConColor(RED);
			else if (t1 == 20)
				ConColor(GREEN);
			cout << "--> " << t1;
			ConColor(WHITE);
		}
		return t1;
	}

	string show() override {
		string str = "sides, count: ";
		str += to_string(count);
		str += ", ";
		str += to_string(sides);
		return str;
	}

private:
	int count;
	int sides;
	OPERATION op;

};





int main()
{
	//SetConsoleTextAttribute(hConsole, RED);
	//string input = "1d6-1d2+76";
	string input = "d8+4*5d6";

	string::iterator isi;
	vector<unique_ptr<unit>> v_units;
	string atom = "";

	print();
	while (true) {
		v_units.clear();
		v_ops.clear();
		atom.clear();
		times_negative1 = false;

		cout << "\n:";
		cin >> input;
		if (input == "clear") {
			system("cls");
			print();
			continue;
		}
		cout << endl;
		if (input == "stats") {
			printStats();
			continue;
		}
		else if (input == "doubles") {
			F_DOUBLES = !F_DOUBLES;
			continue;
		}
		else if (input == "explodes") {
			F_EXPLODES = !F_EXPLODES;
			continue;
		}
		else if (input == "last") {
			ConColor(DARKBLUE);
			cout << ":" << last_statement << endl;
			ConColor(WHITE);
			input = last_statement;
		}
		last_statement = input;

		isi = input.begin();

		v_ops.push_back('+'); // first

		while (isi != input.end()) {
			if (isDelim(*isi)) {
				v_ops.push_back(*isi);

				if (isDice(atom))
					v_units.push_back(unique_ptr<dice>(new dice(atom)));
				else
					v_units.push_back(unique_ptr<modifier>(new modifier(stoi(atom))));

				atom.clear();
			}

			else
				atom += *isi;
			++isi;
		}

		// put the last one on the vector
		if (isDice(atom))
			v_units.push_back(unique_ptr<dice>(new dice(atom)));
		else
			v_units.push_back(unique_ptr<modifier>(new modifier(stoi(atom))));



		total = 0;
		group_total = 0;

		int value = 0;
		int d_count = 0;
		int twenty_mod = 0;


//#ifdef MEASURE_TIME
//auto start = chrono::steady_clock::now();
//#endif

		vector<char>::iterator itr = v_ops.begin();

		for (int i = 0; i < v_units.size(); ++i) {

			if (v_units[i]->d) {
				++d_count;

				if (times_negative1)
					group_total *= -1;

				times_negative1 = false;

				if (d_count > 1) {
					if (twenty_side) {
						if (twenty_mod > 0)
							cout << '+' << twenty_mod;
						else if (twenty_mod < 0)
							cout << twenty_mod;
						ConColor(CYAN);
						cout << " ---> " << group_total << "\n\n";
						ConColor(WHITE);
					}
					else
						cout << "--| " << group_total << "\n\n";
					switch (*itr) {
					//case '*': value = v_units[i]->calc(); total += value * group_total; md_group = true; break;
					//case '/': if (!group_total) group_total = 1; total /= group_total; break;
					case '-': times_negative1 = true;
					case '+':
					default: total += group_total; break;
					} // end.switch
				}
				twenty_side = false;
				group_total = v_units[i]->calc();

				++itr;
			} //end.isdice
			
			else {

				value = v_units[i]->calc();
				switch (*itr) {
				case '*': cout << "--> " << group_total; group_total *= value; cout << '\n'; break;
				case '/': if (!value) value = 1; cout << "--> " << group_total; group_total /= value; cout << '\n'; break;
				case '-': value *= -1;
				case '+': if (twenty_side) twenty_mod += value;
				default: group_total += value; break;
				} //end.switch

				++itr;

			} //end.isnotdice
		}
//--------------------------------------------------------------------------------


		if (d_count <= 1) {
			if (twenty_side) {
				if (twenty_mod > 0)
					cout << '+' << twenty_mod;
				else if (twenty_mod < 0)
					cout << twenty_mod;
				ConColor(CYAN);
				cout << " ---> " << group_total << '\n';
				ConColor(WHITE);
			}
			total = group_total;
		}
		else {
			if (times_negative1)
				group_total *= -1;
			total += group_total;
			if (twenty_side) {
				if (twenty_mod > 0)
					cout << '+' << twenty_mod;
				else if (twenty_mod < 0)
					cout << twenty_mod;
				ConColor(CYAN);
				cout << " ---> " << group_total << "\n\n";
				ConColor(WHITE);
			}
			else
				cout << "--| " << group_total << "\n\n";
		}
		

		if (!twenty_side) {
			ConColor(MAGENTA);
			cout << "\n-----> " << total << endl;
			ConColor(WHITE);
		}
		last_total = total;
		cout.flush();

//		#ifdef MEASURE_TIME
		//auto end = chrono::steady_clock::now();
		//cout << "Elapsed time = " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " [microsec]" << endl;
//		#endif
	}



	return 0;
}




void print() {
	ConColor(DARKBLUE);
	cout << "Dice roller v3.0\nNow with STL!\nA is for Advantage\nD is for dice\nV is for DisadVantage\nC is for Crit\n\n";
	ConColor(WHITE);
}

void printStats() {
	ConColor(GRAY);
	cout << "\nF_DOUBLES:\t" << (F_DOUBLES) << '\n'
	<< "F_EXPLODES:\t" << (F_EXPLODES) << '\n'
	<< "last statement:\t" << last_statement << '\n'
	<< "last result:\t" << last_total << endl << endl;
	ConColor(WHITE);
}



/*

break string into units

once broken down, units can either be: dice unit or modifier unit, classify them and push onto a vector

once classified, run sequentially to the end

*/



/*

recursive descent for regex

nah, cool but unnecessary

*/

