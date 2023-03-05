#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <map>

using namespace std;

vector<char> str;

vector<pair<int, int>> seq = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

struct Individuo {
    vector<pair<pair<int, int>, char>> cadeia;
    int valor = -1;
};

double prob_mutacao = 0.2;

int PosicaoNaCadeia(vector<pair<pair<int, int>, char>> cadeia, pair<int, int> elem)
{
    for (int i = 0; i < cadeia.size(); i++)
    {
        if (cadeia[i].first == elem)
        {
            return i;
        }
    }
    return -1;
}

bool ExisteNaCadeia(vector<pair<pair<int, int>, char>> cadeia, pair<int, int> elem)
{
    for (const auto &item : cadeia)
    {
        if (item.first == elem)
        {
            return true;
        }
    }
    return false;
}

void PrintarIndividuo(Individuo ind)
{
    for (auto it = ind.cadeia.begin(); it != ind.cadeia.end(); it++)
    {
        cout << "{" << it->first.first << ", " << it->first.second << "}: " << it->second << endl;
    }
    cout << "Valor: " << ind.valor << endl;
}

void PrintarPopulacao(vector<Individuo> populacao)
{
    cout << "-=-=-=-=-=-=-=- POPULACAO -=-=-=-=-=-=-=-" << endl;
    for (auto ind : populacao)
    {
        PrintarIndividuo(ind);
        cout << endl;
    }
    cout << "-=-=-=-=-=-=-=-           -=-=-=-=-=-=-=-" << endl;
}

// GERACAO

bool PosicionarLetra(vector<pair<pair<int, int>, char>> &cadeia, pair<int, int> posAtual, int i)
{
    if (i >= str.size())
    {
        return true;
    }

    vector<pair<int, int>> s = seq;
    int p = rand() % s.size();
    while (s.size() != 0)
    {
        pair<int, int> tentativa;

        tentativa.first = posAtual.first + s[p].first;
        tentativa.second = posAtual.second + s[p].second;

        if (ExisteNaCadeia(cadeia, tentativa))
        {
            s.erase(s.begin() + p);
            p = rand() % s.size();
            continue;
        }
        else
        {
            // colocar e ver se da certo
            vector<pair<pair<int, int>, char>> c = cadeia;
            c.push_back({tentativa, str[i]});
            if (PosicionarLetra(c, tentativa, i + 1))
            {
                cadeia = c;
                return true;
            }
        }
        return false; // nenhum é possivel
    }
}

