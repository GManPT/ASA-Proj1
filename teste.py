import time
import subprocess
import random

# Assume-se que está no mesmo sitio que isto
executavel = "./a.out"

# Testes errados
errados = {}

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
    tmp = seq[:m]  # Copiar os primeiros m elementos de seq
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
    
    return tab, seq

def verify_output(tab, original_seq):
    with open("test.out", "r") as f:
        if f.readline().strip() == "0":
            return False
        else:
            expr = f.readline().strip()
            def eval_recursive(expr):
                expr = expr.strip()  # Remove espaços extras nas extremidades
                
                # Caso base: não há parênteses, é um número simples
                if not expr.startswith("("):
                    return int(expr)
                
                # Encontra os parênteses mais externos
                assert expr[0] == "(" and expr[-1] == ")", "Expressão malformada"
                expr = expr[1:-1]  # Remove parênteses externos
                
                # Divide a expressão em partes
                stack = []
                last_split = 0
                parts = []
                
                
                for i, char in enumerate(expr):
                    if len(stack) == 0:
                        return 0
                    if char == '(':
                        stack.append(char)
                    elif char == ')':
                        stack.pop()
                    elif char == ' ' and not stack:
                        # Divide a expressão pelo espaço externo aos parênteses
                        parts.append(expr[last_split:i].strip())
                        last_split = i + 1
                
                # Adiciona a última parte
                parts.append(expr[last_split:].strip())
                
                # Partes devem conter exatamente dois elementos: (left, right)
                assert len(parts) == 2, "Expressão malformada: " + expr
                left = eval_recursive(parts[0])
                right = eval_recursive(parts[1])
                
                # Retorna o valor acessado na matriz
                return tab[left - 1][right - 1]
    
        return eval_recursive(expr) == original_seq[-1]

def run_tests(n_tests):
 
    for n in range(5, 10):
        for m in range(5, 30):
            # Guardar tab e seq para verificar a resposta
            tab, seq = create_test(n, m)
            start = time.time()
            subprocess.run([executavel], stdin=open("test.in"), stdout=open("test.out", "w"))
            total = time.time() - start
            passar = verify_output(tab, seq)
            if not passar:
                errados[(n, m)] = total
                continue
        
            print(f"Teste ({n}, {m}) passou em {total} segundos")
    
    # Escrever os testes errados
    for (n, m), total in errados.items():
        print(f"Teste ({n}, {m}) falhou em {total} segundos")

    

create_test(100,50)
