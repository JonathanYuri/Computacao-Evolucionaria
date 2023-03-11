import sys
from IPython.display import display
import matplotlib.pyplot as plt

nome_arquivos = []
nome_arquivos.append(sys.argv[1])
nome_arquivos.append(sys.argv[2])

x = []
y = []
for nome_arquivo in nome_arquivos:
    arq = open(nome_arquivo)
    linhas = arq.readlines()

    x1 = []
    y1 = []

    for i in linhas:
        j = i.split(' ')
        for n in range(0, len(j)):
            if n == 0:
                x1.append(j[n])
            elif n == 1:
                y1.append(j[n][:-1])
                
    x.append(x1)
    y.append(y1)
    arq.close()
    
plt.figure(dpi=150)
plt.subplot(2, 1, 1)
plt.plot(x[0], y[0])
plt.subplot(2, 1, 2)
plt.plot(x[1], y[1])
plt.show()