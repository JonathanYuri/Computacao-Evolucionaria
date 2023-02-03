#include <iostream>
#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

int SIZE = 8;

struct individuo {
    map<pair<float, float>, int> tab;
    int valor = 0;
};

void RemoverDuplicatas(std::vector<int> &v)
{
	std::vector<int>::iterator itr = v.begin();
	unordered_set<int> s;

	for (auto curr = v.begin(); curr != v.end(); ++curr)
	{
		if (s.insert(*curr).second) {
			*itr++ = *curr;
		}
	}

	v.erase(itr, v.end());
}

individuo GerarIndividuo()
{
    individuo ind;
    
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            ind.tab[{i, j}] = rand() % 2;
        }
    }

    return ind;
}

void PrintarIndividuo(individuo ind)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            cout << ind.tab[{i, j}] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<individuo> GerarPopulacao(int qntIndividuos)
{
    vector<individuo> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        individuo ind = GerarIndividuo();

        cout << "ind " << i << endl;
        PrintarIndividuo(ind);

        populacao.push_back(ind);
    }

    return populacao;
}

int VerificarSeAmeacaADama(map<pair<float, float>, int> tab, pair<float, float> position, pair<float, float> queenPosition)
{
    if (tab[position] == 1 && position != queenPosition)
    {
        return 1;
    }
    return 0;
}

int ContarAmeacas(map<pair<float, float>, int> tab, pair<float, float> queenPosition, map<pair<float, float>, int> &ameacas)
{
    int ameacasADamas = 0;

    // consumir a linha
    for (int j = 0; j < SIZE; j++)
    {
        ameacasADamas += VerificarSeAmeacaADama(tab, {queenPosition.first, j}, queenPosition);
        ameacas.insert({{queenPosition.first, j}, 0});
    }

    // consumir a coluna
    for (int i = 0; i < SIZE; i++)
    {
        ameacasADamas += VerificarSeAmeacaADama(tab, {i, queenPosition.second}, queenPosition);
        ameacas.insert({{i, queenPosition.second}, 0});
    }

    // consumir diagonais

    // 1 diagonal
    for (int i = queenPosition.first - 1, j = queenPosition.second - 1; i >= 0; i--, j--)
    {
        if (j < 0) break;
        
        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
        ameacas.insert({{i, j}, 0});
    }
    for (int i = queenPosition.first + 1, j = queenPosition.second + 1; i < SIZE; i++, j++)
    {
        if (j >= SIZE)  break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
        ameacas.insert({{i, j}, 0});
    }

    // 2 diagonal
    for (int i = queenPosition.first + 1, j = queenPosition.second - 1; i < SIZE; i++, j--)
    {
        if (j < 0) break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
        ameacas.insert({{i, j}, 0});
    }

    for (int i = queenPosition.first - 1, j = queenPosition.second + 1; i >= 0; i--, j++)
    {
        if (j >= SIZE)  break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
        ameacas.insert({{i, j}, 0});
    }
    return ameacasADamas;
}

void AvaliarIndividuo(individuo &ind)
{
    map<pair<float, float>, int> ameacas;
    int ameacasADamas = 0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (ind.tab[{i, j}] == 1)
            {
                ameacasADamas += ContarAmeacas(ind.tab, {i, j}, ameacas);
            }
        }
    }
    //ind.valor = ameacas.size() - ameacasADamas * pow(SIZE, 2);

    ind.valor = ameacas.size() - ameacasADamas * SIZE;
}

int AvaliarPopulacao(vector<individuo> &populacao)
{
    vector<int> desempenhos;

    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i]);

        desempenhos.push_back(populacao[i].valor);
    }

    return *max_element(desempenhos.begin(), desempenhos.end());
}

void OrdenarPopulacao(vector<individuo> &populacao)
{
    // valor -> individuos, para recuperar os indices da populacao
    map<int, vector<int>> valorIndividuos;
    vector<int> desempenhos;

    for (int i = 0; i < populacao.size(); i++)
    {
        desempenhos.push_back(populacao[i].valor);
        valorIndividuos[populacao[i].valor].push_back(i);
    }

    // ordena
    sort(desempenhos.begin(), desempenhos.end(), greater<int>());
    RemoverDuplicatas(desempenhos);

    vector<individuo> populacaoOrdenada;

    // recupera os valores
    for (int a : desempenhos)
    {
        for (auto ind : valorIndividuos[a])
        {
            populacaoOrdenada.push_back(populacao[ind]);
        }
    }

    populacao = populacaoOrdenada;
}

void MutarIndividuo(individuo &i, int tam)
{
    int mutar = rand() % (tam - 1) + 1;

    int qnt = 0;
    for (int k = 0; k < SIZE; k++)
    {
        for (int m = 0; m < SIZE; m++)
        {
            if (qnt == mutar)
            {
                i.tab[{k, m}] == 1 ? i.tab[{k, m}] = 0 : i.tab[{k, m}] = 1;
                return;
            }
            qnt++;
        }
    }
}

void Reproduzir(vector<individuo> &populacao, int tam)
{
    // pegar os dois individuos com melhor valor
    individuo i1 = populacao[0];
    individuo i2 = populacao[1];

    // pegar um numero random entre 0 e SIZE * SIZE para pegar parte de um e parte do outro

    // de 1 a SIZE * SIZE - 1
    int corte = rand() % (tam - 1) + 1;
    // rand() % 63 -> 0 a 62 + 1 -> 1 a 63

    //tirar o ultimo e colocar esse novo
    populacao.pop_back();

    individuo filho;

    int qnt = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (qnt < corte)
            {
                filho.tab.insert({{i, j}, i1.tab[{i,j}]});
            }
            else
            {
                filho.tab.insert({{i, j}, i2.tab[{i,j}]});
            }
            qnt++;
        }
    }

    /*
    if (rand() % 3 == 1)
    {
        MutarIndividuo(filho);
    }
    */

    populacao.push_back(filho);
}

void MutarPopulacao(vector<individuo> &populacao, int tam)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        if (rand() % 4 == 1)
        {
            MutarIndividuo(populacao[i], tam);
        }
    }
}

void OitoDamas()
{
    int qntIndividuos = 10;
    int obj = pow(SIZE, 2);
    
    vector<individuo> populacao = GerarPopulacao(10);
    int maiorAvaliacao = AvaliarPopulacao(populacao);

    while (maiorAvaliacao != obj)
    {
        cout << maiorAvaliacao << endl;
        OrdenarPopulacao(populacao);
        Reproduzir(populacao, obj);

        /*
        cout << "ANTES" << endl;
        for (individuo i : populacao)
        {
            PrintarIndividuo(i);
        }
        */

        MutarPopulacao(populacao, obj);

        /*
        cout << "DEPOIS" << endl;
        for (individuo i : populacao)
        {
            PrintarIndividuo(i);
        }
        */
        maiorAvaliacao = AvaliarPopulacao(populacao);
    }

    PrintarIndividuo(populacao[0]);
}

int main()
{
    srand((unsigned) time(NULL));
    OitoDamas();
    return 0;
}
