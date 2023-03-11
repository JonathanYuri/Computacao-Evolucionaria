#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cmath>
#include <stack>

#include "./gramatica.cpp"

using namespace std;

struct Individuo {
    No inicial;
    vector<string> expressao;
    long long int valor = 0;
    int quantidadeNos = 0;
};

int OBJETIVO = 0;
double prob_mutacao = 0.2;
double prob_reproducao = 0.5;

double taxa_aumento_prob = 0.1;

void PrintarNo(No no)
{
    cout << "NO: " << no.atual << " VALOR: " << no.valor << endl;
    for (auto no : no.prox)
    {
        PrintarNo(no);
    }
}

void PrintarIndividuo(Individuo ind)
{
    //PrintarNo(ind.inicial);
    cout << "Expressao: ";
    for (auto expr : ind.expressao)
    {
        cout << expr << " ";
    }
    cout << endl;
}

void PrintarPopulacao(vector<Individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "Individuo " << i << endl;
        PrintarIndividuo(populacao[i]);
        cout << "Valor: " << populacao[i].valor << endl;
    }
}

void VerificarExpressao(Individuo &ind, No no, int &size)
{
    size++;

    if (no.prox.size() == 0)
    {
        ind.expressao.push_back(no.valor);
        return;
    }

    for (auto n : no.prox)
    {
        VerificarExpressao(ind, n, size);
    }
}

