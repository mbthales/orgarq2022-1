#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char**argv)
{
	FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, bytesPorArquivo;

	int num;
	printf("Digite a quantidade de arquivos que voce deseja gerar para a ordenacao:\n");
	scanf("%d", &num);

	f = fopen("cep.dat","rb");
	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd = posicao/sizeof(Endereco);
	bytesPorArquivo = qtd/num;

	for(int i = 1; i <= num; i++){
		e = (Endereco*) malloc(bytesPorArquivo*sizeof(Endereco));
		rewind(f);
		if(fread(e,sizeof(Endereco),bytesPorArquivo,f) == bytesPorArquivo)
		{
			printf("Lido = OK\n");
		}
		qsort(e,bytesPorArquivo,sizeof(Endereco),compara);
		printf("Ordenado = OK\n");
		char nomeDoArquivo[50];
		sprintf(nomeDoArquivo, "cep_ordenado_%d.dat", i);
		saida = fopen(nomeDoArquivo, "wb");
		fwrite(e,sizeof(Endereco),bytesPorArquivo,saida);
		fclose(saida);
		printf("Escrito = OK\n");
		free(e);
		
		bytesPorArquivo = qtd-bytesPorArquivo;
	}

	fclose(f);
}