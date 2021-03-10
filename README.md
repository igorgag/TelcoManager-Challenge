# TelcoManager-Challenge

## Requisitos:
- Mysql 8.0

## Inicialização
- No termial execute o script seed-bd.sh (Para criação e inicialização do Banco de Dados)
- Substitua o USER e PASSWORD do banco mysql no arquivo ./server/connectDB.h
- Após isso compile o cliente e o servidor:
  - gcc server/server.c server/user/user.c -o server/server -lm \`mysql_config --cflags --libs\`
  - gcc client/client.c -o client/client 
  
## Execução
- Para executar o servidor utilize o script start-server.sh
- Para executar o cliente utilize o script app-client.sh

## Instruções
- Os Clientes (Cliente1, Cliente2, Cliente3) e seus respectivos arquivos estão previamentes criados para testes.
- Cliente1, Cliente2 e Cliente3 são os ids que podem ser acessados no servidor pelos clientes.

## Commandos

    - help - lista opções de comando disponíveis 
    - list <opção de ordenação> <asc/desc> - as opções de ordenação são 'name' e 'size' - Ex: list name asc
    - send - enviar um arquivo no path 'file' - Ex: send send.txt
    - get -  recuperar o arquivo com o nome 'file' - Ex: get recv.txt
    - exit

