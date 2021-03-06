#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "controle.h"
#include "auxiliar.h"
using namespace std;

extern produto* vetor; // vetor de registro com os dados do estoque enquanto o prog roda
extern int tamVet; // tamanho do vetor
extern int tamReal;// espaço ultilizado

void gerar(produto* pedido, int tamP, char* nome, char* cliente) { // gera o recibo do pedido

	int i = 0, j = 0;
	float totalcompra = 0.0f;

	strcpy(strchr(nome, '.'), ".nfc"); // muda de ".txt" para ".nfc"

	ofstream fout;
	fout.open(nome);

	fout << cliente
		 << "\n--------------------------------------\n";

	for (i = 0; i < tamReal; ++i) {
		for (j = 0; j < tamP; ++j) {
			if (!strcmp(pedido[j].nome, vetor[i].nome)) {
				vetor[i].quantidade -= pedido[j].quantidade; // diminui a quantidade do estoque
				
				// escreve no arquivo
				fout << pedido[j].nome << ": " << pedido[j].quantidade
					<< "Kg a R$" << vetor[i].preco << "/Kg = R$"
					<< (float(pedido[j].quantidade) * vetor[i].preco) << endl;

				totalcompra += float(pedido[j].quantidade) * vetor[i].preco; // salva o total do pedido
			}

			if (vetor[i].quantidade == 0) { // se o produto for zerado no estoque
				vetor[i] = vetor[tamReal - 1]; // substitui pelo ultimo do estoque 
				vetor[tamReal - 1].nome[0] = '\0'; // medida redundante de segurança
				--tamReal; // diminui 1 no tamanho de produtos
				i = 0;
			}
		}
	}

	fout << "--------------------------------------\n"
		<< "Compra = R$"
		<< totalcompra
		<< "\nDesconto = R$";

	if (totalcompra >= 1000.00f) { // se houver desconto
		fout << totalcompra * 0.1f // valor do desconto
			<< "\nTotal = R$"
			<< totalcompra * 0.9f;
	}
	else { // não houve desconto
		fout << "0.00"
			<< "\nTotal = R$"
			<< totalcompra;
	}

	fout.close();
}