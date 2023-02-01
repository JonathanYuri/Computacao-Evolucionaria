#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int SIZE = 8;

void PrintBoard(map<pair<float, float>, bool> tabuleiro)
{
    map<pair<float, float>, bool>::iterator it;
    for(it = tabuleiro.begin(); it != tabuleiro.end(); ++it){
        cout << "{" << it->first.first << ", " << it->first.second << "} => " << it->second << '\n';
    }
}

map<pair<float, float>, bool> InitializeBoard()
{
    map<pair<float, float>, bool> tabuleiro;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
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
vector<pair<float, float>> PutQueenInPosition(map<pair<float, float>, bool> *tabuleiro, pair<float, float> position)
{
    vector<pair<float, float>> ameacas;

    // consumir a linha
    for (int j = 0; j < SIZE; j++)
    {
        if (ExistsOnTheBoard(*tabuleiro, {position.first, j}))
        {
            ameacas.push_back({position.first, j});
            tabuleiro->erase({position.first, j});
        }
    }

    // consumir a coluna
    for (int i = 0; i < SIZE; i++)
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
    for (int i = position.first + 1, j = position.second + 1; i < SIZE; i++, j++)
    {
        if (j >= SIZE)  break;
        if (ExistsOnTheBoard(*tabuleiro, {i, j}))
        {
            ameacas.push_back({i, j});
            tabuleiro->erase({i, j});
        }
    }

    // 2 diagonal
    for (int i = position.first + 1, j = position.second - 1; i < SIZE; i++, j--)
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
        if (j >= SIZE)  break;
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

vector<pair<int, int>> SortLosses(map<int, vector<int>> loss, vector<int> losses)
{
    sort(losses.begin(), losses.end());

    //cout << "losses: " << endl;

    vector<pair<int, int>> l;
    for (auto i : losses)
    {
        //cout << i << "-> ";
        
        for (auto a : loss[i])
        {
            //cout << a << ", ";
            l.push_back({a, i});
        }
    }
    return l;
}

vector<pair<int, int>> CalculateLossOfParts(map<pair<float, float>, bool> tabuleiro, int y)
{
    map<int, vector<int>> loss;
    vector<int> losses;

    // 22 -> 0, 1, 2, 3, 4, 5, 6, 7
    for (int i = 0; i < SIZE; i++)
    {
        map<pair<float, float>, bool> tab = tabuleiro;
        vector<pair<float, float>> parts = PutQueenInPosition(&tab, {i, y});
        int lostparts = parts.size();
        
        vector<int>::iterator it = std::find(losses.begin(), losses.end(), lostparts);

        if(it == losses.end()) {
            losses.push_back(lostparts);
        }

        vector<int> a = loss[lostparts];
        a.push_back(i);
        loss[lostparts] = a;
    }

    vector<pair<int, int>> l = SortLosses(loss, losses);

    /*
    for (auto p : l)
    {
        cout << "{" << p.first << ", " << p.second << "}" << endl;
    }
    */

    return l;
}

void OitoDamas(map<pair<float, float>, bool> tabuleiro, vector<pair<float, float>> damas, int y)
{
    if (damas.size() == SIZE)
    {
        PrintSolution(damas);
        exit(0);
    }

    if (tabuleiro.size() == 0)  return;

    vector<pair<int, int>> loss = CalculateLossOfParts(tabuleiro, y);

    for (auto i : loss)
    {
        if (ExistsOnTheBoard(tabuleiro, {i.first, y}))
        {
            vector<pair<float, float>> ameacas = PutQueenInPosition(&tabuleiro, {i.first, y});
            damas.push_back({i.first, y});

            //cout << "--------=== dama na pos {" << l << ", " << y << "} ====---------" << endl;
            //PrintBoard(tabuleiro);

            OitoDamas(tabuleiro, damas, y+1);

            TakeQueenInPosition(&tabuleiro, &damas, ameacas);
        }
    }
}

int main()
{
    // true se esta ocupada
    map<pair<float, float>, bool> tabuleiro;

    int n;
    cout << "Digite o tamanho do tabuleiro: ";
    cin >> n;

    if (n <= 0) return 0;
    SIZE = n;

    tabuleiro = InitializeBoard();
    //PrintBoard(tabuleiro);

    OitoDamas(tabuleiro, {}, 0);
    return 0;
 }