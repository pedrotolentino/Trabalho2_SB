#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string executarPreProcessamento(string nomeArq);
void executarTraducao(string in, string nomeArq);
void escreverFuncoesIO(ofstream& arq);
void escreverInput(ofstream& arq);
void escreverOutput(ofstream& arq);
void escreverC_input(ofstream& arq);
void escreverC_output(ofstream& arq);
void escreverS_input(ofstream& arq);
void escreverS_output(ofstream& arq);

/*TRADUTOR
 * Essa parte do trabalho realiza a tradução de um código em Assembly inventado para Assembly IA32
 * Para a correta execução do programa é preciso passar o nome do arquivo, com a extensão .asm, que se deseja traduzir como parametro
 * Ex.: triangulo.asm
 *
 * A saida do programa é um arquivo em Assembly IA32 com extensão .s
 * Ex.: triangulo.s*/

int main(int argc, char** argv){
	//Verifica se o numero de argumentos passado na execução está correto.
	if(argc != 2 ){
		cout<<"Numero de argumentos invalidos..."<<endl;
		cout<<"Processo de traducao abortado..."<<endl;
		exit(1);
	}

	string arqFormatado;

	//Recebimento do código sem os comentários
	arqFormatado = executarPreProcessamento(argv[1]);

	//Realiza a tradução
	executarTraducao(arqFormatado, argv[1]);

	return 0;
}

/*Função que realiza o pré processamento do arquivo passado a entrada do programa
 * Parametro: nomeArq - Nome do arquivo de entrada com extensão .asm
 *
 * Saida: String que contem o mesmo código de entrada, com exceção aos comentários*/
string executarPreProcessamento(string nomeArq){
	string arqPre;
	string line;
	string aux;
	int j;
	int i;
	int flag;

	ifstream in (nomeArq);

	if (in.is_open())
	    {
	        while (getline (in, line)) /*le cada linha do codigo*/
	        {
	            //cout << line << "\n";
	            j = 0;
	            i = 0;
	            flag = 0;
	            while ((line[j] != ';') && (line[j] != '\0')) /*le ate o fim da linha e ignora comentarios*/
	            {
	                line[j] = toupper(line[j]);
	                if (flag == 1)  /*reduz espacos e tabulacoes seguidas pelo primeiro espaco em apenas um espaco*/
	                {
	                    if ((line[j] != ' ') && (line[j] != '\t'))
	                    {
	                        flag = 0;
	                        aux = aux + line[j];
	                    }
	                }
	                else
	                {
	                    if ((line[j] == ' ') || (line[j] == '\t')) /*elimina espacos e tabulacoes desnecessarios*/
	                    {
	                        if (j != 0)
	                        {
	                            if (line[j] == '\t')
	                            {
	                                line[j] = ' ';
	                            }
	                            aux = aux + line[j];
	                        }
	                        flag = 1;
	                    }
	                    else
	                    {
	                        aux = aux + line[j];
	                    }
	                }
	                j++;
	            }
	            //cout << aux << "\n";
	            while (aux[i] != '\0')
	            {
	                //cout << aux[i] << "\n";
	                i++;
	            }
	            arqPre += aux;
	            arqPre += "\n";
	            //out << aux << "\n";
	            aux.clear();
	        }
	        in.close();
	        //out.close();
	    }
	    else cout << "Arquivo inexistente\n";

	return arqPre;
}

/*Função que realiza a tradução do arquivo em Assembly inventado para Assembly IA32
 * Parametros: in - String com o código original após o pré processamento
 *             nomeArq - Nome do arquivo de entrada com extensão .asm*/
