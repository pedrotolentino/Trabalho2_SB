/*UnB - Universidade de Brasilia
Trabalho 2
Alunos: Alexandre Correia Mesquita de Oliveira ----12/0151294
Pedro Henrique Souza Tolentino --------------------11/0073967
*/


#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <elfio/elfio.hpp>

using namespace std;

string* gerarObjeto(string nomeArq);
string tratarString(string entrada);
void gerarExecutavel(const char* sText, const char* sData, string nomeArq);

/*CARREGADOR
 * Essa parte do trabalho realiza a geração de um arquivo executável em formato ELF a partir de uma entrada em Assemmbly IA32
 * Para a correta execução do programa é preciso passar o nome do arquivo, com a extensão .s, que se deseja obter o executável como parametro
 * Ex.: triangulo.s
 *
 * A saida do programa é um arquivo Linux executável em formato ELF
 * Ex.: triangulo*/

int main( int argc, char** argv ){
	if(argc != 2 ){
		cout<<"Numero de argumentos invalidos..."<<endl;
		cout<<"Processo de traducao abortado..."<<endl;
		exit(1);
	}

	string* codObj; //Vetor de string com as seções, codObj[0] - text, codObj[1] - data

	codObj = gerarObjeto(argv[1]);

	const char* text = tratarString(codObj[0]).c_str();
	const char* data = tratarString(codObj[1]).c_str();

	gerarExecutavel(text, data, argv[1]);
	return 0;
}

string* gerarObjeto(string nomeArq){

	string line;
	string textTeste;
	string dataTeste;
	string result[2];
	int flag = 0;

	ifstream in (nomeArq);
    if(in.is_open())
    {
    	while(getline(in, line))
    	{
    		switch(flag)
    		{
    			case 1:
    				textTeste = textTeste += line;
    			break;
    			case 2:
    				dataTeste = dataTeste += line;
    			break;
    		}
    		if(line == "section .text")
    		{
    			flag = 1;
    			textTeste = line;
    		}
    		else if(line == "section .data")
    		{
    			flag = 2;
    			dataTeste = line;
    		}
    	}
    }

	result[0] = textTeste;
	result[1] = dataTeste;

	return result;
}

string tratarString(string entrada){
	string aux;
	stringstream result;
	int num;

	aux.clear();
	for(string::const_iterator it = entrada.begin(); it != entrada.end(); it++){
		if(*it != ' '){
			aux = aux + *it;
		}else{
			num = stoi(aux, nullptr, 16);
			result << hex << num;
			aux.clear();
		}
	}

	return result.str();
}

/*Função que gera o arquivo Linux executável
 * Parametros: sText - String tratada com os códigos da seção de texto
 *             sData - String tratada com os códigos da seção de dados
 *             nomeArq - String com o nome do arquivo de entrada com extensão .s*/
void gerarExecutavel(const char* sText, const char* sData, string nomeArq){
	ELFIO::elfio escrita;

	escrita.create(ELFCLASS32, ELFDATA2LSB);

	escrita.set_os_abi(ELFOSABI_LINUX);
	escrita.set_type(ET_EXEC);
	escrita.set_machine(EM_386);

	//Criação da section Text
	ELFIO::section* sectionText = escrita.sections.add( ".text" );
	sectionText->set_type( SHT_PROGBITS );
	sectionText->set_flags( SHF_ALLOC | SHF_EXECINSTR );
	sectionText->set_addr_align( 0x10 );

	sectionText->set_data(sText, sizeof(sText));

	//Criação de segmento carregável
	ELFIO::segment* segText = escrita.segments.add();
	segText->set_type( PT_LOAD );
	segText->set_virtual_address( 0x08048000 );
	segText->set_physical_address( 0x08048000 );
	segText->set_flags( PF_X | PF_R );
	segText->set_align( 0x1000 );

	segText->add_section_index(sectionText->get_index(), sectionText->get_addr_align());

	//Cria a section Data
	ELFIO::section* sectionData = escrita.sections.add( ".data" );
	sectionData->set_type( SHT_PROGBITS );
	sectionData->set_flags( SHF_ALLOC | SHF_WRITE );
	sectionData->set_addr_align( 0x4 );

	sectionData->set_data(sData, sizeof(sData));

	//Criação de segmento carregável
	ELFIO::segment* segData = escrita.segments.add();
	segData->set_type( PT_LOAD );
	segData->set_virtual_address( 0x08048020 );
	segData->set_physical_address( 0x08048020 );
	segData->set_flags( PF_W | PF_R );
	segData->set_align( 0x10 );

	segData->add_section_index( sectionData->get_index(), sectionData->get_addr_align() );

	//Criação de ponto de entrada
	escrita.set_entry(0x08048000);

	//Criação do arquivo ELF
	escrita.save(nomeArq.replace(nomeArq.end()-2, nomeArq.end(), ""));
}
