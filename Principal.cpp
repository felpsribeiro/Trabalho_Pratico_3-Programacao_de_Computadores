#include <iostream>
#include "controle.h"
#include "aqBinario.h"
#include "auxiliar.h"
using namespace std;

produto * vetor; // vetor de registro com os dados do estoque enquanto o prog roda 
int tamVet; // tamanho atual do vetor
int tamReal; // espaço ultilizado do vetor

int main() {
	system("chcp 1252 > null");

	if (!read()) { // função que lê o arquivo binário
		vetor = new produto[5]; // atribui uma memória dinâmica com espaço para 5 registros
		tamVet = 5; // tamanho do vetor
		tamReal = 0; // espaço sendo ocupado
	}

	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout.precision(2);
	char leitura;

	do {
		leitura = menu(); // chama o menu principal

		switch (leitura) {
			case 'P': 
				pedir();
				system("pause");
				break;

			case 'A': 
				adicionar();
				break;

			case 'E': 
				excluir();
				break;

			case 'L': 
				listar();
				system("pause");
				break;

			case 'S':
				cout << "\nVocê apertou \"Sair\".\n";
				break;

			default: 
				cout << "\nVocê apertou algo inválido, tente novamente\n";
				system("pause");
				break;
		}
	} while (leitura != 'S');

	write(); // função que grava os dados do vetor no arquivo binário
	cout << "Fechando Sistema....\n";
	delete[] vetor;
}