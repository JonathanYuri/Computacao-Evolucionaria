#include <iostream>
#include <vector>

#include "./expressao.h"

#pragma once

using namespace std;

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