Individuo GerarIndividuo()
{
    Individuo i;
    S(i.inicial, 0);

    int qntNos = 0;
    VerificarExpressao(i, i.inicial, qntNos);
    i.quantidadeNos = qntNos;

    return i;
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

string CalcularExpressao(string valor1, string operador, string valor2)
{
    long long int v1 = stoll(valor1);
    long long int v2 = stoll(valor2);

    if (operador.compare("+") == 0)
    {
        return to_string(v1 + v2);
    }
    else if (operador.compare("-") == 0)
    {
        return to_string(v1 - v2);
    }
    else if (operador.compare("*") == 0)
    {
        return to_string(v1 * v2);
    }
}

bool IsAOperator(string str)
{
    return str.compare("+") == 0 || str.compare("-") == 0 || str.compare("*") == 0;
}

bool IsANumber(string str)
{
    for (auto c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

long long int CalcularValor(vector<string> expr)
{
    stack<string> valores;
    stack<string> operadores;

    for (int i = 0; i < expr.size(); i++)
    {
        if (IsAOperator(expr[i]))
        {
            operadores.push(expr[i]);
        }
        else if (expr[i].compare("(") == 0)
        {
            operadores.push(expr[i]);
        }
        else if (IsANumber(expr[i]))
        {
            valores.push(expr[i]);
        }
        else if (expr[i].compare(")") == 0)
        {
            while (!operadores.empty() && operadores.top().compare("(") != 0)
            {
                string valor1, valor2;
                valor1 = valores.top(); valores.pop();
                valor2 = valores.top(); valores.pop();

                string op = operadores.top(); operadores.pop();

                string valor = CalcularExpressao(valor2, op, valor1);
                valores.push(valor);
            }
            operadores.pop();
        }
    }

    string valor1, valor2;
    valor1 = valores.top(); valores.pop();
    valor2 = valores.top(); valores.pop();

    string op = operadores.top(); operadores.pop();
    string valor = CalcularExpressao(valor2, op, valor1);
    valores.push(valor);
    
    return stoll(valores.top());
}

void AvaliarIndividuo(Individuo &ind)
{
    long long int valor = CalcularValor(ind.expressao);
    ind.valor = abs(valor - OBJETIVO);
}

long long int AvaliarPopulacao(vector<Individuo> &populacao)
{
    long long int melhor;
    for (int i = 0; i < populacao.size(); i++)
    {
        AvaliarIndividuo(populacao[i]);

        if (i == 0)
        {
            melhor = populacao[i].valor;
        }
        else
        {
            if (populacao[i].valor < melhor)
            {
                melhor = populacao[i].valor;
            }
        }
    }
    return melhor;
}

bool compararPorValor(const Individuo& a, const Individuo& b) {
    return a.valor < b.valor;
}

void OrdenarPopulacao(vector<Individuo> &populacao)
{
    sort(populacao.begin(), populacao.end(), compararPorValor);
}

No PegarNoNaPos(No atual, int pos, int &posAtual)
{
    posAtual++;
    if (posAtual == pos)
    {
        return atual;
    }
    for (auto n : atual.prox)
    {
        No no = PegarNoNaPos(n, pos, posAtual);
        if (no.atual.compare("NULL") != 0)
        {
            return no;
        }
    }

    No no;
    no = CriarNo("NULL", "NULL", {});
    return no;
}

bool SubstituirNoDaPos(No &atual, No substituir, int pos, int &posAtual)
{
    posAtual++;
    if (posAtual == pos)
    {
        atual = substituir;
        return true;
    }
    for (No &prox : atual.prox)
    {
        if (SubstituirNoDaPos(prox, substituir, pos, posAtual))
        {
            return true;
        }
    }
    return false;
}

Individuo GerarFilho(Individuo pai, Individuo mae)
{
    Individuo filho;

    int menorNos = pai.quantidadeNos;
    if (mae.quantidadeNos < menorNos)
    {
        menorNos = mae.quantidadeNos;
    }

    int posNo = rand() % menorNos;

    int p = 0;  No noDaMae = PegarNoNaPos(mae.inicial, posNo, p);
    p = 0;  No noDoPai = PegarNoNaPos(pai.inicial, posNo, p);

    int qntNos = 0;
    
    if (IsAOperator(noDoPai.valor) && IsAOperator(noDaMae.valor) ||
        IsANumber(noDoPai.valor) && IsANumber(noDaMae.valor))
    {
        filho.inicial = pai.inicial;

        p = 0;  SubstituirNoDaPos(filho.inicial, noDaMae, posNo, p);
        qntNos = 0; VerificarExpressao(filho, filho.inicial, qntNos);

        filho.quantidadeNos = qntNos;
        AvaliarIndividuo(filho);
    }

    return filho;
}

void ReproduzirPopulacao(vector<Individuo> &populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i + 1; j < populacao.size(); j++)
        {
            double prob = ((double) rand() / ((double)RAND_MAX + 1));
            if (prob < prob_reproducao)
            {
                Individuo pai = populacao[i];
                Individuo mae = populacao[j];

                Individuo filho1 = GerarFilho(pai, mae);
                Individuo filho2 = GerarFilho(mae, pai);

                if (filho1.quantidadeNos != 0)
                {
                    if (filho1.valor < pai.valor)
                    {
                        populacao[i] = filho1;
                    }
                }
                if (filho2.quantidadeNos != 0)
                {
                    if (filho2.valor < mae.valor)
                    {
                        populacao[j] = filho2;
                    }
                }
            }
        }
    }
}

void QuantidadeDeMutaveis(No no, int &qntMutavel)
{
    if (no.atual.compare(VALOR) == 0 || no.atual.compare(OPERADOR) == 0)
    {
        qntMutavel++;
        return;
    }

    for (auto n : no.prox)
    {
        QuantidadeDeMutaveis(n, qntMutavel);
    }
}

bool MutarNo(No &no, int posMutar, int &pos)
{
    if (no.atual.compare(VALOR) == 0 || no.atual.compare(OPERADOR) == 0)
    {
        pos++;
    }

    if (pos == posMutar)
    {
        if (no.atual.compare(VALOR) == 0)
        {
            vector<int> numeros = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            numeros.erase(numeros.begin() + stoi(no.valor));
            no.valor = to_string(numeros[rand() % 9]);
        }

        else if (no.atual.compare(OPERADOR) == 0)
        {
            vector<string> operadores = {"+", "*", "-"};
            for (int i = 0; i < operadores.size(); i++)
            {
                if (operadores[i].compare(no.valor) == 0)
                {
                    operadores.erase(operadores.begin() + i);
                    break;
                }
            }

            int pos = rand() % 2;
            no.valor = operadores[pos];
        }
        return true;
    }

    for (auto &n : no.prox)
    {
        if (MutarNo(n, posMutar, pos))
        {
            return true;
        }
    }
    return false;
}

void MutarIndividuo(Individuo &ind)
{
    int qntMutavel = 0; QuantidadeDeMutaveis(ind.inicial, qntMutavel);

    int posMutar = rand() % qntMutavel;

    int p = 0;  MutarNo(ind.inicial, posMutar, p);
    p = 0;  ind.expressao = {};

    VerificarExpressao(ind, ind.inicial, p);
}

void MutarPopulacao(vector<Individuo> &populacao)
{
    for (Individuo &ind : populacao)
    {
        double prob = ((double) rand() / ((double)RAND_MAX + 1));
        if (prob < prob_mutacao)
        {
            MutarIndividuo(ind);
        }
    }
}

double CalcularDiversidade(vector<Individuo> populacao)
{
    double qnt_iguais = 0;
    double max_iguais = 0;
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = i + 1; j < populacao.size(); j++)
        {
            if (populacao[i].expressao == populacao[j].expressao)
            {
                qnt_iguais++;
            }
            max_iguais++;
        }
    }

    return abs((qnt_iguais / max_iguais) - 1.0);
}

