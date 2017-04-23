/* An�lise Sint�tica
** Trabalho GLC de uma linguagem de programa��o
* Aluno: Bruno Barbosa RGM: 14012103

Inicia em <programa>.

<programa> ::= programa identificador ; <corpo>
<corpo> ::= <declara��o> <bloco> .
<declara��o> ::= var <vari�veis> : <tipo> ; <declara��o> | �
<vari�veis> ::= identificador <mais-vari�veis>
<mais-vari�veis> ::= , <vari�veis> | �
<tipo> ::= inteiro | real
<bloco> ::= inicio <comandos> fim
<comandos> ::= identificador = <express�o> ; <comandos> |
 se ( <condi��o> ) entao <bloco> <sen�o> ; <comandos> |
 enquanto ( <condi��o> ) <bloco> ; <comandos> |
 ( ler | escrever ) ( <vari�veis> ) ; <comandos> | �
<sen�o> ::= senao <bloco> | �
<condi��o> ::= <express�o> <operador-rel> <express�o>
<express�o> ::= <termo> <mais-express�o>
<mais-express�o> ::= ( + | - ) <express�o> | �
<termo> ::= <fator> <mais-termo>
<mais-termo> ::= ( * | / ) <termo> | �
<fator> ::= identificador | n�mero | ( <express�o> )
<operador-rel> ::= != | == | > | < | >= | <=
*/

#include "lexico.h"

void programa();
void corpo();
void declaracao();
void variaveis();
void mais_variaveis();
void tipo();
void bloco();
void comandos();
void senao();
void condicao();
void expressao();
void mais_expressao();
void termo();
void mais_termo();
void fator();
void operador_rel();

main()
{
  char nomearq[256];

  poslinha = -1;
  numlinha = 0;
  fim_arq = (char*) "nao";
  printf("Arquivo: ");
  gets(nomearq);
  parq = fopen(nomearq, "r");
  if (parq == NULL)
  {
    printf("Erro ao abrir o arquivo!!!");
  }
  else // início da análise sintática
  {
  	system("cls");
  	
		proximo_token();
		
		programa();  // primeira regra da GLC
		
		if (token == -1)
			printf("\n\nOk!!");
		else
			printf("\n\nERRO no final do arquivo...");		

  	fclose(parq);
	}
	
  printf("\n\n\n");
  system("pause");
}

void erro(int pcod)
{
	printf("\n\nERRO!! ");
	switch (pcod)
	{
		case 1: printf("Falta 'programa'"); break;
		case 2: printf("Falta um identificador"); break;
		case 3: printf("Falta ';'"); break;
		case 4: printf("Falta '.'"); break;
		case 5: printf("Falta ':'"); break;
		case 6: printf("Falta 'inteiro' ou 'real'"); break;
		case 7: printf("Falta 'inicio'"); break;
		case 8: printf("Falta 'fim'"); break;
		case 9: printf("Falta '='"); break;
		case 10: printf("Falta '('"); break;
		case 11: printf("Falta ')'"); break;
		case 12: printf("Falta 'senao'"); break;
		case 13: printf("Falta identificador, número ou (expressão)"); break;
		case 14: printf("Falta operador de comparação"); break;
	}
	
	printf("  [linha %d]\n\n\n", numlinha);
	
	system("pause");
	exit(1); // sair do programa
}

void programa()
{
	if (strcmp(lexema, "programa") == 0)
	{
		proximo_token();
		if (token == 1)
		{
			proximo_token();
			if (strcmp(lexema, ";") == 0)
			{
				proximo_token();
				corpo();
			}
			else
				erro(3); // Falta ;
		}
		else 
			erro(2); // Falta um identificador
	}
	else
		erro(1); // Falta programa
	
}

void corpo()
{
	declaracao();
	bloco();
	if (strcmp(lexema, ".") == 0)
		proximo_token();
	else 
		erro(4); // Falta .
}

void declaracao()
{
	if (strcmp(lexema, "var") == 0)
	{
		proximo_token();
		variaveis();
		if (strcmp(lexema, ":") == 0)
		{
			proximo_token();
			tipo();
			if (strcmp(lexema, ";") == 0)
			{
				proximo_token();
				declaracao();
			}
			else 
				erro(3); // Falta ;
		}
		else
			erro(5); // Falta :
	}
}

