#include <iostream>
#include <vector>

using namespace std;

vector<int> GerarIndividuo(int n)
{
    vector<int> individuo;
    for (int i = 0; i < n; i++)
    {
        individuo.push_back(rand() % 2);
    }

    return individuo;
}

void GerarPopulacao(int qntIndividuos, int n)
{
    vector<pair<int, vector<int>>> populacao;
    for (int i = 0; i < qntIndividuos; i++)
    {
        vector<int> individuo = GerarIndividuo(n);

        for (auto c : individuo)
        {
            cout << c;
        }
        cout << endl;
        populacao.push_back({i, individuo});
    }
}

void AvaliarPopulacao()
{
    
}

void OitoDamas()
{
    int qntIndividuos = 10;
    GerarPopulacao(10, 64);

    AvaliarPopulacao();
}

int main()
{
    OitoDamas();
    return 0;
}
