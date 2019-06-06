#include <bits/stdc++.h>
#include <crypt.h>
using namespace std;

#define ABC_SIZE 64
const char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void number2word(long num, char* palavra);

int main(int argc, char **argv)
{
    char *palavra = (char*)malloc(sizeof(char) * 9);
    
    long int x = 0;
    for(int i = 1; i <= 8; i++){
        x += 4 * pow(64, i);
    }

    number2word(x, palavra);

    printf("%ld\t%s\n", x, palavra);

}

void number2word(long num, char* palavra){
    //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263 
    //263 = dividir sucessivamente, como de decimal para binário
    // stringstream palavra_ss;
    char palavra_inv[9];
    int cont = 0;

    ldiv_t temp;
    temp.quot = num;

    while(temp.quot != 0){
        temp = ldiv(temp.quot, ABC_SIZE);
        palavra_inv[cont] = alfabeto[temp.rem - 1];
        cont++;
    } 
    palavra_inv[cont] = '\0';
    // string palavra = palavra_ss.str();
    // reverse(palavra.begin(), palavra.end());
    //Inverte palavra
    for(int i = 0, j = cont-1; i < cont+1; i++, j--){
        palavra[i] = palavra_inv[j]; 
    }
    palavra[cont] = '\0';
}