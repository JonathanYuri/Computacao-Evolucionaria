import sys
import numpy as np
import pandas as pd
from IPython.display import display, HTML
import matplotlib.pyplot as plt

nome_arquivo = sys.argv[1]
arq = open(nome_arquivo)
linhas = arq.readlines()

matriz = []
for l in range(1, len(linhas)):
    linha = []
    for n in linhas[l].split(' '):
        linha.append(n)
    if (len(linha) != 0):
        matriz.append(linha)
    
valores_xy = linhas[0].split(' ')

menorX = int(valores_xy[0])
maiorX = int(valores_xy[1])

menorY = int(valores_xy[2])
maiorY = int(valores_xy[3])

data = {}

k = 0

#passar por todas as colunas
for i in range(menorX, maiorX + 1):
    #passar por todas as linhas
    l = []
    for j in range(0, len(matriz)):
        l.append(matriz[j][i - menorX])
    data[i] = l

rows = []
for j in range(maiorY, menorY - 1, -1):
    rows.append(j)

df = pd.DataFrame(data, index=rows)
display(df)