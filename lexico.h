#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

FILE *parq;
char lexema[256], linha[256], *fim_arq;
int poslinha, token, numlinha;


//**************************************************************************

int f(int pestado, char pcrt)
{
  int ret = 0;

  switch (pestado)
  {
    case 1:
      if (isalpha(pcrt) || pcrt == '_')
        ret = 2;
      else
        if (isdigit(pcrt))
          ret = 3;
        else
          if (pcrt == '+' || pcrt == '-')
            ret = 103;  // Operador de adição ou subt
          else
            if (pcrt == '*')
              ret = 104;  // Operador de mult
            else
              if (pcrt == '/')
                ret = 9;
              else
                if (pcrt == '=')
                  ret = 6;
                else
                  if (pcrt == '<' || pcrt == '>')
                    ret = 7;
                  else
                    if (pcrt == '!')
                      ret = 8;
                    else
                      if (pcrt == '.' || pcrt == ',' ||
                          pcrt == ':' || pcrt == ';' ||
                          pcrt == '(' || pcrt == ')')
                        ret = 108; // Símbolo
                      else
                        ret = 1001; // ERRO: caractere desconhecido
      break;
    case 2:
      if (isalnum(pcrt) || pcrt == '_')
        ret = 2;
      else
        ret = 101; // identificador
      break;
    case 3:
      if (isdigit(pcrt))
        ret = 3;
      else
        if (pcrt == '.')
          ret = 4;
        else
          ret = 102; // número inteiro
      break;
    case 4: 
      if (isdigit(pcrt))
        ret = 5;
      else
        ret = 1002; // ERRO: número fracionário incompleto
      break;
    case 5:
      if (isdigit(pcrt))
        ret = 5;
      else
        ret = 102; // número fracionário
      break;
    case 6:
      if (pcrt == '=')
        ret = 105; // operador relacional ==
      else
        ret = 107; // atribuição
      break;
    case 7:
      if (pcrt == '=')
        ret = 105; // operador relacional <= ou >=
      else
        ret = 106; // operador relacional < ou >
      break;
    case 8:
      if (pcrt == '=')
        ret = 105; // operador relacional !=
      else
        ret = 1003; // ERRO: operador relacional incompleto
      break;
    case 9:
      if (pcrt == '/')
        ret = 10;
      else
        ret = 109; // Operador de Divisão
      break;
    case 10:
      if (pcrt == '\n')
        ret = 2000; // Comentário
      else
        ret = 10;
      break;
  }
  return ret;
}

//**************************************************************************

void proximo_caractere()
{
  if (poslinha == -1 || linha[poslinha] == '\n')
  {
    fim_arq = fgets(linha, 255, parq);
    linha[strlen(linha)] = '\n';
    poslinha = 0;
    numlinha++;
  }
  else
    poslinha++;
}

//**************************************************************************

void proximo_token()
{
  int estado, ilex;

  do
  {
    estado = 1;
    ilex = 0;
    proximo_caractere();
    if (fim_arq == NULL) //o arquivo terminou
    {
      token = -1;
      strcpy(lexema, "eof");
      return;
    }
    while (linha[poslinha] == ' ' || linha[poslinha] == '\t' ||
           linha[poslinha] == '\n')
    {
      proximo_caractere();
      if (fim_arq == NULL) //o arquivo terminou
      {
        token = -1;
        strcpy(lexema, "eof");
        return;
      }
    }
    do
    {
      estado = f(estado, linha[poslinha]);
      if (estado < 1000 && estado != 101 &&
          estado != 102 && estado != 106 &&
          estado != 107 && estado != 109) //verifica se deve armazenar o caractere
      {
        lexema[ilex++] = linha[poslinha];
        proximo_caractere();
      }
    }
    while (estado < 100);

    switch (estado)  // Verifica se houve erro
    {
      case 1001:
        printf("Erro: caractere '%c' desconhecido [linha %d]\n", linha[poslinha], 
               numlinha);
        break;
      case 1002:
        lexema[ilex] = '\0';
        printf("Erro: numero '%s' incompleto [linha %d]\n", lexema,
               numlinha);
        break;
      case 1003:
        lexema[ilex] = '\0';
        printf("Erro: operador relacional '%s' incompleto [linha %d]\n", lexema,
               numlinha);
        break;
    }
    
    if (estado != 1001) // Retorna ao caractere anterior se não encontrou um caractere desconhecido
      poslinha--;
  }
  while (estado > 1000);
  lexema[ilex] = '\0'; // finaliza a string lexema
  switch (estado)
  {
    case 101:
    case 102:
    case 103:
      token = estado - 100;
      break;
    case 104:
    case 109:
      token = 4;
      break;
    case 105:
    case 106:
      token = 5;
      break;
    case 107:
    case 108:
      token = estado - 101;
      break;
  }
  if (!strcmp(lexema, "programa") ||
      !strcmp(lexema, "var") ||
      !strcmp(lexema, "inteiro") ||
      !strcmp(lexema, "real") ||
      !strcmp(lexema, "inicio") ||
      !strcmp(lexema, "fim") ||
      !strcmp(lexema, "se") ||
      !strcmp(lexema, "entao") ||
      !strcmp(lexema, "senao") ||
      !strcmp(lexema, "enquanto") ||
      !strcmp(lexema, "ler") ||
      !strcmp(lexema, "escrever"))
    token = 0;
}

