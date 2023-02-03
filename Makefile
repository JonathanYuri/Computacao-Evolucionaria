all:
	g++ Problema_das_N-damas/Algoritmo_Genetico/ndamas.cpp -o ndamasG.exe
	g++ Problema_das_N-damas/Backtracking/ndamas.cpp -o ndamas.exe

clean:
	-del -rf *.exe