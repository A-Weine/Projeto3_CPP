#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "menus.h"
using namespace std;

int main(int argc, char ** argv) {
	// Cria��o das vari�veis.
	ifstream fin; // Vari�vel para leitura de dados de arquivos.
	ofstream fout; // Vari�vel para escritura de dados em arquivos. 

	char escolha; // Vari�veis de escolha das op��es dos menus.

	int quantidade; // Referente a quantidade solicitada ao realizar pedido.
	int quant_cesta = 0; // Quantidade de itens j� na cesta.
	unsigned short quant_estoque = 0; // Quantidade de itens j� no estoque.
	int possui_cesta = 0; // Variavel para verificar se item j� est� na cesta.
	unsigned short TamVet = 0; // Tamanho dos vetores
	int numpedido = 1; // N�mero de pedidos inicial.
	float total = 6, totaldsc = 0; // Referente ao total � pagar.
	float desconto = 0; // Vari�vel referente � quantos % de desconto o usu�rio recebeu.

	char nomebase[24] = {}; // Vari�vel auxiliar para introdu��o do nome do produto no painel de controle.

	produto base = {}; // Vari�vel de produto base para ser utilizada no painel de controle.
	//
	produto* estoque; // Cria ponteiro que vai ser o vetor din�mico de estoque.
	produto* cesta; // Cria ponteiro que vai ser o vetor din�mico para cesta.

	fin.open("estoque.bin", ios_base::in | ios_base::binary); // Tentativa de abertura do arquivo de estoque
	// Teste para ver se o arquivo foi aberto e existe
	if (!fin.is_open()) {
		estoque = nullptr; // Caso n�o exista, � iniciado um vetor de estoque vazio.
		cesta = nullptr;
	}
	else { // Caso exista � inicializado o vetor din�mico com os dados contidos nele.
		fin.read((char*)&TamVet, sizeof(unsigned short));
		estoque = new produto[TamVet];
		cesta = new produto[TamVet]; // Vetor din�mico referente � cesta do cliente.

		for (unsigned short i = 0; i < TamVet; i++) {
			fin.read((char*)&estoque[i], sizeof(produto)); // L� informa��es do arquivo para o vetor din�mico de estoque.
		}
	}
	fin.close();

	cout << fixed;
	cout.precision(2);

	if (argc > 1 && !strcmp(argv[1], "-c")) {
		quant_estoque = TamVet;
		char opcaocmd; // Vari�vel de escolha pro painel de controle.
		char opcaoexcluircmd; // Vari�vel de escolha para op��o de exclus�o pro painel de controle.
		unsigned short possui_estoque = 0; // Vari�vel para verificar se o produto est� no estoque.
		int excluircmd = 0;
		do {
			cout << "Painel de Controle" << endl;
			cout << "===================" << endl;
			cout << "(A)dicionar" << endl;
			cout << "(E)xcluir" << endl;
			cout << "(L)istar" << endl;
			cout << "(S)air" << endl;
			cout << "===================" << endl;
			cout << "Op��o [ ]\b\b";

			cin >> opcaocmd;
			opcaocmd = toupper(opcaocmd); // Padronizando a letra pra maiuscula.

			if (opcaocmd == 'A') { // Caso seja op��o de adicionar.
				cout << "Produto: ";
				cin >> nomebase; // Nome do produto � passado para vari�vel base
				strcpy(base.nome, nomebase); // � passada para vari�vel do tipo produto.

				cout << "Pre�o: ";
				cin >> base.preco;

				cout << "Quantidade: ";
				cin >> base.quant;

				for (int i = 0; i < TamVet; i++) { // Passa pelo vetor de estoque
					if (!strcmp(estoque[i].nome, base.nome)) { // Compara se produto j� est� no estoque e caso estiver, atualiza pre�o e aumenta tamanho do estoque.
						estoque[i].preco = base.preco;
						estoque[i].quant += base.quant;
						possui_estoque = 1; // Altera vari�vel de controle de se est� no estoque.
					}
				}
				if (possui_estoque == 0) { // Caso n�o esteja no estoque.
					if (TamVet == 0) { // Verifica se o vetor ainda n�o existe.
						TamVet++; // Aumenta a vari�vel que controla o tamanho do vetor.
						quant_estoque++; // Aumenta a vari�vel de quantidade de itens no estoque tamb�m.
						estoque = new produto[TamVet]; // Cria vetor din�mico.
						strcpy(estoque[TamVet - 1].nome, base.nome); // Adiciona produto no estoque.
						estoque[TamVet - 1].preco = base.preco;
						estoque[TamVet - 1].quant = base.quant;
					}
					else { // Caso vetor j� exista.
						estoque = expandir_vetorpdt(estoque, TamVet); // Expande o tamanho do vetor, visto que ele � inicializado com tamanho exato de produtos.
						TamVet++; // Aumenta variavel de tamanho.
						quant_estoque++; // Aumenta variavel de quantidade de produtos.
						strcpy(estoque[TamVet - 1].nome, base.nome); // Adiciona item no estoque.
						estoque[TamVet - 1].preco = base.preco;
						estoque[TamVet - 1].quant = base.quant;
					}
				}
				possui_estoque = 0; // Zera variavel de controle.
			}
			else if (opcaocmd == 'E') { // Caso seja selecionado excluir.
				cout << "Excluir" << endl; 
				cout << "-------" << endl;
				if (quant_estoque > 0) { // Verifica se h� produtos no estoque.
					for (int i = 1; i < TamVet + 1; i++) { // Percorre vetor de estoque e mostra produtos.
						cout << i << ") " << estoque[i - 1].nome << endl;
					}
					cout << endl;
					cout << "Produto: [ ]\b\b";
					cin >> excluircmd; // Seleciona qual vai excluir.

					if (excluircmd < TamVet + 1) {
						cout << "Deseja excluir \"" << estoque[excluircmd - 1].nome << " (S/N)? ";
						cin >> opcaoexcluircmd; // Seleciona se deseja realmente excluir.

						opcaoexcluircmd = toupper(opcaoexcluircmd); // Padroniza em maiusculo.
						if (opcaoexcluircmd == 'S') { // Se desejar excluir.
							strcpy(estoque[excluircmd - 1].nome, estoque[quant_estoque - 1].nome); // Coloca ultimo produto do vetor de estoque no local do "excluido"
							estoque[excluircmd - 1].preco = estoque[quant_estoque - 1].preco;
							estoque[excluircmd - 1].quant = estoque[quant_estoque - 1].quant;
							quant_estoque--; // Diminui tamanho da variavel de quantidade de produtos.
						}
					}
					else {
						cout << "Op��o Inv�lida." << endl; // Caso n�o escolha op��o v�lida.
					}
				}
				else {
					cout << "Sem produtos para excluir." << endl;
				}
			}
			else if (opcaocmd == 'L') { // Caso selecione listar.
				cout << "Listagem" << endl;
				cout << "--------" << endl;
				for (int i = 0; i < quant_estoque; i++) { // Percorre vetor de estoque de acordo com a quantidade de produtos existentes.
					cout << estoque[i].nome << " - R$" << estoque[i].preco << " - " << estoque[i].quant << " und." << endl; // Mostra protudos com suas informa��es.
				}
			}
			else if (opcaocmd == 'S') { // Caso deseje sair.
				fout.open("estoque.bin", ios_base::out | ios_base::binary); // Abre arquivo bin�rio para escrita.

				fout.write((char*)&quant_estoque, sizeof(unsigned short)); // Faz cabe�alho, incluindo valor da variavel de estoque no inicio do arquivo.

				for (unsigned short i = 0; i < quant_estoque; i++) { // Escreve os registros dos produtos no arquivo.
					fout.write((char*)&estoque[i], sizeof(produto));
				}
				fout.close(); // Fecha arquivo.
				cout << endl;

			}

		} while (opcaocmd != 'S'); // Finaliza loop quando seleciona sair.

	}
	else {
		do
		{
			cout << " RapiZinho " << endl;
			cout << "===========" << endl;
			if (quant_cesta > 0) { // Verifica se h� itens na cesta.
				for (int i = 0; i < quant_cesta; i++) { // Caso possua � feito um la�o para imprimir os itens com seus pre�os.
					cout << cesta[i].quant << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].preco * float(cesta[i].quant);
					cout << endl;
				}
				cout << "===========" << endl;
			}
			for (int i = 0; i < TamVet; i++) {// Mostrar produtos em forma de lista para possibilitar escolha.
				cout << "(" << char('A' + i) << ") " << estoque[i].nome << endl;
			}
			cout << "(S) Sair" << endl;
			cout << "===========" << endl;
			cout << "Op��o: [ ]\b\b";

			cin >> escolha;
			escolha = toupper(escolha);

			//Condicionais seguindo as op��es de acordo com o que o usuario escolheu:
			if ((escolha >= 'A') && escolha <= ('A' + (TamVet - 1))) { // Caso usu�rio tenha feito uma escolha v�lida para realizar pedido.
				escolha = toupper(escolha);

				cout << "===========" << endl;
				cout << "Pedido\n";
				cout << "===========" << endl;

				cout << estoque[escolha - 'A'].nome << endl;
				cout << "R$" << estoque[escolha - 'A'].preco << endl;

				cout << "===========" << endl;

				cout << "Quantidade: [ ]\b\b";
				cin >> quantidade;
				cout << "===========" << endl;

				if (quantidade > 0) {
					if (quantidade <= estoque[escolha - 'A'].quant) { // Verifica se o estoque possui a quantidade solicitada.
						if (quant_cesta > 0) { // Verifica se a cesta tem produtos
							for (int i = 0; i < quant_cesta; i++) { // La�o para percorrer vetor da cesta.
								if (!strcmp(estoque[escolha - 'A'].nome, cesta[i].nome)) { // Verifica se item solicitado j� est� na cesta.
									if ((quantidade + cesta[i].quant) <= estoque[escolha - 'A'].quant) { // Verifica se estoque tem itens o suficiente para nova quantidade solicitada somada com a quantidade solicitada anteriormente.
										cesta[i].quant += quantidade; // Adiciona quantidade extra na originalmente solicitada na cesta.
									}
									else { // Caso estoque n�o tenha itens o suficiente.
										cout << "Quantidade solicitada � maior do que a possu�da no estoque.\n";
										cout << "Quantidade do estoque atual de " << estoque[escolha - 'A'].nome << ": " << estoque[escolha - 'A'].quant << endl;
										cout << "===========" << endl;
									}
									possui_cesta = 1;
								}
							}
							if (possui_cesta == 0) { // Verifica a vari�vel de controle referente a se o produto j� estava no estoque ou n�o
								strcpy(cesta[quant_cesta].nome, estoque[escolha - 'A'].nome); // Caso n�o esteja, as informa��es do produto s�o postas no espa�o disponivel
								cesta[quant_cesta].quant = quantidade;
								cesta[quant_cesta].preco = estoque[escolha - 'A'].preco;
								++quant_cesta; // ap�s isso � adicionado uma unidade na vari�vel que controla a quantidade de itens na cesta.
							}
						}
						else { // Caso a cesta n�o tenha produtos, ent�o � adicionado o produto na posi��o disponivel que vai ser a inicial da variavel "quant_cesta", ou seja, 0 (Primeira posi��o).
							strcpy(cesta[quant_cesta].nome, estoque[escolha - 'A'].nome);
							cesta[quant_cesta].quant = quantidade;
							cesta[quant_cesta].preco = estoque[escolha - 'A'].preco;
							++quant_cesta; // Vari�vel de controle da quantidade de itens na cesta � aumentada em 1.
						}
						quantidade = possui_cesta = 0;
					}
					else {
						cout << "Quantidade solicitada � maior do que a possu�da no estoque.\n";
						cout << "Quantidade do estoque atual de " << estoque[escolha - 'A'].nome << ": " << estoque[escolha - 'A'].quant << endl;
						cout << "===========" << endl;
					}
				}
				else {
					cout << "Quantidade inv�lida.\n";
					cout << "===========" << endl;
				}
			}
			else if (escolha == 'S' && quant_cesta != 0) { // Caso usu�rio escolha sair e tenha produtos na cesta.

				//Tela de pagamento
				for (int i = 0; i < quant_cesta; i++) {
					total += cesta[i].preco * float(cesta[i].quant); // Adiciona valor dos itens ao total.
				}
				desconto = menupagamento(cesta, quant_cesta, total); // Aciona o menu de pagamento, que retorna o tipo de desconto do usu�rio.

				// Tela de Confirmar Pedido

				confirmarpedido(cesta, quant_cesta, total, desconto, numpedido);

				for (int i = 0; i < TamVet; i++) {
					for (int j = 0; j <= quant_cesta; j++) {
						if (!strcmp(cesta[j].nome, estoque[i].nome)) {
							estoque[i].quant -= cesta[j].quant;
						}
					}
				}

				++numpedido; // Aumenta o n�mero do pedido.
				quant_cesta = 0; // Visto que a cesta vai esvaziar em ambos casos de confirmar ou cancelar, ent�o � zerada a vari�vel que acompanha o preenchimento do vetor.
				escolha = '0'; // � retirado o 'S' da vari�vel de escolha para que o usu�rio possa ver o menu novamente.
				total = 6; // Valor de total � resetado para o valor fixo de entrega, visto que o pedido foi finalizado ou cancelado.
			}
			else if (escolha != 'S') { // Caso usu�rio n�o tenha escolhido sair e nem uma das op��es de pedido.
				cout << "===========" << endl;
				cout << "Escolha Inv�lida." << endl;
				cout << "===========" << endl;
			}
			else if (escolha == 'S') {
				fout.open("estoque.bin", ios_base::out | ios_base::binary);

				fout.write((char*)&TamVet, sizeof(unsigned short));

				for (unsigned short i = 0; i < TamVet; i++) {
					fout.write((char*)&estoque[i], sizeof(produto));
				}
				fout.close();
				cout << endl;
			}
		} while ((escolha != 'S')); // Programa s� encerrado quando usu�rio escolhe sair e est� com a cesta vazia (visto que se tiver produtos � barrado em uma das condicionais).
	}

	if (TamVet > 0) {
		delete[] estoque;
		delete[] cesta;
	}

}