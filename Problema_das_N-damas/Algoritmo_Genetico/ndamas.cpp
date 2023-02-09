#include <iostream>
#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

int SIZE = 8;
int TAM = 64;
double prob_mutacao = 0.2;

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

void PrintarPopulacao(vector<individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "individuo " << i << endl;
        PrintarIndividuo(populacao[i]);
    }
}

vector<individuo> GerarPopulacao(int qntIndividuos)
{
    vector<individuo> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        individuo ind = GerarIndividuo();
        populacao.push_back(ind);
    }

    return populacao;
}

int ContarAmeacas(vector<pair<int, int>> damas)
{
    int ameacas = 0;
    for (int i = 0; i < damas.size(); i++)
    {
        for (int j = i + 1; j < damas.size(); j++)
        {
            if (damas[i].first == damas[j].first) // mesma linha
            {
                ameacas++;
            }
            if (damas[i].second == damas[j].second) // mesma coluna
            {
                ameacas++;
            }
            if (abs(damas[j].first - damas[i].first) == abs(damas[j].second - damas[i].second)) // diagonal
            {
                ameacas++;
            }
        }
    }
    return ameacas;
}

void AvaliarIndividuo(individuo &ind, int tam)
{
    vector<pair<int, int>> damas;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (ind.tab[{i, j}] == 1)
            {
                //ameacasADamas += ContarAmeacas(ind.tab, {i, j});
                damas.push_back({i, j});
            }
        }
    }

    int ameacasADamas = ContarAmeacas(damas);

    int penalidade = ameacasADamas * tam;
    ind.valor = damas.size() - penalidade;
}

int AvaliarPopulacao(vector<individuo> &populacao)
{
    vector<int> desempenhos;

    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i], TAM);

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

void Reproduzir(vector<individuo> &populacao)
{
    // pegar os dois individuos com melhor valor
    individuo i1 = populacao[0];
    individuo i2 = populacao[1];

    // de 1 a SIZE * SIZE - 1 para pegar parte de um e parte do outro
    int corte = rand() % (TAM - 1) + 1;
    // rand() % 63 -> 0 a 62 + 1 ->     1 a 63

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

    populacao.push_back(filho);
}

void MutarIndividuo(individuo &i)
{
    int mutar = rand() % TAM;

    int linha = mutar / SIZE;
    int coluna = mutar % SIZE;

    i.tab[{linha, coluna}] == 1 ? i.tab[{linha, coluna}] = 0 : i.tab[{linha, coluna}] = 1;
}

void MutarPopulacao(vector<individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < prob_mutacao)
        {
            MutarIndividuo(populacao[i]);
        }
    }
}

void NDamas(int qntIndividuos)
{
    vector<individuo> populacao = GerarPopulacao(qntIndividuos);
    //cout << "populacao inicial:" << endl;
    //PrintarPopulacao(populacao);

    int maiorAvaliacao = AvaliarPopulacao(populacao);
    int geracao = 0;
    while (maiorAvaliacao != SIZE)
    {
        //cout << maiorAvaliacao << endl;
        OrdenarPopulacao(populacao);
        Reproduzir(populacao);
        MutarPopulacao(populacao);
        maiorAvaliacao = AvaliarPopulacao(populacao);
        geracao++;
    }

    OrdenarPopulacao(populacao);

    cout << "geracoes: " << geracao << " individuo:" << endl;
    PrintarIndividuo(populacao[0]);
}

int main()
{
    srand((unsigned) time(NULL));

    cout << "Digite o tamanho do tabuleiro (> 3): ";
    cin >> SIZE;

    if (SIZE <= 3)
    {
        cout << "Digite um numero maior do que 3";
        return 0;
    }
    
    int qntIndividuos;
    cout << "Digite a quantidade de individuos (> 2): ";
    cin >> qntIndividuos;

    if (qntIndividuos <= 2)
    {
        cout << "Digite um numero maior do que 2" << endl;
        return 0;
    }

    TAM = pow(SIZE, 2);
    NDamas(qntIndividuos);
    return 0;
}
