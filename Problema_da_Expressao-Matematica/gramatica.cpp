#include "./arvore.h"

using namespace std;

No CriarNo(string atual, string valor, vector<No> prox)
{
    No no;
    no.atual = atual;
    no.valor = valor;
    no.prox = prox;

    return no;
}

void D(No &c)
{
    int i = rand() % 10;
    c = CriarNo(VALOR, to_string(i), {});
}

void T(No &c, int n);

void S(No &c, int n)
{
    int i = rand() % 3;
    if (i == 0)
    {
        No prox1, prox2, prox3;

        T(prox1, n + 1);
        prox2 = CriarNo(OPERADOR, "+", {});
        T(prox3, n + 2);

        c = CriarNo("S", "T+T", {prox1, prox2, prox3});
    }
    else if (i == 1)
    {
        No prox1, prox2, prox3;

        T(prox1, n + 1);
        prox2 = CriarNo(OPERADOR, "*", {});
        T(prox3, n + 2);

        c = CriarNo("S", "T*T", {prox1, prox2, prox3});
    }
    else if (i == 2)
    {
        No prox1, prox2, prox3;

        T(prox1, n + 1);
        prox2 = CriarNo(OPERADOR, "-", {});
        T(prox3, n + 2);

        c = CriarNo("S", "T-T", {prox1, prox2, prox3});
    }
}

void T(No &c, int n)
{
    // Limitando o tamanho da arvore
    if (n > 10)
    {
        No prox1;
        D(prox1);
        c = CriarNo("T", "D", {prox1});
        return;
    }

    int i = rand() % 2;
    if (i == 0)
    {
        No prox1, prox2, prox3;

        prox1 = CriarNo(DELIMITADOR, "(", {});
        S(prox2, n + 1);
        prox3 = CriarNo(DELIMITADOR, ")", {});
        
        c = CriarNo("T", "(S)", {prox1, prox2, prox3});
    }
    else if (i == 1)
    {
        No prox1;
        D(prox1);
        c = CriarNo("T", "D", {prox1});
    }
}