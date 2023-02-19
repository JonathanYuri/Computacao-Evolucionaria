#include <vector>
#include <string>

#pragma once

using namespace std;

struct No {
    string atual;
    string valor;
    vector<No> prox;
};

void S(No &c, int n);
void T(No &c, int n);
void D(No &c);