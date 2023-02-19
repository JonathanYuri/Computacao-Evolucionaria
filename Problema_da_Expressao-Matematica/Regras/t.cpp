#include <iostream>
#include <vector>

#include "./expressao.h"

#pragma once

using namespace std;

void T(No &c, int n)
{
    c.atual = "T";

    if (n > 10)
    {
        //cout << "!!!!!!!!!!!!!!!!!!!!!!!! PARANDO !!!!!!!!!!!!!!!!!!!!!!!!!";
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