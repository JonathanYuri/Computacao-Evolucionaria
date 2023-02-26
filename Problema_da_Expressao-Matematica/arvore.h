#include <string>
#include <vector>

using namespace std;

#define VALOR "VALOR"
#define OPERADOR "OP"
#define DELIMITADOR "DL"

struct No {
    string atual;
    string valor;
    vector<No> prox;
};