void executarTraducao(string in, string nomeArq){
	string line;
	string  aux;
	string op1;
	string arqout = nomeArq;       /*arquivo de saida*/
	string tokens[100][5];  /*matriz de [linhas] x [tokens]*/
	string tabinst[18] = {"ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP", "C_INPUT", "C_OUTPUT", "S_INPUT", "S_OUTPUT"}; /*tabela de instrucoes*/
	int i;
	int j;
	int n = 0;

	for(string::const_iterator it = in.begin(); it != in.end(); it++){
		if(*it != '\n'){
			line = line + *it;
		}else{
			//cout << line << "\n";
			j = 0;
			i = 0;
			/*faz um vetor de tokens de cada linha*/
			while (line[j] != '\0')
			{
				if ((line[j] != ' ') && (line[j] != '\t'))
				{
					/*if (isalnum(line[j]) == 0)
					{
						if ((line[j] != '_') && (line[j] != ':'))
						{
							cout << "Erro lexico! Tokens invalidos. Linha do erro: " << n << "\n";
							return 0;
						}
						else
						{
							aux = aux + line[j];
						}
					}
					else
					{*/
						aux = aux + line[j];
					/*}*/
				}
				else
				{
					if (aux != ":")
					{
						if (aux.back() == ':')  /*armazenar um indicador de rotulos*/
						{
							aux.pop_back();
							tokens[n][i] = aux;
							i++;
							aux.clear();
							tokens[n][i] = ":";
							i++;
						}
						else        /*caso nao for um rotulo*/
						{
							tokens[n][i] = aux;
							i++;
							aux.clear();
						}
					}
					else
					{
						tokens[n][i] = aux;
						i++;
						aux.clear();
					}
				}
				j++;
			}
			tokens[n][i] = aux;
			aux.clear();
			n++;
			line.clear();
		}

	}

	ofstream out;
	out.open(arqout.replace(arqout.end()-3, arqout.end(), "s"));

//	    for(j = 0; j < n; j++)
//	    {
//	        for (i = 0; i < 10; i++)
//	        {
//	            cout << tokens[j][i] << "\n";
//	        }
//	        getchar();
//	    }

	for(j = 0; j < n; j++)
	{
		i = 0;
		int inst = 0;
		int flag = 0;

		/*verifica se eh a diretiva section*/
		if(tokens[j][i] == "SECTION")
		{
			if(tokens[j][i+1] == "TEXT")
			{
				out << "section .text\n";
				out << "global _start\n";
				out << "_start:\n";
			}
			else
			{
				//Escrita das funções de Leitura e escrita de dados
				escreverFuncoesIO(out);
				out<<endl;

				//Definições de dados comuns a todos os programas
				out << "section .data\n";
				out << "cont 	db 0"<<endl;
				out << "char	db 0"<<endl;
				out << "tam_char equ $-char"<<endl;
				out << "num 	dd 0"<<endl;
				out << "tamNum	equ	4"<<endl;
				out << "string		times 100 db 0"<<endl;
				out << "tamString   equ 100"<<endl;
				out << "msg_erro	db 'Tentativa de acesso a memoria nao reservada... Programa será encerrado', 0x0d, 0x0a"<<endl;
				out << "tam_m_erro	equ $-msg_erro"<<endl;
			}
		}
		/*caso nao for section*/
		else
		{
			/*traducao de rotulos*/
			if(tokens[j][i+1] == ":")
			{
				out << tokens[j][i] << tokens[j][i+1] << " ";
				i = i + 2;
			}

			/*busca de instrucao*/
			for(int a = 0; (a < 18)&&(flag!=1); a++)
			{
				if (tokens[j][i] == tabinst[a])
				{
					inst = a + 1;
					flag = 1;
				}
			}
			if(flag == 1)
			{
				switch(inst)
				{
					case 1: /*ADD*/
						out << "mov DWORD ebx, [" << tokens[j][i+1] << "]\n";
						out << "add eax, ebx" << "\n";
					break;
					case 2: /*SUB*/
						out << "mov DWORD ebx, [" << tokens[j][i+1] << "]\n";
						out << "sub eax, ebx" << "\n";
					break;
					case 3: /*MUL*/
						out << "mov DWORD ebx, [" << tokens[j][i+1] << "]\n";
						out << "mul ebx" << "\n";
					break;
					case 4: /*DIV*/
						out << "mov DWORD ebx, [" << tokens[j][i+1] << "]\n";
						out << "div ebx" << "\n";
					break;
					case 5: /*JMP*/
						out <<  "jmp " << tokens[j][i+1] << "\n";
					break;
					case 6: /*JMPN*/
						out << "cmp eax, 0" << endl;
						out << "jl " << tokens[j][i+1] << "\n";
					break;
					case 7: /*JMPP*/
						out << "cmp eax, 0" << endl;
						out << "jg " << tokens[j][i+1] << "\n";
					break;
					case 8: /*JMPZ*/
						out << "cmp eax, 0" << endl;
						out << "je " << tokens[j][i+1] << "\n";
					break;
					case 9: /*COPY*/
						op1 = tokens[j][i+1];
						op1 = op1.replace(op1.end()-1, op1.end(), "");
						out << "mov DWORD eax, [" << op1 << "]\n ";
						out << "mov DWORD [" << tokens[j][i+2] << "], eax" << "\n";
					break;
					case 10: /*LOAD*/
						out << "mov DWORD eax, ["<< tokens[j][i+1] << "]\n";
					break;
					case 11: /*STORE*/
						out << "mov DWORD [" << tokens[j][i+1] << "], eax\n";
					break;
					case 12: /*INPUT*/
	//                        out << "mov eax, 4\n";
	//                        out << "mov ebx, 1\n";
	//                        out << "mov ecx, " << tokens[j][i+1] << "\n";
	//                        out << "mov edx, 3\n";
	//                        out << "int 80h\n";
						out<<"call lerInteiro"<<endl;
						out<<"mov DWORD ["<<tokens[j][i+1]<<"], eax"<<endl;
					break;
					case 13: /*OUTPUT*/
	//                        out << "mov eax, 3\n";
	//                        out << "mov ebx, 0\n";
	//                        out << "mov ecx, " << tokens[j][i+1] << "\n";
	//                        out << "mov edx, 3\n";
	//                        out << "int 80h\n";
						out<<"mov eax, ["<<tokens[j][i+1]<<"]"<<endl;
						out<<"mov esi, 0"<<endl;
						out<<"call escreverInteiro"<<endl;
					break;
					case 14: /*STOP*/
						out << "mov eax, 1\n";
						out << "mov ebx, 0\n";
						out << "int 80h\n";
					break;
					case 15: /*C_INPUT*/
						out<<"call lerChar"<<endl;
						out<<"mov DWORD ["<<tokens[j][i+1]<<"], eax"<<endl;
						break;
					case 16: /*C_OUTPUT*/
						out<<"mov eax, ["<<tokens[j][i+1]<<"]"<<endl;
						out<<"call escreverChar"<<endl;
						break;
					case 17: /*S_INPUT*/
						out<<"call lerString"<<endl;
						out<<"mov ["<<tokens[j][i+1]<<"], eax"<<endl;
						break;
					case 18: /*S_OUTPUT*/
						out<<"mov eax, ["<<tokens[j][i+1]<<"]"<<endl;
						out<<"mov esi, 0"<<endl;
						out<<"call escreverChar"<<endl;
						break;
					default:
						cout<<"Opcode inválido"<<endl;
						break;
				}
			}
			else /*caso nao for instrucao*/
			{
				if(tokens[j][i] == "SPACE")
				{
					out << "dd 0\n";
				}
				else if(tokens[j][i] == "CONST")
				{
					out << "dd " << tokens[j][i+1] << "\n";
				}
			}
		}
	}
	out.close();
}

