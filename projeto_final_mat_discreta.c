#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int maior_primo = 0, qtd_primo_conf = 0;
int p0 = 0, p1 = 0, p2 = 0, p3 = 0;

void chave_publica();
    //short int func_eh_primo(unsigned long long int);
    unsigned long long int func_euclides(unsigned long long int, unsigned long long int);
int criptografar();
void descriptografar();
    unsigned long long int func_variavel_d(unsigned long long int a, unsigned long long int mod);
    int conferir_se_eh_primo(unsigned long long int n_func);
    int verificar_paridade_de_p(int);
    int primen(int*, int*, unsigned long long int);
    int p_menorigual2(int*, int*, unsigned long long int);
    void debug_print(int*, int, int*);
    unsigned long long int func_primo_em_relacao_a_e(unsigned long long int);
    int mdc_func(int, unsigned long long int);

int main() {
    //setlocale(LC_ALL, "Portuguese_Brasil"); // !!! Sem uso !!!
    int func_true = 1;
    while(func_true == 1) {
        func_true = 0;
        printf("Criptografia RSA\n");
        printf("Escolha uma das opcoes abaixo\n"); // acent
        printf("1. Gerar chave publica;\n"); // acent
        printf("2. Criptografar uma mensagem;\n");
        printf("3. Descriptografar uma mensagem.\n");
        printf("Digite o numero da opcao: "); // acent
        int escolha;
        scanf("%d", &escolha);
        switch(escolha) {
            case 1:
                printf("Você escolheu: gerar chave publica.\n");
                chave_publica();
                break;
            case 2:
                printf("Você escolheu: Criptografar.\n");
                int retorno = criptografar();
                while(retorno == 2){
                    printf("\nVocê está gerando uma chave pública.\n");
                    chave_publica();
                    retorno = criptografar();
                }
                break;
            case 3:
                printf("Você escolheu: Descriptografar.\n");
                descriptografar();
                break;
            default:
                printf("Escolha inválida!\n");
                func_true = 1;
        }
    }
    return 0;
}

void chave_publica() {
    FILE *chave_p_pasta;
    chave_p_pasta = fopen("chave_publica.txt", "w");
    if(chave_p_pasta == NULL)
    {
        /* Arquivo não foi criado, saindo */
        printf("Nao foi possivel criar o arquivo.\n"); //acent
        exit(EXIT_FAILURE);
    }
    unsigned long long int p, q, e;
    short int retorno;
    printf("Digite dois numeros primos (p e q): ");
    scanf("%llu%llu", &p, &q);
    if(p != 2) {
        retorno = conferir_se_eh_primo(p); // Função para verificar se P e Q são primos;
        if(retorno != 0) {
            if(q != 2) {
                retorno = conferir_se_eh_primo(q);
                if(retorno != 1) {
                    printf("Numero invalido!\n");
                    exit(5); // Código do número inválido        
                }
            }
        }
    }

    else if(!(p == 2)){
        printf("Numero invalido!\n");
        exit(5);
    }
    unsigned long long int z = (p - 1)*(q - 1);
    printf("Digite o expoente (e), que esteja entre 1 e (p - 1)*(q - 1): ");
    scanf("%llu", &e);
    if(func_euclides(z, e) != 1) {
        printf("Numero invalido!\n");
        exit(6);
    }
    fprintf(chave_p_pasta, "n = %lld, e = %lld", p * q, e);
    printf("Chave criada com sucesso!\n");
    fclose(chave_p_pasta);
}

int criptografar() {
    printf("Digite uma mensagem a ser criptografada: ");
    char mensagem[500000];
    unsigned long long int array[500000];
    unsigned long long int chave_publica_n, chave_publica_e;
    getchar();
    fgets(mensagem, 500000, stdin);
    printf("Digite a chave pública. (Dica: se não tiver uma chave pública, submeta 0 para fazê-la)\n");
    printf("Digite o 'n' ou o produto de 'p' e 'q': ");
    scanf("%llu", &chave_publica_n);
    if(chave_publica_n == 0) {
        return 2;
    }
    printf("Digite o expoente 'e': ");
    scanf("%llu", &chave_publica_e);
    if(chave_publica_e == 0) {
        return 2;
    }
    FILE *mensagem_criptografada;
    mensagem_criptografada = fopen("mensagem_criptografada.txt", "w");
    if(mensagem_criptografada == NULL)
    {
        /* Arquivo não foi criado, saindo */
        printf("Nao foi possivel criar o arquivo.\n"); //acent
        exit(EXIT_FAILURE);
    }
    int check_lower;
    int string_lenght = strlen(mensagem);
    for(int i = 0; i < string_lenght; i++) {
        if(mensagem[i] == 32) {
            array[i] = mensagem[i] - 4;
            continue;
        }
        check_lower = islower(mensagem[i]);
        if(check_lower == 512) {
            mensagem[i] = toupper(mensagem[i]);
        }
        array[i] = mensagem[i] - 63;
    }
    printf("Mensagem recebida: %s", mensagem);
    double pow_result;
    for(int k = 0; k < string_lenght - 1; k++) {
        printf("%llu antes\n", array[k]);
        pow_result = pow((double)array[k], (double)chave_publica_e);
        (unsigned long long int)pow_result = pow_result;
        printf("pow_result = %.llu\n", pow_result);
        array[k] = (unsigned long long int)pow_result % chave_publica_n;
        printf("%llu após\n", array[k]);
        fprintf(mensagem_criptografada, "%llu\n", array[k]);
    }
    fclose(mensagem_criptografada);
    return 0;
}

