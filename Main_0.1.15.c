#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <string.h> 
#include <dos.h>
#include <dir.h>
#include <windows.h>
#define MAX 5
#define USERNAME "aprog"
#define PASS "aproglindo123"
#define CODIGOATENDE "aten"
#define CODIGORECEBE "rece"

// Diogo - 454 ----- �ngelo - 532 -------- Tiago - 608
 
typedef struct ticket {
	int num;
	char letra;
	struct tm* _hora;
	int recebido;	
	int atendido;
	int balcao;
}ticket;

typedef struct mapas{
	int tik;
	float valor;
	int notas;
	char defeito[500];
	int tempo;
}mapas;

void SetColor(int ForgC)
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                 //Mask out all but the background attribute, and add in the forgournd     color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }
 
void gera(int *ultimoA, int *ultimoB, int *ultimoS, ticket _tickets[]){ //gera tickets do tipo a e b
	int erro1=0,menu;
	time_t segundos;
	if(*ultimoS<MAX){
		do{
			printf("\n___________________\nGerar Tickets\n\nTipo de Ticket\n1. Repara��o.\n2. Entrega.\n3. Retroceder\n___________________\n");
			scanf("%d", &menu);
			switch (menu){
				case 1:
					_tickets[*ultimoS].letra='A';
					*ultimoA=*ultimoA+1;
					_tickets[*ultimoS].num=*ultimoA;
					_tickets[*ultimoS].atendido=0;
					_tickets[*ultimoS].recebido=1;
					time(&segundos);
					_tickets[*ultimoS]._hora=localtime(&segundos);
					printf("O seu ticket � A%d.\n",_tickets[*ultimoS].num);
					*ultimoS=*ultimoS+1;
					break;
				case 2:
					_tickets[*ultimoS].letra='B';
					*ultimoB=*ultimoB+1;
					_tickets[*ultimoS].num=*ultimoB;
					_tickets[*ultimoS].atendido=0;
					_tickets[*ultimoS].recebido=1;
					time(&segundos);
					_tickets[*ultimoS]._hora=localtime(&segundos);
					printf("O seu ticket � B%d.\n",_tickets[*ultimoS].num);
					*ultimoS=*ultimoS+1;
					break;
				case 3:
					break;
				default:
					SetColor(4);
					printf("\n       ERRO\n");
					fflush(stdin);
					SetColor(15);
			}
		}while(erro1==1);
	}
	else{
		SetColor(4);
		printf("Sala de espera cheia.\n");
		SetColor(15);
		fflush(stdin);
	}
}

int escolheBalcao(int balcoes[]){ // retorna que balc�o est� livre, -1 se estiverem todos ocupados
	int i;
	for(i=0;i<4;i++){
		if(balcoes[i]==0){
			return i;
		}
	}	
	return -1;
}

void remover(int i,ticket _tickets[],int *ultimoS){//remove clientes do vetor
	for(i;i<*ultimoS;i++){
		_tickets[i]=_tickets[i+1];
	}
	(*ultimoS)--;
}

