#define _CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<fstream>
#include	<cstdlib>
#include	<cstdio>
#include	<ctime>
#include    <string>
#include	"BST.h"
#include    "Student.h"

using namespace std;

int menu();
bool readFile(const char*, BST*);

int main() {
    menu();
    cout << "\n\t\tEXIT FROM THE SYSTEM . . .\n\n\t\t";
    system("pause");
    return 0;
}

int menu() {
    BST tree;
    BST subTree;
    Student temp;
    int option;
    bool con = true;

    do {
        cout << "\n\nMenu\n";
        cout << "\n1. Read data to BST\n";
        cout << "2. Print deepest nodes\n";
        cout << "3. Display student\n";
        cout << "4. Clone Subtree\n";
        cout << "5. Print Level Nodes\n";
        cout << "6. Print Path\n";
        cout << "7. Exit\n";
        cout << "Enter your option: ";
        cin >> option;
        cin.clear();
        cin.ignore(1000, '\n');
        switch (option) {
        case 1:
            cout << "\nREAD DATA TO BST\n\n";
            if (!readFile("student.txt", &tree)) { //condition where file not read/found
                cout << "Could not read file.\n";
            }
            break;
        case 2:
            cout << "\nPRINT DEEPEST NODES\n\n";
            if (!tree.deepestNodes()) { //empty tree
                cout << "Empty tree!\n";
            }
            break;
        case 3:
            int op1;
            int op2;
            cout << "\nDISPLAY STUDENT\n\n";
            cout << "Choose the order of ID for printing student's information (1 - Ascending / 2 - Descending): ";
            cin >> op1; //option for order
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nWhere do you want to display the output (1 - Screen / 2 - File): ";
            cin >> op2; //option for screen or file for output displaying
            cin.clear();
            cin.ignore(1000, '\n');
            if (tree.display(op1, op2))
            {
                if (op2 == 2) {
                    cout << "\nSuccessfully display output into file!\n";
                }
                else if (op2 == 1) {
                    cout << "\nSuccessfully display output on screen!\n";
                }
            }
            break;
        case 4:
            cout << "\nCLONE SUBTREE\n\n";
            cout << "Please enter a student ID as the root of subtree to be cloned: ";
            cin >> temp.id;
            cin.clear();
            cin.ignore(1000, '\n');
            subTree.clear(); // Clear the existing subTree
            if (!subTree.CloneSubtree(tree, temp)) { //tree is empty, ID not found, cloned tree still empty
                cout << "\nCannot clone subtree.\n";
            }
            break;
        case 5:
            cout << "\nPRINT LEVEL NODES\n\n";
            if (!tree.printLevelNodes()) { //empty tree
                cout << "Empty tree!\n";
            }
            break;
        case 6:
            cout << "\nPRINT PATH\n\n";
            if (!tree.printPath()) //empty tree
            {
                cout << "Empty tree!\n";
            }
            break;
        case 7:
            con = false; //exit
            break;
        default: //invalid input in choosing option
            cout << "Invalid option. Please try again.\n";
            break;
        }
    } while (con);

    return 0;
}

bool readFile(const char* fileName, BST* t1) {


    ifstream inFile;
    inFile.open(fileName);
    if (!inFile)
    {
        return false; // file not found
    }

    Student student;
    int noRecord = 0;
    string line;

    while (getline(inFile, line)) {
        if (line.find("Student Id = ") != string::npos) { //the condition return true if string inside " " is found
            char tempId[11];
            strncpy_s(tempId, line.substr(13).c_str(), 10);
            student.id = stoi(tempId); //stoi convert char array (aka string) into int type
        }//extracts substrings starting from the position specified (13) n c_str() return a *char array with at most 10 character
        else if (line.find("Name = ") != string::npos) {
            strncpy_s(student.name, line.substr(7).c_str(), 30);
        }
        else if (line.find("Address = ") != string::npos) {
            strncpy_s(student.address, line.substr(10).c_str(), 100);
        }
        else if (line.find("DOB = ") != string::npos) {
            strncpy_s(student.DOB, line.substr(6).c_str(), 20);
        }
        else if (line.find("Phone Number = ") != string::npos) {
            strncpy_s(student.phone_no, line.substr(15).c_str(), 10);

        }
        else if (line.find("Course = ") != string::npos) {
            strncpy_s(student.course, line.substr(9).c_str(), 3);
        }
        else if (line.find("CGPA = ") != string::npos) {
            char cgpa[9];
            strncpy_s(cgpa, line.substr(7).c_str(), 9);
            student.cgpa = stod(cgpa); // stod convert char array(aka string) into double type
            if (t1->findNode(t1->root, student)) {
                cout << "Duplicate student ID " << student.id << " with name " << student.name << " found and ignored.\n";
            }		// Check for duplicate before inserting
            else {
                t1->insert(student);
                noRecord++;
            }
        }
    }
    inFile.close();
    if (noRecord == 0) //file is empty or read the same file twice (duplicate record cause no new record is read)
    {
        cout << "\nNo new record has been successfully read.\n";
        return true;
    }
    cout << noRecord << " records have been successfully read.\n"; //display no of record has been read
    return true;
}

