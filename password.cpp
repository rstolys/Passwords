#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>

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

    int           i;

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
      if( sType == pList[i].sName )
      {

        cout << "\n\tThis password already exists\n";
        return;
      }
    }

    //Get password to encrypt
    cout << endl << "Enter the password to encrypt: ";
    cin >> sEncrypt;


    //Encrypt password and save in file
    encrpyt(sEncrypt.c_str(), sType.c_str(), state);


    //Find first open location for password to be stored
    for( i = 0; i < *piNumPass; i++ )
    {

      if( !pList[i].sName.length() )
      {

         //Use this index
         break;
      }
    }

    //Save type to data structure
    pList[ i ].sName = sType;

    //Save encrypted password to data strcuture
    for( i = 0; i < 16; i++ )
    {

      //Add element to string
      pList[ i ].sPassword += state[ i / 4 ][ i % 4 ];
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

    string    sDecrypted;

    bool      bExit = false;

    uint8_t   state[4][4];


    //Show passwords
    show_passwords( pList );


    while( !bExit )
    {

      cout << "Enter the password you want to look up: ";
      cin >> sName;

      //Find password and show
      for ( i = 0; i < iNumPass; i++ )
      {

        if ( sName == pList[i].sName )
        {

          //Create state to decrypt
          for( j = 0; j < 16; j++ )
          {

            state[ j / 4 ][ j % 4 ] = pList[i].sPassword[j] & 0xff;
          }

          //Decrypt password
          decrpyt(pList[i].sName.c_str(), state);

          //Save decrypted password
          for( i = 0; i < 16; i++ )
          {

            //Add element to string
            sDecrypted += state[ i / 4 ][ i % 4 ];
          }

          //Show password
          cout << "Password for : " << sName << " : " << sDecrypted << endl;

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
  show_passwords( pList );


  while( !bExit )
  {

    cout << "Enter the password you want to delete: ";
    cin >> sName;

    //Find password and show
    for ( i = 0; i < iNumPass; i++ )
    {

      if ( sName == pList[i].sName )
      {

        //Show password
        cout << "Password for : " << sName << " : " << pList[i].sPassword << " has been deleted" << endl;

        pList[i].sName = "";
        pList[i].sPassword = "";

        //Ensure exit from fucntion is made
        i--;
        break;
      }
    }

    if ( i == iNumPass )
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
///
/////////////////////////////////////////////////////////////////////
void show_passwords( password_t *pList ) {

  int    iPrinted = 0;
  int    i;

  //Show all passwords
  cout << "\t\tHere is a list of all the password names: \n\n";

  for ( i = 0; i < iNumPass; i++ )
  {

    if( !pList[i].sName.length() )
    {

      //No password saved
      continue;
    }

    cout << pList[i].sName << "\t";

    //Increment the number of printed passwords
    iPrinted++;

    //Every 5 passwords go to next line
    if( 4 < iPrinted )
    {

      cout << endl;
    }
  }

  return;
}
