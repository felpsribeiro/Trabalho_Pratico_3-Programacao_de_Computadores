#include <iostream>
#include <cmath>
#include "controle.h"
using namespace std;

extern produto * vetor; // vetor de registro com os dados do estoque enquanto o prog roda
extern int tamVet; // tamanho do vetor
extern int tamReal;// espaço ultilizado

void cabecalho() {
	system("cls");
	cout << "\n============================================================\n"
		<< "SVCEJ: Sistema de Vendas e Controle de Estoque de Joãozinho!\n"
		<< "============================================================\n";
}

void expandir() { // expadandir o vetor com os produtos
	static int nExpan = 0; // salva a quantidades de expanções

	int novoTam = (tamVet + (int)pow(2, nExpan));
	produto* novoVet = new produto[novoTam]; // vetor com novo tamanho

	for (int i = 0; i < tamVet; i++)
		novoVet[i] = vetor[i]; // copia as informações do vetor antigo

	delete[] vetor; // apaga a memória dinâmica usada pelo vetor antigo
	vetor = novoVet; // atribui o endereço de memória do novo vetor ao velho vetor
	tamVet = novoTam; // e atribiu o novo tamanho do vetor
	nExpan++; // adiciona 1 na variável que guarda o número de expanções
}

void formatar(char* entrada) { // formatar palavras
	*entrada = toupper(*(entrada)); // primeira letra maiscula

	int i = 1;
	while (entrada[i]) {
		*(entrada + i) = tolower(*(entrada + i)); //restante minuscula
		i++;
	}
}

void formatarC(char* carac) { // formatar caracteres
	char c;
	cin >> c;
	*carac = toupper(c);
}