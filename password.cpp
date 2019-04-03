#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <string.h>

#include "password.hpp"
#include "AESencryption.hpp"


using namespace std;


/////////////////////////////////////////////////////////////////////
/// adds password to data structure
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in/out]  *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void add_pass( password_t *pList, int *piNumPass ) {

    string        sType;

    string        sEncrypt;
    char          szEncrypt[16];

    int           i, j, k;

    uint8_t       state[4][4];

    //verify we won't go over password bounds
    if( 49 < *piNumPass )
    {

      cout << "The password limit has been reached\n";
      return;
    }

    //Get name of password
    cout << "Enter a password Name to generate: ";
    cin >> sType;


    ////////////////////////////////////
    /// Check if password alredy exists
    ////////////////////////////////////

    for ( i = 0; i < *piNumPass; i++ )
    {

      //If password already exists, return
      if( !strcmp( sType.c_str(), pList[i].szName) )
      {

        cout << "\n\tThis password already exists\n";
        return;
      }
    }

    //Get password to encrypt
    cout << endl << "Enter the password to encrypt: ";
    cin >> sEncrypt;

    //Create char* for password to encrypt
    strcpy( szEncrypt, sEncrypt.c_str() );

    //Encrypt password and save in file
    encrpyt( szEncrypt, pList[i].szName, state);

    //Find first open location for password to be stored
    for( j = 0; j < *piNumPass; j++ )
        {
        if( 0 == pList[j].szName[0] )
            {
            break;
            }
        }

    //Save type to data structure
    strcpy( pList[j].szName, sType.c_str() );

    //Save encrypted password to data strcuture
    for( k = 0; k < 16; k++ )
        {
        sprintf( &pList[j].szPassword[k], "%c", state[ k / 4 ][ k % 4 ] );
        }
    
    //Increment number of passwords
    (*piNumPass)++;

    return;
}

/////////////////////////////////////////////////////////////////////
/// Looks up and displays password for user
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in]      iNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void look_up( password_t *pList, int iNumPass ) {

    int       i, j;
    string    sName;

    char      szDecrypted[16];
    
    bool      bExit = false;

    uint8_t   state[4][4];


    //Show passwords
    show_passwords( pList, iNumPass);


    while( !bExit )
    {

      cout << "Enter the password you want to look up: ";
      cin >> sName;

      //Find password and show
      for ( i = 0; i < iNumPass; i++ )
      {

        if ( !strcmp( sName.c_str(), pList[i].szName ) )
        {

          //Create state to decrypt
          createState( pList[i].szPassword, state ); 

          //Decrypt password
          decrpyt( pList[i].szName, state);

          //Save decrypted password
          for( i = 0; i < 16; i++ )
          {

            //Add element to string
            szDecrypted[i] = state[ i / 4 ][ i % 4 ];
          }

          //Show password
          cout << "\nPassword for : " << sName << " : " << szDecrypted << endl;

          //Ensure exit from fucntion is made
          i--;
          break;
        }
      }

      //if password was not found
      if ( i == iNumPass )
      {

          //Password no found
          cout << "That entry did not match one of the options\n";
          cout << "Try Again\n\n";
      }
      else
      {

        //Exit entry
        bExit = true;
      }
    }

    return;
}

/////////////////////////////////////////////////////////////////////
/// Creates new state from password
///
/// @param[in]    szPassword    Password to set state with
/// @param[out]   state         state to decrypt
///
/////////////////////////////////////////////////////////////////////
void createState( char szPassword[16], uint8_t state[4][4] )
{
  for (int i = 0; i < 16; i++)
  {
    state[ i / 4 ][ i % 4 ] = szPassword[0] & 0xff;
  }

  return;
}

/////////////////////////////////////////////////////////////////////
/// Deletes password from data strcuture
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in/out]  *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void delete_pass( password_t *pList, int *piNumPass ) {

  int       i;
  int       iPrinted = 0;
  string    sName;

  bool      bExit = false;


  //Show passwords
  show_passwords( pList, *piNumPass );


  while( !bExit )
  {

    cout << "Enter the password you want to delete: ";
    cin >> sName;

    //Find password and show
    for ( i = 0; i < *piNumPass; i++ )
    {

      if ( !strcmp( sName.c_str(), pList[i].szName ) )
      {

        //Show password
        cout << "Password for : " << sName << " : " << pList[i].szPassword << " has been deleted" << endl;

        //Set passwords to empty
        pList[i].szName[0] = 0;
        pList[i].szPassword[0] = 0;

        //Ensure exit from fucntion is made
        i--;
        break;
      }
    }

    if ( i == *piNumPass )
    {

        //Password no found
        cout << "That entry did not match one of the options\n";
        cout << "Try Again\n\n";
    }
    else
    {

      //Decrement number of Passwords
      (*piNumPass)--;

      //Exit entry
      bExit = true;
    }
  }


  return;
}

/////////////////////////////////////////////////////////////////////
/// Displays passwords in data structure
///
/// @param[in]     *pList       Passwords data strcuture
/// @param[in]     iNumPass     Number of passwords
///
/////////////////////////////////////////////////////////////////////
void show_passwords( password_t *pList, int iNumPass ) {

  int    iPrinted = 0;
  int    i;

  //Show all passwords
  cout << "\t\tHere is a list of all the password names: \n\n";

  for ( i = 0; i < iNumPass; i++ )
  {

    if( 0 == pList[i].szName[0] )
    {

      //No password saved
      continue;
    }

    cout << pList[i].szName << "\t";

    //Increment the number of printed passwords
    iPrinted++;

    //Every 5 passwords go to next line
    if( 4 < iPrinted )
    {

      cout << endl;
    }
  }

  cout << endl << endl;

  return;
}
