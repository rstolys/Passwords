#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void generate(std::string word);


int main() {
    string word = "youtube"; 
    generate(word);

    return 0; 
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


    
    srand(result_i[1]); 
    location_s = rand() % length;        //location where special character will be placed

    srand(result_i[0]); 
    location_C = rand() % length;        //location where capital letter will be placed

    for (int i = 0; i < length && i < 10; i++) {
        if (i == location_s) {                              //Insert Special Character
            srand(result_i[i]);
            char_i =  (rand() % special_chars) + 33;
            cout << (char)char_i;
        }

        if (i == location_C) {                              //Insert Capital Letter
            srand(result_i[i]);
            char_i =  (rand() % capital) + 65;
            cout << (char)char_i;
        }
        cout << std::hex <<(int)word[i]; 
    }
    cout << endl;
}
