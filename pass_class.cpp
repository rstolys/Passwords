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
    char        szType[25];
    char        szPassword[16];

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
    encrpyt( szEncrypt, vList[i].szName, state );

    //Save Password
    strcpy( szType, sType.c_str() );
    
    //Save encrypted password to character array
    for( k = 0; k < 16; k++ )
        {
        sprintf( &szPassword[k], "%c", state[ k / 4 ][ k % 4 ] );
        }
    
    addElementToList( szType, szPassword );

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

    char*       szPassword;
    char*       szName;


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
                szPassword = accessPasswordElementFromList( i );
                createState( szPassword, state );              

                //Decrypt password
                szName = accessNameElementFromList( i );
                decrpyt( szName, state );

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
void passClass::createState( char szPassword[16], uint8_t state[4][4] )
    {
    for ( int i = 0; i < 16; i++ )
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
        if ( !strcmp( sName.c_str(), accessNameElementFromList( i ) ) )
            {
            //Show password
            cout << "Password for : " << sName << " : " << accessPasswordElementFromList( i ) << " has been deleted" << endl;

            //Remove element
            removeElementFromList( i );      
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

    char*   szName;

    //Show all passwords
    cout << "\t\tHere is a list of all the password names: \n\n";

    for ( i = 0; i < iNumPass; i++ )
        {
        //Get name element from list 
        szName = accessNameElementFromList( i );
        
        //If password doesn't exist, shouldn't occur
        if( 0 == szName[0] )
            {
            continue;
            }

        cout << szName << "\t";

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

/////////////////////////////////////////////////////////////////////
/// Inserts new element into vector
/// 
/// @param[in]    name      Name of password to be stored
/// @param[in]    password  Password to be stored
///
/////////////////////////////////////////////////////////////////////
void passClass::addElementToList( char* name, char* password )
    {
    password_t addElement; 

    //create element to add to vector
    strcpy( addElement.szName, name ); 
    strcpy( addElement.szPassword, password ); 

    vList.push_back( addElement );

    return;
    }

/////////////////////////////////////////////////////////////////////
/// Removes element from vector
///
/// @param[in]    index    index of element to be removed
///
/////////////////////////////////////////////////////////////////////
void passClass::removeElementFromList( int index )
    {
    vList.erase( vList.begin() + index );

    return;
    }

/////////////////////////////////////////////////////////////////////
/// Access Name Element in list
///
/// @param[in]    index    index of element to be removed
///
/////////////////////////////////////////////////////////////////////
char* passClass::accessNameElementFromList( int index )
    {
    return vList[index].szName;
    }

/////////////////////////////////////////////////////////////////////
/// Access password Element in list
///
/// @param[in]    index    index of element to be removed
///
/////////////////////////////////////////////////////////////////////
char* passClass::accessPasswordElementFromList( int index )
    {
    return vList[index].szPassword;
    }

/////////////////////////////////////////////////////////////////////
/// Modify name Element in list
///
/// @param[in]    index     index of element to be removed
/// @param[in]    name      name to modify in vector
///
/////////////////////////////////////////////////////////////////////
void passClass::modifyNameElementFromList( int index, char* name )
    {
    strcpy( vList[index].szName, name );

    return;
    }

/////////////////////////////////////////////////////////////////////
/// Modify password Element in list
///
/// @param[in]    index     index of element to be removed
/// @param[in]    password  password to modify in vector
///
/////////////////////////////////////////////////////////////////////
void passClass::modifyPasswordElementFromList( int index, char* password )
    {
    strcpy( vList[index].szPassword, password );

    return;
    }

/////////////////////////////////////////////////////////////////////
/// Checks if password list is empty
///
/// @return       int      0 == list is not empty, 1 == list is empty
///
/////////////////////////////////////////////////////////////////////
bool passClass::isListEmpty()
    {
    return vList.empty();
    }