void receber(int *ultimoS, int balcoes[], ticket _tickets[]){ // ocupa balcoes
	int balcaoEscolhido, _presenca, i, ultimoAtendido, contador=0, erro=1;
	char codigo[strlen(CODIGORECEBE)];
	for(i=0;i<*ultimoS;i++){
		if(_tickets[i].recebido==1){
			contador++;
		}
	}
	if(*ultimoS>0 && contador>0){
		do{
			balcaoEscolhido=escolheBalcao(balcoes);
			if(balcaoEscolhido!=-1){
				printf("\nDigite o c�digo do funcion�rio: ");
				fflush(stdin);
				scanf("%[^\n]", codigo);
				fflush(stdin);
				if(strcmp(codigo, CODIGORECEBE)==0){
					SetColor(10);
					printf("\nFuncion�rio reconhecido\n");
					SetColor(15);
					for(i=0;i<*ultimoS;i++){
						if(_tickets[i].atendido<3){
							if(_tickets[i].recebido==1){
								if(!(_tickets[i].letra=='B' && balcaoEscolhido+1==4)){
									printf("\nO pr�ximo ticket a ser atendido � o: %c%d\nBalc�o: %d", _tickets[i].letra, _tickets[i].num, balcaoEscolhido+1);
									_presenca=presenca();
									if(_presenca==0){ //n�o est� presente
										_tickets[i].atendido++;
									}
									else{
										if(_presenca==1){ //est� presente
											balcoes[balcaoEscolhido]=1;
											_tickets[i].balcao=balcaoEscolhido;
											_tickets[i].recebido=0;
											SetColor(10);
											printf("\nCliente recebido.\n");
											SetColor(15);
											erro=0;
											break;
										}
										else{
											erro=0;
											break;
										}
									}
								}
							}
						}
						else{ //perdeu a vez
							//guardar info de tickets perdidos aqui
							printf("\nTicket %c%d fora da validade.\n",_tickets[i].letra, _tickets[i].num);
							remover(i, _tickets, &*ultimoS);
						}
					}
					break;	
				}
				else{
					SetColor(4);
					printf("\nFuncion�rio n�o reconhecido\n");
					SetColor(15);
					break;
				}
			}
			else{
				SetColor(4);
				printf("Balc�es  cheios, atenda clientes primeiro.\n");
				SetColor(15);
				erro=0;
				break;
			}	
		}while(erro==1);
	}
	else{
		SetColor(4);
		printf("\nNenhum ticket para receber.\n");
		SetColor(15);
	}
}

int presenca(){ // Testa a presen�a de um ticket e retorna 1 para presente e 0 para ausente
	int i;
	char presenca;
		do{
			printf("\nO cliente est� presente? [S/N]\nEscreva escreva T para retroceder.\n");
			fflush(stdin);
			scanf("%c", &presenca);
			fflush(stdin);
			if(toupper(presenca)=='S'){
				return 1;
			}
			else{
				if(toupper(presenca)=='N'){
					return 0;
				}
				else
				{
					if(toupper(presenca)=='T'){
					return 2;	
					}
				}
			}	
		}while(toupper(presenca)!='S'||toupper(presenca)!='N'||toupper(presenca)!='T');
}

void atender(int balcoes[], ticket _tickets[], int *ultimoS, int *atendido, mapas mapa[]){ //atende clientes, preenchendo o struct dos mapas, e liberando balc�es
	char codigo[strlen(CODIGOATENDE)];
	int balcaoAT, menu, i, erro=1, erro1=1, contador=0;
	for(i=0;i<4;i++){
		if(balcoes[i]==1){
			contador++;
		}
	}
	if(contador!=0){
		do{
			printf("\nDigite o c�digo de funcion�rio: ");
			fflush(stdin);
			scanf("%[^\n]", codigo);
			fflush(stdin);
			if(strcmp(codigo, CODIGOATENDE)==0){
				SetColor(10);
				printf("\nFuncion�rio reconhecido.\n");
				SetColor(15);
				printf("\nQue balc�o pretende atender? (0 para sair.) ");
				fflush(stdin);
				scanf("%d", &balcaoAT);
				fflush(stdin);
				if(balcaoAT==0){
					break;
				}
				else{
					if(balcaoAT==1 || balcaoAT==2 || balcaoAT==3 || balcaoAT==4){
						if(balcoes[balcaoAT-1]==1){	
							for(i=0;i<*ultimoS;i++){
								if(_tickets[i].balcao==balcaoAT-1);
								break;
							}
							do{
								printf("\nMENU\n1.Notas\n2.Valor do equipamento\n3.Defeito\n4.Terminar atendimento\n");
								menu=-1;
								fflush(stdin);
								scanf("%d", &menu);
								fflush(stdin);
								switch(menu){
									case 1:
										printf("\nEditar notas: \n");
										//editar notas
										break;
									case 2:
										do{
											printf("\nQual o valor do equipamento? ");
											scanf("%f", &mapa[*atendido].valor);
											if(&mapa[*atendido].valor<=0){
												SetColor(4);
												printf("\nValor inv�lido\n");
												SetColor(15);
											}
										}while(mapa[*atendido].valor<=0);
										break;
									case 3:
										printf("\n\nQual o defeito?");
										//escolher motivo, editar se for um nao listado
										break;
									case 4: //validar se os outros campos foram preenchidos
										balcoes[balcaoAT-1]=0;
										remover(i,_tickets,&*ultimoS);
										erro=0;
										erro1=0;
										break;
									(atendido)++;
									default:
										SetColor(4);
										printf("\nEscolha inv�lida\n");
										SetColor(15);
								}
							}while(erro==1);
						}
						else{
							SetColor(4);
							printf("\nBalc�o vazio\n");
							SetColor(15);
							break;
						}
					}
					else{
						SetColor(4);
						printf("\nBalc�o inv�lido.\n");
						SetColor(15);
					}
				}
			}
			else{
				SetColor(4);
				printf("\nFuncion�rio n�o reconhecido\n");
				SetColor(15);
				break;
			}
		}while(erro1==1);
	}
	else{
		SetColor(4);
		printf("\nNenhum cliente para atender.\n");
		SetColor(15);
	}
}