/*Função principal que chama das demais funções que tratam dados de entrada e saida em Assembly*/
void escreverFuncoesIO(ofstream& arq){
    escreverInput(arq);
    escreverOutput(arq);
    escreverC_input(arq);
    escreverC_output(arq);
    escreverS_input(arq);
    escreverS_output(arq);
}

/*Função que contem o procedimento Assembly IA32 para leitura de inteiro do teclado*/
void escreverInput(ofstream& arq){
    arq<<"lerInteiro:\tmov eax, 3\n\t\tmov ebx, 0\n\t\tmov ecx, num\n\t\tmov edx, tamNum\n\t\tint 0x80\n\t\t"
            "mov esi, 0\n\t\tmov eax, 0\n\t\tmov ebx, 0\nconverteNum:\tmov BYTE bl, [num+esi]\n\t\tcmp bl, 0xA\n\t\t"
            "je f_lerInteiro\n\t\tsub bl, 0x30\n\t\tmov edx, 0xA\n\t\tmul edx\n\t\tmovzx edx, bl\n\t\tadd eax, edx\n\t\t"
            "inc esi\n\t\tmov ebx, 0\n\t\tjmp converteNum\n\n\t\tf_lerInteiro: ret"<<endl;
}

/*Função que contem o procedimento Assembly IA32 para escrita de inteiro no monitor*/
void escreverOutput(ofstream& arq){
    arq<<"escreverInteiro:\tmov edx, 0\n\t\tmov ebx, 0XA\n\t\tdiv ebx\n\t\tadd edx, 0X30\n\t\tmov [num+esi], edx\n\t\tinc esi\n\t\t"
    		"cmp DWORD eax, 0\n\t\tje f_escreverInteiro\n\t\tjmp escreverInteiro\n\t\tf_escreverInteiro:\tmov eax, 4\n\t\t"
    		"mov ebx, 1\n\t\tlea ecx, [num+esi]\n\t\tmov edx, 1\n\t\tint 0X80\n\t\tdec esi\n\t\tcmp DWORD esi, 0\n\t\t"
    		"jge f_escreverInteiro\n\t\tret"<<endl;
}