void variaveis()
{
	if (token == 1)
	{
		proximo_token();
		mais_variaveis();
	}
	else 
		erro(2); // Falta um identificador
}

void mais_variaveis()
{
	if (strcmp(lexema, ",") == 0)
	{
		proximo_token();
		variaveis();
	}
}

void tipo()
{
	if (strcmp(lexema, "inteiro") == 0 || strcmp(lexema, "real") ==0)
		proximo_token();
	else
		erro(6); // Falta inteiro ou real
}

void bloco()
{
	if (strcmp(lexema, "inicio") == 0)
	{
		proximo_token();
		comandos();
		if (strcmp(lexema, "fim") == 0)
			proximo_token();
		else
			erro(8); // Falta fim
	}
	else
		erro(7); // Falta inicio
}

void comandos()
{
	if (strcmp(lexema, "se") == 0)
	{
		proximo_token();
		if (strcmp(lexema, "(") == 0)
		{
			proximo_token();
			condicao();
			if (strcmp(lexema, ")") == 0)
			{
				proximo_token();
				if (strcmp(lexema, "entao") == 0)
				{
					proximo_token();
					bloco();
					senao();
					if (strcmp(lexema, ";") == 0)
					{
						proximo_token();
						comandos();
					}
					else
						erro(3); // Falta ;
				}
				else
					erro(12); // Falta senao
			}
			else
				erro(11); // Falta )
		}
		else
			erro(10); // Falta (
	}
	else if (strcmp(lexema, "enquanto") == 0)
	{
		proximo_token();
		if (strcmp(lexema, "(") == 0)
		{
			proximo_token();
			condicao();
			if (strcmp(lexema, ")") == 0)
			{
				proximo_token();
				bloco();
				if (strcmp(lexema, ";") == 0)
				{
					proximo_token();
					comandos();
				}
				else
					erro(3); // Falta ;
			}
			else
				erro(11); // Falta )
		}
		else
			erro(10); // Falta ( 
	}
	else if (strcmp(lexema, "ler") == 0 || strcmp(lexema, "escrever") == 0)
	{
		proximo_token();
		if (strcmp(lexema, "(") == 0)
		{
			proximo_token();
			variaveis();
			if (strcmp(lexema, ")") == 0)
			{
				proximo_token();
				if (strcmp(lexema, ";") == 0)
				{
					proximo_token();
					comandos();
				}
				else
					erro(3); // Falta ;
			}
			else
				erro(11); // Falta )
		}
		else
			erro(10); // Falta ()
	}
	else if (token == 1)
	{
		proximo_token();
		if (strcmp(lexema, "=") == 0)
		{
			proximo_token();
			expressao();
			if (strcmp(lexema, ";") == 0)
			{
				proximo_token();
				comandos();
			}
			else
				erro(3); // Falta ; 
		}
		else 
			erro(9); // Falta = 
	}
}

void senao()
{
	if (strcmp(lexema, "senao") == 0)
	{
		proximo_token();
		bloco();
	}
}

void condicao()
{
	expressao();
	operador_rel();
	expressao();
}

void expressao()
{
	termo();
	mais_expressao();
}

void mais_expressao()
{
	if (token == 3) // operador + ou -
	{
		proximo_token();
		expressao();
	}
}

void termo()
{
	fator();
	mais_termo();
}

void mais_termo()
{
	if (token == 4) // operador * ou /
	{
		proximo_token();
		termo();
	}
}

void fator()
{
	if (token == 1 || token == 2) // identificador ou número
		proximo_token();
	else if (strcmp(lexema, "(") == 0)
	{
		proximo_token();
		expressao();
		if (strcmp(lexema, ")") == 0)
			proximo_token();
		else
			erro(11); // Falta )
	}
	else 
		erro(13); // Falta identificador, número ou (expressão)
}

void operador_rel()
{
	if (token == 5) // operador de comparação
		proximo_token();
	else
		erro(14); // Falta operador de comparação
}