int main () {
	int i, menu, n, ultimoA=0, ultimoB=0, ultimoS=0, ultimoAT=0, balcoes[4],atendido=0;
	for(i=0;i<4;i++) balcoes[i]=0;
	char username[strlen(USERNAME)], pass[strlen(PASS)];
	mapas mapa[100];
	ticket _tickets[MAX];
	setlocale(LC_ALL,"Portuguese");
	printf("Sistema autom�tico de senhas,\nFa�a login no sistema\nDigite 'sair' no username ou na password para sair \n");
	do{	
		printf("\nUsername: ");
		fflush(stdin);
		scanf("%[^\n]", username);
		fflush(stdin);
		if(strcmp(username, USERNAME)==0){
			SetColor(10);
			printf("\nUsername reconhecido\n");
			SetColor(15);
			do{
				printf("\nPassword:  "); 
				int p=0; 
				do{ 
	  				pass[p]=getch(); 
	   				if(pass[p]!='\r'){ 
	    				printf("*"); 
	   				} 
					p++; 	
				}while(pass[p-1]!='\r'); 
				pass[p-1]='\0'; 
				fflush(stdin);
				if(strcmp(pass, PASS)==0){
					SetColor(10);
					printf("\n\nLogin bem sucedido\n\n");
					SetColor(15);
					do{
						SetColor(1);
						printf("___________________\n       Menu");
						SetColor(15);
						printf("\n\n1. Gerar Ticket\n2. Chamar Tickets\n3. Atender Tickets\n4. Gerar mapas\n5. Sair\n");
						SetColor(1);
						printf("___________________\n");
						SetColor(15);
						menu=-1;
						fflush(stdin);
						scanf("%d",&menu);
						fflush(stdin);
						switch (menu){
						case 1: 
							gera(&ultimoA,&ultimoB, &ultimoS, _tickets);		 
							break;
						case 2:
							receber(&ultimoS, balcoes, _tickets);
							break;
						case 3:
							atender(balcoes, _tickets, &ultimoS, &atendido, mapa);
							break;
						case 4:
							printf("Funcao ainda nao implementada!");
							break;
						case 5:
							return 0;
						default:
							SetColor(4);
							printf("\nEscolha inv�lida.\n");
							fflush(stdin);
							SetColor(15);
						}
					}while(1);
				}
				else{
						SetColor(4);
						printf("\n\nPassWord inv�lida.\n");
						SetColor(15);
				}
			}while(1);
		}
		else{
				SetColor(4);
				printf("\nUsername inv�lido.\n");
				SetColor(15);
		}
	}while(1);
	return 0;
}