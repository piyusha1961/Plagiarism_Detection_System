
#include <iostream>
#include <limits>
#include "plagiarism.h"

using namespace std;

// ============================================================
// DISPLAY MENU
// ============================================================
void displayMenu()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "           PLAGIARISM DETECTION SYSTEM\n";
    cout << "====================================================\n";
    cout << "\n";
    cout << "1. Compare two text files\n";
    cout << "2. Enter text manually\n";
    cout << "3. View algorithm information\n";
    cout << "4. Exit\n";
    cout << "\n";
    cout << "Enter your choice: ";
}


// ============================================================
// OPTION 1: COMPARE TWO FILES
// ============================================================
void compareFiles()
{
    string file1, file2;

    cout << "\n";
    cout << "====================================================\n";
    cout << "              FILE COMPARISON\n";
    cout << "====================================================\n";

    cout << "\nEnter first document filename: ";
    cin >> file1;

    cout << "Enter second document filename: ";
    cin >> file2;

    // Read both files
    string text1 = readFile(file1);
    string text2 = readFile(file2);

    // Check if files were successfully read
    if (text1.empty() || text2.empty())
    {
        cout << "\nUnable to process documents.\n";
        cout << "Please check that both files exist and contain text.\n";
        return;
    }

    // Perform complete plagiarism analysis
    analyzeDocuments(text1, text2);
}


// ============================================================
// OPTION 2: ENTER TEXT MANUALLY
// ============================================================
void compareManualText()
{
    string text1;
    string text2;
    string line;

    cout << "\n";
    cout << "====================================================\n";
    cout << "               MANUAL TEXT INPUT\n";
    cout << "====================================================\n";

    // Clear the newline left by cin >> choice
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // ---------------- DOCUMENT 1 ----------------
    cout << "\nEnter Document 1.\n";
    cout << "Type END on a new line when you are finished.\n\n";

    while (true)
    {
        getline(cin, line);

        if (line == "END")
        {
            break;
        }

        text1 += line;
        text1 += "\n";
    }

    // ---------------- DOCUMENT 2 ----------------
    cout << "\nEnter Document 2.\n";
    cout << "Type END on a new line when you are finished.\n\n";

    while (true)
    {
        getline(cin, line);

        if (line == "END")
        {
            break;
        }

        text2 += line;
        text2 += "\n";
    }

    // Check whether both documents contain text
    if (text1.empty() || text2.empty())
    {
        cout << "\nBoth documents must contain text.\n";
        return;
    }

    // Perform complete plagiarism analysis
    analyzeDocuments(text1, text2);
}


// ============================================================
// MAIN FUNCTION
// ============================================================
int main()
{
    int choice;

    do
    {
        displayMenu();

        // Check for valid numeric input
        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Please enter a number.\n";

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            continue;
        }

        switch (choice)
        {
            // --------------------------------------------
            // OPTION 1
            // --------------------------------------------
            case 1:
                compareFiles();
                break;

            // --------------------------------------------
            // OPTION 2
            // --------------------------------------------
            case 2:
                compareManualText();
                break;

            // --------------------------------------------
            // OPTION 3
            // --------------------------------------------
            case 3:
                displayAlgorithmInformation();
                break;

            // --------------------------------------------
            // OPTION 4
            // --------------------------------------------
            case 4:
                cout << "\n";
                cout << "Thank you for using "
                        "Plagiarism Detection System!\n";
                cout << "Goodbye!\n";
                break;

            // --------------------------------------------
            // INVALID OPTION
            // --------------------------------------------
            default:
                cout << "\nInvalid choice.\n";
                cout << "Please select a number between 1 and 4.\n";
        }

    } while (choice != 4);

    return 0;
}
