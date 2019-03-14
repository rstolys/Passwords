#include <iostream> 
#include <vector> 
#include <string>
#include <fstream>

#include "password.hpp"
#include "AESencryption.hpp"

using namespace std;

bool authentification(); 
void menu(); 

int main() {
    
    if (authentification()) {
        
        //Call function to handle user inputs
        menu();
    }

    return 0;
}

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

void menu() {

    int     iChoice;             

    //Loop until exit 
    while (true) {

        //Display menu options
        cout << "Options: \n"; 
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
            
            add_pass();
            break;

        case LOOK_UP: 
        
            look_up(); 
            break;

        case DELETE_PASSWORD: 
            
            delete_pass();
            break;

        case EXIT: 
            
            exit(-1);

        default: 

            cout << "Error in entry. Please one of the options\n\n"; 
            break; 
        }
    }
    return;
}