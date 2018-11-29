//#include <iostream>
#include "api.h"
using namespace std;

string a0 = "exit";
string a1 = "mkdir";
string a2 = "touch";
string a3 = "ls";
string a4 = "rm";
string a5 = "stat";
string a6 = "mv";
string a7 = "cd";
string a8 = "pwd";
string a100 = " ";


int runAPI(string input)
{

	if ( !input.compare(0, 3, a0, 0, 3)) return 0;
	else if ( !input.compare(0, 4, a1, 0, 4)) return 1;
	else if ( !input.compare(0, 4, a2, 0, 4)) return 2;
	else if ( !input.compare(0, 1, a3, 0, 1)) return 3;
	else if ( !input.compare(0, 1, a4, 0, 1)) return 4;
	else if ( !input.compare(0, 3, a5, 0, 3)) return 5;
	else if ( !input.compare(0, 1, a6, 0, 1)) return 6;
	else if ( !input.compare(0, 1, a7, 0, 1)) return 7;
	else if ( !input.compare(0, 2, a8, 0, 2)) return 8;
	else if ( !input.compare(0, 0, a100, 0, 0)) return 100;
	else 
	{
		cout << "Can not found the command \"" << input << "\"" << endl;
		return 10;
	}
}

int addFile(string command1, string command2)
{
	if (!command1.compare(0, 4, "-size", 0, 4)) return 0;
}