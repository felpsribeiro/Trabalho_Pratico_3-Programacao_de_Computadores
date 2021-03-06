#pragma once

struct produto
{
	char nome[20];
	float preco;
	unsigned quantidade;
};

char menu();
void pedir();
void adicionar();
void excluir();
void listar();