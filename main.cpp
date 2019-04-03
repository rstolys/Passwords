#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sstream>

#include "password.hpp"
#include "AESencryption.hpp"

using namespace std;

/////////////////////////////////////////////////////////////////////
/// Initial authentification to access program
///
/// @param   NONE
///
/////////////////////////////////////////////////////////////////////
bool authentification()
{

    string master_pass = "SFUnc2019!"; //Master Password
    string guess;                      //Master Password Guess
    int trys = 1;                      //First attempt is try

    while (true)
    { //Master Password Authentification

        //Prompt user to enter password
        cout << "Enter Password: \n";
        cin >> guess;

        //If guess is correct
        if (guess == master_pass)
        { //Must be perfect match

            //Display successful authentification
            cout << "\n\nAuthentification Successful\n\n";

            //Permit access
            return true;
        }
        else
        {

            //Indicate incorrect password
            cout << "Password is wrong\n";

            //If incorrect password is entered 3 times, terminate program
            if (trys > 2)
            {

                cout << "Program Terminating\n";
                return false;
            }

            //Increment number of tries
            trys++;
        }
    }
}

/////////////////////////////////////////////////////////////////////
/// Main menu of program
///
/// @param[in/out]   *pList       Passwords data strcuture
/// @param[in/out]   *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void menu(password_t *pList, int *piNumPass)
{

    int iChoice;
    bool bContinue = true;

    //Loop until exit
    while (bContinue)
    {

        //Display menu options
        cout << "\nOptions: \n";
        cout << "\t 1. Add Password\n";
        cout << "\t 2. Look Up Password\n";
        cout << "\t 3. Delete Password\n";
        cout << "\t 4. Exit Program\n";
        cout << "Enter your choice: ";

        //If input is not an integer
        if (!(cin >> iChoice))
        {

            //indicate error and prompt another attempt
            cout << "Error in entry. Please enter an integer\n\n";

            cin.clear();
            cin.ignore(100, '\n');

            continue;
        }

        switch (iChoice)
        {
        case ADD_PASSWORD:

            add_pass(pList, piNumPass);
            break;

        case LOOK_UP:

            look_up(pList, *piNumPass);
            break;

        case DELETE_PASSWORD:

            delete_pass(pList, piNumPass);
            break;

        case EXIT:

            //Prompt program exit
            bContinue = false;

            break;

        default:

            cout << "Error in entry. Please one of the options\n\n";
            break;
        }
    }
    return;
}

/////////////////////////////////////////////////////////////////////
/// writes all passwords from data strcutre to file
///
/// @param[in]       *pList       Passwords data strcuture
/// @param[in]       filename     Name of file storing passwords
/// @param[in/out]   *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void write_passwords(password_t *pList, string filename, int iNumPass)
{
    FILE *ofWrite;

    int i, j;

    //Open file for writing
    ofWrite = fopen(filename.c_str(), "w");

    //Print the number of passwords
    fprintf(ofWrite, "%d\n", iNumPass);

    //continue printing until name element is empty
    for (i = 0; i < iNumPass; i++)
    {

        //If password element is empty
        if (0 == pList[i].szName[0])
        {
            continue;
        }

        //Write the name to the file
        fprintf(ofWrite, "%s:", pList[i].szName);

        //Write the passsword to the file
        for (j = 0; j < 16; j++)
        {

            //Add password element by element
            fprintf(ofWrite, "%02x ", pList[i].szPassword[j] & 0xff);
        }

        //Add endline
        fprintf(ofWrite, "\n");

        //increment the password
        iNumPass++;

        if (49 < iNumPass)
        {
            break;
        }
    }

    //close file
    fclose(ofWrite);

    //File is updated
    return;
}

/////////////////////////////////////////////////////////////////////
/// Loads all passwords into data strcutre from file
///
/// @param[out]      *pList       Passwords data strcuture
/// @param[in]       filename     Name of file storing passwords
/// @param[in/out]   *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void load_passwords(password_t *pList, string filename, int *piNumPass)
{

    ifstream ifRead(filename);
    int iNumLines;
    string sNumLines;

    string sElement[50];
    char szElement[40];
    char *szToken;
    char *szToken2;

    stringstream ss;

    int i, j;

    //If file did not open
    if (!(ifRead.good()))
    {
        cout << "File did not open\n";
        cout << "File does not exist\n\n";
    }
    else
    {

        //get the number of passwords in file
        getline(ifRead, sNumLines);

        if (atoi(sNumLines.c_str()))
        {
            //Fill string stream
            ss << sNumLines;

            ss >> iNumLines;

            for (j = 0; j < iNumLines; j++)
            {

                //Check that we are not at capacity for current storage
                if (49 < *piNumPass)
                {
                    cout << "ERROR: Reached maximum password storage\n";
                    break;
                }

                //Get line from file
                getline(ifRead, sElement[j]);

                //Convert to c string to use strtok
                strcpy(szElement, sElement[j].c_str());

                //Tokenize name element and save into structure
                szToken = strtok(szElement, ":");
                strcpy(pList[*piNumPass].szName, szToken);

                //Save each character into each element of password array
                szToken2 = strtok(NULL, " ");
                pList[*piNumPass].szPassword[0] = szToken2[0];

                //Loop for all other elements
                for (i = 1; i < 16; i++)
                {
                    szToken2 = strtok(NULL, " ");
                    pList[*piNumPass].szPassword[i] = szToken2[0];
                }

                //Increment to next password
                (*piNumPass)++;
            }

            //All passwords loaded
            cout << "\n\tPasswords Loaded\n\n";

            //Close file
            ifRead.close();
        }
        else
        {
            cout << "ERROR: Num Lines not a digit\n";
            exit(-1);
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////
/// MAIN FUNCTION
///
/// @param[in]     argc       number of arguments passed to program
/// @param[in]     argv       array of arguments passed to program
///
/////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

    password_t pList[50];

    int iNumPass = 0;

    //Verify proper arguments are given
    if (2 > argc)
    {

        cout << "Usage: password <passwords file>\n";
        exit(1);
    }

    if (authentification())
    {

        //Load all the passwords to data strcuture
        load_passwords(pList, argv[1], &iNumPass);

        //Call function to handle user inputs
        menu(pList, &iNumPass);
    }
    else
    {

        //Exit program
        return 1;
    }

    //Write passwords to file beofre exiting
    write_passwords(pList, argv[1], iNumPass);

    return 0;
}
