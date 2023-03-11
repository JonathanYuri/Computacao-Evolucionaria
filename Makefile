NDAMAS_DIR = ./Problema_das_N-damas
CAIXEIRO_DIR = ./Problema_do_Caixeiro-Viajante
EXPRESSAO_DIR = ./Problema_da_Expressao-Matematica
HP_DIR = ./Problema_HP

ndamas:
	g++ $(NDAMAS_DIR)/Algoritmo_Genetico/ndamas.cpp -o ndamasG.exe
	g++ $(NDAMAS_DIR)/Algoritmo_Genetico/ndamasv2.cpp -o ndamasG2.exe
	g++ $(NDAMAS_DIR)/Backtracking/ndamas.cpp -o ndamas.exe

	./ndamasG.exe
	./ndamasG2.exe
	python3 grafico.py $(NDAMAS_DIR)/Algoritmo_Genetico/pontos.txt $(NDAMAS_DIR)/Algoritmo_Genetico/pontos-ADP.txt

caixeiro:
	g++ $(CAIXEIRO_DIR)/caixeiro.cpp -o caixeiro.exe
	./caixeiro.exe $(CAIXEIRO_DIR)/caixeiroEntrada.txt

expressao:
	g++ $(EXPRESSAO_DIR)/expressao-matematica.cpp -o matematica.exe
	g++ $(EXPRESSAO_DIR)/expressao-matematicav2.cpp -o matematicav2.exe

	./matematica.exe
	./matematicav2.exe
	python3 grafico.py $(EXPRESSAO_DIR)/pontos.txt $(EXPRESSAO_DIR)/pontos-ADP.txt

hp:
	g++ $(HP_DIR)/hp.cpp -o hp.exe
	./hp.exe
	python3 $(HP_DIR)/draw_matrix.py $(HP_DIR)/matrix.txt

clean:
	-del -rf *.exe