/*Função que contem o procedimento Assembly IA32 para leitura de um caracter do teclado*/
void escreverC_input(ofstream& arq){
    arq<<"lerChar:\tmov eax, 3\n\t\tmov ebx, 0\n\t\tmov ecx, char\n\t\tmov edx, tam_char\n\t\t"
            "int 0x80\n\t\tmov byte bl, [char]\n\t\tmovzx eax, bl\n\t\tret"<<endl;
}

/*Função que contem o procedimento Assembly IA32 para escrita de um caracter no monitor*/
void escreverC_output(ofstream& arq){
    arq<<"escreverChar: mov eax, 4\n\t\tmov ebx, 1\n\t\tmov ecx, char\n\t\tmov edx, tam_char\n\t\tint 0x80\n\t\tret"<<endl;
}

/*Função que contem o procedimento Assembly IA32 para leitura de um string de até 100 caracteres do teclado*/
void escreverS_input(ofstream& arq){
    arq<<"lerString:\tmov BYTE [cont], 0x0\n\t\tmov esi, 0\n\t\tjmp inicializaString\n\t\tlacoLeitura:\tmov eax, 3\n\t\tmov ebx, 0\n\t\t"
    		"mov ecx, string\n\t\tmov edx, tamString\n\t\tint 0x80\n\t\tmov esi, 99\n\t\tcmp BYTE [string+esi], 0x0\n\t\tje fimLeitura\n\t\t"
    		"cmp BYTE [string+esi], 0xA\n\t\tje fimLeitura\n\t\tjmp lancarMsgErro\n\t\tfimLeitura:	ret\n\t\t"
    		"inicializaString: cmp BYTE [cont], 100\n\t\tje lacoLeitura\n\t\tmov BYTE [string+esi], 0\n\t\tinc BYTE [cont]\n\t\t"
    		"inc esi\n\t\tjmp inicializaString\n\t\tlancarMsgErro:\tmov eax, 4\n\t\tmov ebx, 1\n\t\tmov ecx, msg_erro\n\t\t"
    		"mov edx, tam_m_erro\n\t\tint 0x80\n\t\tmov eax, 1\n\t\tmov ebx, 0\n\t\tint 0x80"<<endl;
}

/*Função que contem o procedimento Assembly IA32 para escrita de uma string de até 100 caracteres no monitor*/
void escreverS_output(ofstream& arq){
    arq<<"escreverString:\tmov BYTE bl, [string+esi]\n\t\tcmp bl , 0xA\n\t\tje fimEscrita\n\t\tmov eax, 4\n\t\tmov ebx, 1\n\t\t"
    		"lea ecx, [string+esi]\n\t\tmov edx, 1\n\t\tint 0x80\n\t\tinc esi\n\t\tjmp escreverString\n\t\tfimEscrita:\tmov eax, 4\n\t\t"
    		"mov ebx, 1\n\t\tlea ecx, [string+esi]\n\t\tmov edx, 1\n\t\tint 0x80\n\t\tret"<<endl;
}
