import sys
from IPython.display import display
import matplotlib.pyplot as plt

nome_arquivos = []
nome_arquivos.append(sys.argv[1])
nome_arquivos.append(sys.argv[2])
nome_arquivos.append(sys.argv[3])

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

fig, axes = plt.subplots(nrows=1, ncols=3, sharex=False, sharey=False)
fig.set_figheight(7)
fig.set_figwidth(10)

ax1, ax2, ax3 = axes.flatten()

ax1.plot(x[0], y[0] , 'r')
ax2.plot(x[1], y[1], 'g')
ax3.plot(x[2], y[2], 'b')
plt.show()