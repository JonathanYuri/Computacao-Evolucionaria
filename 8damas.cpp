#include <iostream>
#include <vector>
#include <map>

using namespace std;

void PrintBoard(map<pair<float, float>, bool> tabuleiro)
{
    map<pair<float, float>, bool>::iterator it;
    for(it = tabuleiro.begin(); it != tabuleiro.end(); ++it){
        cout << "{" << it->first.first << ", " << it->first.second << "} => " << it->second << '\n';
    }
}

map<pair<float, float>, bool> InitializeBoard(int n, int m)
{
    map<pair<float, float>, bool> tabuleiro;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            pair<float, float> p = {i, j};
            tabuleiro.insert(make_pair(p, true));
        }
    }
    return tabuleiro;
}

void PrintSolution(vector<pair<float, float>> p)
{
    for (auto position : p)
    {
        cout << "{" << position.first << ", " << position.second << "}" << endl;
    }
}

bool ExistsOnTheBoard(map<pair<float, float>, bool> tabuleiro, pair<float, float> position)
{
    //cout << "position: {" << position.first << ", " << position.second << "}" << endl;
    auto res = tabuleiro.find(position);
    //cout << "ta sendo ameacada no tabuleiro? " << (res != tabuleiro.end()) << endl;
    return res != tabuleiro.end();
}

// remover casas do tabuleiro
vector<pair<float, float>> PutQueenInPosition(map<pair<float, float>, bool> *tabuleiro, vector<pair<float, float>> *damas, pair<float, float> position)
{
    vector<pair<float, float>> ameacas;

    damas->push_back(position);

    // consumir a linha
    for (int j = 0; j < 8; j++)
    {
        if (ExistsOnTheBoard(*tabuleiro, {position.first, j}))
        {
            ameacas.push_back({position.first, j});
            tabuleiro->erase({position.first, j});
        }
    }

    // consumir a coluna
    for (int i = 0; i < 8; i++)
    {
        if (ExistsOnTheBoard(*tabuleiro, {i, position.second}))
        {
            ameacas.push_back({i, position.second});
            tabuleiro->erase({i, position.second});
        }
    }

    // consumir diagonais

    // 1 diagonal
    for (int i = position.first - 1, j = position.second - 1; i >= 0; i--, j--)
    {
        if (j < 0) break;
        if (ExistsOnTheBoard(*tabuleiro, {i, j}))
        {
            ameacas.push_back({i, j});
            tabuleiro->erase({i, j});
        }
    }
    for (int i = position.first + 1, j = position.second + 1; i < 8; i++, j++)
    {
        if (j >= 8)  break;
        if (ExistsOnTheBoard(*tabuleiro, {i, j}))
        {
            ameacas.push_back({i, j});
            tabuleiro->erase({i, j});
        }
    }

    // 2 diagonal
    for (int i = position.first + 1, j = position.second - 1; i < 8; i++, j--)
    {
        if (j < 0) break;
        if (ExistsOnTheBoard(*tabuleiro, {i, j}))
        {
            ameacas.push_back({i, j});
            tabuleiro->erase({i, j});
        }
    }

    for (int i = position.first - 1, j = position.second + 1; i >= 0; i--, j++)
    {
        if (j >= 8)  break;
        if (ExistsOnTheBoard(*tabuleiro, {i, j}))
        {
            ameacas.push_back({i, j});
            tabuleiro->erase({i, j});
        }
    }

    return ameacas;
}

// colocar casas no tabuleiro que foram tiradas anteriormente
void TakeQueenInPosition(map<pair<float, float>, bool> *tabuleiro, vector<pair<float, float>> *damas, vector<pair<float, float>> ameacas)
{
    damas->pop_back();

    for (auto a : ameacas)
    {
        tabuleiro->insert({a, true});
    }
}

void OitoDamas(map<pair<float, float>, bool> tabuleiro, vector<pair<float, float>> damas, int y)
{
    if (damas.size() == 8)
    {
        PrintSolution(damas);
        exit(0);
    }

    if (tabuleiro.size() == 0)  return;

    int l = 0;
    while (l < 8)
    {
        if (ExistsOnTheBoard(tabuleiro, {l, y}))
        {
            vector<pair<float, float>> ameacas = PutQueenInPosition(&tabuleiro, &damas, {l, y});

            //cout << "--------=== dama na pos {" << l << ", " << y << "} ====---------" << endl;
            //PrintBoard(tabuleiro);

            OitoDamas(tabuleiro, damas, y+1);

            TakeQueenInPosition(&tabuleiro, &damas, ameacas);
        }
        l++;
    }
}

int main()
{
    // true se esta ocupada
    map<pair<float, float>, bool> tabuleiro;

    tabuleiro = InitializeBoard(8, 8);
    //PrintBoard(tabuleiro);

    OitoDamas(tabuleiro, {}, 0);
    return 0;
 }