#define _CRT_SECURE_NO_WARNINGS
#pragma once

struct produto {
	char nome[24];
	float preco;
	unsigned int quant;
};

produto* expandir_vetorpdt(produto*, int);

float menupagamento(produto* cesta, int quant_cesta, float total);

void confirmarpedido(produto* cesta, int quant_cesta, float total, float desconto, int numpedido);