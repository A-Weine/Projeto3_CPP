#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "menus.h"
#include "cstring"
using namespace std;

produto* expandir_vetorpdt(produto* produtos, int tamanho) { // Fun��o para expandir tamanho de vetor din�mico.
	produto* temporario = new produto[tamanho + 1]; // Cria vetor dinamico temporario com tamanho acrescido de uma unidade.
	for (int i = 0; i < tamanho; i++) { // Loop percorrendo novo vetor para fazer c�pia do conte�do do vetor antigo.
		strcpy(temporario[i].nome, produtos[i].nome); // Copia nome do produto da posi��o atual para vetor novo.
		temporario[i].preco = produtos[i].preco; // Copia pre�o do produto da posi��o atual para vetor novo.
		temporario[i].quant = produtos[i].quant; // Copia quantidade do produto da posi��o atual para vetor novo.
	}
	delete[] produtos; // Deleta mem�ria alocada dinamicamente do vetor antigo

	return temporario; // Retorna endere�o da nova mem�ria alocada.
}
float menupagamento(produto* cesta, int quant_cesta, float total) {
	float desconto = 0;
	char pagamento;
	//Tela de pagamento
	cout << "===========" << endl;
	cout << " RapiZinho " << endl;
	cout << "===========" << endl;
		
	for (int i = 0; i < quant_cesta; i++) { // La�o para imprimir os itens com seus pre�os.
		cout << cesta[i].quant << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].preco * float(cesta[i].quant); // Imprime itens e seus valores.
		cout << endl;
	}

	cout << "Taxa de entrega = R$6.00" << endl;
	cout << "===========" << endl;
	cout << "Total = R$" << total << endl << endl;

	cout << "[P] Pix" << endl;
	cout << "[C] Cart�o" << endl << endl;

	do {
		cout << "Pagamento: [ ]\b\b";
		cin >> pagamento; // Usu�rio seleciona tipo de pagamento.
		pagamento = toupper(pagamento);


		if (pagamento == 'P') { // Caso escolha pix possui desconto de 10%.
			desconto = 10;
		}
		else if (pagamento == 'C') { // Caso escolha cart�o possui desconto de 5%.
			desconto = 5;
		}
		else {
			cout << "===========" << endl;
			cout << "Escolha inv�lida" << endl;
			cout << "===========" << endl;
		}
	} while (pagamento != 'P' && pagamento != 'C');

	return desconto;
}

void confirmarpedido(produto* cesta, int quant_cesta, float total, float desconto, int num_pedido) {
	ofstream fout; // Criada vari�vel para realizar manipula��o de arquivos (Inserir dados).
	char num_pedidostr[30] = " "; // Vari�vel que vai receber n�mero do pedido convertido de inteiro para caractere.
	char nomearquivo[30] = "pedido_"; // Base do nome do arquivo.
	char conf_pedido; // Vari�vel de escolha.
	float totaldsc = 0; // Vari�vel de total com desconto.

	cout << "===========" << endl;
	cout << " RapiZinho " << endl;
	cout << "===========" << endl;
	for (int i = 0; i < quant_cesta; i++) { // La�o para imprimir os itens com seus pre�os.
		cout << cesta[i].quant << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].preco * float(cesta[i].quant);
		cout << endl;
	}
	cout << "Taxa de entrega = R$6.00" << endl;
	cout << "Desconto de " << desconto << "% = " << total * (desconto / 100); // Mostra desconto em % e no valor em reais.
	cout << endl << "===========" << endl;
	totaldsc = total - total * (desconto / 100); // Calcula total com desconto.
	cout << "Total = R$" << totaldsc << endl;

	cout << "===========" << endl;

	cout << "Confirma Pedido (S/N): [ ]\b\b";
	cin >> conf_pedido;
	cout << "===========" << endl;

	conf_pedido = toupper(conf_pedido);
	if (conf_pedido == 'S') {
		_itoa(num_pedido, num_pedidostr, 10); // Transforma o n�mero do pedido (inteiro) em um vetor de caracteres e coloca na outra variavel.
		strcat(nomearquivo, num_pedidostr); // Junta o nome do arquivo com o n�mero gerado.
		strcat(nomearquivo, ".txt"); // Adiciona a extens�o do arquivo no final do vetor de caracteres.

		fout.open(nomearquivo); // Feita abertura do arquivo com o nome gerado.

		fout << fixed;
		fout.precision(2); // Define casas de ponto flutuante para 2.
		
		fout << "Pedido #" << num_pedido << endl;
		fout << "--------------------------------------------------" << endl;
		for (int i = 0; i < quant_cesta; i++) { // La�o para imprimir os itens com seus pre�os.
			fout << cesta[i].quant << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].preco * float(cesta[i].quant);
			fout << endl;
		}
		fout << "Taxa de entrega = R$6.00" << endl;
		fout << "Desconto de " << desconto << "% = " << total * (desconto / 100) << endl;
		fout << "--------------------------------------------------" << endl;
		fout << "Total = R$" << totaldsc << endl;

		fout.close();
	}
	else if (conf_pedido == 'N') {
		for (int i = 0; i < quant_cesta; i++) { // Caso cliente escolha n�o finalizar o pedido, a cesta � esvaziada.
			strcpy(cesta[i].nome, " ");
			cesta[i].preco = 0;
			cesta[i].quant = 0;
		}
	}
}