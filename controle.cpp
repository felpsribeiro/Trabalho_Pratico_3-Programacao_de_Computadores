#include <iostream>
#include <fstream>
#include "controle.h"
#include "aqTexto.h"
#include "auxiliar.h"
using namespace std;

extern produto * vetor; // vetor de registro com os dados do estoque enquanto o prog roda
extern int tamVet; // tamanho do vetor
extern int tamReal;// espa�o ultilizado


char menu() { // fun��o para escolher o comando
	cabecalho();
	
	cout << "\nSistema de Controle\n"
 		 << "===================\n"
		 << "(P)edir\n"
		 << "(A)dicionar\n"
		 << "(E)xcluir\n"
		 << "(L)ista\n"
		 << "(S)air\n"
		 << "===================\n"
		 << "Op��o: [ ]\b\b";

	char c;
	formatarC(&c);
	cabecalho();
	return (c); // retorna o caracter escolhido
}

void pedir() {
	cout << "\nPedir\n"
		 << "-----\n"
		 << "Arquivo: ";
	char arquivo[40]; // nome do arquivo de texto
	cin >> arquivo;
	//cin.getline(arquivo, 40); 

	ifstream fin;
	fin.open(arquivo);

	if (!fin.is_open()) {// se a abertura do arquivo falhar
		cout << "O arquivo " << arquivo << " falhou na abertura\n";
		return;
	}

	int tamPed = -2; // conta a quantidade de produtos 
	//inicializado em -2 para desprezar as duas linha do cabe�alho padr�o do arquivo

	char leitor[40];
	while (fin.good()) { // conta a quantidade de linhas, isso �, a quantidade de produtos no pedido
		fin.getline(leitor, sizeof(leitor));
		if (!fin.eof() && !(leitor[0] == '\0')) 
			++tamPed;
	} 

	if (fin.eof())
		fin.close();

	else { // caso a leitura do arquivo apresente erro
		if (fin.fail())
			cout << "Tipo incorreto de dado no arquivo.\n";
		else
			cout << "Leitura do arquivo encerrada por raz�o desconhecida.\n";
		fin.close();
		return;
	}

	produto * pedido = new produto[tamPed]; // vetor com espa�o para os produtos do arquivo

	char cliente[40];

	fin.open(arquivo);
	fin.getline(cliente, sizeof(cliente)); // primeira linha do cabe�alho
	fin.getline(leitor, sizeof(leitor)); // segunda linha do cabe�alho

	int i = 0;
	for (i; i < tamPed; ++i) {
		fin >> pedido[i].nome;
		formatar(pedido[i].nome); // fun��o para padronizar o nome dos produtos (forma padr�o: "Massa")
		fin >> pedido[i].quantidade;
	}
	fin.close();

	int j = 0;
	for (i = 0; i < tamPed; ++i) {
		for (j = i + 1; j < tamPed; ++j) { 
			// se houver repei��es de pedidos no vetor
			if (!strcmp(pedido[i].nome, pedido[j].nome)) {
				pedido[i].quantidade += pedido[j].quantidade; // soma as quantidades
				pedido[j] = pedido[tamPed -1]; // move o ultimo produto para a posi��o do produto que se repetiu
				pedido[tamPed -1].nome[0] = '\0'; // medida redudante de seguran�a
				--tamPed; // diminui o tamanho utilizado em 1
			}
		}
	}

	bool falhaSemEstoque = true, aviso = false; 
	// pressup�e q vai haver um erro do tipo "sem estoque"
	// e que "Pedido falhou" n�o foi escrito ainda
	int local; // guarda o local quando a compara��o for verdadeira

	for (i = 0; i < tamPed; ++i) {
		for (j = 0; j < tamReal; ++j) {
			if (!strcmp(pedido[i].nome, vetor[j].nome)) {
				// se houver o produto do pedido em estoque
				// nega q haver� erro do tipo "sem estoque"
				falhaSemEstoque = false;
				local = j; // salva a posi��o
			}
		}
		if (falhaSemEstoque) {
			// houve erro do tipo "sem estoque"
			if (!aviso) { // se for a primeira vez q houve erro
				aviso = true;
				cout << "\nPedido falhou!\n\n";
			}
			cout << pedido[i].nome << ": Sem estoque!\n";
		}
		else { // existe em estoque e agora verificasse se tem a quantidade necess�ria
			if (pedido[i].quantidade > vetor[local].quantidade) {
				// se o pedido for maior que o estoque, erro
				if (!aviso) { // se for a primeira vez q houve erro
					aviso = true;
					cout << "\nPedido falhou!\n\n";
				}
				cout << pedido[i].nome << ": Solicitado = "
					 << pedido[i].quantidade << "Kg / Em estoque = "
					 << vetor[local].quantidade << "Kg\n";
			}
		}
		falhaSemEstoque = true; // avalia os outros produtos do pedido partindo
		// do pressuposto de que vai haver falha de "sem estoque"
	}
	
	if (!aviso) {// se n�o houve falha, o pedido pode ser realizado
		gerar(pedido, tamPed, arquivo, cliente); // realiza o abate no estoque e gera o arquivo
		cout << "Pedido realizado com sucesso, um recibo no nome de \"" << arquivo <<"\" foi criado.\n";
	}

	delete[] pedido; 
}

