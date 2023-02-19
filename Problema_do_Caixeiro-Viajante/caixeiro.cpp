#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <map>

using namespace std;

int VERTICES = 8;
int UNDEFINED = -1;

struct Individuo {
    vector<int> caminho;
    int valor = 0;
};

// aresta -> peso
map<pair<int, int>, int> grafo;

int RetornarValorDaAresta(int u, int v)
{
    if (u < v)
    {
        return grafo[{u, v}];
    }
    return grafo[{v, u}];
}

vector<int> EscolherNPosicoesDiferentes(int n)
{
    vector<int> posicoes;
    vector<int> vertices;
    for (int i = 0; i < VERTICES; i++)
    {
        vertices.push_back(i);
    }

    for (int i = 0; i < n; i++)
    {
        int pos = rand() % (VERTICES - i);
        
        posicoes.push_back(vertices[pos]);
        vertices.erase(vertices.begin() + pos);
    }

    return posicoes;
}

bool ExisteNoCaminho(vector<int> caminho, int valor)
{
    find(caminho.begin(), caminho.end(), valor) != caminho.end();
}

void AdicionarNoCaminho(vector<int> &caminho, int valor)
{
    for (int i = 0; i < VERTICES; i++)
    {
        if (caminho[i] == UNDEFINED)
        {
            caminho[i] = valor;
            return;
        }
    }
}

void PrintarIndividuo(Individuo ind)
{
    cout << "caminho: ";
    for (int i = 0; i < ind.caminho.size(); i++)
    {
        cout << ind.caminho[i] << " ";
    }
    cout << " valor: " << ind.valor;
    cout << endl;
}

void PrintarPopulacao(vector<Individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "individuo " << i << endl;
        PrintarIndividuo(populacao[i]);
        cout << endl;
    }
}

Individuo GerarIndividuo()
{
    Individuo ind;
    vector<int> caminho = EscolherNPosicoesDiferentes(VERTICES);
    for (int i : caminho)
    {
        ind.caminho.push_back(i);
    }

    return ind;
}

vector<Individuo> GerarPopulacao(int qntIndividuos)
{
    vector<Individuo> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        populacao.push_back(GerarIndividuo());
    }
    return populacao;
}

void AvaliarIndividuo(Individuo &ind)
{
    int custo = 0;
    for (int i = 0; i < ind.caminho.size() - 1; i++)
    {
        //cout << "custo de: " << ind.caminho[i] << " para " << ind.caminho[i + 1] << " = " << grafo[{ind.caminho[i], ind.caminho[i + 1]}] << endl;

        custo += RetornarValorDaAresta(ind.caminho[i], ind.caminho[i + 1]);
    }

    custo += RetornarValorDaAresta(ind.caminho[0], ind.caminho[ ind.caminho.size() - 1 ]);

    ind.valor = custo;
}

void AvaliarPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i]);
    }
}

bool compararPorValor(const Individuo &a, const Individuo &b)
{
    return a.valor < b.valor;
}

void OrdenarPopulacao(vector<Individuo> &populacao)
{
    sort(populacao.begin(), populacao.end(), compararPorValor);
}

Individuo GerarFilho(Individuo pai, Individuo mae)
{
    int qntPosicoesHerdadas = 3;
    Individuo filho;

    for (int i = 0; i < VERTICES; i++)
    {
        filho.caminho.push_back(UNDEFINED);
    }

    // herdar do pai
    int posHerdado = rand() % (VERTICES - qntPosicoesHerdadas);
    for (int i = posHerdado; i < posHerdado + qntPosicoesHerdadas; i++)
    {
        filho.caminho[i] = pai.caminho[i];
    }

    // escolher o resto da mae
    for (int i = 0; i < VERTICES; i++)
    {
        if (!ExisteNoCaminho(filho.caminho, mae.caminho[i]))
        {
            AdicionarNoCaminho(filho.caminho, mae.caminho[i]);
        }
    }

    return filho;
}

void ReproduzirPopulacao(vector<Individuo> &populacao)
{
    Individuo i1 = populacao[0];
    Individuo i2 = populacao[1];

    Individuo filho = GerarFilho(i1, i2);

    populacao.pop_back();
    populacao.push_back(filho);
}

void MutarIndividuo(Individuo &ind)
{
    vector<int> posicoes = EscolherNPosicoesDiferentes(2);

    // trocar
    int aux = ind.caminho[ posicoes[0] ];
    ind.caminho[ posicoes[0] ] = ind.caminho[ posicoes[1] ];
    ind.caminho[ posicoes[1] ] = aux;
}

void MutarPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        MutarIndividuo(populacao[i]);
    }
}

void CaixeiroViajante(int qntGeracoes)
{
    vector<Individuo> populacao = GerarPopulacao(10);
    PrintarPopulacao(populacao);

    Individuo melhor;
    for (int i = 0; i < qntGeracoes; i++)
    {
        //cout << melhor.valor << endl;
        AvaliarPopulacao(populacao);
        OrdenarPopulacao(populacao);

        if (i == 0 || populacao[0].valor < melhor.valor)
        {
            melhor = populacao[0];
        }

        ReproduzirPopulacao(populacao);
        MutarPopulacao(populacao);
        //PrintarPopulacao(populacao);
    }

    cout << "melhor:" << endl;
    PrintarIndividuo(melhor);
}

int main()
{
    srand((unsigned) time(NULL));
    ifstream file;

    file.open("caixeiroEntrada.txt");
    if (!file)
    {
        cout << "Arquivo caixeiroEntrada.txt nao pode ser aberto" << endl;
        abort();
    }

    string linha;
    getline(file, linha);
    VERTICES = stoi(linha);

    while(getline(file, linha))
    {
        stringstream ss(linha);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> tokens(begin, end);

        int u, v, peso;
        u = stoi(tokens[0]);
        v = stoi(tokens[1]);
        peso = stoi(tokens[2]);
        grafo[{u, v}] = peso;
    }

    file.close();

    int qntGeracoes = 0;
    cout << "Digite a quantidade de geracoes: ";
    cin >> qntGeracoes;
    CaixeiroViajante(qntGeracoes);

    return 0;
}