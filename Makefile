NDAMAS_DIR = ./Problema_das_N-damas
CAIXEIRO_DIR = ./Problema_do_Caixeiro-Viajante
EXPRESSAO_DIR = ./Problema_da_Expressao-Matematica
DNA_DIR = ./Problema_DNA
HP_DIR = ./Problema_HP
OUTDIR = ./dist

ndamas:
	g++ $(NDAMAS_DIR)/Algoritmo_Genetico/ndamas.cpp -o $(OUTDIR)/ndamasG.exe
	g++ $(NDAMAS_DIR)/Algoritmo_Genetico/ndamasv2.cpp -o $(OUTDIR)/ndamasG2.exe
	g++ $(NDAMAS_DIR)/Algoritmo_Genetico/ndamasv3.cpp -o $(OUTDIR)/ndamasG3.exe
	g++ $(NDAMAS_DIR)/Backtracking/ndamas.cpp -o $(OUTDIR)/ndamas.exe

	$(OUTDIR)/ndamasG.exe
	$(OUTDIR)/ndamasG2.exe
	$(OUTDIR)/ndamasG3.exe
	python3 grafico.py $(NDAMAS_DIR)/Algoritmo_Genetico/pontos.txt $(NDAMAS_DIR)/Algoritmo_Genetico/pontos-ADP.txt $(NDAMAS_DIR)/Algoritmo_Genetico/pontos-AAE2.txt

dna:
	g++ $(DNA_DIR)/dna.cpp -o $(OUTDIR)/dna.exe
	$(OUTDIR)/dna.exe

caixeiro:
	g++ $(CAIXEIRO_DIR)/caixeiro.cpp -o $(OUTDIR)/caixeiro.exe
	$(OUTDIR)/caixeiro.exe $(CAIXEIRO_DIR)/caixeiroEntrada.txt

expressao:
	g++ $(EXPRESSAO_DIR)/expressao-matematica.cpp -o $(OUTDIR)/matematica.exe
	g++ $(EXPRESSAO_DIR)/expressao-matematicav2.cpp -o $(OUTDIR)/matematicav2.exe
	g++ $(EXPRESSAO_DIR)/expressao-matematicav3.cpp -o $(OUTDIR)/matematicav3.exe

	$(OUTDIR)/matematica.exe
	$(OUTDIR)/matematicav2.exe
	$(OUTDIR)/matematicav3.exe
	python3 grafico.py $(EXPRESSAO_DIR)/pontos.txt $(EXPRESSAO_DIR)/pontos-ADP.txt $(EXPRESSAO_DIR)/pontos-AAE2.txt

hp:
	g++ $(HP_DIR)/hp.cpp -o $(OUTDIR)/hp.exe
	g++ $(HP_DIR)/hpv2.cpp -o $(OUTDIR)/hpv2.exe
	g++ $(HP_DIR)/hpv3.cpp -o $(OUTDIR)/hpv3.exe

	$(OUTDIR)/hp.exe
	python3 $(HP_DIR)/draw_matrix.py $(HP_DIR)/matrix.txt

	$(OUTDIR)/hpv2.exe
	python3 $(HP_DIR)/draw_matrix.py $(HP_DIR)/matrix.txt

	$(OUTDIR)/hpv3.exe
	python3 $(HP_DIR)/draw_matrix.py $(HP_DIR)/matrix.txt

	python3 grafico.py $(HP_DIR)/pontos.txt $(HP_DIR)/pontos-ADP.txt $(HP_DIR)/pontos-AAE2.txt

clean:
	del $(OUTDIR)/*.exe