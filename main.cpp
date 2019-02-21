#include <iostream> 
#include <vector> 
#include <string>
#include <fstream>
#include "password.hpp"

bool authentification(); 
void menu(); 

int main() {
    
    if (authentification()) {
        menu();
    }

    return 0;
}

bool authentification() {
    std::string master_pass = "1";          //Master Password
    std::string guess;                      //Master Password Guess
    int trys = 1;                           //First attempt is try 

    while (true) {                       //Master Password Authentification
        std::cout << "Enter Password: \n"; 
        std::cin >> guess; 
        if (guess == master_pass) {         //Must be perfect match             
            return true;
        }
        else {
            std::cout << "Password is wrong\n";
            if (trys == 3) {            //Allow only 3 trys
                std::cout << "Program Terminating\n"; 
                return false;
            }
            trys++; 
        }
    }
}

void menu() {
    std::string choice_s; 
    int choice;                         //Menu Selection

    while (true) {
        std::cout << "Options: \n"; 
        std::cout << "\t 1. Add Password\n";
        std::cout << "\t 2. Look Up Password\n";
        std::cout << "\t 3. Delete Password\n";
        std::cout << "\t 4. Exit Program\n";
        std::cout << "Enter your choice: "; 
        
        if (!(std::cin >> choice)) {
            std::cout << "Error in entry. Please enter an integer\n\n";
            std::cin.clear(); 
            std::cin.ignore(100,'\n');
            continue; 
        }  

        switch (choice) {
            case 1: 
                add_pass();
                break;
            case 2: 
                look_up(); 
                break;
            case 3: 
                delete_pass();
                break;   
            case 4: 
                exit(-1);
            default: 
                std::cout << "Error in entry. Please one of the options\n\n"; 
                break; 
        }
    }
    return;
}