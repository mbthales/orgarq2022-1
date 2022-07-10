#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void intercalaArquivos(int numDeArqs)
{
	FILE *a, *b, *saida;
	Endereco ea, eb;
	char arquivoA[50];
	char arquivoB[50];
	char arquivoSaida[50];

	int auxEntrada = 1;
	int auxSaida = 1;

	for(int i = 1; i <= (numDeArqs*2) - 3; i+=2){
		if(i <= numDeArqs){
			sprintf(arquivoA, "cep_ordenado_%d.dat", i);
			sprintf(arquivoB, "cep_ordenado_%d.dat", i+1);
		} else {
			sprintf(arquivoA, "cep_intercalado_%d.dat", auxEntrada);
			sprintf(arquivoB, "cep_intercalado_%d.dat", auxEntrada+1);
			auxEntrada+=2;
		}

		if(i == (numDeArqs*2) - 3){
			printf("Gerando Arquivo Ordenado Final\n\n");
			sprintf(arquivoSaida, "cep_ordenado.dat");
		} else {
			printf("Gerando Arquivo Intercalado - %d\n\n", auxSaida);
			sprintf(arquivoSaida, "cep_intercalado_%d.dat", auxSaida);
		}

		a = fopen(arquivoA,"rb");
		b = fopen(arquivoB,"rb");
		saida = fopen(arquivoSaida,"wb");

		fread(&ea,sizeof(Endereco),1,a);
		fread(&eb,sizeof(Endereco),1,b);

		while(!feof(a) && !feof(b))
		{
			if(compara(&ea,&eb)<0)
			{
				fwrite(&ea,sizeof(Endereco),1,saida);
				fread(&ea,sizeof(Endereco),1,a);
			}
			else
			{
				fwrite(&eb,sizeof(Endereco),1,saida);
				fread(&eb,sizeof(Endereco),1,b);
			}
		}

		while(!feof(a))
		{
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);		
		}
		while(!feof(b))
		{
			fwrite(&eb,sizeof(Endereco),1,saida);
			fread(&eb,sizeof(Endereco),1,b);		
		}

		fclose(a);
		fclose(b);
		fclose(saida);
		auxSaida++;
	}
}

void excluaArquivos(int numDeArqs)
{
	char arquivo[50];

	for(int i = 1; i <= numDeArqs; i++)
	{
		printf("Apagando Arquivo Ordenado - %d\n\n", i);
		sprintf(arquivo, "cep_ordenado_%d.dat", i);
		remove(arquivo);
	}

	for(int i = 1; i <= numDeArqs - 2; i++)
	{
		printf("Apagando Arquivo Intercalado - %d\n\n", i);
		sprintf(arquivo, "cep_intercalado_%d.dat", i);
		remove(arquivo);
	}
}

int main(int argc, char**argv)
{
	FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, bytesPorArq;
	int numDeArqs, aux;

	printf("Digite a quantidade de arquivos que voce deseja gerar para a ordenacao:\n");
	scanf("%d", &numDeArqs);

	f = fopen("cep.dat","rb");
	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd = posicao/sizeof(Endereco);
	bytesPorArq = qtd/numDeArqs;
	rewind(f);

	aux = 0;

	for(int i = 1; i <= numDeArqs; i++)
	{
		printf("Gerando Arquivo Ordenado - %d\n", i);

		if(i == numDeArqs)
		{
			bytesPorArq = qtd-aux;
		}

		e = (Endereco*) malloc((bytesPorArq)*sizeof(Endereco));
		if(fread(e,sizeof(Endereco),bytesPorArq,f) == bytesPorArq)
		{
			printf("Arquivo Ordenado %d = OK\n\n",i);
		}
		qsort(e,bytesPorArq,sizeof(Endereco),compara);
		char nomeDoArquivo[50];
		sprintf(nomeDoArquivo, "cep_ordenado_%d.dat", i);
		saida = fopen(nomeDoArquivo,"wb");
		fwrite(e,sizeof(Endereco),bytesPorArq,saida);
		fclose(saida);
		free(e);

		aux += bytesPorArq;
	}

	fclose(f);
	intercalaArquivos(numDeArqs);
	excluaArquivos(numDeArqs);

	printf("O programa gerou um arquivo ordenado com sucesso!\n");

	return 0;
}