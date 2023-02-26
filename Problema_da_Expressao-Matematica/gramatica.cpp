#include "./arvore.h"

using namespace std;

void D(No &c)
{
    int i = rand() % 10;
    c.atual = "VALOR";
    c.valor = to_string(i);
    c.prox = {};
}

void T(No &c, int n);

void S(No &c, int n)
{
    c.atual = "S";

    int i = rand() % 3;
    if (i == 0)
    {
        c.valor = "T+T";

        No prox1, prox2, prox3;
        T(prox1, n + 1);
        c.prox.push_back(prox1);

        prox2.atual = "OP";
        prox2.valor = "+";
        prox2.prox = {};
        c.prox.push_back(prox2);

        T(prox3, n + 2);
        c.prox.push_back(prox3);
        return;
    }
    if (i == 1)
    {
        c.valor = "T*T";

        No prox1, prox2, prox3;
        T(prox1, n + 1);
        c.prox.push_back(prox1);

        prox2.atual = "OP";
        prox2.valor = "*";
        prox2.prox = {};
        c.prox.push_back(prox2);

        T(prox3, n + 2);
        c.prox.push_back(prox3);
        return;
    }
    c.valor = "T-T";
    No prox1, prox2, prox3;
    T(prox1, n + 1);
    c.prox.push_back(prox1);

    prox2.atual = "OP";
    prox2.valor = "-";
    prox2.prox = {};
    c.prox.push_back(prox2);

    T(prox3, n + 2);
    c.prox.push_back(prox3);
}

void T(No &c, int n)
{
    c.atual = "T";

    // Limitando o tamanho da arvore
    if (n > 10)
    {
        c.valor = "D";
        No prox1;
        D(prox1);
        c.prox.push_back(prox1);
        return;
    }

    int i = rand() % 2;
    if (i == 0)
    {
        c.valor = "(S)";
        No prox1, prox2, prox3;

        prox1.atual = "DL";
        prox1.valor = "(";
        prox1.prox = {};
        c.prox.push_back(prox1);

        S(prox2, n + 1);
        c.prox.push_back(prox2);

        prox3.atual = "DL";
        prox3.valor = ")";
        prox3.prox = {};
        c.prox.push_back(prox3);
        return;
    }
    c.valor = "D";
    No prox1;
    D(prox1);
    c.prox.push_back(prox1);
}