void RecalcularTaxas(vector<Individuo> populacao)
{
    double diversidade = CalcularDiversidade(populacao);
    if (diversidade > 0.5)
    {
        // - mut + rep
        if (prob_mutacao - taxa_aumento_prob > 0)
        {
            prob_mutacao -= taxa_aumento_prob;
        }
        if (prob_reproducao + taxa_aumento_prob < 1)
        {
            prob_reproducao += taxa_aumento_prob;
        }
    }
    else
    {
        // + mut - rep
        if (prob_mutacao + taxa_aumento_prob < 1)
        {
            prob_mutacao += taxa_aumento_prob;
        }
        if (prob_reproducao - taxa_aumento_prob > 0)
        {
            prob_reproducao -= taxa_aumento_prob;
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

void AcharExpressaoMatematica(int qntIndividuos)
{
    ofstream arquivo;
    arquivo.open("Problema_da_Expressao-Matematica/pontos-ADP.txt");
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);
    PrintarPopulacao(populacao);

    long long int melhor = AvaliarPopulacao(populacao);
    int geracao = 0;
    while (melhor != 0)
    {
        //cout << "MELHOR: " << melhor << endl;

        // Ordenar
        OrdenarPopulacao(populacao);

        double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
        arquivo << geracao << " " << adaptacaoMedia << endl;

        RecalcularTaxas(populacao);

        // Reproduzir
        ReproduzirPopulacao(populacao);

        // Mutar
        MutarPopulacao(populacao);

        // Avaliar
        melhor = AvaliarPopulacao(populacao);
        geracao++;
    }
    cout << "ACHEI A EXPRESSAO em " << geracao << " geracoes" << endl;
    OrdenarPopulacao(populacao);

    double adaptacaoMedia = CalcularAdaptacaoMedia(populacao);
    arquivo << geracao << " " << adaptacaoMedia << endl;

    PrintarIndividuo(populacao[0]);
    arquivo.close();
}

int main()
{
    srand((unsigned) time(NULL));

    prob_mutacao = ((double) rand() / ((double)RAND_MAX + 1));
    prob_reproducao = ((double) rand() / ((double)RAND_MAX + 1));

    int qntIndividuos = 10;
    cout << "Digite o valor que deseja achar uma expressao matematica: ";
    cin >> OBJETIVO;
    cout << endl;

    AcharExpressaoMatematica(qntIndividuos);

    return 0;
}