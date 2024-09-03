#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "menus.h"
using namespace std;

int main(int argc, char ** argv) {
	// Criação das variáveis.
	ifstream fin; // Variável para leitura de dados de arquivos.
	ofstream fout; // Variável para escritura de dados em arquivos. 

	char escolha; // Variáveis de escolha das opções dos menus.

	int quantidade; // Referente a quantidade solicitada ao realizar pedido.
	int quant_cesta = 0; // Quantidade de itens já na cesta.
	unsigned short quant_estoque = 0; // Quantidade de itens já no estoque.
	int possui_cesta = 0; // Variavel para verificar se item já está na cesta.
	unsigned short TamVet = 0; // Tamanho dos vetores
	int numpedido = 1; // Número de pedidos inicial.
	float total = 6, totaldsc = 0; // Referente ao total à pagar.
	float desconto = 0; // Variável referente à quantos % de desconto o usuário recebeu.

	char nomebase[24] = {}; // Variável auxiliar para introdução do nome do produto no painel de controle.

	produto base = {}; // Variável de produto base para ser utilizada no painel de controle.
	//
	produto* estoque; // Cria ponteiro que vai ser o vetor dinâmico de estoque.
	produto* cesta; // Cria ponteiro que vai ser o vetor dinâmico para cesta.

	fin.open("estoque.bin", ios_base::in | ios_base::binary); // Tentativa de abertura do arquivo de estoque
	// Teste para ver se o arquivo foi aberto e existe
	if (!fin.is_open()) {
		estoque = nullptr; // Caso não exista, é iniciado um vetor de estoque vazio.
		cesta = nullptr;
	}
	else { // Caso exista é inicializado o vetor dinâmico com os dados contidos nele.
		fin.read((char*)&TamVet, sizeof(unsigned short));
		estoque = new produto[TamVet];
		cesta = new produto[TamVet]; // Vetor dinâmico referente à cesta do cliente.

		for (unsigned short i = 0; i < TamVet; i++) {
			fin.read((char*)&estoque[i], sizeof(produto)); // Lê informações do arquivo para o vetor dinâmico de estoque.
		}
	}
	fin.close();

	cout << fixed;
	cout.precision(2);

	if (argc > 1 && !strcmp(argv[1], "-c")) {
		quant_estoque = TamVet;
		char opcaocmd; // Variável de escolha pro painel de controle.
		char opcaoexcluircmd; // Variável de escolha para opção de exclusão pro painel de controle.
		unsigned short possui_estoque = 0; // Variável para verificar se o produto está no estoque.
		int excluircmd = 0;
		do {
			cout << "Painel de Controle" << endl;
			cout << "===================" << endl;
			cout << "(A)dicionar" << endl;
			cout << "(E)xcluir" << endl;
			cout << "(L)istar" << endl;
			cout << "(S)air" << endl;
			cout << "===================" << endl;
			cout << "Opção [ ]\b\b";

			cin >> opcaocmd;
			opcaocmd = toupper(opcaocmd); // Padronizando a letra pra maiuscula.

			if (opcaocmd == 'A') { // Caso seja opção de adicionar.
				cout << "Produto: ";
				cin >> nomebase; // Nome do produto é passado para variável base
				strcpy(base.nome, nomebase); // É passada para variável do tipo produto.

				cout << "Preço: ";
				cin >> base.preco;

				cout << "Quantidade: ";
				cin >> base.quant;

				for (int i = 0; i < TamVet; i++) { // Passa pelo vetor de estoque
					if (!strcmp(estoque[i].nome, base.nome)) { // Compara se produto já está no estoque e caso estiver, atualiza preço e aumenta tamanho do estoque.
						estoque[i].preco = base.preco;
						estoque[i].quant += base.quant;
						possui_estoque = 1; // Altera variável de controle de se está no estoque.
					}
				}
				if (possui_estoque == 0) { // Caso não esteja no estoque.
					if (TamVet == 0) { // Verifica se o vetor ainda não existe.
						TamVet++; // Aumenta a variável que controla o tamanho do vetor.
						quant_estoque++; // Aumenta a variável de quantidade de itens no estoque também.
						estoque = new produto[TamVet]; // Cria vetor dinâmico.
						strcpy(estoque[TamVet - 1].nome, base.nome); // Adiciona produto no estoque.
						estoque[TamVet - 1].preco = base.preco;
						estoque[TamVet - 1].quant = base.quant;
					}
					else { // Caso vetor já exista.
						estoque = expandir_vetorpdt(estoque, TamVet); // Expande o tamanho do vetor, visto que ele é inicializado com tamanho exato de produtos.
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
				if (quant_estoque > 0) { // Verifica se há produtos no estoque.
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
						cout << "Opção Inválida." << endl; // Caso não escolha opção válida.
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
					cout << estoque[i].nome << " - R$" << estoque[i].preco << " - " << estoque[i].quant << " und." << endl; // Mostra protudos com suas informações.
				}
			}
			else if (opcaocmd == 'S') { // Caso deseje sair.
				fout.open("estoque.bin", ios_base::out | ios_base::binary); // Abre arquivo binário para escrita.

				fout.write((char*)&quant_estoque, sizeof(unsigned short)); // Faz cabeçalho, incluindo valor da variavel de estoque no inicio do arquivo.

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
			if (quant_cesta > 0) { // Verifica se há itens na cesta.
				for (int i = 0; i < quant_cesta; i++) { // Caso possua é feito um laço para imprimir os itens com seus preços.
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
			cout << "Opção: [ ]\b\b";

			cin >> escolha;
			escolha = toupper(escolha);

			//Condicionais seguindo as opções de acordo com o que o usuario escolheu:
			if ((escolha >= 'A') && escolha <= ('A' + (TamVet - 1))) { // Caso usuário tenha feito uma escolha válida para realizar pedido.
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
							for (int i = 0; i < quant_cesta; i++) { // Laço para percorrer vetor da cesta.
								if (!strcmp(estoque[escolha - 'A'].nome, cesta[i].nome)) { // Verifica se item solicitado já está na cesta.
									if ((quantidade + cesta[i].quant) <= estoque[escolha - 'A'].quant) { // Verifica se estoque tem itens o suficiente para nova quantidade solicitada somada com a quantidade solicitada anteriormente.
										cesta[i].quant += quantidade; // Adiciona quantidade extra na originalmente solicitada na cesta.
									}
									else { // Caso estoque não tenha itens o suficiente.
										cout << "Quantidade solicitada é maior do que a possuída no estoque.\n";
										cout << "Quantidade do estoque atual de " << estoque[escolha - 'A'].nome << ": " << estoque[escolha - 'A'].quant << endl;
										cout << "===========" << endl;
									}
									possui_cesta = 1;
								}
							}
							if (possui_cesta == 0) { // Verifica a variável de controle referente a se o produto já estava no estoque ou não
								strcpy(cesta[quant_cesta].nome, estoque[escolha - 'A'].nome); // Caso não esteja, as informações do produto são postas no espaço disponivel
								cesta[quant_cesta].quant = quantidade;
								cesta[quant_cesta].preco = estoque[escolha - 'A'].preco;
								++quant_cesta; // após isso é adicionado uma unidade na variável que controla a quantidade de itens na cesta.
							}
						}
						else { // Caso a cesta não tenha produtos, então é adicionado o produto na posição disponivel que vai ser a inicial da variavel "quant_cesta", ou seja, 0 (Primeira posição).
							strcpy(cesta[quant_cesta].nome, estoque[escolha - 'A'].nome);
							cesta[quant_cesta].quant = quantidade;
							cesta[quant_cesta].preco = estoque[escolha - 'A'].preco;
							++quant_cesta; // Variável de controle da quantidade de itens na cesta é aumentada em 1.
						}
						quantidade = possui_cesta = 0;
					}
					else {
						cout << "Quantidade solicitada é maior do que a possuída no estoque.\n";
						cout << "Quantidade do estoque atual de " << estoque[escolha - 'A'].nome << ": " << estoque[escolha - 'A'].quant << endl;
						cout << "===========" << endl;
					}
				}
				else {
					cout << "Quantidade inválida.\n";
					cout << "===========" << endl;
				}
			}
			else if (escolha == 'S' && quant_cesta != 0) { // Caso usuário escolha sair e tenha produtos na cesta.

				//Tela de pagamento
				for (int i = 0; i < quant_cesta; i++) {
					total += cesta[i].preco * float(cesta[i].quant); // Adiciona valor dos itens ao total.
				}
				desconto = menupagamento(cesta, quant_cesta, total); // Aciona o menu de pagamento, que retorna o tipo de desconto do usuário.

				// Tela de Confirmar Pedido

				confirmarpedido(cesta, quant_cesta, total, desconto, numpedido);

				for (int i = 0; i < TamVet; i++) {
					for (int j = 0; j <= quant_cesta; j++) {
						if (!strcmp(cesta[j].nome, estoque[i].nome)) {
							estoque[i].quant -= cesta[j].quant;
						}
					}
				}

				++numpedido; // Aumenta o número do pedido.
				quant_cesta = 0; // Visto que a cesta vai esvaziar em ambos casos de confirmar ou cancelar, então é zerada a variável que acompanha o preenchimento do vetor.
				escolha = '0'; // É retirado o 'S' da variável de escolha para que o usuário possa ver o menu novamente.
				total = 6; // Valor de total é resetado para o valor fixo de entrega, visto que o pedido foi finalizado ou cancelado.
			}
			else if (escolha != 'S') { // Caso usuário não tenha escolhido sair e nem uma das opções de pedido.
				cout << "===========" << endl;
				cout << "Escolha Inválida." << endl;
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
		} while ((escolha != 'S')); // Programa só encerrado quando usuário escolhe sair e está com a cesta vazia (visto que se tiver produtos é barrado em uma das condicionais).
	}

	if (TamVet > 0) {
		delete[] estoque;
		delete[] cesta;
	}

}