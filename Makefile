all:
	g++ Problema_das_N-damas/Algoritmo_Genetico/ndamas.cpp -o ndamasG.exe
	g++ Problema_das_N-damas/Algoritmo_Genetico/ndamasv2.cpp -o ndamasG2.exe
	g++ Problema_das_N-damas/Backtracking/ndamas.cpp -o ndamas.exe

caixeiro:
	g++ Problema_do_Caixeiro-Viajante/caixeiro.cpp -o caixeiro.exe

clean:
	-del -rf *.exe