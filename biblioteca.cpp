#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

const int TAM_MAX_BIBLIOTECA = 50;
const int TAM_MAX_NOME = 30;
const int TAM_MAX_EDITORA = 15;

const int PARAMETRO_CPF = 1;
const int PARAMETRO_NOME = 2;
const int PARAMETRO_CODIGO = 3;
const int PARAMETRO_TITULO = 4;

const char ENTRADA_CPF[] = "CPF";
const char ENTRADA_ANO[] = "Ano";
const char ENTRADA_NOME[] = "Nome";
const char ENTRADA_CODIGO[] = "Código";
const char ENTRADA_TITULO[] = "Titulo";
const char ENTRADA_TIPO[] = "Tipo (L = livro/R = revista)";
const char ENTRADA_NOME_AUTOR[] = "Nome do Autor";
const char ENTRADA_NOME_EDITORA[] = "Nome da Editora";
const char ENTRADA_DATA_DEVOLUCAO[] = "Data da Devolução";

const int TAMANHO_CPF = 11;
const int TAMANHO_CODIGO = 6;
const int TAMANHO_MINIMO_TEXTO = 4;
const int TAMANHO_ANO = 4;

const int DIAS_MES[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

struct Data
{
    int dia;
    int mes;
    int ano;
};

struct Item
{
    int codigo;
    char tipo;
    char titulo[TAM_MAX_NOME];
    char nome_autor[TAM_MAX_NOME];
    char nome_editora[TAM_MAX_EDITORA];
    long long ano;
    bool emprestado = false;
};

struct Usuario
{
    long long cpf;
    char nome[TAM_MAX_NOME];
    Item item;
    Data emprestimo;
    Data devolucao;
    bool com_emprestimo = false;
};

Data data_hora_atual (int variacao) {
    
    time_t t = time(NULL);
    struct tm lt = *localtime(&t);

    lt.tm_mday += variacao;

    mktime(&lt);

    Data data;

    data.ano = lt.tm_year + 1900;
    data.mes = lt.tm_mon + 1;
    data.dia = lt.tm_mday;

    return data;
}

long long conta_ano_bisexto (Data data)
{
    int anos = data.ano;
 
    if (data.mes <= 2)
    {
        anos--;
    }
 
    return anos / 4 - anos / 100 + anos / 400;
}

long long pega_diferenca_datas (Data data1, Data data2)
{
    
    long long n1 = data1.ano * 365 + data1.dia;
 
    for (int i = 0; i < data1.mes - 1; i++)
    {
        n1 += DIAS_MES[i];
    }
 
    n1 += conta_ano_bisexto(data1);
 
    long long n2 = data2.ano * 365 + data2.dia;

    for (int i = 0; i < data2.mes - 1; i++)
    {
        n2 += DIAS_MES[i];
    }

    n2 += conta_ano_bisexto(data2);
 
    return (n2 - n1);
}

long long entrada_numero (const char entrada[], int tamanho) {

    long long temp;
    long digitos = 0;

    do
    {
        printf("%s: ", entrada);
        scanf("%lld", &temp);

        if (temp > 0)
        {
            for (long long i = temp; i > 0;)
            {
                i /= 10;
                digitos++;
            }
        }
        
        if (digitos != tamanho)
        {
            printf("\nErro no cadastro. O %s não possui %d digitos. Tente novamente.\n", entrada, tamanho);

            digitos = 0;
        }

        getchar();

    } while (digitos != tamanho);

    return temp;
}

int entrada_codigo (const char entrada[], int tamanho) {

    int temp;
    int digitos = 0;

    do
    {
        printf("%s: ", entrada);
        scanf("%d", &temp);

        if (temp > 0)
        {
            for (int i = temp; i > 0;)
            {
                i /= 10;
                digitos++;
            }
        }
        
        if (digitos > tamanho && temp < 0)
        {
            printf("\nErro no cadastro. O %s não pode possuir mais do que %d digitos. Tente novamente.\n", entrada, tamanho);

            digitos = 0;
        }

        getchar();

    } while (digitos > tamanho && temp < 0);

    return temp;
}

void entrada_texto (const char entrada[], char string_entrada[], int tamanho_max) {

    char temp[tamanho_max];

    do
    {
        printf("%s: ", entrada);
        fgets(temp, tamanho_max, stdin);
        
        char *indice;
        if (indice = strchr(temp, '\n'))
        {
            *indice = 0;
        }
        else
        {
            scanf("%*[^\n]");
            scanf("%*c");
        }

        if (strlen(temp) < 4)
        {
            printf("\nErro no cadastro. O %s precisa ter no mínimo 4 letras. Tente novamente.\n", entrada);
        }
        
    } while (strlen(temp) < 4);
    
    int quantidade_caracteres = 0;

    for (int i = 0; i < strlen(temp); i++)
    {
        string_entrada[i] = toupper(temp[i]);
        quantidade_caracteres++;
    }
    
    string_entrada[quantidade_caracteres] = 0;
}

char entrada_tipo (const char entrada[]) {

    char temp;

    do
    {
        printf("%s: ", entrada);
        temp = getchar();

        if (temp != 'L' && temp != 'l' && temp != 'R' && temp != 'r')
        {
            printf("\nO valor do tipo precisa ser L (livro) ou R (revista). Tente novamente.\n");
        }

        getchar();
        
    } while (temp != 'L' && temp != 'l' && temp != 'R' && temp != 'r');
    
    return toupper(temp);
}

Data entrada_data (const char entrada[]) {

    Data temp;

    printf("%s: ", entrada);
    scanf("%d/%d/%d", &temp.dia, &temp.mes, &temp.ano);

    return temp;
}

int retorna_ano_abreviado (int ano) {
    return ano % 100;
}

void retorna_tipo_extenso (char letra, char extenso[]) {
    if (letra == 'L')
    {
        strcpy(extenso, "Livro");
    }
    else
    {
        strcpy(extenso, "Revista");
    }
}

Usuario procura_livro_emprestado (Usuario usuarios[], int codigo, int tamanho_usuarios) {

    Usuario temp;

    for (int i = 0; i < tamanho_usuarios; i++)
    {
        if (usuarios[i].com_emprestimo)
        {
            if (usuarios[i].item.codigo == codigo)
            {
                temp = usuarios[i];
                return temp;
            }
        }
    }
    
    return temp;
}

void tira_emprestimo_item (Item itens[], int codigo, int tamanho_itens) {

    for (int i = 0; i < tamanho_itens; i++)
    {
        if (itens[i].emprestado)
        {
            if (itens[i].codigo == codigo)
            {
                itens[i].emprestado = false;
            }
        }
    }
}

void ordena_usuarios (int parametro, Usuario usuarios[], int tamanho_usuarios, Usuario usuarios_ordenados[]) {

    for (int i = 0; i < tamanho_usuarios; i++)
    {
        usuarios_ordenados[i] = usuarios[i];
    }

    bool foi_trocado = true;

    for (int k = tamanho_usuarios - 1; k > 0 && foi_trocado; k--)
    {
        foi_trocado = false;

        for (int i = 0; i < k; i++)
        {
            if (parametro == PARAMETRO_CPF)
            {
                if (usuarios_ordenados[i].cpf > usuarios_ordenados[i + 1].cpf)
                {
                    Usuario temp = usuarios_ordenados[i];
                    usuarios_ordenados[i] = usuarios_ordenados[i + 1];
                    usuarios_ordenados[i + 1] = temp;
                    foi_trocado = true;
                }
            }
            else
            {
                if (strcasecmp(usuarios_ordenados[i].nome, usuarios_ordenados[i + 1].nome) > 0)
                {
                    Usuario temp = usuarios_ordenados[i];
                    usuarios_ordenados[i] = usuarios_ordenados[i + 1];
                    usuarios_ordenados[i + 1] = temp;
                    foi_trocado = true;
                }
            }
        }
    }
}

void ordena_itens (int parametro, Item itens[], int tamanho_itens, Item itens_ordenados[]) {

    for (int i = 0; i < tamanho_itens; i++)
    {
        itens_ordenados[i] = itens[i];
    }

    bool foi_trocado = true;

    for (int k = tamanho_itens - 1; k > 0 && foi_trocado; k--)
    {
        foi_trocado = false;

        for (int i = 0; i < k; i++)
        {
            if (parametro == PARAMETRO_CODIGO)
            {
                if (itens_ordenados[i].codigo > itens_ordenados[i + 1].codigo)
                {
                    Item temp = itens_ordenados[i];
                    itens_ordenados[i] = itens_ordenados[i + 1];
                    itens_ordenados[i + 1] = temp;
                    foi_trocado = true;
                }
            }
            else
            {
                if (strcasecmp(itens_ordenados[i].titulo, itens_ordenados[i + 1].titulo) > 0)
                {
                    Item temp = itens_ordenados[i];
                    itens_ordenados[i] = itens_ordenados[i + 1];
                    itens_ordenados[i + 1] = temp;
                    foi_trocado = true;
                }
            }
        }
    }
}

int cpf_existe (Usuario usuarios[], long long cpf, int tamanho_usuarios) {

    for (int i = 0; i < tamanho_usuarios; i++)
    {
        if (usuarios[i].cpf == cpf)
        {
            return i;
        }
        
    }
    
    return -1;
}

int codigo_existe (Item itens[], int codigo, int tamanho_itens) {

    for (int i = 0; i < tamanho_itens; i++)
    {
        if (itens[i].codigo == codigo)
        {
            return i;
        }
        
    }
    
    return -1;
}

void menu_principal (Usuario usuarios[], int &tamanho_usuarios, Item itens[], int &tamanho_itens);

void emprestar_item (Usuario usuarios[], int tamanho_usuarios, Item itens[], int tamanho_itens) {

    int temp_codigo;
    long long temp_cpf;

    int codigo_repetido, cpf_repetido;

    temp_codigo = entrada_codigo(ENTRADA_CODIGO, TAMANHO_CODIGO);
    codigo_repetido = codigo_existe(itens, temp_codigo, tamanho_itens);

    if (codigo_repetido != -1)
    {
        if (!itens[codigo_repetido].emprestado)
        {
            temp_cpf = entrada_numero(ENTRADA_CPF, TAMANHO_CPF);
            cpf_repetido = cpf_existe(usuarios, temp_cpf, tamanho_usuarios);

            if (cpf_repetido != -1)
            {
                if (!usuarios[cpf_repetido].com_emprestimo)
                {
                    usuarios[cpf_repetido].item = itens[codigo_repetido];
                    itens[codigo_repetido].emprestado = true;
                    usuarios[cpf_repetido].com_emprestimo = true;
                    usuarios[cpf_repetido].emprestimo = data_hora_atual(0);
                    usuarios[cpf_repetido].devolucao = data_hora_atual(7);

                    printf("\nEmpréstimo concedido!\n");
                    printf("Nome: %s\n", usuarios[cpf_repetido].nome);
                    printf("Título: %s\n", usuarios[cpf_repetido].item.titulo);
                    printf("Data do Empréstimo: %02d/%02d/%04d\n", usuarios[cpf_repetido].emprestimo.dia, usuarios[cpf_repetido].emprestimo.mes, usuarios[cpf_repetido].emprestimo.ano);
                    printf("Data da Devolução: %02d/%02d/%04d\n", usuarios[cpf_repetido].devolucao.dia, usuarios[cpf_repetido].devolucao.mes, usuarios[cpf_repetido].devolucao.ano);
                }
                else
                {
                    printf("\nErro no empréstimo. Usuario já possui empréstimo!\n");
                    printf("\nCódigo: %d\n", usuarios[cpf_repetido].item.codigo);
                    printf("Título: %s\n", usuarios[cpf_repetido].item.titulo);
                }
            }
            else
            {
                printf("\nErro no empréstimo. Usuário não cadastrado!\n");
            }
        }
        else
        {
            Usuario temp_usuario = procura_livro_emprestado(usuarios, temp_codigo, tamanho_usuarios);
            printf("\nErro no empréstimo. O item já está emprestado!\n");
            printf("\nCPF: %lld\n", temp_usuario.cpf);
            printf("Nome: %s\n", temp_usuario.nome);
        }
    }
    else
    {
        printf("\nErro no empréstimo. Item não encontrado!\n");
    }
}

void devolver_item (Usuario usuarios[], int tamanho_usuarios, Item itens[], int tamanho_itens) {

    long long temp_cpf;
    Data temp_data;

    int cpf_repetido;

    temp_cpf = entrada_numero(ENTRADA_CPF, TAMANHO_CPF);
    cpf_repetido = cpf_existe(usuarios, temp_cpf, tamanho_usuarios);

    if (cpf_repetido != -1)
    {
        if (usuarios[cpf_repetido].com_emprestimo)
        {
            temp_data = entrada_data(ENTRADA_DATA_DEVOLUCAO);

            long long diferença;

            diferença = pega_diferenca_datas(usuarios[cpf_repetido].devolucao, temp_data);

            printf("\nNome: %s\n", usuarios[cpf_repetido].nome);
            printf("Título: %s\n", usuarios[cpf_repetido].item.titulo);

            if (diferença <= 0)
            {
                printf("\nDevolução realizada com sucesso no prazo!\n");
            }
            else
            {
                printf("Data da Devolução: %02d/%02d/%04d", usuarios[cpf_repetido].devolucao.dia, usuarios[cpf_repetido].devolucao.mes, usuarios[cpf_repetido].devolucao.ano);

                printf("\nDevolução realizada com atraso de %lld dias.\n", diferença);

                printf("Multa de R$ %.2lf\n", diferença * 1.50);
            }

            usuarios[cpf_repetido].com_emprestimo = false;
            tira_emprestimo_item(itens, usuarios[cpf_repetido].item.codigo, tamanho_itens);
        }
        else
        {
            printf("\nO usuário não possui empréstimo!\n");
        }
    }
    else
    {
        printf("\nO usuário não está cadastrado!\n");
    }
}

void listar_emprestimos (Usuario usuarios[], int tamanho_usuarios) {

    Usuario emprestimos_ordenados[TAM_MAX_BIBLIOTECA];

    ordena_usuarios(PARAMETRO_CPF, usuarios, tamanho_usuarios, emprestimos_ordenados);

    puts("----------------------------------------------------------");
    puts("CPF         Nome     Código Título    Empréstimo Devolução");
    puts("----------------------------------------------------------");

    for (int i = 0; i < tamanho_usuarios; i++)
    {
        if (emprestimos_ordenados[i].com_emprestimo)
        {
            printf("%-11lld %-8.8s %-6d %-8.8s    %02d/%02d/%02d  %02d/%02d/%02d\n", emprestimos_ordenados[i].cpf, emprestimos_ordenados[i].nome,
            emprestimos_ordenados[i].item.codigo, emprestimos_ordenados[i].item.titulo, emprestimos_ordenados[i].emprestimo.dia,
            emprestimos_ordenados[i].emprestimo.mes, retorna_ano_abreviado(emprestimos_ordenados[i].emprestimo.ano), emprestimos_ordenados[i].devolucao.dia,
            emprestimos_ordenados[i].devolucao.mes, retorna_ano_abreviado(emprestimos_ordenados[i].devolucao.ano));
        }
    }
    
    puts("----------------------------------------------------------");
}

void menu_emprestimo_devolucao (Usuario usuarios[], int &tamanho_usuarios, Item itens[], int &tamanho_itens) {

    int opcao;

    do
    {
        printf("Menu do Emprestimo/Devolucao\n1-Emprestar item\n2-Devolver item\n3-Listar emprestimos\n4-Voltar\n");
        scanf("%d", &opcao);

        if (opcao > 4 || opcao < 1)
        {
            printf("\nOpção não reconhecida, tente novamente.\n");
        }
        
    } while (opcao > 4 || opcao < 1);

    putchar('\n');
    getchar();

    
    switch (opcao)
    {
    case 1:
        if (tamanho_usuarios > 0 && tamanho_itens > 0)
        {
            emprestar_item(usuarios, tamanho_usuarios, itens, tamanho_itens);
        }
        else if (tamanho_usuarios <= 0)
        {
            printf("Não existem usuários cadastrados.\n");
        }
        else if (tamanho_itens <= 0)
        {
            printf("Não existem itens cadastrados.\n");
        }
        break;
    
    case 2:
        if (tamanho_usuarios > 0 && tamanho_itens > 0)
        {
            devolver_item(usuarios, tamanho_usuarios, itens, tamanho_itens);
        }
        else if (tamanho_usuarios <= 0)
        {
            printf("Não existem usuários cadastrados.\n");
        }
        else if (tamanho_itens <= 0)
        {
            printf("Não existem itens cadastrados.\n");
        }
        break;
    
    case 3:
        if (tamanho_usuarios > 0 && tamanho_itens > 0)
        {
            listar_emprestimos(usuarios, tamanho_usuarios);
        }
        else if (tamanho_usuarios <= 0)
        {
            printf("Não existem usuários cadastrados.\n");
        }
        else if (tamanho_itens <= 0)
        {
            printf("Não existem itens cadastrados.\n");
        }
        break;
    
    case 4:
        menu_principal(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    default:
        break;
    }

    putchar('\n');

    if (opcao != 4)
    {
        menu_emprestimo_devolucao(usuarios, tamanho_usuarios, itens, tamanho_itens);
    }
}

void cadastrar_item (Item itens[], int &tamanho_itens) {

    int temp;
    int codigo_repetido;

    temp = entrada_codigo(ENTRADA_CODIGO, TAMANHO_CODIGO);
    codigo_repetido = codigo_existe(itens, temp, tamanho_itens);

    if (codigo_repetido != -1)
    {
        printf("\nErro no cadastro. Código já cadastrado!\n");
    }
    else
    {
        itens[tamanho_itens].codigo = temp;

        itens[tamanho_itens].tipo = entrada_tipo(ENTRADA_TIPO);

        entrada_texto(ENTRADA_TITULO, itens[tamanho_itens].titulo, TAM_MAX_NOME);

        if (itens[tamanho_itens].tipo == 'L')
        {
            entrada_texto(ENTRADA_NOME_AUTOR, itens[tamanho_itens].nome_autor, TAM_MAX_NOME);
        }

        entrada_texto(ENTRADA_NOME_EDITORA, itens[tamanho_itens].nome_editora, TAM_MAX_EDITORA);

        itens[tamanho_itens].ano = entrada_numero(ENTRADA_ANO, TAMANHO_ANO);

        tamanho_itens++;

        printf("\nCadastro realizado com sucesso!\n");
    }
}

void excluir_item (Item itens[], int &tamanho_itens) {

    int temp;
    int codigo_repetido;

    temp = entrada_codigo(ENTRADA_CODIGO, TAMANHO_CODIGO);
    codigo_repetido = codigo_existe(itens, temp, tamanho_itens);

    if (codigo_repetido != -1)
    {
        if (itens[codigo_repetido].emprestado)
        {
            printf("\nErro na exclusão. O item está emprestado!\n");
        }
        else
        {
            for (int i = codigo_repetido; i < tamanho_itens - 1; i++)
            {
                itens[i] = itens[i + 1];
            }
            
            tamanho_itens--;

            printf("\nExclusão realizada com sucesso!\n");
        }
    }
    else
    {
        printf("\nErro na exclusão. O item não está cadastrado!\n");
    }
}

void listar_itens (int parametro, Item itens[], int tamanho_itens) {

    Item itens_ordenados[TAM_MAX_BIBLIOTECA];

    ordena_itens(parametro, itens, tamanho_itens, itens_ordenados);

    puts("-------------------------------------------------------------");
    puts("Código T Título          Autor           Editora          Ano");
    puts("-------------------------------------------------------------");

    for (int i = 0; i < tamanho_itens; i++)
    {
        printf("%-6d %c %-15.15s %-15.15s %-15.15s %4lld\n", itens_ordenados[i].codigo, itens_ordenados[i].tipo,
        itens_ordenados[i].titulo, itens_ordenados[i].nome_autor, itens_ordenados[i].nome_editora, itens_ordenados[i].ano);
    }
    
    puts("-------------------------------------------------------------");
}

void menu_cadastro_itens (Usuario usuarios[], int &tamanho_usuarios, Item itens[], int &tamanho_itens) {

    int opcao;

    do
    {
        printf("Menu do Cadastro de Itens da Biblioteca\n1-Cadastrar novo item\n2-Excluir item\n3-Listar itens (ordenados por codigo)\n4-Listar itens (ordenados por título)\n5-Voltar\n");
        scanf("%d", &opcao);

        if (opcao > 5 || opcao < 1)
        {
            printf("\nOpção não reconhecida, tente novamente.\n");
        }
        
    } while (opcao > 5 || opcao < 1);

    putchar('\n');
    getchar();
    
    switch (opcao)
    {
    case 1:
        if (tamanho_itens < TAM_MAX_BIBLIOTECA)
        {
            cadastrar_item(itens, tamanho_itens);
        }
        else
        {
            printf("Não é possível ter mais do que 50 itens cadastrados, exclua algum antes de cadastrar novamente.\n");
        }
        break;
    
    case 2:
        if (tamanho_itens > 0)
        {
            excluir_item(itens, tamanho_itens);
        }
        else
        {
            printf("Não existem itens para serem excluidos.\n");
        }
        break;
    
    case 3:
        if (tamanho_itens > 0)
        {
            listar_itens(PARAMETRO_CODIGO, itens, tamanho_itens);
        }
        else
        {
            printf("Não existem itens para serem listados.\n");
        }
        break;
    
    case 4:
        if (tamanho_itens > 0)
        {
            listar_itens(PARAMETRO_TITULO, itens, tamanho_itens);
        }
        else
        {
            printf("Não existem itens para serem listados.\n");
        }
        break;
    
    case 5:
        menu_principal(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    default:
        break;
    }

    putchar('\n');

    if (opcao != 5)
    {
        menu_cadastro_itens(usuarios, tamanho_usuarios, itens, tamanho_itens);
    }
}

void cadastrar_usuario (Usuario usuarios[], int &tamanho_usuarios) {

    long long temp;
    int cpf_repetido;

    temp = entrada_numero(ENTRADA_CPF, TAMANHO_CPF);
    cpf_repetido = cpf_existe(usuarios, temp, tamanho_usuarios);

    if (cpf_repetido != -1)
    {
        printf("\nErro no cadastro. CPF já cadastrado!\n");
    }
    else
    {
        usuarios[tamanho_usuarios].cpf = temp;

        entrada_texto(ENTRADA_NOME, usuarios[tamanho_usuarios].nome, TAM_MAX_NOME);

        tamanho_usuarios++;

        printf("\nCadastro realizado com sucesso!\n");
    }
}

void excluir_usuario (Usuario usuarios[], int &tamanho_usuarios) {

    long long temp;
    int cpf_repetido;

    temp = entrada_numero(ENTRADA_CPF, TAMANHO_CPF);
    cpf_repetido = cpf_existe(usuarios, temp, tamanho_usuarios);

    if (cpf_repetido != -1)
    {
        if (usuarios[cpf_repetido].com_emprestimo)
        {
            printf("\nErro na exclusão. O usuario possui um emprestimo!\n");
        }
        else
        {
            for (int i = cpf_repetido; i < tamanho_usuarios - 1; i++)
            {
                usuarios[i] = usuarios[i + 1];
            }

            tamanho_usuarios--;

            printf("\nExclusão realizada com sucesso!\n");
        }
    }
    else
    {
        printf("\nErro na exclusão. O usuário não está cadastrado!\n");
    }
}

void listar_usuarios (int parametro, Usuario usuarios[], int tamanho_usuarios) {

    Usuario usuarios_ordenados[TAM_MAX_BIBLIOTECA];

    ordena_usuarios(parametro, usuarios, tamanho_usuarios, usuarios_ordenados);

    puts("--------------------------------------------------");
    puts("CPF         Nome                                  ");
    puts("--------------------------------------------------");

    for (int i = 0; i < tamanho_usuarios; i++)
    {
        printf("%11lld %-30s\n", usuarios_ordenados[i].cpf, usuarios_ordenados[i].nome);
        if (usuarios_ordenados[i].com_emprestimo)
        {
            char tipo_extenso[TAM_MAX_NOME];
            retorna_tipo_extenso(usuarios_ordenados[i].item.tipo, tipo_extenso);

            printf("            Emprestimo: %02d/%02d/%04d a %02d/%02d/%04d\n", usuarios_ordenados[i].emprestimo.dia,
            usuarios_ordenados[i].emprestimo.mes, usuarios_ordenados[i].emprestimo.ano, usuarios_ordenados[i].devolucao.dia,
            usuarios_ordenados[i].devolucao.mes, usuarios_ordenados[i].devolucao.ano);
            printf("            Tipo: %s\n", tipo_extenso);
            printf("            Título: %-30s\n", usuarios_ordenados[i].item.titulo);
        }
    }

    puts("--------------------------------------------------");
}

void menu_cadastro_usuarios (Usuario usuarios[], int &tamanho_usuarios, Item itens[], int &tamanho_itens) {

    int opcao;

    do
    {
        printf("Menu do Cadastro de Usuários\n1-Cadastrar novo usuário\n2-Excluir usuário\n3-Listar usuários (ordenados por CPF)\n4-Listar usuários (ordenados por nome)\n5-Voltar\n");
        scanf("%d", &opcao);

        if (opcao > 5 || opcao < 1)
        {
            printf("\nOpção não reconhecida, tente novamente.\n");
        }
        
    } while (opcao > 5 || opcao < 1);

    putchar('\n');
    getchar();
    
    switch (opcao)
    {
    case 1:
        if (tamanho_usuarios < TAM_MAX_BIBLIOTECA)
        {
            cadastrar_usuario(usuarios, tamanho_usuarios);
        }
        else
        {
            printf("Não é possível ter mais do que 50 usuários cadastrados, exclua algum antes de cadastrar novamente.\n");
        }
        break;
    
    case 2:
        if (tamanho_usuarios > 0)
        {
            excluir_usuario(usuarios, tamanho_usuarios);
        }
        else
        {
            printf("Não existem usuários para serem excluidos.\n");
        }
        break;
    
    case 3:
        if (tamanho_usuarios > 0)
        {
            listar_usuarios(PARAMETRO_CPF, usuarios, tamanho_usuarios);
        }
        else
        {
            printf("Não existem usuários para serem listados.\n");
        }
        break;
    
    case 4:
        if (tamanho_usuarios > 0)
        {
            listar_usuarios(PARAMETRO_NOME, usuarios, tamanho_usuarios);
        }
        else
        {
            printf("Não existem usuários para serem listados.\n");
        }
        break;
    
    case 5:
        menu_principal(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    default:
        break;
    }

    putchar('\n');

    if (opcao != 5)
    {
        menu_cadastro_usuarios(usuarios, tamanho_usuarios, itens, tamanho_itens);
    }
}

void menu_principal (Usuario usuarios[], int &tamanho_usuarios, Item itens[], int &tamanho_itens) {

    int opcao;

    do
    {
        printf("Menu Principal\n1-Cadastro de usuarios\n2-Cadastro de itens da biblioteca\n3-Emprestimo/devolucao\n4-Fim\n");
        scanf("%d", &opcao);

        if (opcao > 4 || opcao < 1)
        {
            printf("\nOpção não reconhecida, tente novamente.\n");
        }
        
    } while (opcao > 4 || opcao < 1);

    putchar('\n');
    getchar();
    
    switch (opcao)
    {
    case 1:
        menu_cadastro_usuarios(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    case 2:
        menu_cadastro_itens(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    case 3:
        menu_emprestimo_devolucao(usuarios, tamanho_usuarios, itens, tamanho_itens);
        break;
    
    default:
        break;
    }
}

int main () {

    Item itens[TAM_MAX_BIBLIOTECA];
    Usuario usuarios[TAM_MAX_BIBLIOTECA];

    int tamanho_usuarios = 0, tamanho_itens = 0;

    menu_principal(usuarios, tamanho_usuarios, itens, tamanho_itens);

    return 0;
}