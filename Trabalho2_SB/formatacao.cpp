#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cctype>

using namespace std;

int main()
{
    string line;
    string aux;
    string arqin = "triangulo.asm";   /*arquivo de entrada*/
    string arqout = "trianguloform.asm"; /*arquivo de saida*/
    int j, i, flag;

    ifstream in (arqin);
    ofstream out (arqout);
    if (in.is_open())
    {
        while (getline (in, line)) /*le cada linha do codigo*/
        {
            cout << line << "\n";
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
            cout << aux << "\n";
            while (aux[i] != '\0')
            {
                cout << aux[i] << "\n";
                i++;
            }
            out << aux << "\n";
            aux.clear();
        }
        in.close();
        out.close();
    }
    else cout << "Arquivo inexistente\n";

    return 0;
}
