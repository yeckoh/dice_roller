#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <iomanip>
#include <Windows.h>

using namespace std;


void printHello(HANDLE&);
void getString(string&);
void readString(string&, int[], HANDLE&);




//Define the class
class Dice
{
private:
	int howMany;
	int sidedDice;
	int modifier;
	int diceResult;

public:
	Dice();
	int GetMany();
	int GetSided();
	int GetModifier();
	int GetdiceResult();
	void SetMany(int);
	void SetSided(int);
	void SetModifier(int);
	void Print();
	int Calculate(HANDLE);
	void Clean(int = 0);
};


//flag setup
bool crit = false;
bool _floor = false;
bool explode = true;
bool dblCrit = false;
bool gwf = false;
bool _verb = false;

int savedResult = 0;


//Define the methods within the class

//Constructor, sets everything to 0
Dice::Dice()
{
	int a = 0;
	howMany = a;
	sidedDice = a;
	modifier = a;
	diceResult = a;
}

int Dice::GetMany()
{
	return howMany;
}
int Dice::GetSided()
{
	return sidedDice;
}
int Dice::GetModifier()
{
	return modifier;
}
int Dice::GetdiceResult()
{
	return diceResult;
}

void Dice::SetMany(int numbah)
{
	howMany = howMany + numbah;
	if (howMany == 0)
		howMany = 1;
	return;
}

void Dice::SetSided(int numbah)
{
	sidedDice = sidedDice + numbah;
	return;
}

void Dice::SetModifier(int numbah)
{
	modifier = modifier + numbah;
	return;
}


void Dice::Print()
{
	cout << endl
		<< "howMany: " << howMany << endl
		<< "sidedDice: " << sidedDice << endl
		<< "modifier: " << modifier << endl;
	return;
}

int Dice::Calculate(HANDLE colorer)
{
	random_device rando;
	mt19937_64 rng(rando());
	int perDice = 0;
	int resultStorer = 0; // for exploding dice
	int diceRoll = 0;		// for moar control
	if (dblCrit && crit) {
		howMany = howMany * 2;
	}
	if (_verb && sidedDice != 20) {
		SetConsoleTextAttribute(colorer, 3);
		cout << ":";
		SetConsoleTextAttribute(colorer, 15);
	}
	for (int i = 1; i <= howMany; i++)
	{
		uniform_int_distribution<int> uni(1, sidedDice);
		resultStorer = diceResult; // for exploding dice
		diceRoll = uni(rng);	// get isolated result for moar control
		diceResult = diceResult + diceRoll;	//manual assignment
		//diceResult = diceResult + uni(rng);
		resultStorer = diceResult - resultStorer; // for exploding dice

		if (sidedDice == 20)
		{
			if (diceRoll == 20)
				SetConsoleTextAttribute(colorer, 10);
			if (diceRoll == 1)
				SetConsoleTextAttribute(colorer, 12);
			cout << "--| " << diceRoll;
			SetConsoleTextAttribute(colorer, 15);
			if (modifier != 0)
			{
				cout << "+" << modifier << " ---> ";
				SetConsoleTextAttribute(colorer, 11);
				cout << diceRoll + modifier;	// << diceResult - perDice + modifier;
				SetConsoleTextAttribute(colorer, 15);
			}
			cout << endl;
			perDice = diceResult;
			continue;
		}
		else if (resultStorer == 1 && _floor)
		{
			SetConsoleTextAttribute(colorer, 4);
			cout << "\nWe Floored it!\n";
			SetConsoleTextAttribute(colorer, 15);
			diceResult++;
		}
		else if (resultStorer <= 2 && gwf)
		{
			SetConsoleTextAttribute(colorer, 4);
			cout << "\nreRolling GWF!\n";
			SetConsoleTextAttribute(colorer, 15);
			diceResult = diceResult - resultStorer + uni(rng);
		}

		if (crit && explode)
		{
			while (resultStorer == sidedDice) //exploding dice routine
			{
				if (_verb)
				{
					SetConsoleTextAttribute(colorer, 3);
					cout << " " << diceRoll;
				}
				SetConsoleTextAttribute(colorer, 4);
				cout << "\nWe Exploded!\n";
				resultStorer = diceResult;
				diceResult = diceResult + uni(rng);
				resultStorer = diceResult - resultStorer;
				if (resultStorer == 1 && _floor)
				{
					cout << "\nWe Floored it!\n";
					diceResult++;
				}
				if (resultStorer <= 2 && gwf)
				{
					cout << "\nreRolling GWF\n";
					diceResult = diceResult - resultStorer + uni(rng);
				}
				SetConsoleTextAttribute(colorer, 15);
			}
		}
		if (_verb) {
			SetConsoleTextAttribute(colorer, 3);
			cout << " " << diceRoll;
			SetConsoleTextAttribute(colorer, 15);
		}
	}
	if (crit && !dblCrit && sidedDice != 20)
		diceResult += howMany*sidedDice;
	diceResult = diceResult + modifier;
	return diceResult;
}

