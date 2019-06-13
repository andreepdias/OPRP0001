#include <bits/stdc++.h>
#include <crypt.h>
using namespace std;

#define ABC_SIZE 64
const char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int abc_power[9];

void number2word(long num, char* palavra);

int main(int argc, char **argv)
{
    char *palavra = (char*)malloc(sizeof(char) * 9);

    for(int i = 0; i < 9; i++){
        abc_power[i] = std::pow(ABC_SIZE, i);
    }
    
    long int x = 0;
    for(x = 4000; x < 5000; x++){
        number2word(x, palavra);

        printf("%ld\t%s\n", x, palavra);
    } 


    // x = 4227;

    // number2word(x, palavra);

    // printf("%ld\t%s\n", x, palavra);

}

void number2word(long num, char* palavra){
    char palavra_inv[9];
    int cont = 0;
    
// First figure out how many digits there are.
    num += 1; // This line is funky
    int c = 0;
    int x = 1;      
    while (num >= x) {
        c++;
        num -= x;
        x *= ABC_SIZE;
    }

    // Now you can do normal base conversion.
    for (int i = 0; i < c; i++) {
        palavra_inv[i] = alfabeto[num%ABC_SIZE];
        num = num/ABC_SIZE;
    }
    palavra_inv[9] = '\0';

    for(int i = 0, j = c-1; i < c; i++, j--){
        palavra[i] = palavra_inv[j]; 
    }
    palavra[c] = '\0';
}

// void number2word(long num, char* palavra){
//     //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263 
//     //263 = dividir sucessivamente, como de decimal para binário
//     // stringstream palavra_ss;
//     char palavra_inv[9];
//     int cont = 0;
//     bool multiplo = num%ABC_SIZE == 0;

//     if(multiplo) num--;

//     ldiv_t temp;
//     temp.rem = num;


//     while(temp.rem > ABC_SIZE){
//         for(int i = 8-cont; i >= 1; i--){
//             temp = ldiv(num, abc_power[i]);
//             if(temp.quot != 0) break;
//         }
//         palavra[cont] = alfabeto[temp.quot-1];
//         num = temp.rem;
//         cont++;
        
//     }
//     palavra[cont] = alfabeto[temp.rem-1];
//     if(multiplo)
//         palavra[cont] = alfabeto[ABC_SIZE-1];
//     palavra_inv[cont+1] = '\0';
// }



// void number2word(long num, char* palavra){
//     //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263 
//     //263 = dividir sucessivamente, como de decimal para binário
//     // stringstream palavra_ss;
//     char palavra_inv[9];
//     int cont = 0;

//     ldiv_t temp;
//     temp.quot = num+1;

//     do{
//         temp = ldiv(temp.quot, ABC_SIZE);
//         if(temp.rem == 0){
//             temp.rem = ABC_SIZE;
//         }
//         palavra_inv[cont] = alfabeto[temp.rem - 1];
//         cont++;
//     }while(temp.quot != 0);

//     palavra_inv[cont] = '\0';
//     // string palavra = palavra_ss.str();
//     // reverse(palavra.begin(), palavra.end());
//     //Inverte palavra
//     for(int i = 0, j = cont-1; i < cont+1; i++, j--){
//         palavra[i] = palavra_inv[j]; 
//     }
//     palavra[cont] = '\0';
// }