void descriptografar() {
    unsigned long long int p, q, e;
    printf("Digite os valores de 'p', 'q', e 'e'\n");
    printf("p: ");
    scanf("%llu", &p);
    printf("q: ");
    scanf("%llu", &q);
    printf("e: ");
    scanf("%llu", &e);
    unsigned long long int msg_criptografada_int[500000];
    int contador = 0;
    char msg_descriptografada_char[500000];
    FILE *mensagem_criptografada_arquivo;
    mensagem_criptografada_arquivo = fopen("mensagem_criptografada.txt", "r");
    while(!feof(mensagem_criptografada_arquivo)) {
        fscanf(mensagem_criptografada_arquivo, "%llu", &msg_criptografada_int[contador]);
        contador++;
    }
    fclose(mensagem_criptografada_arquivo);
    FILE *mensagem_descriptografada_arquivo;
    mensagem_descriptografada_arquivo = fopen("mensagem_descriptografada.txt", "w");

    unsigned long long int d_inverso_descripto;
    d_inverso_descripto = func_variavel_d(e, ((p - 1) * (q - 1)));
    /*if(d_inverso_descripto == e) {
        d_inverso_descripto = d_inverso_descripto + ((p - 1)*(q - 1));
        printf("d_inverso após if: %llu\n", d_inverso_descripto);
    }*/

    double pow_result_descripto;
    long long int resultado_slide_59;
    resultado_slide_59 = (d_inverso_descripto * e) % ((p - 1) * (q - 1));
    printf("Resultado do mod segundo o slide 59: %lld\n", resultado_slide_59);
   
    long long int resultado;
    unsigned long long int array[50000];

    for(int k = 0; k < 100; k++) {
        printf("msg_criptografada[%d] = %llu\n", k, msg_criptografada_int[k]);
    }

    for(int i = 0; i < (contador - 1); i++) {
        printf("Potência: %.2lf^%.2lf = %.2lf\n", (double)msg_criptografada_int[i], (double)d_inverso_descripto, pow((double)msg_criptografada_int[i], (double)d_inverso_descripto));
        pow_result_descripto = pow((double)msg_criptografada_int[i], (double)d_inverso_descripto);
        printf("Resultado da potência: %.2lf\n", pow_result_descripto);
        resultado = ((long long int)pow_result_descripto) % (p * q);
        printf("Resultado após tirar o módulo por p * q: %lld\n", resultado);
        array[i] = resultado;
        printf("Array[%d] = %lld\n", i, array[i]);
    }


    
    /* !!! Área com tretas no código !!!

    unsigned long long int d_inverso_descripto;
    d_inverso_descripto = func_variavel_d(e, ((p - 1) * (q - 1)));

    double pow_result_descripto;
    for(int i = 0; i < contador - 1; i++) {
        printf("%llu^%llu\n", msg_criptografada_int[i], d_inverso_descripto);
        pow_result_descripto = pow(msg_criptografada_int[i], d_inverso_descripto);
        printf("%lf\n", pow_result_descripto);
        printf("%llu\n\n", (unsigned long long int)pow_result_descripto % (p * q));
    }

    !!! Área com tretas no código !!!

    for(int i = 0; i < contador - 1; i++) {
        pow_result_descripto = pow(msg_criptografada_int[i], d_inverso_descripto);
        msg_descriptografada_char[i] = (int)(((unsigned long long int)pow_result_descripto) % (p * q));
    }

    for(int k = 0; k < contador; k++) {
        if(msg_descriptografada_char[k] == 28) {
            msg_descriptografada_char[k] = 32;
            fprintf(mensagem_descriptografada_arquivo, "%c\n", msg_descriptografada_char[k]);
            continue;
        }

        msg_descriptografada_char[k] = msg_descriptografada_char[k] + 63;
        fprintf(mensagem_descriptografada_arquivo, "%c\n", msg_descriptografada_char[k]);

    }
     !!! Área com tretas no código !!! */

    fclose(mensagem_descriptografada_arquivo);
}

unsigned long long int func_euclides(unsigned long long int x, unsigned long long int y) {
    unsigned long long int res, rest;
    rest = x % y;
    if(rest == 0) {
       res = y;
       return res;
    }
    else {
        return func_euclides(y, rest); 
    }
}

