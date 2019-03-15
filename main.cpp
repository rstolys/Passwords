#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>

#include "password.hpp"
#include "AESencryption.hpp"

using namespace std;


/////////////////////////////////////////////////////////////////////
/// Initial authentification to access program
///
/// @param   NONE
///
/////////////////////////////////////////////////////////////////////
bool authentification() {

    string master_pass = "SFUnc2019!";          //Master Password
    string guess;                               //Master Password Guess
    int trys = 1;                               //First attempt is try

    while (true) {                       //Master Password Authentification

        //Prompt user to enter password
        cout << "Enter Password: \n";
        cin >> guess;

        //If guess is correct
        if (guess == master_pass) {         //Must be perfect match

            //Display successful authentification
            cout << "\n\nAuthentification Successful\n\n";

            //Permit access
            return true;
        }
        else {

            //Indicate incorrect password
            cout << "Password is wrong\n";

            //If incorrect password is entered 3 times, terminate program
            if (trys > 2) {

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
void menu( password_t *pList, int *piNumPass ) {

    int     iChoice;
    bool    bContinue = true;

    //Loop until exit
    while ( bContinue ) {

        //Display menu options
        cout << "\nOptions: \n";
        cout << "\t 1. Add Password\n";
        cout << "\t 2. Look Up Password\n";
        cout << "\t 3. Delete Password\n";
        cout << "\t 4. Exit Program\n";
        cout << "Enter your choice: ";

        //If input is not an integer
        if (!(cin >> iChoice)) {

            //indicate error and prompt another attempt
            cout << "Error in entry. Please enter an integer\n\n";

            cin.clear();
            cin.ignore(100,'\n');

            continue;
        }

        switch (iChoice)
        {
        case ADD_PASSWORD:

            add_pass( pList, piNumPass );
            break;

        case LOOK_UP:

            look_up( pList, *piNumPass );
            break;

        case DELETE_PASSWORD:

            delete_pass( pList, piNumPass );
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
void write_passwords( password_t *pList, string filename, int iNumPass )
{
  ofstream    ofWrite;

  int         i, j;

  //Open file for writing
  ofWrite.open( filename );

  //continue printing until name element is empty
  for( i = 0;  i < iNumPass; i++ )
  {

    //If password element is empty
    if ( !(pList[i].sName.length()) )
    {

      //Skip write
      continue;
    }

    //Write the name to the file
    ofWrite << pList[i].sName << ":";

    //Write the passsword to the file
    for( j = 0; j < 16; j++ )
    {

      //Add password element by element
      ofWrite << pList[i].sPassword[j] << " ";
    }

    //Add endline
    ofWrite << endl;
    
    //increment the password
    iNumPass++;

    if( 49 < iNumPass )
    {

      //Out of Passwords
      break;
    }
  }

  //close file 
  ofWrite.close();

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
void load_passwords( password_t *pList, string filename, int *piNumPass )
{

  ifstream    ifRead;

  string      sElement;
  char        szElement[40];
  char        *szToken;

  //Open file
  ifRead.open( filename );

  //If file did not open
  if( !( ifRead.good() ) ) 
  {

    cout << "File did not open\n"; 
    cout << "File does not exist\n\n";

    //no passwords to load 
    return;
  }

  while ( !( ifRead.eof() ) )
  {

    //Check that we are not at capacity for current storage
    if( 49 < *piNumPass )
    {

      cout << "ERROR: Reached maximum password storage\n";
      return;
    }


    //Get line from file
    getline( ifRead, sElement );

    //Ensure line isnt empty
    if( !sElement.length() )
    {

      //Line is empty, end file load 
      return;
    }

    //Convert to c string
    strcpy( szElement, sElement.c_str() );

    //Tokenize name element and save into structure
    szToken = strtok( szElement, ":" );
    pList[ *piNumPass ].sName = szToken;

    //Tokenize encrytpted password element and save into structure
    szToken = strtok( NULL, ":" );
    pList[ *piNumPass ].sPassword = szToken;


    //Increment to net passwords
    (*piNumPass)++;
  }

  //All passwords loaded
  cout << "\n\tPasswords Loaded\n\n";

  //close file 
  ifRead.close();

  return;
}

/////////////////////////////////////////////////////////////////////
/// MAIN FUNCTION
///
/// @param[in]     argc       number of arguments passed to program
/// @param[in]     argv       array of arguments passed to program
///
/////////////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {

  password_t   pList[50];

  int          iNumPass = 0;

  //Verify proper arguments are given
  if (2 > argc)
  {

    cout << "Usage: password <passwords file>\n";
    exit(1);
  }

  if (authentification()) {

      //Load all the passwords to data strcuture
      load_passwords( pList, argv[1], &iNumPass );

      //Call function to handle user inputs
      menu( pList, &iNumPass );
  }

    //Write passwords to file beofre exiting
    write_passwords( pList, argv[1], iNumPass );

    return 0;
}
