#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void escreverFuncoesIO(ofstream& arq);
void escreverInput(ofstream& arq);
void escreverOutput(ofstream& arq);
void escreverC_input(ofstream& arq);
void escreverC_output(ofstream& arq);
void escreverS_input(ofstream& arq);
void escreverS_output(ofstream& arq);

int main ()
{

    string line;
    string  aux;
    string arqin = "trianguloform.asm";     /*arquivo de entrada*/
    string arqout = "triangulo.s";       /*arquivo de saida*/
    string tokens[100][5];  /*matriz de [linhas] x [tokens]*/
    string tabsimb[100][2]; /*tabela de simbolos*/
    string tabinst[14] = {"ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"}; /*tabela de instrucoes*/
    string tabdir[3] = {"SECTION", "SPACE", "CONST"};      /*tabela de diretivas*/
    int i, j, n;

    /*leitura*/

    ifstream in (arqin);
    if(in.is_open())
    {
        n = 0;
        while (getline (in, line)) /*le cada linha do arquivo*/
        {
            cout << line << "\n";
            j = 0;
            i = 0;
            /*faz um vetor de tokens de cada linha*/
            while (line[j] != '\0')
            {
                if ((line[j] != ' ') && (line[j] != '\t'))
                {
                    if (isalnum(line[j]) == 0)
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
                    {
                        aux = aux + line[j];
                    }
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
        }
        in.close();
    }
    else cout << "Arquivo inexistente\n";

    /*traducao*/

    cout << "lista de tokens\n";
    for(int z = 0; z < n; z++)
    {
        for(int w = 0; w < 5; w++)
        {
            cout << tokens[z][w] << "\n";
        }
        getchar();
    }

    ofstream out;
    out.open(arqout);

    for(j = 0; j < n; j++)
    {
        i = 0;
        int inst = 0;
        int flag = 0;
        cout << tokens[j][i] << "\n";
        getchar();

        /*verifica se eh a diretiva section*/
        if(tokens[j][i] == "SECTION")
        {
            if(tokens[j][i+1] == "TEXT")
            {
                out << "section .text\n";
            }
            else
            {
                out << "section .data\n";
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
            for(int a = 0; a < 14; a++)
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
                        out << "add eax, " << "\n";
                    break;
                    case 2: /*SUB*/
                        out << "sub eax, " << "\n";
                    break;
                    case 3: /*MUL*/
                        out << "mul eax, " << "\n";
                    break;
                    case 4: /*DIV*/
                        out << "div eax, " << "\n";
                    break;
                    case 5: /*JMP*/
                        out <<  "jmp " << tokens[j][i+1] << "\n";
                    break;
                    case 6: /*JMPN*/
                        out << "js eax, " << tokens[j][i+1] << "\n";
                    break;
                    case 7: /*JMPP*/
                        out << "jns eax, " << tokens[j][i+1] << "\n";
                    break;
                    case 8: /*JMPZ*/
                        out << "cmp eax, 0\n";
                        out << "je " << tokens[j][i+1] << "\n";
                    break;
                    case 9: /*COPY*/
                        out << "mov " << tokens[j][i+1] << ", " << tokens[j][i+2] << "\n";
                    break;
                    case 10: /*LOAD*/
                        out << "mov eax, "<< tokens[j][i+1] << "\n";
                    break;
                    case 11: /*STORE*/
                        out << "mov " << tokens[j][i+1] << ", eax\n";
                    break;
                    case 12: /*INPUT*/
//                        out << "mov eax, 4\n";
//                        out << "mov ebx, 1\n";
//                        out << "mov ecx, " << tokens[j][i+1] << "\n";
//                        out << "mov edx, 3\n";
//                        out << "int 80h\n";
                    	out<<"call lerInteiro"<<endl;
                    	out<<"mov "<<tokens[j][i+1]<<", [eax]"<<endl;
                    break;
                    case 13: /*OUTPUT*/
//                        out << "mov eax, 3\n";
//                        out << "mov ebx, 0\n";
//                        out << "mov ecx, " << tokens[j][i+1] << "\n";
//                        out << "mov edx, 3\n";
//                        out << "int 80h\n";
                    	out<<"mov eax, "<<tokens[j][i+1]<<endl;
                    	out<<"call escreverInteiro"<<endl;
                    break;
                    case 14: /*STOP*/
                        out << "mov eax, 1\n";
                        out << "mov ebx, 0\n";
                        out << "int 80h\n";
                    break;
                    case 15: /*C_INPUT*/
                    	out<<"call lerChar"<<endl;
                    	out<<"mov "<<tokens[j][i+1]<<", [eax]"<<endl;
                    	break;
                    case 16: /*C_OUTPUT*/
                    	out<<"mov eax, "<<tokens[j][i+1]<<endl;
                    	out<<"call escreverChar"<<endl;
                    	break;
                    case 17: /*S_INPUT*/
                    	out<<"call lerString"<<endl;
                    	out<<"mov "<<tokens[j][i+1]<<", eax"<<endl;
                    	break;
                    case 18: /*S_OUTPUT*/
                    	out<<"mov eax, "<<tokens[j][i+1]<<endl;
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
                    out << "resb 00\n";
                }
                else if(tokens[j][i] == "CONST")
                {
                    out << "db " << tokens[j][i+1] << "\n";
                }
            }
        }
    }
    escreverFuncoesIO(out);
    out.close();

    return 0;

}

void escreverFuncoesIO(ofstream& arq){
	escreverInput(arq);
	escreverOutput(arq);
	escreverC_input(arq);
	escreverC_output(arq);
	escreverS_input(arq);
	escreverS_output(arq);
}

void escreverInput(ofstream& arq){
	arq<<""<<endl;
}

void escreverOutput(ofstream& arq){
	arq<<""<<endl;
}

void escreverC_input(ofstream& arq){
	arq<<""<<endl;
}

void escreverC_output(ofstream& arq){
	arq<<""<<endl;
}

void escreverS_input(ofstream& arq){
	arq<<""<<endl;
}

void escreverS_output(ofstream& arq){
	arq<<""<<endl;
}