void adicionar() {
	produto novo;
	cout << "\nAdicionar\n"
		 << "---------\n"
		 << "Produto: "; 
	cin >> novo.nome;
	cout << "Pre�o: "; 
	cin >> novo.preco;
	cout << "Quantidade: ";
	cin >> novo.quantidade;

	formatar(novo.nome);

	for (int i = 0; i < tamReal; ++i) {
		if (!strcmp(vetor[i].nome, novo.nome)) { // verifica se o produto digitado j� tem no estoque
			vetor[i].preco = novo.preco; // atualiza o pre�o
			vetor[i].quantidade += novo.quantidade; // soma a quantidade que j� tem em estoque com a q foi digitada
			return;
		}
	}
	// se caso for digitado algo que n�o tem em estoque, verifica se tem espa�o para um novo produto 
	if (tamReal == tamReal)
		//n�o tem espa�o, � preciso expandir o vetor
		expandir();

	vetor[tamReal++] = novo; // aumenta o tamanho utilizado em 1
}

void excluir() {
	cout << "\nExcluir\n"
	   	 << "-------\n";

	if (!tamReal) { // caso o estoque esteja seco
		cout << "N�o existem produtos no estoque.\n";
		system("pause");
		return;
	}

	int i;
	for (i = 1; i <= tamReal; ++i) { // lista os produtos
		cout << i << ") " << vetor[i-1].nome << endl;
	}
	cout << "\nN�mero do produto: ";
	cin >> i;

	if (i < 1 || i > tamReal) { 
		// houve um erro
		cout << "n�mero inv�lido\n";
		system("pause");
		return;
	}

	cout << "Deseja excluir \"" << vetor[i-1].nome << "\" (S/N)? ";
	char cara;
	formatarC(&cara);

	if (cara == 'S') { // se a resposta for "s"
		vetor[i - 1] = vetor[tamReal - 1]; // substitui o produto escolhido pelo ultimo produto salvo no vetor
		vetor[tamReal - 1].nome[0] = '\0'; // medida redundante de seguran�a
		--tamReal; // diminui o tamanho utilizado em 1
	}
}

void listar() {
	cout << "\nListagem\n"
		<< "--------\n";

	if (!tamReal) { // caso o estoque esteja seco
		cout << "N�o existem produtos no estoque.\n";
		return;
	}

	for (int i = 0; i < tamReal; ++i) { // lista os produtos
		cout.width(8);
		cout << left << vetor[i].nome;
		cout << " - R$";
		cout.width(5);
		cout << left << vetor[i].preco;
		cout << " - " << vetor[i].quantidade << "kg"<< endl;
	}
}