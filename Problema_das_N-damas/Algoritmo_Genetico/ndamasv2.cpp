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

struct Individuo {
    map<pair<float, float>, int> tab;
    int valor = 0;
};

Individuo GerarIndividuo()
{
    Individuo ind;
    
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            ind.tab[{i, j}] = rand() % 2;
        }
    }

    return ind;
}

void PrintarIndividuo(Individuo ind)
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

void PrintarPopulacao(vector<Individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "individuo " << i << endl;
        PrintarIndividuo(populacao[i]);
    }
}

vector<Individuo> GerarPopulacao(int qntIndividuos)
{
    vector<Individuo> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        Individuo ind = GerarIndividuo();
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

void AvaliarIndividuo(Individuo &ind)
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

    int penalidade = ameacasADamas * TAM;
    ind.valor = damas.size() - penalidade;
}

int AvaliarPopulacao(vector<Individuo> &populacao)
{
    vector<int> desempenhos;

    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i]);

        desempenhos.push_back(populacao[i].valor);
    }

    return *max_element(desempenhos.begin(), desempenhos.end());
}

bool compararPorValor(const Individuo& a, const Individuo& b) {
    return a.valor > b.valor;
}

void OrdenarPopulacao(vector<Individuo> &populacao)
{
    sort(populacao.begin(), populacao.end(), compararPorValor);
}

Individuo Herdar(Individuo pai, Individuo mae)
{
    Individuo filho;
    // de 1 a SIZE * SIZE - 1 para pegar parte de um e parte do outro
    int corte = rand() % (TAM - 1) + 1;

    int qnt = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (qnt < corte)
            {
                filho.tab.insert({{i, j}, pai.tab[{i,j}]});
            }
            else
            {
                filho.tab.insert({{i, j}, mae.tab[{i,j}]});
            }
            qnt++;
        }
    }

    return filho;
}

vector<Individuo> GerarFilhos(Individuo pai, Individuo mae)
{
    Individuo filho1 = Herdar(pai, mae);
    Individuo filho2 = Herdar(mae, pai);

    return {filho1, filho2};
}

void Reproduzir(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i + 1; j < populacao.size(); j++)
        {
            Individuo pai = populacao[i];
            Individuo mae = populacao[j];

            vector<Individuo> filhos = GerarFilhos(pai, mae);

            Individuo filho1 = filhos[0];
            Individuo filho2 = filhos[1];

            AvaliarIndividuo(filho1);
            AvaliarIndividuo(filho2);

            if (pai.valor < filho1.valor)
            {
                populacao[i] = filho1;
            }
            if (mae.valor < filho2.valor)
            {
                populacao[j] = filho2;
            }
        }
    }
}

void MutarIndividuo(Individuo &i)
{
    int mutar = rand() % TAM;

    int linha = mutar / SIZE;
    int coluna = mutar % SIZE;

    Individuo mutado = i;
    mutado.tab[{linha, coluna}] == 1 ? mutado.tab[{linha, coluna}] = 0 : mutado.tab[{linha, coluna}] = 1;

    AvaliarIndividuo(mutado);
    if (i.valor < mutado.valor)
    {
        i = mutado;
    }
}

void MutarPopulacao(vector<Individuo> &populacao)
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
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);
    //cout << "populacao inicial:" << endl;
    //PrintarPopulacao(populacao);

    int maiorAvaliacao = AvaliarPopulacao(populacao);
    int geracao = 0;
    while (maiorAvaliacao != SIZE)
    {
        cout << maiorAvaliacao << endl;
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
