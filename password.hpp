#ifndef password
#define password

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

class order;

void add_pass();
void look_up(); 
void delete_pass();

void generate(std::string word);
void show_pass(std::string word);
void print_file();
void sort(order* pass, int n);
void swap(order* a, order* b);

class order {
public:
    order() {}

    void setup(std::string word);
    void set_int();
    int value ();
    std::string show();

private:
    int letter;
    std::string program;
};






#endif 