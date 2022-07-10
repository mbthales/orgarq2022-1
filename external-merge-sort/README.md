# External Merge Sort

Este código gera um arquivo ordenado atráves da intecalação de vários arquivos menores ordenados de um arquivo muito grande de CEP.

## Para rodar o código:

- Baixe o código.
- Se tiver usando o Windows, instale o [MinGW](https://sourceforge.net/projects/mingw/). Se estiver usando alguma distribuição Linux, verifique se o gcc está instalado. Se não, rode no terminal o seguinte comando: `sudo apt install gcc`.
- Baixe o [arquivo que contém os CEP's](https://www.dropbox.com/sh/8i46wy3q0rmasu7/AACxLU8MxYm-_IV9UvuagxzIa/aula03?dl=0&preview=cep_ordenado.dat&subfolder_nav_tracking=1) e coloque na mesma pasta do código.
- Abra o terminal na pasta do código e escreva nele: `gcc externalMergeSort.c -o externalMergeSort.exe` (tire o .exe se estiver usando alguma distribuição Linux)
- Após a compilação do arquivo, rode no terminal: `./externalMergeSort.exe` (tire o .exe se estiver usando alguma distribuição Linux)
- Será pedido pelo terminal o número de arquivos menores (ou chunks) que você deseja gerar. O código só funcionará se o número do input for uma potência de 2 (2, 4, 8, 16, 32, 64...).
- Após o programa rodar por completo, você terá um arquivo cep ordenado dentro da mesma pasta do código.