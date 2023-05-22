#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

vector<string> cadeia;

struct Individuo {
    vector<string> fita;
    int valor;
};

double prob_reproducao = 0.8;
double prob_mutacao = 0.5;

void PrintarIndividuo(Individuo individuo)
{
    for (int i = 0; i < individuo.fita.size(); i++)
    {
        cout << individuo.fita[i] << endl;
    }
    cout << "valor: " << individuo.valor << endl;
}

void PrintarPopulacao(vector<Individuo> populacao)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        cout << "individuo: " << i << endl;
        PrintarIndividuo(populacao[i]);
        cout << endl;
    }
}

void MutarFita(string &fita)
{
    vector<int> posicoesDosTracos;
    for (int i = 0; i < fita.size(); i++)
    {
        if (fita[i] == '-')
        {
            posicoesDosTracos.push_back(i);
        }
    }

    if (posicoesDosTracos.size() == 0)
    {
        return;
    }

    // escolher - para mutar
    int f = rand() % posicoesDosTracos.size();

    // escolher posicao para mandar
    int p = rand() % fita.size();

    fita.erase(fita.begin() + posicoesDosTracos[f]);
    fita.insert(fita.begin() + p, '-');
}

int AvaliarFitas(string fita1, string fita2)
{
    int valor = 0;
    for (int c = 0; c < fita1.size(); c++)
    {
        if (fita1[c] == '-' || fita2[c] == '-')
        {
            valor += 0;
            continue;
        }
        
        if (fita1[c] == fita2[c])
        {
            valor += 1;
            continue;
        }

        if (fita1[c] != fita2[c])
        {
            valor -= 1;
            continue;
        }
    }
    return valor;
}

void AvaliarIndividuo(Individuo &ind)
{
    int valor = 0;
    for (int i = 0; i < ind.fita.size(); i++)
    {
        for (int j = i + 1; j < ind.fita.size(); j++)
        {
            valor += AvaliarFitas(ind.fita[i], ind.fita[j]);
        }
    }
    ind.valor = valor;
}

Individuo GerarIndividuo()
{
    Individuo ind;
    for (int i = 0; i < cadeia.size(); i++)
    {
        string fita = cadeia[i];
        MutarFita(fita);
        ind.fita.push_back(fita);
    }

    AvaliarIndividuo(ind);
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

bool compararPorValor(const Individuo& a, const Individuo &b)
{
    return a.valor > b.valor;
}

void OrdenarPopulacao(vector<Individuo> &populacao)
{
    sort(populacao.begin(), populacao.end(), compararPorValor);
}

Individuo GerarFilho(Individuo pai, Individuo mae)
{
    Individuo filho;
    int posCorteFita = rand() % (pai.fita.size() - 1);

    for (int i = 0; i <= posCorteFita; i++)
    {
        filho.fita.push_back(pai.fita[i]);
    }

    for (int i = posCorteFita + 1; i < mae.fita.size(); i++)
    {
        filho.fita.push_back(mae.fita[i]);
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
                Individuo filho1 = GerarFilho(populacao[i], populacao[j]);
                Individuo filho2 = GerarFilho(populacao[j], populacao[i]);

                AvaliarIndividuo(filho1);
                AvaliarIndividuo(filho2);

                // cout << "PAI: ";
                // PrintarIndividuo(populacao[i]);
                // cout << endl << "MAE: ";
                // PrintarIndividuo(populacao[j]);

                // cout << endl << "FILHO1: ";
                // PrintarIndividuo(filho1);

                // cout << endl << "FILHO2: ";
                // PrintarIndividuo(filho2);

                // exit(0);

                if (filho1.valor > populacao[i].valor) {
                    populacao[i] = filho1;
                }
                if (filho2.valor > populacao[j].valor) {
                    populacao[j] = filho2;
                }
            }
        }
    }
}

void MutarIndividuo(Individuo &ind)
{
    // escolher qual fita mutar
    int fita = rand() % ind.fita.size();
    MutarFita(ind.fita[fita]);
    AvaliarIndividuo(ind);
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

void Dna(int qntIndividuos, int qntGeracoes)
{
    vector<Individuo> populacao = GerarPopulacao(qntIndividuos);
    Individuo melhor = populacao[0];

    for (int i = 0; i < qntGeracoes; i++)
    {
        cout << "I: " << i << endl;
        OrdenarPopulacao(populacao);
        if (populacao[0].valor > melhor.valor) {
            melhor = populacao[0];
        }

        ReproduzirPopulacao(populacao);
        MutarPopulacao(populacao);
    }

    PrintarIndividuo(melhor);
}

string upper(string fita)
{
    for (char &c : fita) {
        c = toupper(c);
    }
    return fita;
}

bool isValid(string fita)
{
    for (char c : fita) {
        if (c != 'A' && c != 'T' && c != 'G' && c != 'C') {
            return false;
        }
    }
    return true;
}

int lerInteiro(string msg)
{
    int num;
    bool numeroValido = false;
    while (!numeroValido) {
        cout << msg;
        cin >> num;

        if (cin.fail()) {
            cout << "Entrada invalida. Digite um numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            numeroValido = true;
        }
    }
    return num;
}

int main()
{
    srand((unsigned) time(NULL));
    int qntFitas = lerInteiro("Digite a quantidade de fitas que terao: ");

    for (int i = 0; i < qntFitas; i++)
    {
        cout << "Digite a cadeia da fita [" << i << "]: ";
        string str;
        cin >> str;

        str = upper(str);
        if (!isValid(str)) {
            cout << "A cadeia so pode ter as letras A T G C";
            return 0;
        }

        cadeia.push_back(str);
    }

    int posMaiorCadeia = 0;
    int qntMaiorCadeia = 0;
    for (int i = 0; i < cadeia.size(); i++)
    {
        if (cadeia[i].size() > qntMaiorCadeia) {
            qntMaiorCadeia = cadeia[i].size();
            posMaiorCadeia = i;
        }
    }

    // maior cadeia + 50 % de -
    qntMaiorCadeia += (int)(qntMaiorCadeia / 2);

    for (int i = 0; i < cadeia.size(); i++)
    {
        while (cadeia[i].size() < qntMaiorCadeia)
        {
            cadeia[i].push_back('-');
        }
    }

    //cadeia.push_back("ATAGCCT-");
    //cadeia.push_back("TACGT---");
    
    Dna(10, 1000);

    return 0;
}