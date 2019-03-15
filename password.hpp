#ifndef password
#define password

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;

#define ADD_PASSWORD      1
#define LOOK_UP           2
#define DELETE_PASSWORD   3
#define EXIT              4

typedef struct
{
  string    sName;
  string    sPassword;
} password_t;

/////////////////////////////////////////////////////////////////////
/// adds password to data structure
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in/out]  *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void add_pass( password_t *pList, int *piNumPass );

/////////////////////////////////////////////////////////////////////
/// Looks up and displays password for user
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in]      iNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void look_up( password_t *pList, int iNumPass );

/////////////////////////////////////////////////////////////////////
/// Creates new state from password
///
/// @param[in]    sPassword    Password to set state with
/// @param[out]   state        state to decrypt
///
/////////////////////////////////////////////////////////////////////
void createState( string sPassword, uint8_t state[4][4] );

/////////////////////////////////////////////////////////////////////
/// Deletes password from data strcuture
///
/// @param[out]     *pList       Passwords data strcuture
/// @param[in/out]  *piNumPass   Number of passwords saved
///
/////////////////////////////////////////////////////////////////////
void delete_pass( password_t *pList, int *piNumPass );

/////////////////////////////////////////////////////////////////////
/// Displays passwords in data structure
///
/// @param[in]     *pList       Passwords data strcuture
/// @param[in]     iNumPass     Number of passwords
///
/////////////////////////////////////////////////////////////////////
void show_passwords( password_t *pList, int iNumPass );



#endif
