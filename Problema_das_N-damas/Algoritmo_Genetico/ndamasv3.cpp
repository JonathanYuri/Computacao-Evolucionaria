#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

int SIZE = 8;
double taxa_aumento_prob = 0.1;

struct Individuo {
    vector<int> posicaoNaLinha;
    int valor = 0;
    double prob_mutacao;
    double prob_reproducao;
};

Individuo GerarIndividuo()
{
    Individuo ind;
    
    vector<int> possibilidades;
    for (int i = 0; i < SIZE; i++)
    {
        possibilidades.push_back(i);
    }

    for (int i = 0; i < SIZE; i++)
    {
        int escolha = rand() % (SIZE - i);
        ind.posicaoNaLinha.push_back(possibilidades[escolha]);
        possibilidades.erase(possibilidades.begin() + escolha);
    }

    return ind;
}

void PrintarIndividuo(Individuo ind)
{
    cout << "Damas: {linha, coluna}" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << "{" << i << ", " << ind.posicaoNaLinha[i] << "}" << endl;
    }
    cout << endl;
    cout << "prob mutacao: " << ind.prob_mutacao << endl;
    cout << "prob reproducao: " << ind.prob_reproducao << endl;
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
        ind.prob_mutacao = ((double) rand() / ((double)RAND_MAX + 1));
        ind.prob_reproducao = ((double) rand() / ((double)RAND_MAX + 1));
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
        damas.push_back({i, ind.posicaoNaLinha[i]});
    }

    int ameacasADamas = ContarAmeacas(damas);

    int penalidade = ameacasADamas * SIZE;
    ind.valor = damas.size() - penalidade;
}

int AvaliarPopulacao(vector<Individuo> &populacao)
{
    int melhor = 0;
    for (auto it = populacao.begin(); it != populacao.end(); ++it) {
        AvaliarIndividuo(*it);

        if (it == populacao.begin() || it->valor > melhor) {
            melhor = it->valor;
        }
    }
    return melhor;
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
    // de 1 a SIZE - 1 para pegar parte de um e parte do outro
    int corte = rand() % (SIZE - 1) + 1;
    // rand() % 63 -> 0 a 62 + 1 ->     1 a 63

    int qnt = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (qnt < corte)
        {
            filho.posicaoNaLinha.push_back(pai.posicaoNaLinha[i]);
        }
        else
        {
            filho.posicaoNaLinha.push_back(mae.posicaoNaLinha[i]);
        }
        qnt++;
    }

    filho.prob_mutacao = ((double) rand() / ((double)RAND_MAX + 1));
    filho.prob_reproducao = ((double) rand() / ((double)RAND_MAX + 1));

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
            double prob_rep = (populacao[i].prob_reproducao + populacao[j].prob_reproducao) / 2;
            if (prob < prob_rep)
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
    int mutar = rand() % SIZE;

    i.posicaoNaLinha[mutar] = rand() % SIZE;
}

void MutarPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < populacao[i].prob_mutacao)
        {
            MutarIndividuo(populacao[i]);
        }
    }
}

void RecalcularTaxas(vector<Individuo> &populacao)
{
    int min = 0, max = 0;
    for (int i = 0; i < populacao.size(); i++)
    {
        if (i == 0) {
            min = populacao[i].valor;
            max = populacao[i].valor;
        }
        else {
            if (populacao[i].valor < min) {
                min = populacao[i].valor;
            }
            if (populacao[i].valor > max) {
                max = populacao[i].valor;
            }
        }
    }

    int media = (max + min) / 2;

    for (Individuo &ind : populacao)
    {
        if (ind.valor > media)
        {
            if (ind.prob_reproducao + taxa_aumento_prob < 1)
            {
                ind.prob_reproducao += taxa_aumento_prob;
            }
            if (ind.prob_mutacao - taxa_aumento_prob > 0)
            {
                ind.prob_mutacao -= taxa_aumento_prob;
            }
        }
        else
        {
            if (ind.prob_mutacao + taxa_aumento_prob < 1)
            {
                ind.prob_mutacao += taxa_aumento_prob;
            }
            if (ind.prob_reproducao - taxa_aumento_prob > 0)
            {
                ind.prob_reproducao -= taxa_aumento_prob;
            }
        }
    }
}

double CalcularAdaptacaoMedia(vector<Individuo> populacao)
{
    double media = 0;
    for (Individuo ind : populacao)
    {
        media += ind.valor;
    }
    media /= populacao.size();
    return media;
}

void NDamas(int qntIndividuos)
{
    ofstream arquivo;
    arquivo.open("Problema_das_N-damas/Algoritmo_Genetico/pontos-AAE2.txt");
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);

    int maiorAvaliacao = AvaliarPopulacao(populacao);
    int geracao = 0;
    while (maiorAvaliacao != SIZE)
    {
        //cout << maiorAvaliacao << endl;

        double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
        arquivo << geracao << " " << adaptacaoMedia << endl;
        RecalcularTaxas(populacao);

        Reproduzir(populacao);
        MutarPopulacao(populacao);
        maiorAvaliacao = AvaliarPopulacao(populacao);
        geracao++;
    }

    OrdenarPopulacao(populacao);

    double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
    arquivo << geracao << " " << adaptacaoMedia << endl;

    cout << "geracoes: " << geracao << endl << "individuo:" << endl;
    PrintarIndividuo(populacao[0]);
    arquivo.close();
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
