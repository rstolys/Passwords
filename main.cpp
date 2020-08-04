#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sstream>

#include "pass_class.hpp"
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
    bool    rc = false;

    string  master_pass = "SFUnc2020!";     //Master Password
    string  guess;                          //Master Password Guess
    int     trys = 1;                       //First attempt is try

    while (true)
        {
        cout << "Enter Password: \n";
        cin >> guess;

        //If guess is correct
        if (guess == master_pass)
            {
            cout << "\n\nAuthentification Successful\n\n";
            rc = true;
            break;
            }
        else
            {
            cout << "Password is wrong\n";

            //If incorrect password is entered 3 times, terminate program
            if (trys > 2)
                {
                cout << "Program Terminating...\n";
                rc = false;
                break;
                }

            trys++;
            }
        }

    return rc;
    }

/////////////////////////////////////////////////////////////////////
/// Main menu of program
///
/// @param[in/out]   *pList       Passwords data strcuture
/// @param[in/out]   *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void menu(passClass *pVault)
{
    int  iChoice;
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
                {
                pVault->add_pass();
                break;
                }
            case LOOK_UP:
                {
                pVault->look_up();
                break;
                }
            case DELETE_PASSWORD:
                {
                pVault->delete_pass();
                break;
                }
            case EXIT:
                {
                //Prompt program exit
                bContinue = false;

                break;
                }
            default:
                {
                cout << "Error in entry. Please one of the options\n\n";
                break;
                }
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
void write_passwords(passClass *pVault, string filename)
{
    FILE    *ofWrite;
    int     iNumPass = pVault->getNumPass();

    char*   szPassword;
    char*   szName;

    int     i, j;

    //Open file for writing
    ofWrite = fopen(filename.c_str(), "w");

    //Print the number of passwords
    fprintf(ofWrite, "%d\n", iNumPass);

    //continue printing until name element is empty
    for (i = 0; i < iNumPass; i++)
        {
        //If password element is empty
        if (pVault->isListEmpty())
            {
            continue;
            }

        //Write the name to the file
        fprintf( ofWrite, "%s:", pVault->accessNameElementFromList( i ) );

        //Get password from list
        szPassword = pVault->accessPasswordElementFromList( i );

        //Write the passsword to the file
        for (j = 0; j < 16; j++)
            {
            fprintf(ofWrite, "%02x ", szPassword[j] & 0xff);
            }

        //Add endline
        fprintf(ofWrite, "\n");
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
void load_passwords(passClass *pVault, string filename)
    {

    ifstream        ifRead(filename);
    int             iNumLines;
    string          sNumLines;

    string          sElement[50];
    char            szElement[40];
    char            *szToken;
    char            *szToken2;

    char            szNameElement[25];
    char            szPasswordElement[16];

    stringstream    ss;

    int             i, j;

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

            //Make number of lines into an integer
            ss >> iNumLines;

            for ( j = 0; j < iNumLines; j++ )
                {

                //Get line from file
                getline(ifRead, sElement[j]);

                //Convert to c string to use strtok
                strcpy(szElement, sElement[j].c_str());

                //Tokenize name element and save into structure
                szToken = strtok( szElement, ":" );
                strcpy( szNameElement, szToken );
                
                //Save each character into each element of password array
                //Loop for all other elements
                for (i = 0; i < 16; i++)
                    {
                    szToken = strtok( NULL, " " );
                    szPasswordElement[i] = szToken[0] & 0xff;
                    }

                //Add element to vector 
                pVault->addElementToList( szNameElement, szPasswordElement );
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
    int         rc = 0; 
    passClass   pVault;

    //Verify proper arguments are given
    if (2 > argc)
        {
        cout << "Usage: password <passwords file>\n";
        exit(1);
        }

    if (authentification())
        {

        //Load all the passwords to data strcuture
        load_passwords(&pVault, argv[1]);

        //Call function to handle user inputs
        menu(&pVault);

        //Write passwords to file beofre exiting
        write_passwords(&pVault, argv[1]);
        }
    else
        {
        rc = 1;
        }

    return rc;
    }
