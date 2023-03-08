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
double prob_reproducao = 0.5;

double taxa_aumento_prob = 0.1;

double menor_prob_mutacao = 0.1;
double menor_prob_reproducao = 0.1;
double maior_prob_mutacao = 0.9;
double maior_prob_reproducao = 0.9;

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

Individuo GerarFilho(Individuo pai, Individuo mae)
{
    Individuo filho;
    // de 1 a SIZE * SIZE - 1 para pegar parte de um e parte do outro
    int corte = rand() % (TAM - 1) + 1;
    // rand() % 63 -> 0 a 62 + 1 ->     1 a 63

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

    AvaliarIndividuo(filho);
    return filho;
}

void Reproduzir(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i + 1; j < populacao.size(); j++)
        {
            double prob = ((double) rand() / ((double)RAND_MAX + 1));
            if (prob < prob_reproducao)
            {
                Individuo filho1 = GerarFilho(populacao[i], populacao[j]);
                Individuo filho2 = GerarFilho(populacao[j], populacao[i]);

                if (filho1.valor > populacao[i].valor)
                {
                    populacao[i] = filho1;
                }
                if (filho2.valor > populacao[j].valor)
                {
                    populacao[j] = filho2;
                }
            }
        }
    }
}

void MutarIndividuo(Individuo &i)
{
    int mutar = rand() % TAM;

    int linha = mutar / SIZE;
    int coluna = mutar % SIZE;

    i.tab[{linha, coluna}] == 1 ? i.tab[{linha, coluna}] = 0 : i.tab[{linha, coluna}] = 1;
}

void MutarPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < prob_mutacao)
        {
            cout << "MUTOU" << endl;
            MutarIndividuo(populacao[i]);
        }
    }
}

double CalcularDiversidade(vector<Individuo> populacao)
{
    double max_iguais = 0;
    double qnt_iguais = 0;
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i + 1; j < populacao.size(); j++)
        {
            if (populacao[i].tab == populacao[j].tab)
            {
                qnt_iguais++;
            }
            max_iguais++;
        }
    }

    cout << "QNT IGUAIS: " << qnt_iguais << " MAX IGUAIS: " << max_iguais << " DIVERSIDADE: " << abs((qnt_iguais / max_iguais) - 1.0) << endl;

    // quando qnt_iguais = 0 => diversidade = 1, quando qnt_iguais = max_iguais => diversidade = 0

    return abs((qnt_iguais / max_iguais) - 1.0);
}

void RecalcularTaxas(vector<Individuo> populacao)
{
    double diversidade = CalcularDiversidade(populacao);
    if (diversidade > 0.5)
    {
        // - mut + rep
        if (prob_mutacao - taxa_aumento_prob > menor_prob_mutacao)
        {
            prob_mutacao -= taxa_aumento_prob;
        }
        if (prob_reproducao + taxa_aumento_prob < maior_prob_reproducao)
        {
            prob_reproducao += taxa_aumento_prob;
        }
    }
    else
    {
        // + mut - rep
        if (prob_mutacao + taxa_aumento_prob < maior_prob_mutacao)
        {
            prob_mutacao += taxa_aumento_prob;
        }
        if (prob_reproducao - taxa_aumento_prob > menor_prob_reproducao)
        {
            prob_reproducao -= taxa_aumento_prob;
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
        cout << "MUTACAO: " << prob_mutacao << ", REPRODUCAO: " << prob_reproducao << endl;
        //if (geracao == 5)   exit(0);
        OrdenarPopulacao(populacao);
        RecalcularTaxas(populacao);

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
