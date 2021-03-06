#include <iostream>
#include <fstream>
#include "controle.h"
#include "auxiliar.h"
using namespace std;


extern produto * vetor; // vetor de registro com os dados do estoque enquanto o prog roda
extern int tamVet; // tamanho do vetor
extern int tamReal;// espa�o ultilizado

bool read() { // fun��o que l� o arquivo bin�rio
	ifstream fin;

	fin.open("estoque.bin", ios_base::in | ios_base::binary);
	if (!fin.is_open())
		return (false);

	fin.read((char*)&tamVet, sizeof(int)); // recebe a quantidade de produtos salvos no arquivo
	vetor = new produto[tamVet]; // cria um vetor dinamico com o tamanho de produtos no arquivo

	for (int i = 0; i < tamVet; ++i)
		fin.read((char*)&vetor[i], sizeof(produto)); // recebe os dados do arquivo bin�rio e  salva o vetor

	tamReal = tamVet; // nesse momento, o tamanho do vetor � o mesmo do tamaho utilizado 

	fin.close();
	return (true);
}

void write() { // fun��o que escreve o arquivo bin�rio
	ofstream fout;
	fout.open("estoque.bin", ios_base::out | ios_base::trunc | ios_base::binary);

	fout.write((char*)&tamReal, sizeof(int)); // escreve a quantidade de produtos que v�o estar salvos no arquivo
	for (int i = 0; i < tamReal; ++i)
		fout.write((char*)&vetor[i], sizeof(produto)); // grava os dados dos produtos

	fout.close();
	cout << "Estoque salvo....\n";
}