void Dice::Clean(int a)
{
	howMany = a;
	sidedDice = a;
	modifier = a;
	diceResult = a;
}



int main()
{
	//program vars
	string input = "clear";
	int numbahStorer[7] = { -1, -1, -1, -1, -1, -1, -1, };


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// colors???
	// uses: SetConsoleTextAttribute(hConsole, #); <-- # goes from 1 to 255
	//SetConsoleTextAttribute(hConsole, 15);

	printHello(hConsole);
	//system("Color 05");
	while (true)
	{
		getString(input);
		readString(input, numbahStorer, hConsole);
	}
	system("pause");
	return 0;
}



void printHello(HANDLE& textcolorer)
{
	SetConsoleTextAttribute(textcolorer, 9);
	//cout << endl << "Dice Roller 2.171\n"
	cout << endl << "Dice Roller 2.6\n"
		<< "Valid inputs: #d#, #d#+-#, #d#+#d#," << endl
		<< "#/#, #x#, #*#, /2, x2, *2," << endl
		<< "crit, floor, expl, stats, dbls, gwf, verb" << endl << endl
		<< "The max diceGroups per statement is 10" << endl << endl;
	SetConsoleTextAttribute(textcolorer, 15);
	return;
}


void getString(string& inMe)
{
	cout << ":";
	getline(cin,inMe);// >> inMe; // try this instead of using getline(cin,inMe). This doesnt allow spaces.
	return;
}


void printHelp()
{
	system("cls");
	cout << "How dose it work?" << endl
		<< "Enter in dicegroups such as \"5d6 or 5d6+2+1d8+4 or d6\"" << endl
		<< "d6+1+2-3+4+5-500 will also work" << endl
		<< "\nHow do the flags work?" << endl
		<< "Floor: dmg 1 become 2. IE: Elemental Adept" << endl
		<< "Explosions continues rolling crit dice until it is no longer max\n    eg: 1d10 -> 10, 10, 7 = 27 total for 1d10\n"
		<< "Doubles 5d6 = 10d6. off(0): 5d6 + 5*6\n"
		<< "GWF: reroll 1s and 2s once\n"
		<< "Verbose: show all dice numbers\n"
		<< "Last Result stores the last output for dividing or multiplying\n"
		<< "You can also enter binary operations with * or /" << endl;
	return;
}


