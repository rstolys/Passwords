#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "password.hpp"



//Order class functions
void order::setup(std::string word) {
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

std::string order::show() {
    return program;
}




void add_pass() { 
    std::string program;
    std::string line; 
    std::ofstream file;
    std::ifstream read; 
    bool exists = false;


    std::cout << "Enter Program: "; 
    std::cin >> program;

    read.open("ryan_pass.txt");
    file.open("ryan_pass.txt", std::ios_base::app); 

     if(!(read.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }

    if(!(file.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }

    while (getline(read, line)) {

        if(line == program) {
            std::cout << "This Password already exists in the file \n";
            exists = true;
            break;
        }
        else if (line != program) {
            continue; 
        }
    }

    if (!exists) {
        file << program << std::endl;
    }


    show_pass(program);

    file.close(); 
    return;
}



void look_up() {
    std::string program;

    std::cout << "Here is a list of all the programs with passwords: \n";
    print_file(); 

    std::cout << "Enter the password you want to look up: ";
    std::cin >> program; 

    show_pass(program);
    return;
}

void delete_pass() {
    std::ifstream file;
    std::ifstream temp_i;
    std::ofstream temp;
    std::ofstream output;  
    file.open("ryan_pass.txt");
    temp.open("temp.txt");
    std::string line;
    std::string word;

    std::cout << "Enter Program to Delete: "; 
    std::cin >> word;

    if(!(file.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }

    if(!(temp.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }

    while (!file.eof()) {
        getline(file, line);

        if (line == word) {
            continue;
        } 
        else {
            temp << line << std::endl;
        }
    }

    remove("ryan_pass.txt");
    temp.close();

    temp_i.open("temp.txt");
    output.open("ryan_pass.txt");
    if(!(temp_i.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }
    if(!(output.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
        exit(-1);
    }

    while(!temp_i.eof()) {
        getline(temp_i, line);
        output << line << std::endl;
    }

    remove("temp.txt");
    output.close(); 
    return;
}

void generate(std::string word) {
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
        std::cout << "5646\n";
        return;
    }
    */    
     
    location_s = special_chars % length;        //location where special character will be placed
 
    location_C = capital % length;        //location where capital letter will be placed

    for (int i = 0; i < length && i < 10; i++) {
        if (i == location_s) {                              //Insert Special Character
            char_i =  (location_s % special_chars) + 33;
            std::cout << (char)char_i;
        }

        if (i == location_C) {                              //Insert Capital Letter
            char_i =  (location_C % capital) + 65;
            std::cout << (char)char_i;
        }
        std::cout << std::hex <<(int)word[i]; 
    }
    std::cout << "\n";
}

void show_pass(std::string word) {
    std::cout << "The password for " << word << " is: \n"; 
    generate(word); 
    std::cout << "\n";
    return;
}

void print_file() {
    //Open file
    std::ifstream file; 
    file.open("ryan_pass.txt"); 
    std::string program;
    std::string dummy;
    int lines = 0;
    order* pass; 

    if(!(file.good())) {
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
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
        std::cout << "File could not be opened\n";
        std::cout << "Program Terminating\n";
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
        std::cout << pass[i].show();
        std::cout << std::endl;
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