import time
import subprocess
import random
import matplotlib.pyplot as plt
import numpy as np

# Assume-se que está no mesmo sitio que isto
executavel = "./a.out"
valoresnm = []
tempos = []

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

def run_tests():
    i = 1
    for n in range(5, 101, 5):
        for m in range(10, 1001, 15):
            # Gerar um teste válido
            create_test(n, m)
            
            # Correr os testes
            start = time.time()
            subprocess.run([executavel], stdin=open("test.in"), stdout=open("test.out", "w"))
            total = time.time() - start
            
            # Verifica se o tempo é aceitável
            if total <= 15:
                tempos.append(total)

                # Editar para melhor efeito
                fmn = (n ** 3) * (m ** 3)
                
                valoresnm.append(fmn)
                print(f"Teste {i}: n={n}, m={m} -> Tempo médio de execução: {total:.4f} segundos")
            else:
                print(f"Teste {i}: n={n}, m={m} -> Tempo médio de execução: {total:.4f} segundos (excedeu 15 segundos e não será plotado)")
            
            i += 1

# Correr 10 testes, cada um com 10 execuções (predefinição)
run_tests()

# Plota os pontos de dados originais (média dos tempos)
plt.scatter(valoresnm, tempos, label="Dados experimentais", alpha=0.5, color="blue")

# Ajusta uma curva polinomial de tendência para todos os dados combinados
degree = 1
coef = np.polyfit(valoresnm, tempos, degree)
print(f"Coeficientes da curva de tendência: y = {coef[0]}x + {coef[1]}")
poly_fn = np.poly1d(coef)
plt.plot(valoresnm, poly_fn(valoresnm), '--', label="Tendência global", color="red")

plt.xlabel("f(n, m)")
plt.ylabel("Time (s)")
plt.title("Curva de tendência para tempo de execução em função de f(n, m)")
plt.legend()
plt.show()
