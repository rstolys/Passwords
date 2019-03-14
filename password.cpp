#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "password.hpp"
#include "AESencryption.hpp"


using namespace std;



//Order class functions
void order::setup(string word) {

        program = word; 
        set_int();
}

void order::set_int() {
    
    int first = (int) program[0];

    if (first >= 65 && first <= 90) {

        //Change capital letters to lowercase for sorting
        first += 32;        
    }

    //Set index of word; 
    letter = first;    
}

int order::value () {
    return letter;
}

string order::show() {
    return program;
}


void add_pass() { 

    char type[50];
    char to_encrypt[16];

    uint8_t   state_t[4][4];

    cout << "Enter a password Name to generate: ";
    cin >> type; 


    //Check if password alredy exists 


    cout << endl << "Enter the password to encrypt: "; 
    cin >> to_encrypt;


    //Encrypt password and save in file 
    encrpyt(to_encrypt, type, state_t);

    //Print to file 
        //in format that we can extract the state again

    // File format 
    // type:password (in hex seperated by spaces)
}


void add_pass2() { 

    string program;
    string line; 
    ofstream file;
    ifstream read; 
    bool exists = false;


    cout << "Enter Program: "; 
    cin >> program;


    read.open("ryan_pass.txt");
    file.open("ryan_pass.txt", ios_base::app); 

     if(!(read.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    if(!(file.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    while (getline(read, line)) {

        if(line == program) {
            cout << "This Password already exists in the file \n";
            exists = true;
            break;
        }
        else if (line != program) {
            continue; 
        }
    }

    if (!exists) {
        file << program << endl;
    }


    show_pass(program);

    file.close(); 
    return;
}



void look_up() {
    string program;

    cout << "Here is a list of all the programs with passwords: \n";
    print_file(); 

    cout << "Enter the password you want to look up: ";
    cin >> program; 

    show_pass(program);
    return;
}

void delete_pass() {
    ifstream file;
    ifstream temp_i;
    ofstream temp;
    ofstream output;  
    file.open("ryan_pass.txt");
    temp.open("temp.txt");
    string line;
    string word;

    cout << "Enter Program to Delete: "; 
    cin >> word;

    if(!(file.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    if(!(temp.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    while (!file.eof()) {
        getline(file, line);

        if (line == word) {
            continue;
        } 
        else {
            temp << line << endl;
        }
    }

    remove("ryan_pass.txt");
    temp.close();

    temp_i.open("temp.txt");
    output.open("ryan_pass.txt");
    if(!(temp_i.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }
    if(!(output.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    while(!temp_i.eof()) {
        getline(temp_i, line);
        output << line << endl;
    }

    remove("temp.txt");
    output.close(); 
    return;
}

void generate(string word) {
    int location_s; 
    int location_C;
    int special_chars = 14;         //Number of special characters in ASCII
    int capital = 26;               //Number of capital letters
    int char_i;                     //Character to use
    int* result_i = new int[10];
    int* result_f = new int[10]; 
    int length = word.length(); 


    //Set specific Passwords for other accounts
    //Example
    /*
    if(word == "bell") {
        cout << "5646\n";
        return;
    }
    */    
     
    location_s = special_chars % length;        //location where special character will be placed
 
    location_C = capital % length;        //location where capital letter will be placed

    for (int i = 0; i < length && i < 10; i++) {
        if (i == location_s) {                              //Insert Special Character
            char_i =  (location_s % special_chars) + 33;
            cout << (char)char_i;
        }

        if (i == location_C) {                              //Insert Capital Letter
            char_i =  (location_C % capital) + 65;
            cout << (char)char_i;
        }
        cout << hex <<(int)word[i]; 
    }
    cout << "\n";
}

void show_pass(string word) {
    cout << "The password for " << word << " is: \n"; 
    generate(word); 
    cout << "\n";
    return;
}

void print_file() {
    //Open file
    ifstream file; 
    file.open("ryan_pass.txt"); 
    string program;
    string dummy;
    int lines = 0;
    order* pass; 

    if(!(file.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    while (!file.eof()) {
        getline(file, dummy);
        if (dummy == "") {
            continue;
        }
        lines++;
    }

    pass = new order[lines];

    file.close();                           //close and reopen file to read it
    file.open("ryan_pass.txt");
    if(!(file.good())) {
        cout << "File could not be opened\n";
        cout << "Program Terminating\n";
        exit(-1);
    }

    for (int i = 0; i < lines; i++) {
        file >> program;
        pass[i].setup(program);

        if(file.eof()){                     //Should not execute since it is a while loop
            break;
        }
    }

    sort(pass, lines); 
    for (int i = 0; i < lines; i++) {
        cout << pass[i].show();
        cout << endl;
    }

    delete[] pass;

    file.close(); 
    return;
}

void sort(order* pass, int n) { 
   int i, j; 
   for (i = 0; i < n-1; i++) {
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  {
            if (pass[j].value() > pass[j+1].value()) {
                swap(&pass[j], &pass[j+1]);  
            }     
       }   
   }      
}

void swap(order* a, order* b) {
    order* temp = a; 
    a = b; 
    b = temp; 
}