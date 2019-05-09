#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

vi IP = {2, 6, 3, 1, 4, 8, 5, 7};
vi IP1 = {4, 1, 3, 5, 7, 2, 8, 6};

vvi S0 = {{1, 0, 3, 2},
          {3, 2, 1, 0},
          {0, 2, 1, 3},
          {3, 1, 3, 2}};

vvi S1 = {{0, 1, 2, 3},
          {2, 0, 1, 3},
          {3, 0, 1, 0},
          {2, 1, 0, 3}};

vi EP = {4, 1, 2, 3, 2, 3, 4, 1};

vi P4 = {2, 4, 3, 1};

vi P10 = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
vi P8 = {6, 3, 7, 4, 8, 5, 10, 9};

vi LS1 = {2, 3, 4, 5, 1};
vi LS2 = {3, 4, 5, 1, 2};

vi texto = {0, 1, 1, 0, 1, 0, 0, 0};
vi chave = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
vi K1 = {0, 1, 1, 0, 1, 0, 0, 0};
vi K2 = {1, 0, 0, 1, 0, 1, 1, 0};

void print(vi &texto)
{

    for (auto x : texto)
    {
        printf("%d ", x);
    }
    printf("\n");
}

void permutacao(vi &texto, vi &P)
{
    vi aux;
    for (auto x : P)
    {
        aux.push_back(texto[x - 1]);
    }
    texto = aux;
}

vi half(vi &texto, int idx)
{
    int s = texto.size();
    int m = s / 2;
    int b = idx;
    int e = b + m;

    vi aux;
    for (int i = b; i < e; i++)
    {
        aux.push_back(texto[i]);
    }
    return aux;
}

void do_xor(vi &V1, vi &V2)
{
    vi aux;
    for (int i = 0; i < V1.size(); i++)
    {
        int x = V1[i] ^ V2[i];
        aux.push_back(x);
    }
    V1 = aux;
}

void s(vi &texto, vvi &S)
{
    // print(texto);
    int l = texto[0] * pow(2, 1) + texto[3] * pow(2, 0);
    int c = texto[1] * pow(2, 1) + texto[2] * pow(2, 0);

    vi aux(2);
    int x = S[l][c];
    // printf("l: %d\tc: %d\tx: %d\n", l, c, x);
    if (x == 0)
    {
        aux[0] = 0;
        aux[1] = 0;
    }
    else if (x == 1)
    {
        aux[0] = 0;
        aux[1] = 1;
    }
    else if (x == 2)
    {
        aux[0] = 1;
        aux[1] = 0;
    }
    else if (x == 3)
    {
        aux[0] = 1;
        aux[1] = 1;
    }
    texto = aux;
}

void do_union(vi &texto, vi &h1, vi &h2)
{
    vi aux;
    for (int i = 0; i < h1.size(); i++)
    {
        aux.push_back(h1[i]);
    }
    for (int i = 0; i < h1.size(); i++)
    {
        aux.push_back(h2[i]);
    }
    texto = aux;
}

vi H1, H2, aux_texto1, aux_texto2;
vi sdes(vi &texto, vi &K1, vi &K2)
{
    permutacao(texto, IP);

    H1 = half(texto, 0);
    H2 = half(texto, 4);

    aux_texto1 = H1;
    aux_texto2 = H2;

    texto = H2;

    permutacao(texto, EP);
    do_xor(texto, K1);

    H1 = half(texto, 0);
    H2 = half(texto, 4);

    s(H1, S0);
    s(H2, S1);

    do_union(texto, H1, H2);
    permutacao(texto, P4);

    do_xor(texto, aux_texto1);

    /*-----------*/
    aux_texto1 = texto;
    do_union(texto, aux_texto2, aux_texto1);
    /*-----------*/

    H1 = half(texto, 0);
    H2 = half(texto, 4);

    aux_texto1 = H1;
    aux_texto2 = H2;

    texto = H2;

    permutacao(texto, EP);
    do_xor(texto, K2);

    H1 = half(texto, 0);
    H2 = half(texto, 4);

    s(H1, S0);
    s(H2, S1);

    do_union(texto, H1, H2);
    permutacao(texto, P4);

    do_xor(texto, aux_texto1);

    aux_texto1 = texto;
    do_union(texto, aux_texto1, aux_texto2);

    permutacao(texto, IP1);
    return texto;
}

void build(vi &v, string &s)
{
    for (int i = 0; i < s.size(); i++)
        v[i] = s[i] - '0';
}

void build_keys()
{
    vi H1, H2;
    vi aux = chave;
    permutacao(aux, P10);

    H1 = half(aux, 0);
    H2 = half(aux, 5);
    aux_texto1 = H1;
    aux_texto2 = H2;

    permutacao(aux_texto1, LS1);
    permutacao(aux_texto2, LS1);

    do_union(aux, aux_texto1, aux_texto2);
    permutacao(aux, P8);
    K1 = aux;
    print(K1);

    permutacao(aux_texto1, LS2);
    permutacao(aux_texto2, LS2);

    do_union(aux, aux_texto1, aux_texto2);
    permutacao(aux, P8);
    K2 = aux;
    print(K2);
}

int main(int argc, char const *argv[])
{
    string text, key;
    text = argv[1];
    key = argv[2];

    build(texto, text);
    build(chave, key);

    build_keys();

    vi encrypt = sdes(texto, K1, K2);
    print(encrypt);
    vi decrypt = sdes(encrypt, K2, K1);
    print(decrypt);
}