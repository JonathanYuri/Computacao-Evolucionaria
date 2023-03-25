#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

vector<char> str;

vector<pair<int, int>> seq = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

struct Individuo {
    vector<pair<pair<int, int>, char>> cadeia;
    int valor = -1;
    double prob_mutacao = 0.5;
    double prob_reproducao = 0.5;
};

ofstream arquivo;

double taxa_aumento_prob = 0.1;

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

void PrintarMatriz(vector<pair<pair<int, int>, char>> cadeia)
{
    int menorX, maiorX, menorY, maiorY;

    for (int i = 0; i < cadeia.size(); i++)
    {
        if (i == 0)
        {
            maiorX = cadeia[i].first.first;
            menorX = cadeia[i].first.first;

            maiorY = cadeia[i].first.second;
            menorY = cadeia[i].first.second;
            continue;
        }

        if (cadeia[i].first.first > maiorX) maiorX = cadeia[i].first.first;
        if (cadeia[i].first.second > maiorY) maiorY = cadeia[i].first.second;

        if (cadeia[i].first.first < menorX) menorX = cadeia[i].first.first;
        if (cadeia[i].first.second < menorY) menorY = cadeia[i].first.second;
    }

    arquivo << menorX << " " << maiorX << " " << menorY << " " << maiorY << endl;

    for (int j = maiorY; j >= menorY; j--)
    {
        //cout << " - ";
        for (int i = menorX; i <= maiorX; i++)
        {
            int pos = PosicaoNaCadeia(cadeia, {i, j});
            if (pos != -1) // existe na cadeia
            {
                cout << "(" << i << ", " << j << "): " << cadeia[pos].second;
                //cout << cadeia[pos].second;
                arquivo << cadeia[pos].second << " ";
            }
            else
            {
                cout << "(" << i << ", " << j << "): X";
                //cout << "X";
                arquivo << "X"<< " ";
            }
        }
        cout << endl;
        arquivo << endl;
    }
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

    ind.prob_mutacao = ((double) rand() / ((double)RAND_MAX + 1));
    ind.prob_reproducao = ((double) rand() / ((double)RAND_MAX + 1));
    
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

    filho.prob_mutacao = ((double) rand() / ((double)RAND_MAX + 1));
    filho.prob_reproducao = ((double) rand() / ((double)RAND_MAX + 1));

    AvaliarIndividuo(filho);
    return filho;
}

void ReproduzirPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i+1; j < populacao.size(); j++)
        {
            double prob = ((double) rand() / ((double)RAND_MAX + 1));
            double prob_rep = (populacao[i].prob_reproducao + populacao[j].prob_reproducao) / 2;
            if (prob < prob_rep)
            {
                Individuo pai = populacao[i];
                Individuo mae = populacao[j];

                Individuo filho1 = GerarFilho(pai, mae);
                Individuo filho2 = GerarFilho(mae, pai);

                if (filho1.valor != -1)
                {
                    if (filho1.valor > populacao[i].valor)
                    {
                        populacao[i] = filho1;
                    }
                }
                if (filho2.valor != -1)
                {
                    if (filho2.valor > populacao[j].valor)
                    {
                        populacao[j] = filho2;
                    }
                }
            }
        }
    }
}

// MUTAR

void MutarIndividuo(Individuo &ind)
{
    int pos = (rand() % (str.size() - 1)) + 1;
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
        if (prob < ind.prob_mutacao)
        {
            MutarIndividuo(ind);
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

void RecalcularTaxas(vector<Individuo> &populacao)
{
    int min = 0, max = 0;
    for (auto it = populacao.begin(); it != populacao.end(); ++it) {
        if (it == populacao.begin() || it->valor < min) {
            min = it->valor;
        }

        if (it == populacao.begin() || it->valor > max) {
            max = it->valor;
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

void HP(int qntIndividuos, int qntGeracoes)
{
    ofstream arquivo;
    arquivo.open("Problema_HP/pontos-AAE2.txt");
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);
    int melhor = 0;
    //PrintarPopulacao(populacao);

    for (int i = 0; i < qntGeracoes; i++)
    {
        melhor = AvaliarPopulacao(populacao);
        //cout << "Geracao: " << i << ", Maior Avaliacao: " << melhor << endl;

        double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
        arquivo << i << " " << adaptacaoMedia << endl;

        RecalcularTaxas(populacao);

        ReproduzirPopulacao(populacao);

        MutarPopulacao(populacao);
    }

    OrdenarPopulacao(populacao);

    double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
    arquivo << qntGeracoes << " " << adaptacaoMedia << endl;

    cout << "INDIVIDUO MAIS EVOLUIDO:" << endl;
    PrintarIndividuo(populacao[0]);
    PrintarMatriz(populacao[0].cadeia);

    arquivo.close();
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

    arquivo.open("Problema_HP/matrix.txt");

    HP(10, qntGeracoes);

    arquivo.close();

    return 0;
}