unsigned long long int func_variavel_d(unsigned long long int a, unsigned long long int mod) {
    unsigned long long int d_inverso_func;
    for(d_inverso_func = 1; d_inverso_func > 0; d_inverso_func++) {
        unsigned long long int sol = (a * d_inverso_func) % mod;
        if(sol == 1) {
            printf("Inverso na função (D): %llu\n", d_inverso_func);
            return d_inverso_func;
        }
    }  
    return d_inverso_func;
}

int conferir_se_eh_primo(unsigned long long int n_func) {
    int primo10e7[1000000];     // Iniciar o array 10^7 para receber todos os valores entre 0 e 10^7
    int primo_conf[1000000];   // Receber os valores primos confirmados
    int result_p_menor = p_menorigual2(primo10e7, primo_conf, n_func);
    if(result_p_menor == 0) {
        return 0;
    }
    int result = primen(primo10e7, primo_conf, n_func);
    if(result == 0) {
        return 0;
    }
    return 1;
}

int verificar_paridade_de_p(int p_even)
{
    if(p_even % 2 == 0)
    {
        return p_even - 1;
    }
    else
    {
        return p_even - 2;
    }
}

int primen(int * primo10e7_primen, int * primo_conf_primen, unsigned long long int n_func) {
    int i, sqrtp;
    for(int p = 3; p < 1000000; p++)    // Rodar os 10^7 do array.
    {                           
        i = qtd_primo_conf;        
        primo10e7_primen[p] = 1;             // Assinar 1 (verdadeiro) à posição p por default.
        for(int m = p; m >= 2; m = m)
        {
            sqrtp = (int)sqrt(p);
            m = primo_conf_primen[qtd_primo_conf - i];
            if((p % m) == 0)// se real, o número não é primo e p deverá ser desconsiderado
            {
                primo10e7_primen[p] = 0;
                return 1;
                break;
            }
            if(m < primo_conf_primen[qtd_primo_conf]) // Para chegar ao primeiro dos primos armazenados (primo_conf_primen[0])
            {
                i--;
                continue;
            }
            if((m > 2)&&(m > sqrtp)) {
                primo10e7_primen[p] = 1;
                debug_print(primo10e7_primen, p, primo_conf_primen); // Printar na tela o número primo
                if(p == n_func) {
                    return 0;                    
                }
                break;
            }
            else if(m == primo_conf_primen[qtd_primo_conf])
            {
                if((p % m) != 0)
                {
                    debug_print(primo10e7_primen, p, primo_conf_primen); // Printar na tela o número primo
                    if(p == n_func) {
                        return 0;
                    }
                    break;
                }
            }
        }
    }
    return 1;
}

int p_menorigual2(int * primo10e7_pmenor, int * primo_conf_menor, unsigned long long int n_func)
{
    for(int i = 0; i < 3; i++)
    {
        if(i == 2)
        {
            if(n_func == 2) {
                return 0;
            }
            primo10e7_pmenor[i] = 1;
            primo_conf_menor[0] = 2;
            debug_print(primo10e7_pmenor, i, primo_conf_menor);
            break;
        }
        primo10e7_pmenor[i] = 0;
        debug_print(primo10e7_pmenor, i, primo_conf_menor);
    }
    return 1;
}

void debug_print(int * primo10e7_d, int p_d, int * primos_conf_d)
{
        if(primo10e7_d[p_d] == 1)
        {
            maior_primo = p_d;
            qtd_primo_conf++;
            primos_conf_d[qtd_primo_conf] = p_d;
        }
}

unsigned long long int func_primo_em_relacao_a_e(unsigned long long int func_e) {
    printf("Func_primo_em_relacao\n");
    int resultado_slide_59;
    for(int i = 2; i > 1; i++) {
        printf("rodada func_primo = %d", i);
        resultado_slide_59 = mdc_func(i, func_e);
        if(resultado_slide_59 == 1) {
            return i;
        }
    }
    return 1;
}

int mdc_func(int num1_mdc, unsigned long long int num2_mdc) {
    int resto = 1;
    int i;
    while(resto != 0) {
        printf("mdc func = %d\n", i);
        resto = num1_mdc % num2_mdc;
        num1_mdc = num2_mdc;
        num2_mdc = resto;
        printf("num1_mdc = %d | resto = %d\n\n", num1_mdc, resto);
        i++;
    }
    return num1_mdc;
}

/*short int func_eh_primo(unsigned long long int n_func) {
    if((n_func == 1) || (n_func == 0)) {
        return 0;
    }
    if(n_func <= 10) {
        for(int i = 2; i < n_func; i++) {
            if((n_func % i) == 0) {
                return 0;
            }
        }
        return 1;
    }
    else {
        for(int i = 2; i <= 10; i++){
            if((n_func % i) == 0){
                return 0;
            }
        }
        return 1;
    }
}*/

/*  Função do Teorema de Fermat para lidar com números muito grandes (slide 61/66)
    Substituir função "eh_primo" para descobrir que dado número é primo ou não de uma forma mais rápida*/