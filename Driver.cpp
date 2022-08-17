#include <Python.h>
#include <iostream>
#include <iomanip>
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include <cmath>
#include <string>
#include <fstream>

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

void displayMenu() {
	//Local variables used to run menu
	int i;
	int userChoice = 0;
	int wordCount = 0;
	int itemQuantity = 0;
	string userSearchTerm;
	string itemName;
	ifstream fileInput;

	while (userChoice != 4) {
		//Outputs menu choices
		cout << "1: Calculate the number of times each item was purchased" << endl;
		cout << "2: Calculate the number of times a specific item was purchased" << endl;
		cout << "3: Create a histogram based on the number of purchases of each item" << endl;
		cout << "4: Exit program" << endl;
		cin >> userChoice;
		//Validates user input
		while (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Please enter a valid selection (1-4): ";
			cin >> userChoice;
		}
		switch (userChoice) { //Switch to direct user choices
		case 1: //Counts number of purchases for all items
			CallProcedure("CountAll");
			cout << endl;
			break;
		case 2: //Counts number of purchases for a specific item
			cout << "What item would you like to search for?" << endl;
			cin >> userSearchTerm;
			wordCount = callIntFunc("CountInstances", userSearchTerm);
			cout << endl;
			cout << userSearchTerm << " : " << wordCount << " purchases" << endl;
			break;
		case 3: //Outputs histogram for number of purchases for each item
			CallProcedure("CollectData");
			fileInput.open("frequency.dat");
			fileInput >> itemName;
			fileInput >> itemQuantity;
			while (!fileInput.fail()) {
				cout << itemName << " ";
				for (i = 0; i < itemQuantity; ++i) {
					cout << "*" << endl;
				}
				cout << endl;
				fileInput >> itemName;
				fileInput >> itemQuantity;
			}
			fileInput.close();
			cout << endl;
			break;
		case 4: //Exits program
			return;
		default: //Detects invalid input
			cout << "Please input a valid selection (1-4)." << endl;
			break;


		}
	}

}


void main()
{
	displayMenu(); // Calls menu

}