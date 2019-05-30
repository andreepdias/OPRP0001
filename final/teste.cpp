#include <bits/stdc++.h>
#include <crypt.h>
using namespace std;

#define ABC_SIZE 64
const char alfabeto[65] = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

string number2word(long num);

int main(int argc, char **argv)
{

    int cifrassize;
    char *cifras;

    stringstream ss;

    ifstream file(argv[1]);

    string line;
    while (getline(file, line))
    {
        ss << line;
    }
    // cout << ss.str();
    file.close();

    cifrassize = ss.str().size() + 1;
    cifras = new char[cifrassize];

    strcpy(cifras, ss.str().c_str());

    /* ----------------------------------- */

    for(int i = 0; i < cifrassize; i++){
        if(cifras[i] == '\0') cout << "opa" << i << endl;;
    }

    string cifras_str = string(cifras);

    int numero_cifras = (cifrassize - 1) / 13; //Número de palavras codificas (cifras) a processar
    int cifra_atual = 0;                       //Em qual cifra estamos no momento

    int tamanho_cifra = 1;        //Tamanho de senha testado no momento
    int tamanho_maximo_cifra = 8; //Máximo tamanho possível da senha (resposta)


    long numero_possibilidades;
    for (tamanho_cifra = 1; tamanho_cifra <= tamanho_maximo_cifra; tamanho_cifra++)
    {
        numero_possibilidades += std::pow(ABC_SIZE, tamanho_cifra);
    }

    int size = 1;

    long rank_slice = numero_possibilidades; //quanto cada rank vai processar


    while (numero_cifras--)
    {
        int a = cifra_atual * 13;
        int b = 13;
        char *cifra = strdup(cifras_str.substr(a, b).c_str());
        printf("a: %d\tb: %d\tcifra: /%s/\n", a, b, cifra);

        for (int i = 0; i < (rank_slice * 0) + rank_slice; i++)
        {
            char *palavra = strdup(number2word(i).c_str()); //Talvez seja bom colocar a função inline depois

            if (strcmp(crypt(palavra, cifra), cifra) == 0)
            {

                cout << "cifra:" << cifra_atual << " senha:" << string(palavra) << endl;
                break;
            }
            // MPI_Bcast(buffer, 9, MPI_CHAR, MPI_ANY_SOURCE, MPI_COMM_WORLD);
            palavra = NULL;
        }
        cifra_atual++;
    }

    return 0;
}

string number2word(long num)
{
    //14 (abc(4)abc(7))  = 4*64^1 + 7*64^0 = 263
    //263 = dividir sucessivamente, como de decimal para binário
    stringstream palavra_ss;

    ldiv_t temp;
    temp.quot = num;

    while (temp.quot != 0)
    {
        temp = ldiv(temp.quot, ABC_SIZE);
        palavra_ss << alfabeto[temp.rem - 1];
    }
    string palavra = palavra_ss.str();
    reverse(palavra.begin(), palavra.end());

    return palavra;
}
