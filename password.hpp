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

class order;

void add_pass();
void look_up(); 
void delete_pass();

void generate(string word);
void show_pass(string word);
void print_file();
void sort(order* pass, int n);
void swap(order* a, order* b);

class order {
public:
    order() {}

    void setup(string word);
    void set_int();
    int value ();
    string show();

private:
    int     letter;
    string  program;
};






#endif 