Individuo GerarIndividuo()
{
    Individuo ind;
    pair<int, int> atual = {0, 0};

    ind.cadeia.push_back({atual, str[0]});
    PosicionarLetra(ind.cadeia, atual, 1);
    
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

// AVALIACAO

bool IgualAH(vector<pair<pair<int, int>, char>> cadeia, pair<int, int> posicao)
{
    int pos = PosicaoNaCadeia(cadeia, posicao);
    if (pos != -1)
    {
        return cadeia[pos].second == 'H';
    }

    return false;
}

int QuantidadeDeHsVizinhos(vector<pair<pair<int, int>, char>> cadeia, pair<int, int> posicao)
{
    int h = 0;
    pair<int, int> p = posicao;

    if (IgualAH(cadeia, {p.first + 1, p.second}))   h++;
    
    if (IgualAH(cadeia, {p.first - 1, p.second}))   h++;

    if (IgualAH(cadeia, {p.first, p.second + 1}))   h++;

    if (IgualAH(cadeia, {p.first, p.second - 1}))   h++;

    return h;
}

void AvaliarIndividuo(Individuo &ind)
{
    int valor = 0;
    // quantidade de H's vizinhos de H's
    for (auto it = ind.cadeia.begin(); it != ind.cadeia.end(); it++)
    {
        if (it->second == 'H')
        {
            valor += QuantidadeDeHsVizinhos(ind.cadeia, it->first);
        }
    }

    ind.valor = valor;
}

void AvaliarPopulacao(vector<Individuo> &populacao)
{
    for (auto &ind : populacao)
    {
        AvaliarIndividuo(ind);
    }
}

// ORDENAR

bool compararPorValor(const Individuo &a, const Individuo &b)
{
    return a.valor > b.valor;
}

void OrdenarPopulacao(vector<Individuo> &populacao)
{
    sort(populacao.begin(), populacao.end(), compararPorValor);
}

// REPRODUZIR

vector<pair<int, int>> TransladarPosicoes(vector<pair<pair<int, int>, char>> cadeia, int pos)
{
    vector<pair<int, int>> transladados;
    pair<int, int> origem = cadeia[pos].first;
    for (int i = pos + 1; i < cadeia.size(); i++)
    {
        pair<int, int> atual = cadeia[i].first;
        atual.first -= origem.first;
        atual.second -= origem.second;

        transladados.push_back(atual);
    }
    return transladados;
}

Individuo GerarFilho(Individuo pai, Individuo mae)
{
    Individuo filho;

    int corte = (rand() % (str.size() - 2)) + 1; // tambem indica a pos do vetor cadeia que esta

    vector<pair<int, int>> posicoesTransladadasPai = TransladarPosicoes(pai.cadeia, corte);

    // cout << "PAI" << endl;
    // PrintarIndividuo(pai);

    // cout << "MAE" << endl;
    // PrintarIndividuo(mae);
    // cout << "CORTE: " << corte << endl;
    
    // cout << "TRANSLADADOS PAI" << endl;
    // for (auto p : posicoesTransladadasPai)
    // {
    //     cout << p.first << ", " << p.second << endl;
    // }

    for (int i = 0; i <= corte; i++)
    {
        filho.cadeia.push_back(mae.cadeia[i]);
    }

    int t = 0;
    pair<int, int> posicao = mae.cadeia[corte].first;
    for (int i = corte + 1; i < str.size(); i++)
    {
        pair<int, int> elem = {posicao.first + posicoesTransladadasPai[t].first, posicao.second + posicoesTransladadasPai[t].second};
        if (ExisteNaCadeia(filho.cadeia, elem))
        {
            //cout << "MONSTRO" << endl;
            return filho;
        }
        else
        {
            filho.cadeia.push_back({elem, str[i]});
            t++;
        }
    }

    AvaliarIndividuo(filho);
    return filho;
}

void ReproduzirPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i+1; j < populacao.size(); j++)
        {
            Individuo pai = populacao[i];
            Individuo mae = populacao[j];

            Individuo filho1 = GerarFilho(pai, mae);
            if (filho1.valor == -1)
            {
                // monstro
                continue;
            }

            Individuo filho2 = GerarFilho(mae, pai);
            if (filho2.valor == -1)
            {
                // monstro
                continue;
            }

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

// MUTAR

void MutarIndividuo(Individuo &ind)
{
    int pos = rand() % str.size();
    //cout << "pos mutar: " << pos << endl;

    vector<pair<int, int>> posicoesVizinhasAo1;
    vector<pair<int, int>> posicoesVizinhasAo2;
    if (pos - 1 >= 0)
    {
        pair<int, int> vizinho1 = ind.cadeia[pos - 1].first;
        if (!ExisteNaCadeia(ind.cadeia, {vizinho1.first + 1, vizinho1.second}))
        {
            posicoesVizinhasAo1.push_back({vizinho1.first + 1, vizinho1.second});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho1.first - 1, vizinho1.second}))
        {
            posicoesVizinhasAo1.push_back({vizinho1.first - 1, vizinho1.second});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho1.first, vizinho1.second + 1}))
        {
            posicoesVizinhasAo1.push_back({vizinho1.first, vizinho1.second + 1});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho1.first, vizinho1.second - 1}))
        {
            posicoesVizinhasAo1.push_back({vizinho1.first, vizinho1.second - 1});
        }
    }
    if (pos + 1 < str.size())
    {
        pair<int, int> vizinho2 = ind.cadeia[pos + 1].first;
        if (!ExisteNaCadeia(ind.cadeia, {vizinho2.first + 1, vizinho2.second}))
        {
            posicoesVizinhasAo2.push_back({vizinho2.first + 1, vizinho2.second});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho2.first - 1, vizinho2.second}))
        {
            posicoesVizinhasAo2.push_back({vizinho2.first - 1, vizinho2.second});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho2.first, vizinho2.second + 1}))
        {
            posicoesVizinhasAo2.push_back({vizinho2.first, vizinho2.second + 1});
        }
        if (!ExisteNaCadeia(ind.cadeia, {vizinho2.first, vizinho2.second - 1}))
        {
            posicoesVizinhasAo2.push_back({vizinho2.first, vizinho2.second - 1});
        }
    }

    // so tenho o vizinho1
    if (pos - 1 >= 0 && pos + 1 >= str.size() && posicoesVizinhasAo1.size() != 0)
    {
        ind.cadeia[pos].first = posicoesVizinhasAo1[0];
        //cout << "MUTOU POR VIZINHO1" << endl;
        return;
    }

    // so tenho o vizinho2
    if (pos - 1 < 0 && pos + 1 < str.size() && posicoesVizinhasAo2.size() != 0)
    {
        ind.cadeia[pos].first = posicoesVizinhasAo2[0];
        //cout << "MUTOU POR VIZINHO2" << endl;
        return;
    }

    if (posicoesVizinhasAo1.size() == 0 || posicoesVizinhasAo2.size() == 0)
    {
        //cout << "VAZEI DA MUTACAO" << endl;
        return;
    }

    // se eles tem posicoes em comum posso colocar para lá que vai continuar se mantendo vizinhos
    for (auto p : posicoesVizinhasAo1)
    {
        if (find(posicoesVizinhasAo2.begin(), posicoesVizinhasAo2.end(), p) != posicoesVizinhasAo2.end())
        {
            ind.cadeia[pos].first = p;
            //cout << "MUTEI" << endl;
            return;
        }
    }
}

void MutarPopulacao(vector<Individuo> &populacao)
{
    for (auto &ind : populacao)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < prob_mutacao)
        {
            MutarIndividuo(ind);
        }
    }
}

void HP(int qntIndividuos, int qntGeracoes)
{
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);
    //PrintarPopulacao(populacao);

    for (int i = 0; i < qntGeracoes; i++)
    {
        cout << "Geracao: " << i << endl;
        AvaliarPopulacao(populacao);

        OrdenarPopulacao(populacao);

        ReproduzirPopulacao(populacao);

        MutarPopulacao(populacao);
    }

    OrdenarPopulacao(populacao);
    cout << "INDIVIDUO MAIS EVOLUIDO:" << endl;
    PrintarIndividuo(populacao[0]);
}

int main()
{
    srand((unsigned) time(NULL));

    cout << "DIGITE A QUANTIDADE DE GERACOES: ";
    int qntGeracoes = 0;
    cin >> qntGeracoes;

    cout << "DIGITE A STRING: ";
    string s;
    cin >> s;

    for (auto c : s)
    {
        str.push_back(toupper(c));
    }

    HP(10, qntGeracoes);

    return 0;
}