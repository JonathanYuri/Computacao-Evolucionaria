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

struct individuo {
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

void PrintarIndividuo(individuo ind)
{
    cout << "caminho: ";
    for (int i = 0; i < ind.caminho.size(); i++)
    {
        cout << ind.caminho[i] << " ";
    }
    cout << " valor: " << ind.valor;
    cout << endl;
}

void PrintarPopulacao(vector<individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "individuo " << i << endl;
        PrintarIndividuo(populacao[i]);
        cout << endl;
    }
}

individuo GerarIndividuo()
{
    individuo ind;

    vector<int> vertices;
    for (int i = 0; i < VERTICES; i++)
    {
        vertices.push_back(i);
    }

    for (int i = 0; i < VERTICES; i++)
    {
        int pos = rand() % (VERTICES - i);
        
        ind.caminho.push_back(vertices[pos]);
        vertices.erase(vertices.begin() + pos);
    }
    return ind;
}

vector<individuo> GerarPopulacao(int qntIndividuos)
{
    vector<individuo> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        populacao.push_back(GerarIndividuo());
    }
    return populacao;
}

void AvaliarIndividuo(individuo &ind)
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

void AvaliarPopulacao(vector<individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i]);
    }
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
    sort(desempenhos.begin(), desempenhos.end());
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

individuo GerarFilho(individuo pai, individuo mae)
{
    int qntPosicoesHerdadas = 3;
    individuo filho;

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

void ReproduzirPopulacao(vector<individuo> &populacao)
{
    individuo i1 = populacao[0];
    individuo i2 = populacao[1];

    individuo filho = GerarFilho(i1, i2);

    populacao.pop_back();
    populacao.push_back(filho);
}

void MutarIndividuo(individuo &ind)
{
    // escolher as duas posicoes que vao ser trocadas

    // garantindo que as posicoes nao serao iguais
    vector<int> vertices;
    for (int i = 0; i < VERTICES; i++)
    {
        vertices.push_back(i);
    }

    int u = rand() % VERTICES;
    u = vertices[u];
    vertices.erase(vertices.begin() + u);

    int v = rand() % (VERTICES - 1);
    v = vertices[v];

    // trocar
    int aux = ind.caminho[u];
    ind.caminho[u] = ind.caminho[v];
    ind.caminho[v] = aux;
}

void MutarPopulacao(vector<individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        MutarIndividuo(populacao[i]);
    }
}

void CaixeiroViajante(int qntGeracoes)
{
    vector<individuo> populacao = GerarPopulacao(10);
    PrintarPopulacao(populacao);

    individuo melhor;
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