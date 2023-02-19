#include <iostream>
#include <vector>

#include "./expressao.h"

#pragma once

using namespace std;

void D(No &c)
{
    int i = rand() % 10;
    c.atual = "VALOR";
    c.valor = to_string(i);
    c.prox = {};
}