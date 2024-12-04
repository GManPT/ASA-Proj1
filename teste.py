import time
import subprocess
import random
import matplotlib.pyplot as plt
import numpy as np

# Assume-se que está no mesmo sitio que isto
executavel = "./a.out"
tempos = []
valoresnm = []

def build_table(n):
    # Alocar tabela
    tab = [[0] * n for _ in range(n)]

    # Preencher tabela
    for i in range(n):
        for j in range(n):
            tab[i][j] = random.randint(1, n)

    return tab

def build_sequence(tab, n, m):
    # Alocar sequência
    seq = [0] * (m + 1)

    # Preencher sequência
    for i in range(m):
        seq[i] = random.randint(1, n)

    # Calcular uma saída válida eliminando vizinhos escolhidos aleatoriamente
    tmp = seq[:m]
    while len(tmp) > 1:
        i = random.randint(0, len(tmp) - 2)  # Escolher um índice aleatório
        tmp[i] = tab[tmp[i] - 1][tmp[i + 1] - 1]  # Atualizar tmp[i]
        tmp.pop(i + 1)  # Remover o próximo elemento

    # O último elemento do array seq é a resposta
    seq[m] = tmp[0]
    return seq

def create_test(n, m):
    tab = build_table(n)
    seq = build_sequence(tab, n, m)
    
    with open("test.in", "w") as f:
        f.write(f"{n} {m}\n")
        for linha in tab:
            f.write(" ".join(map(str, linha)) + "\n")
        f.write(" ".join(map(str, seq[:-1])) + "\n")
        f.write(str(seq[-1]) + "\n")

def run_tests(n_testes=10):
    for i in range(1,n_testes + 1):
        n = i * 10
        m = i * 100

        # Gerar um teste valido
        create_test(n, m)
        
        # Executar o teste e medir o tempo
        start = time.time()
        subprocess.run([executavel], stdin=open("test.in"), stdout=open("test.out", "w"))
        total = time.time() - start

        # Adicionar o tempo à listagem
        tempos.append(total)
        valoresnm.append(n * m)

        print(f"Teste {i}: n={n}, m={m}, f(n, m)={n*m} -> Tempo de execução: {total:.4f} segundos")

# Correr 10 testes
run_tests()

# Plota os pontos de dados originais
plt.scatter(valoresnm, tempos, label="Dados experimentais", alpha=0.5, color="blue")

# Ajusta uma curva polinomial de tendência para todos os dados combinados
degree = 2
coef = np.polyfit(valoresnm, tempos, degree)
poly_fn = np.poly1d(coef)

# Plota a curva de ajuste
sorted_nm_values = sorted(valoresnm)  # Ordena para uma curva suave
plt.plot(sorted_nm_values, poly_fn(sorted_nm_values), '--', label="Tendência global", color="red")

plt.xlabel("f(n, m)")
plt.ylabel("Time (s)")
plt.title("Curva de tendência para tempo de execução em função de f(n, m)")
plt.legend()
plt.show()