//now we get to pass 7 addresses, 2many4me
void readString(string& readIn, int numbahStorer[], HANDLE& colorer)
{
	if (readIn == "clear" || readIn == "claer" || readIn == "cls")
	{
		system("cls");
		cout << "We cleared!\n";
		printHello( colorer);
		return;
	}
	if (readIn == "help")
	{
		printHelp();
		printHello(colorer);
		return;
	}
	if (readIn == "nat" || readIn == "crit")
	{
		crit = !crit;
		//system("cls");
		SetConsoleTextAttribute(colorer, 6);
		cout << "\nCrit flag is now: " << crit << endl;
		SetConsoleTextAttribute(colorer, 15);
		//printHello(colorer);
		return;
	}
	if (readIn == "floor")
	{
		_floor = !_floor;
		system("cls");
		cout << "We just set the floor flag!\n";
		printHello(colorer);
		return;
	}
	if (readIn == "explosions" || readIn == "expl")
	{
		explode = !explode;
		system("cls");
		cout << "We just set the explosion flag!\n";
		printHello(colorer);
		return;
	}
	if (readIn == "double" || readIn == "doublecrits" || readIn == "dbls" || readIn == "dblcrits")
	{
		dblCrit = !dblCrit;
		system("cls");
		cout << "We just set the doubleCrit flag!\n";
		printHello(colorer);
		return;
	}
	
	if (readIn == "flags" || readIn == "flagstatus" || readIn == "status" || readIn == "stats")
	{
		system("cls");
		SetConsoleTextAttribute(colorer, 14);
		cout << "Flag settings\n";
		SetConsoleTextAttribute(colorer, 8);
		int setPad = 12;
		cout << right << setw(setPad) << "Critical: " << crit << endl
			<< setw(setPad) << "Floor: " << _floor << endl
			<< setw(setPad) << "Explosions: " << explode << endl
			<< setw(setPad) << "Dbl Crits: " << dblCrit << endl
			<< setw(setPad) << "GWF: " << gwf << endl
			<< setw(setPad) << "Verb: " << _verb << endl
			<< setw(setPad) << "Last #: " << savedResult << left;
		printHello(colorer);
		return;
	}
	
	if (readIn == "gwf" || readIn == "GWF")
	{
		gwf = !gwf;
		system("cls");
		cout << "We just set the GWF flag!\n";
		printHello(colorer);
		return;
	}

	if (readIn == "verb" || readIn == "all")
	{
		_verb = !_verb;
		system("cls");
		cout << "We just set the verbose flag!\n";
		printHello(colorer);
		return;
	}

	/// ENDOF FLAG CHECKING
	/// START CALCULATOR CHECKING
	
	
	if (readIn == "/2" || readIn == "half" || readIn == "div" || readIn == "/")
	{
		SetConsoleTextAttribute(colorer, 3);
		cout << "\n\n" << savedResult << "/2 ";
		savedResult = savedResult / 2;
		SetConsoleTextAttribute(colorer, 13);
		cout << "----->" << savedResult << "\n\n";
		SetConsoleTextAttribute(colorer, 15);
		return;
	}
	if (readIn == "*2" || readIn == "x2")
	{
		SetConsoleTextAttribute(colorer, 3);
		cout << "\n\n" << savedResult << "x2";
		savedResult = savedResult * 2;
		SetConsoleTextAttribute(colorer, 13);
		cout << "----->" << savedResult << "\n\n";
		SetConsoleTextAttribute(colorer, 15);
		return;
	}
	
	
	///START DICE ROLLING
	//basic input verification
	//if input ends in a d it dies
	//also i dont feel like checking for 'D' so kys
	bool quti = true;
	bool calculator = false;
	int iForCalc = 0;
	for (int i = 0; i < readIn.length(); i++)
	{
		switch (readIn[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '+':
		case '-': break;
		case 'D':
		case 'd': if (i == readIn.length() - 1) break; quti = false; break;
		case '/':
		case 'x':
		case '*': quti = false; calculator = true; iForCalc = i; break;
		default: quti = true;
		}
	}
	
	if (quti)
	{
		SetConsoleTextAttribute(colorer, 14);
		cout << "\nTry again\n";
		SetConsoleTextAttribute(colorer, 15);
		return;
	}
	
	if (calculator)
	{
		int op1 = 0;
		int op2 = 0;
		int tensPlace = 0;
		// left
		for (int i = iForCalc - 1; i >= 0;i--) {
			op1 = op1 + (readIn[i] - 48) * pow(10, tensPlace);
			tensPlace++;
		}
		// right
		tensPlace = 0;
		for (int i = readIn.length() - 1; i >iForCalc; i--) {
			op2 = op2 + (readIn[i] - 48) * pow(10, tensPlace);
			tensPlace++;
		}
		// div
		if (readIn[iForCalc] == '/') {
			if (op2 == 0)
				op2 = 1;
			savedResult = op1 / op2;
			SetConsoleTextAttribute(colorer, 3);
			cout << "\n\n" << op1 << "/" << op2;
			SetConsoleTextAttribute(colorer, 13);
			cout << "----->" << savedResult << "\n\n";
			SetConsoleTextAttribute(colorer, 15);
			return;
		}
		// mult
		else {
			savedResult = op1 * op2;
			SetConsoleTextAttribute(colorer, 3);
			cout << "\n\n" << op1 << "*" << op2;
			SetConsoleTextAttribute(colorer, 13);
			cout << "----->" << savedResult << "\n\n";
			SetConsoleTextAttribute(colorer, 15);
			return;
		}
	}
	
	int indexCounter = 0;
	int indexSaver = 0;
	int inputLength = 0; //from 1 to whatever, actual index starts at 0;
						//Dice d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	Dice diceGroup[10]; //use an array so we can count through the index instead of using specific names
	int whichDice = 0; // we'll need to know which dice it is then

	bool areWeDone = false;
	inputLength = readIn.length(); //might as well get the length of the actual input by now


	static float base = 10;

	//reset the temporary space
	for (int i = 0; i < 7; i++)
		numbahStorer[i] = -1;

	//if i want to check the validity of input, do it here.
	while (!areWeDone)
	{
		int realValue = 0; // the actual number in the string index after figuing out what tens place its in
		float exponentiometer = 0; //exponentialmonitor, outside the loops so we dont reset each read
		//readingRainbow = readingRainbow.c_str();

		//begin reading string, this is to get howMany
		//we're storing it into an array backwards
		while (readIn[indexCounter] != 'd' && readIn[indexCounter] != 'D') //until we get a D/////////////////////////////////////////////////////////////////////////
		{
			numbahStorer[6 - realValue] = readIn[indexCounter];
			indexCounter++;
			realValue++;
		}

		realValue = 0;
		for (int i = 0; i < 7; i++)
		{
			if (numbahStorer[i] != -1)
			{
				realValue = realValue + (numbahStorer[i] - 48) * int(pow(base, exponentiometer)); //calc actual value accounting for tens place
				exponentiometer++;
			}
		}
		diceGroup[whichDice].SetMany(realValue);


		indexCounter++; //we've reached a 'd' or 'D', so we need to skip that and go to the next thing in the index


		//reset the temporary space
		for (int i = 0; i < 7; i++)
			numbahStorer[i] = -1;



		//continue reading, this is for sidedDice
		exponentiometer = 0; //need this again, same reason, it still exists so just reset it
		int temp4Sided = 0;
		while (readIn[indexCounter] != '+' && readIn[indexCounter] != '-')//until we get to a + or a -//////////////////////////////////////////////////////////////////
		{
			numbahStorer[6 - temp4Sided] = readIn[indexCounter];
			temp4Sided++;
			if (indexCounter == (inputLength - 1)) //check if it was only one sided dice and theres also nothing left eg: 24d3 or 3d45
			{
				areWeDone = true;
				break;
			}
			indexCounter++;
		}

		realValue = 0;
		for (int i = 0; i < 7; i++)
		{
			if (numbahStorer[i] != -1)
			{
				realValue = realValue + (numbahStorer[i] - 48) * int(pow(base, exponentiometer)); //calc actual value accounting for tens place
				exponentiometer++;
			}
		}
		diceGroup[whichDice].SetSided(realValue);
		if (areWeDone)
			continue;
		indexCounter++; //otherwise we've reached a + or -, so skip it go to the next one

		//reset the temporary space
		for (int i = 0; i < 7; i++)
			numbahStorer[i] = -1;

		//possible inputs to get this far: 5d4+3..., 5d4+3+2..., 5d4+3d2...

		//begin looking ahead for the next procedure
		indexSaver = indexCounter; // bookmark so we can read the rest
		temp4Sided = 0;


		while (readIn[indexCounter] <= '9' && readIn[indexCounter] >= '0' && indexCounter < inputLength) //keep counting up until we get to something that isnt a number
			indexCounter++;
		while (indexCounter == inputLength || readIn[indexCounter] == '+' || readIn[indexCounter] == '-') //that means its just 1 modifier
		{
			indexCounter = indexSaver; //we reached one of the 3 above so go back to the bookmark
			temp4Sided = 0;
			while (indexCounter < inputLength && readIn[indexCounter] != '+' && readIn[indexCounter] != '-')	//if its the end, we got 4d3+2... or 4d3+21...		does not account for the second pass? unless we update indexSAver
			{
				numbahStorer[6 - temp4Sided] = readIn[indexCounter];
				temp4Sided++;
				indexCounter++;
			}
			//reset valuyes
			realValue = 0;
			exponentiometer = 0;

			for (int i = 0; i < 7; i++)
			{
				if (numbahStorer[i] != -1)
				{
					realValue = realValue + (numbahStorer[i] - 48) * int(pow(base, exponentiometer)); //calc actual value accounting for tens place
					exponentiometer++;
				}
			}

			if (readIn[indexSaver - 1] == '-')
				realValue = -realValue;
			diceGroup[whichDice].SetModifier(realValue);

			//reset the temporary space
			for (int i = 0; i < 7; i++)
				numbahStorer[i] = -1;

			if (indexCounter == inputLength)
				break;
			indexCounter++;
			indexSaver = indexCounter;
			while (readIn[indexCounter] <= '9' && readIn[indexCounter] >= '0' && indexCounter < inputLength) //keep counting up until we get to something that isnt a number
				indexCounter++;
		}
		if (indexCounter == inputLength)
			break;

		if (readIn[indexCounter] == 'd' || readIn[indexCounter] == 'D')
		{
			indexCounter = indexSaver;
			whichDice++; //if we found a D, we have no more modifiers left but theres another dice roll, so move onto the next object
		}
	}
	int completeTotal = 0;
	int perDoyce = 0;

	for (int i = 0; i <= whichDice; i++)
	{
		completeTotal = completeTotal + diceGroup[i].Calculate(colorer);

		if (whichDice != 0 && diceGroup[i].GetSided() != 20)
			cout << "\n--| " << completeTotal - perDoyce << endl;
		perDoyce = completeTotal;
	}
	SetConsoleTextAttribute(colorer, 13);
	cout << "\n\n" << "----->" << completeTotal << "\n\n";
	SetConsoleTextAttribute(colorer, 15);
	
	savedResult = completeTotal;

	for (int i = 0; i <= 9; i++)
	{
		//diceGroup[i].Print();
		diceGroup[i].Clean();
	}
	crit = false;
}



/*
Predicted procedure ovewview
-1. (Initialize) Print Hello's
Print Request for an input
Valid inputs: #d#	||	#d#+-#	||	clear
0. (Prep) MAYBE: If it has spaces, get rid of them
1. (Get) Determine if its a request for a dice roll, clear, or none of the above, ?and whether or not its an actual valid input?
2. (Set) Parse the actual dice roll, and spit out specifics to a class object "dice"
3. (Calculate) Roll the dice and store the results
4. (Print)	4.a Were there multiple dice? State both, the interpreted operand(probably a +) and calculate the entire result
5. (Clean) clear the objects to free memury, reset any flags
6. (Return) loop back for another round



Variables not applicable to the dice objects needed:
inputString <- store in main so Get and Set have access to it? Otherwise throw Set into Get
two index counters (1 for reading ahead for another character)
inputLength
objectCount
multiDiceOperand[howManySeparateDiceWillBeTheMaxAllowed] <- bool type? 0 = - and 1 = + ?     <- Just place a hard limit of 10 objects
isNat


The Dice Object will need:
(howMany)		How many are there
(sidedDice)		What is the dice sided
(modifier)		What is the actual modifier total if any, keep it signed
(diceResult)	What is the total result of the dice by themselves
(nextObject)	MIGHT need to create a stack



Planned future functionalities:
DONE. Expand input to accept multiple dice types at once: 1d10+4+2d6, MAKE SURE TO SEPERATE THE RESULTS UNITL THE END FOR DIFFERENTIATING DAMAGE TYPES [can be done in print function]
Add a statistics mode?
(could use the multidice function for this through print ->) Roll the same input x number of times and supply both results as seperate, eg: 1d20 with advantages
Set a NAT flag to automatically add a MAX value for the next roll in print?







(Get)
Receive input
is it 'clear'? run that function (Clear) and continue for another input
[MAYBE] is it 'NAT'? set a flag for (Print) and continue for another input
does it start with numbers, have a 'd' somewhere, and end with a number? run (Set)
if its none of these, tell them I didnt get it, and continue for another input





(Set)
The hard part: How eaxctly do I parse the input and assign values to my object/s
Given its a dice roll with the correct format...
1. Start reading.
For each character,
a. Get the length
b. run through and check how many D's
depreciated by having 10 objects always				c. create that many objects (may need to use a stack), set objectCount as well

d. until it is a 'd' || 'D'
e. start converting from ASCII to a number and assign it to 'howMany' through a loop.
f. Each next character increases the value by 10, reuse the expoential thing from previous program.
g. continue to the next character index

h. until it is a + || -
i. assign it to 'sidedDice', gonna need f. again.
j. if this is also the end of the index, dont go over one and break instead. not be4 assignment because it will always be at least one valid number
k. continue to the next index

l. if it was the end of the index, break because we got everything we need. eg: 4d3
m. (otherwise it must be a + || -, so) bookmark this index and send the + or - to multiDiceOperand[0] and look head for another +- or d or end of the index. [maybe use a case]
n. if its a 'd', go to a. because this first object has no modifiers. eg: 4d3+5d6
o. if we get to the end of the index, then its just a single modifier. eg: 4d3+3 or 4d3+45
p. start adding to the 'modifier' until we get to the end, then break out of everything.
q. go ahead and start adding to the 'modifier' until we get to it. because if its neither then it was a + or -, then it must have at least 2 modifiers or 1 modifier & another dice. eg: 4d3+3+3 or 5d4+3+2+1 or 4d3+2+5d6...
r. when we do, loop back to l to check again for the next +-end.



(Calculate)
for how many objects did we have again? (objectCount)
for howMany there are
run the uni distributor with a range of (1,sidedDice)
each time adding it to the 'diceResult'
for++
//we have everything we need to print



(Print)
do
print the object's (diceResult + modifier)									<---- for showing the equation eg: 1d20+1d20 -> 15 + 7 = 22
if theres another, check the multiDiceOperand[], print the actual 'char'	<----
while(we havent reached the objectCount)

<< '='
check for NAT flag
print << the object's diceResullt + modifier +- second diceResult + modifer...
*/