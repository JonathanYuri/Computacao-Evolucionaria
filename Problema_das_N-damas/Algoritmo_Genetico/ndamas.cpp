#include <iostream>
#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

int SIZE = 8;
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

int VerificarSeAmeacaADama(map<pair<float, float>, int> tab, pair<float, float> position, pair<float, float> queenPosition)
{
    if (tab[position] == 1 && position != queenPosition)
    {
        return 1;
    }
    return 0;
}

int ContarAmeacas(map<pair<float, float>, int> tab, pair<float, float> queenPosition)
{
    int ameacasADamas = 0;

    // consumir a linha
    for (int j = 0; j < SIZE; j++)
    {
        ameacasADamas += VerificarSeAmeacaADama(tab, {queenPosition.first, j}, queenPosition);
    }

    // consumir a coluna
    for (int i = 0; i < SIZE; i++)
    {
        ameacasADamas += VerificarSeAmeacaADama(tab, {i, queenPosition.second}, queenPosition);
    }

    // consumir diagonais

    // 1 diagonal
    for (int i = queenPosition.first - 1, j = queenPosition.second - 1; i >= 0; i--, j--)
    {
        if (j < 0) break;
        
        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
    }
    for (int i = queenPosition.first + 1, j = queenPosition.second + 1; i < SIZE; i++, j++)
    {
        if (j >= SIZE)  break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
    }

    // 2 diagonal
    for (int i = queenPosition.first + 1, j = queenPosition.second - 1; i < SIZE; i++, j--)
    {
        if (j < 0) break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
    }

    for (int i = queenPosition.first - 1, j = queenPosition.second + 1; i >= 0; i--, j++)
    {
        if (j >= SIZE)  break;

        ameacasADamas += VerificarSeAmeacaADama(tab, {i, j}, queenPosition);
    }
    return ameacasADamas;
}

void AvaliarIndividuo(individuo &ind, int tam)
{
    int ameacasADamas = 0;

    int qntDamas = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (ind.tab[{i, j}] == 1)
            {
                ameacasADamas += ContarAmeacas(ind.tab, {i, j});
                qntDamas++;
            }
        }
    }
    int penalidade = ameacasADamas * tam;
    ind.valor = qntDamas - penalidade;
}

int AvaliarPopulacao(vector<individuo> &populacao, int tam)
{
    vector<int> desempenhos;

    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i], tam);

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

void Reproduzir(vector<individuo> &populacao, int tam)
{
    // pegar os dois individuos com melhor valor
    individuo i1 = populacao[0];
    individuo i2 = populacao[1];

    // de 1 a SIZE * SIZE - 1 para pegar parte de um e parte do outro
    int corte = rand() % (tam - 1) + 1;
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

    /*
    if (rand() % 3 == 1)
    {
        MutarIndividuo(filho);
    }
    */

    populacao.push_back(filho);
}

void MutarIndividuo(individuo &i, int tam)
{
    int mutar = rand() % tam;

    int linha = mutar / SIZE;
    int coluna = mutar % SIZE;

    i.tab[{linha, coluna}] == 1 ? i.tab[{linha, coluna}] = 0 : i.tab[{linha, coluna}] = 1;
}

void MutarPopulacao(vector<individuo> &populacao, int tam)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < prob_mutacao)
        {
            MutarIndividuo(populacao[i], tam);
        }
    }
}

void NDamas(int qntIndividuos)
{
    int tam = pow(SIZE, 2);
    
    vector<individuo> populacao = GerarPopulacao(qntIndividuos);
    cout << "populacao inicial:" << endl;
    PrintarPopulacao(populacao);

    int maiorAvaliacao = AvaliarPopulacao(populacao, tam);
    int geracao = 0;
    while (maiorAvaliacao != SIZE)
    {
        //cout << maiorAvaliacao << endl;
        OrdenarPopulacao(populacao);
        Reproduzir(populacao, tam);
        MutarPopulacao(populacao, tam);
        maiorAvaliacao = AvaliarPopulacao(populacao, tam);
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

    NDamas(qntIndividuos);
    return 0;
}
