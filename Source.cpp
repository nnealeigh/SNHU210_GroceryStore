// Source.cpp : 
/*
	Nick Nealeigh
	CS 210 - T4445: Programming Languages
	16 APR 2022
	7-3 Assignment: Project 3

	Code description:
	Integrates c++ and python for a grocery store database
	calls functions from python to handle data and writing to files
	c++ manages output and controls the main menu loop
	outputs quantities of all items in grocery store list
	outputs specific quantity of an item in the list
	generates a .dat file based on frequencys
	generates a histogram based on .dat file
	has input validation for menu loop
*/
#include <Python.h>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>

// Colors provided by shuttle87 https://stackoverflow.com/questions/9158150/colored-output-in-c
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

//Simple menu display using colors
void displayMenu() {
	cout << MAGENTA;
	cout << "1: Outputting All Produce Quantities" << endl;
	cout << "2: Output Quantity of Purchases For Specific Item" << endl;
	cout << "3: Generate Histogram" << endl;
	cout << "4: Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3 or 4." << endl;
	cout << RESET;
}

//calls python function, outputs formatted line with item and quantity
void printSingleOccurance(string produce) {
	int count = callIntFunc("CountSingleOccurance", produce);

	cout << produce << ' ' << count << endl;
}

/* reads frequency.dat
*  colors and formats print statement for histogram
*  aligns output to ensure histogram looks appropriate
*/
void printHistogram() {
	string produce;
	int count;
	ifstream readFile("frequency.dat");
	if (!readFile) {
		cout << "Could not open file frequency.dat." << endl;
	}
	while (readFile >> produce >> count) {
		cout << BOLDWHITE << left << setw(12) <<  produce << '|';
		for (int i = 0; i < count; i++) {
			cout << BOLDCYAN << "*";
		}
		cout << RESET << endl;
	}
	if (!readFile.eof()) {
		cout << "Input failure before reaching end of file." << endl;
	}
	readFile.close();
}

void main()
{
	//variable declaration/initialization
	int input;
	string produce;
	bool quitMenu = true;

	//main menu loop
	while (quitMenu) {
		displayMenu();

		/*
		Input validation loop to ensure type
			credit: chemistpp
			https://stackoverflow.com/questions/18728754/checking-cin-input-stream-produces-an-integer
		*/
		cin >> input;
		while (cin.fail()) {
			cout << "Enter your selection as a number 1, 2, 3, or 4. \n" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> input;
		}

		// Switch case menu selection with default for invalid entries
		switch (input)
		{
		case 1:
			cout << BOLDYELLOW << "\nOutputting All Produce Quantities:" << YELLOW << endl;
			CallProcedure("CountAllOccurance");
			cout << RESET << endl;
			break;

		case 2:
			cout << GREEN << "Outputting Quantity of Purchases For Specific Item\nEnter an Item: " ;
			cin >> produce;
			printSingleOccurance(produce);
			cout << RESET << endl;
			break;

		case 3:
			cout << "\nGenerating Histogram\n";
			CallProcedure("WriteOccurance");
			printHistogram();
			cout << endl;
			break;

		case 4:
			cout << RED << "Exiting...\n" << RESET;
			quitMenu = false;
			break;

		default:
			cout << "\n** Invalid Menu Selection!! **\n\n";
			cin.clear();
			break;
		}
	}
}