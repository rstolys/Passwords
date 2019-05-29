#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <string.h>
#include <vector>

#include "pass_class.hpp"
#include "AESencryption.hpp"


using namespace std;

/////////////////////////////////////////////////////////////////////
/// gets the number of passwords in the structure
///
/// @return      number of passwords
///
/////////////////////////////////////////////////////////////////////
int passClass::getNumPass()
    {
    return vList.size();
    }

/////////////////////////////////////////////////////////////////////
/// adds password to data structure
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in/out]  *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void passClass::add_pass() 
    {
    int         iNumPass = getNumPass();
    string      sType;

    string      sEncrypt;
    char        szEncrypt[16];

    int         i, j, k;

    uint8_t     state[4][4];

    //verify we won't go over password bounds
    if( 49 < iNumPass )
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

    for ( i = 0; i < iNumPass; i++ )
        {
        //Check if password already exists
        if( !strcmp( sType.c_str(), vList[i].szName) )
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
    encrpyt( szEncrypt, vList[i].szName, state);

    //Store Password
    vList.push_back(sType.c_str(), "temp");     //How do i initalize the vector element

    //Save encrypted password to data strcuture
    for( k = 0; k < 16; k++ )
        {
        sprintf( &vList[iNumPass].szPassword[k], "%c", state[ k / 4 ][ k % 4 ] );
        }

    return;
}

/////////////////////////////////////////////////////////////////////
/// Looks up and displays password for user
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in]      iNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void passClass::look_up() 
{
    int         iNumPass = getNumPass();

    int         i, j;
    string      sName;
    
    bool        bExit = false;

    char        szDecrypted[16];
    uint8_t     state[4][4];


    //Show passwords
    show_passwords();


    while( !bExit )
        {
        cout << "Enter the password you want to look up: ";
        cin >> sName;

        //Find password and show
        for ( i = 0; i < iNumPass; i++ )
            {
            if ( !strcmp( sName.c_str(), vList[i].szName ) )
                {
                //Create state to decrypt
                createState( vList[i].szPassword, state ); 

                //Decrypt password
                decrpyt( vList[i].szName, state);

                //Save decrypted password
                for( i = 0; i < 16; i++ )
                    {
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
            cout << "That entry did not match one of the options\n";
            cout << "Try Again\n\n";
            }
        else
            {
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
        state[ i / 4 ][ i % 4 ] = szPassword[i] & 0xff;
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
void passClass::delete_pass() 
    {
    int     iNumPass = getNumPass();
     
    int     i;
    int     iPrinted = 0;
    string  sName;

    bool    bExit = false;


  //Show passwords
  show_passwords();


    while( !bExit )
    {
    cout << "Enter the password you want to delete: ";
    cin >> sName;

    //Find password and show
    for ( i = 0; i < iNumPass; i++ )
        {
        if ( !strcmp( sName.c_str(), vList[i].szName ) )
            {
            //Show password
            cout << "Password for : " << sName << " : " << vList[i].szPassword << " has been deleted" << endl;

            //Remove element
            vList.remove(i);        //How do i remove a vector element
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
void passClass::show_passwords() 
    {
    int     iNumPass = getNumPass();

    int     iPrinted = 0;
    int     i;

    //Show all passwords
    cout << "\t\tHere is a list of all the password names: \n\n";

    for ( i = 0; i < iNumPass; i++ )
        {
        if( 0 == vList[i].szName[0] )
            {
            continue;
            }

        cout << vList[i].szName << "\t";

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