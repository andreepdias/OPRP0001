#include <bits/stdc++.h>
#include <crypt.h>
using namespace std;

#define ABC_SIZE 64
const char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int abc_power[9];


int main(int argc, char **argv)
{

    char* palavra = new char[9];
    palavra[0] = 'a';
    palavra[1] = 'a';
    palavra[2] = 'a';
    palavra[3] = '\0';

    map<char*, int> mapa;

    mapa[palavra] = 3;

    cout << mapa[palavra] << endl;


}
