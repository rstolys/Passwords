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
    char          szID[25];

    string        sEncrypt;
    char          szEncrypt[16];

    char          szPassword[16];

    int           i, j;

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

    //Create char* for ID
    strcpy( szID, sType.c_str() );

    //Create char* for password to encrypt
    strcpy( szEncrypt, sEncrypt.c_str() );

    //Encrypt password and save in file
    encrpyt( szEncrypt, szID, state);

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
    for( j = 0; j < 16; j++ )
    {

      //Add element to string
      sprintf( &szPassword[j],  "%c", state[ j / 4 ][ j % 4 ] );
    }
    
    //Save password
    pList[i].sPassword = szPassword;

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
    char      szID[25];

    string    sDecrypted;
    
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

        if ( sName == pList[i].sName )
        {

          //Create state to decrypt
          createState( pList[i].sPassword, state );   //seg fault*****************

          //Create char* for ID
          strcpy( szID, pList[i].sName.c_str() );

          //Decrypt password
          decrpyt( szID, state);

          //Save decrypted password
          for( i = 0; i < 16; i++ )
          {

            //Add element to string
            sDecrypted += state[ i / 4 ][ i % 4 ];
          }

          //Show password
          cout << "\nPassword for : " << sName << " : " << sDecrypted << endl;

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
/// @param[in]    sPassword    Password to set state with
/// @param[out]   state        state to decrypt
///
/////////////////////////////////////////////////////////////////////
void createState( string sPassword, uint8_t state[4][4] )
{

  char    szPassword[16]; 
  char    *szToken;

  //Copy password into char *
  strcpy( szPassword, sPassword.c_str() );

  //Get first token of password 
  szToken = strtok( szPassword, " " ); 

  //Save into state 
  state[0][0] = szToken[0] & 0xff;

  //Loop for rest of password
  for (int i = 1; i < 16; i++)
  {

    //Get token 
    szToken = strtok( NULL, " " );

    //Save into state 
    state[ i / 4 ][ i % 4 ] = szToken[0] & 0xff;

  }

  cout << "Password: \n";
  printProgress(state);

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

  cout << endl << endl;

  return;
}
