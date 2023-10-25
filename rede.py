import subprocess

def teste_de_rede():
    # Executar o comando 'nslookup' para verificar a resolução de DNS
    try:
        dominio = input("Digite um domínio para fazer uma consulta de DNS (por exemplo, google.com): ")
        resultado_dns = subprocess.check_output(["nslookup", dominio], text=True)
        print("\nResultados do nslookup:")
        print(resultado_dns)
    except subprocess.CalledProcessError as e:
        print("Erro ao executar nslookup:", e)

    # Executar o comando 'netstat' para verificar as conexões de rede
    try:
        resultado_netstat = subprocess.check_output(["netstat", "-n", "-t", "-u"], text=True)
        print("\nResultados do netstat:")
        print(resultado_netstat)
    except subprocess.CalledProcessError as e:
        print("Erro ao executar netstat:", e)

if __name__ == "__main__":
    teste_de_rede()
