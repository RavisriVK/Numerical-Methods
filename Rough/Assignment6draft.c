#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int HammondDistance(char *a1, char *a2, int n)
{
	int hdist=0;
	for(int i=0; i<n; i++)
	{
		if(a1[i]!=a2[i])
		hdist++;
	}
	return hdist;
}

void main(int argc, char **argv)
{
	int N=(int)atoi(argv[1]);
	double P=strtod(argv[2], NULL);
	double Q=strtod(argv[3], NULL);
	int M=(int)atoi(argv[4]);
	int Pn=(int)floor(P*N);
	int min=Pn-1;
	int minloc;
	int hdist=0;
	int Parray[Pn][2];
	char *BitArray=(char *)malloc(N*sizeof(char));
	char *BitArrayOffset=(char *)malloc((N-M)*sizeof(char));
	int Hammonds[(int)(1-P)*N];
	for(int i=0; i<N; i++)
	{
		BitArray[i]=(rand()%2);
	}
	for(int i=0; i<Pn; i++)
	{
		Parray[i][0]=rand()%N;
		if(BitArray[Parray[i][0]]!=2)
		{
			Parray[i][1]= BitArray[Parray[i][0]];
			BitArray[Parray[i][0]]=2;
		} else {
			i--;
			printf("hh");
		}
	}
	for(int i=0; i<Pn; i++)
	{
		if(BitArray[Parray[i][0]]==2)
		BitArray[Parray[i][0]]=Parray[i][1];
		
		if((((1.0)*rand())/RAND_MAX)<=Q)
		Parray[i][1]=(-1)*Parray[i][1];
	}
	for(int i=0; i<(int)(1-P)*N; i++)
	{
		hdist=0;
		for(int j=0; j<Pn; j++)
		{
			if(Parray[j][1]!=BitArray[Parray[j][0]])
			hdist++;
		}
		Hammonds[i]=hdist;
		if(min>hdist)
		{
			min=hdist;
			minloc=i;
		}
	}
	printf("\nOffset=%d\nCalculated offset=%d\n",M,minloc);
	
}
