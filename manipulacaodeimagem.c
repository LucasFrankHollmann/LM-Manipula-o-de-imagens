#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/time.h>   
#include <sys/resource.h>

int a, i; //variável para armazenar o tamanho do vetor da imagem
unsigned char h[2]; //vetor para identificar os caracteres de little endian ou big endian
unsigned char *vet; //vetor para armazenar os bytes da imagem
unsigned int *vet2; //vetor auxiliar para SSE


unsigned char* Brightness(int tam, unsigned char* pixels)
{
	int a;
	unsigned char bright = 75;
	for (int i = 0; i < tam; i++)
	{
		a = (int)pixels[i] + (int)bright;
		if(a>255)
		{
			pixels[i] = 255;
		}
		else if(i%4!=3)
		{
			pixels[i] +=bright;
		}
	}
	
	return pixels;
}

unsigned char* Noise(int tam, unsigned char* pixels)
{
	int a;
	char noise;
	for (int i = 0; i < tam; i++)
	{
		noise = (rand()%100)-50;
		
		a = (int)pixels[i] + (int)noise;
		if(a>255)
		{
			pixels[i] = 255;
		}
		else if(a<0)
		{
			pixels[i] = 0;
		}
		else if(i%4!=3)
		{
			
			pixels[i] +=noise;
		}
	}
	
	return pixels;
}




int main()
{
	
	struct timespec start, finish; 
     
	extern unsigned int *brilho(int tam, unsigned int *pixels);
	extern unsigned int *ruido(int tam, unsigned int *pixels);

	FILE *fp, *fp1, *fp2, *fp3;
	fp = fopen("./2560x1920 brilho asm.tif", "rw+");
	fp1 = fopen("./2560x1920 brilho c.tif", "rw+");
	fp2 = fopen("./2560x1920 ruido asm.tif", "rw+");
	fp3 = fopen("./2560x1920 ruido c.tif", "rw+");
	//fread(h, 1, 2, fp);
	//printf("little endian ou big endian -- %c %c\n", h[0], h[1]);
	fseek(fp, 4, SEEK_SET);
	fread(&a, sizeof(int), 1, fp);
	
	
	a=a-8;
	printf("tamanho (em bytes) da imagem -- %d\n", a);
	vet = (unsigned char *)malloc(sizeof(unsigned char)*a);
	vet2 = (unsigned int *)malloc(sizeof(unsigned int)*a);
	
	fseek(fp, 8, SEEK_SET);
	fread(vet, 1, a, fp);
	
	
	//Passagem do valor de vet para vet2, para usar a operação de soma de 4 bytes em SSE
	for (int i = 0; i < a; i++)
	{
		vet2[i] = (int)vet[i];
	}
	
	clock_gettime(CLOCK_REALTIME, &start);
	//Chamada da função para aplicar a operação na imagem
	vet2 = brilho(a,vet2);
	clock_gettime(CLOCK_REALTIME, &finish);
	
	printf("Brilho com assembler (ns): %lli\n", (long long)(finish.tv_nsec - start.tv_nsec));
	
	for (int i = 0; i < a; i++)
	{
		//printf("%d\n", vet2[i]);
		if(vet2[i]>255)
		{
			vet2[i] = 255;
		}
		vet[i] = (unsigned char)vet2[i];
	}
	//Retorno do valor de vet2 para vet
	
	
	
	fseek(fp, 8, SEEK_SET);
	fwrite(vet,1,a,fp);
	
	fclose(fp);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	fseek(fp1, 8, SEEK_SET);
	fread(vet, 1, a, fp1);
	
	
	//Chamada da função para aplicar a operação na imagem
	clock_gettime(CLOCK_REALTIME, &start);
	vet = Brightness(a,vet);
	clock_gettime(CLOCK_REALTIME, &finish);
	printf("Brilho em c (ns): %lli\n", (long long)(finish.tv_nsec - start.tv_nsec));

	
	fseek(fp1, 8, SEEK_SET);
	fwrite(vet,1,a,fp1);
	
	fclose(fp1);
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	fseek(fp2, 8, SEEK_SET);
	fread(vet, 1, a, fp2);
	
	
	//Passagem do valor de vet para vet2, para usar a operação de soma de 4 bytes em SSE
	for (int i = 0; i < a; i++)
	{
		vet2[i] = (int)vet[i];
	}
	
	//Chamada da função para aplicar a operação na imagem
	clock_gettime(CLOCK_REALTIME, &start);
	vet2 = ruido(a,vet2);
	clock_gettime(CLOCK_REALTIME, &finish);
	
	printf("Ruido com assembler (ns): %lli\n", (long long)(finish.tv_nsec - start.tv_nsec));
	
	for (int i = 0; i < a; i++)
	{
		//printf("%d\n", vet2[i]);
		if(vet2[i]>255)
		{
			vet2[i] = 255;
		}
		vet[i] = (unsigned char)vet2[i];
	}
	//Retorno do valor de vet2 para vet
	
	
	fseek(fp2, 8, SEEK_SET);
	fwrite(vet,1,a,fp2);
	
	fclose(fp2);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	fseek(fp3, 8, SEEK_SET);
	fread(vet, 1, a, fp3);
	
	
	//Chamada da função para aplicar a operação na imagem
	clock_gettime(CLOCK_REALTIME, &start);
	vet = Noise(a,vet);
	clock_gettime(CLOCK_REALTIME, &finish);
	
	printf("Ruido em c (ns): %lli\n", (long long)(finish.tv_nsec - start.tv_nsec));

	
	fseek(fp3, 8, SEEK_SET);
	fwrite(vet,1,a,fp3);
	
	fclose(fp3);
	
	
	return 0;
}
