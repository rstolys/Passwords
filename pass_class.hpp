
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

#define ADD_PASSWORD      1
#define LOOK_UP           2
#define DELETE_PASSWORD   3
#define EXIT              4

class passClass;

typedef struct
{
  char      szName[25];
  char      szPassword[16];
} password_t;

class passClass 
    {
    public: 
        /////////////////////////////////////////////////////////////////////
        ///
        /// gets the number of passwords in the structure
        ///
        /////////////////////////////////////////////////////////////////////
        int getNumPass();

        /////////////////////////////////////////////////////////////////////
        ///
        /// adds password to data structure
        ///
        /////////////////////////////////////////////////////////////////////
        void add_pass();

        /////////////////////////////////////////////////////////////////////
        ///
        /// Looks up and displays password for user
        ///
        /////////////////////////////////////////////////////////////////////
        void look_up();

        /////////////////////////////////////////////////////////////////////
        /// Creates new state from password
        ///
        /// @param[in]    szPassword    Password to set state with
        /// @param[out]   state         state to decrypt
        ///
        /////////////////////////////////////////////////////////////////////
        void createState( char szPassword[16], uint8_t state[4][4] );

        /////////////////////////////////////////////////////////////////////
        ///
        /// Deletes password from data strcuture
        ///
        /////////////////////////////////////////////////////////////////////
        void delete_pass();
        
        /////////////////////////////////////////////////////////////////////
        ///
        /// Displays passwords in data structure
        ///
        /////////////////////////////////////////////////////////////////////
        void show_passwords();

        /////////////////////////////////////////////////////////////////////
        /// Inserts new element into vector
        /// 
        /// @param[in]    name      Name of password to be stored
        /// @param[in]    password  Password to be stored
        ///
        /////////////////////////////////////////////////////////////////////
        void addElementToList( char* name, char* password );

        /////////////////////////////////////////////////////////////////////
        /// Removes element from vector
        ///
        /// @param[in]    index    index of element to be removed
        ///
        /////////////////////////////////////////////////////////////////////
        void removeElementFromList( int index );

        /////////////////////////////////////////////////////////////////////
        /// Access Name Element in list
        ///
        /// @param[in]    index    index of element to be removed
        ///
        /////////////////////////////////////////////////////////////////////
        char* accessNameElementFromList( int index );

        /////////////////////////////////////////////////////////////////////
        /// Access password Element in list
        ///
        /// @param[in]    index    index of element to be removed
        ///
        /////////////////////////////////////////////////////////////////////
        char* accessPasswordElementFromList( int index );

        /////////////////////////////////////////////////////////////////////
        /// Modify name Element in list
        ///
        /// @param[in]    index     index of element to be removed
        /// @param[in]    name      name to modify in vector
        ///
        /////////////////////////////////////////////////////////////////////
        void modifyNameElementFromList( int index, char* name );

        /////////////////////////////////////////////////////////////////////
        /// Modify password Element in list
        ///
        /// @param[in]    index     index of element to be removed
        /// @param[in]    password  password to modify in vector
        ///
        /////////////////////////////////////////////////////////////////////
        void modifyPasswordElementFromList( int index, char* password );

        /////////////////////////////////////////////////////////////////////
        /// Checks if password list is empty
        ///
        /// @return       bool      true if empty, false if not empty
        ///
        /////////////////////////////////////////////////////////////////////
        bool isListEmpty();

    private:
        /////////////////////////////////////////////////////////////////////
        ///
        /// Class Variables
        ///
        /////////////////////////////////////////////////////////////////////
        vector<password_t>  vList;          

    };


