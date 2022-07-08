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

void mergeFiles(int numOfFiles)
{
	FILE *a, *b, *hello;
	Endereco ea, eb;
	int aux = 1;

	for(int i = 1; i <= numOfFiles/2; i++){
		for(int j = 1; j <= numOfFiles/i; j+=2){
			char arquivoA[50];
			char arquivoB[50];
			char arquivoSaida[50];
			if(i == 1){
				sprintf(arquivoA, "cep_ordenado_%d.dat", j);
				sprintf(arquivoB, "cep_ordenado_%d.dat", j+1);
			} else{
				sprintf(arquivoA, "cep_saida_%d.dat", j);
				sprintf(arquivoB, "cep_saida_%d.dat", j+1);
			}
			sprintf(arquivoSaida, "cep_saida_%d.dat", aux);
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
	}
}

int main(int argc, char**argv)
{
	FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, metade;
	int numOfFiles, aux = 0;

	printf("Digite a quantidade de arquivos que voce deseja gerar para a ordenacao:\n");
	scanf("%d", &numOfFiles);

	f = fopen("cep.dat","rb");
	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd = posicao/sizeof(Endereco);
	metade = qtd/numOfFiles;
	rewind(f);

	for(int i = 1; i <= numOfFiles; i++)
	{
		if(i != numOfFiles){
			e = (Endereco*) malloc((metade)*sizeof(Endereco));
			if(fread(e,sizeof(Endereco),metade,f) == metade)
			{
				printf("Lido = OK\n");
			}
			qsort(e,metade,sizeof(Endereco),compara);
			printf("Ordenado = OK\n");
			char nomeDoArquivo[50];
			sprintf(nomeDoArquivo, "cep_ordenado_%d.dat", i);
			saida = fopen(nomeDoArquivo,"wb");
			fwrite(e,sizeof(Endereco),metade,saida);
			fclose(saida);
			printf("Escrito = OK\n");
			free(e);
			aux += metade;
		} else {
			e = (Endereco*) malloc((qtd-aux)*sizeof(Endereco));
			if(fread(e,sizeof(Endereco),qtd-aux,f) == qtd-aux)
			{
				printf("Lido = OK\n");
			}
			qsort(e,qtd-aux,sizeof(Endereco),compara);
			printf("Ordenado = OK\n");
			char nomeDoArquivo[50];
			sprintf(nomeDoArquivo, "cep_ordenado_%d.dat", i);
			saida = fopen(nomeDoArquivo,"wb");
			fwrite(e,sizeof(Endereco),qtd-aux,saida);
			fclose(saida);
			printf("Escrito = OK\n");
			free(e);
		}
	}

	fclose(f);
	mergeFiles(numOfFiles);
	return 0;
}
