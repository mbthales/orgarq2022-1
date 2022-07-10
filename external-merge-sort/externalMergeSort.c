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
	FILE *a, *b, *hello;
	Endereco ea, eb;
	int aux = 1;
	int auxFile = 1;

	for(int i = 1; i <= log2(numDeArqs); i++){
		for(int j = 1; j <= numDeArqs/auxFile; j+=2){
			char arquivoA[50];
			char arquivoB[50];
			char arquivoSaida[50];
			if(i == 1)
			{
				printf("Gerando Arquivo Intercalado - %d\n\n", aux);
				sprintf(arquivoA, "cep_ordenado_%d.dat", j);
				sprintf(arquivoB, "cep_ordenado_%d.dat", j+1);
				sprintf(arquivoSaida, "cep_saida_%d.dat", aux);
			} else if (i == log2(numDeArqs))
			{
				printf("Gerando Arquivo Final Ordenado\n\n");
				sprintf(arquivoA, "cep_saida_%d.dat", numDeArqs - 3);
				sprintf(arquivoB, "cep_saida_%d.dat", numDeArqs - 2);
				sprintf(arquivoSaida, "cep_ordenado.dat");
			}
			else
			{
				printf("Gerando Arquivo Intercalado - %d\n\n", aux);
				sprintf(arquivoA, "cep_saida_%d.dat", j);
				sprintf(arquivoB, "cep_saida_%d.dat", j+1);
				sprintf(arquivoSaida, "cep_saida_%d.dat", aux);
			}
		
			a = fopen(arquivoA,"r");
			b = fopen(arquivoB,"r");
			hello = fopen(arquivoSaida,"w");

			fread(&ea,sizeof(Endereco),1,a);
			fread(&eb,sizeof(Endereco),1,b);

			while(!feof(a) && !feof(b))
			{
				if(compara(&ea,&eb)<0)
				{
					fwrite(&ea,sizeof(Endereco),1,hello);
					fread(&ea,sizeof(Endereco),1,a);
				}
				else
				{
					fwrite(&eb,sizeof(Endereco),1,hello);
					fread(&eb,sizeof(Endereco),1,b);
				}
			}
			while(!feof(a))
			{
				fwrite(&ea,sizeof(Endereco),1,hello);
				fread(&ea,sizeof(Endereco),1,a);		
			}
			while(!feof(b))
			{
				fwrite(&eb,sizeof(Endereco),1,hello);
				fread(&eb,sizeof(Endereco),1,b);		
			}

			fclose(a);
			fclose(b);
			fclose(hello);
			aux++;
		}
		
		auxFile *= 2;
	}
}

void excluirArquivos(int numDeArqs)
{
	char arquivo[50];

	for(int i = 1; i <= numDeArqs; i++)
	{
		char arquivo[50];
		sprintf(arquivo, "cep_ordenado_%d.dat", i);
		remove(arquivo);
	}

	for(int i = 1; i <= numDeArqs - 2; i++)
	{
		char arquivo[50];
		sprintf(arquivo, "cep_saida_%d.dat", i);
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
	//excluirArquivos(numDeArqs);

	return 0;
}