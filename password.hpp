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
} Password_t;

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
///
/////////////////////////////////////////////////////////////////////
void show_passwords( password_t *pList );



#endif
