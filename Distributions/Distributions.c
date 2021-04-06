/******************************************************************************************
*Purpose: To generate random numbers for a given distribution and find their histogram
*Authors: Ravisri Valluri, Kevin Martin, J Phani Jayanth
*Date: 28/08/2019
*Inputs: The name of distribution and size of distribution to be generated
*Outputs: Text file with random numbers of distribution arranged in bins and with frequency
*******************************************************************************************/


/* Header Files */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

/* This function assorts the data into bins and find the frequency for each bin, then writes to a file */

void histogram(float *dis, int N)
{
FILE *fp;
fp=fopen("gfile.txt", "w");
int binc=(int)ceil(1+(log(N)/log(2)));
int bins[binc];
double min=dis[0]; 
double max=dis[0];
float binsize=0;
int count=0;

/* Finding maximum and minimum for the generated data */

	for(int i=1; i<N; i++) 	{
		if(dis[i]>max)
			max=dis[i];
		if(dis[i]<min)
			min=dis[i];
	}
	printf("\nMax and min and binc are: %f %f %d \n\n", max,min, binc );

	binsize=(max-min)/binc;

/* This loop system runs through each element and allots it to a given bin */

	while(count<binc) {	
		bins[count]=0;
		for(int i=0; i<N; i++) {
			if((dis[i]>=(min+count*binsize))&&(dis[i]<(min+(1+count)*binsize)))
				bins[count]++;
		}
		count++;
	}

	bins[binc-1]++;
/* This for loop prints the mean of each interval for a bin and the frequency allotted */

	for(int i=0; i<binc; i++)
	{
		fprintf(fp,"%g %d\n",(2*min+(i+0.5)*binsize)/2, bins[i]);
		printf("%g %d\n",(2*min+(i+0.5)*binsize)/2, bins[i]);
	}
}

/* Following functions generate respective distributions and then call histogram function*/

void lorentzian_distribution(int N)
{
	srand(time(0));
	float ud[N];
	float ld[N];
	float sum=0;
	for(int i=0;i<N;i++)
	{
		ud[i]=((float)rand())/RAND_MAX;
		ld[i]=0.5*tan(M_PI*(ud[i]-0.5));
		sum+=ld[i];
	}
	printf("\nAverage=%g\n\n",sum/N);
	histogram(ld, N);
}

void normal_distribution(int N)
{
	srand(time(0));
	float ud1[N];
	float ud2[N];
	float nd[N];
	float sum=0;
	for(int i=0;i<N;i++)
	{
		ud1[i]=((float)rand())/RAND_MAX;
		ud2[i]=((float)rand())/RAND_MAX;
		nd[i]=sqrt((-2)*(log(ud1[i])))*cos(2*ud2[i]*M_PI);
		sum+=nd[i];
	}
	printf("\nAverage=%g\n\n",sum/N);
	histogram(nd, N);
}

void rayleigh_distribution(int N)
{
	srand(time(0));
	float ud[N];
	float rd[N];
	float sig=0.3;
	float sum=0;

        for(int i=0; i<N; i++) {
	    ud[i]=((float)rand())/RAND_MAX;
	    rd[i]=sig*sqrt((-2)*log(ud[i]));
	    printf("%g\n",rd[i]);
            sum+=rd[i];
	}

        printf("\nAverage=%g\n\n",sum/N);
        histogram(rd, N);

}   

/* 
      '''''''''''''''''''''''
	Main Function
      '''''''''''''''''''''''
*/
void main(int argc,char **argv)
{
	char	line[500], upper_line[500];
	int     N=0,i=0;
	
	/* Check if number of arguments aren't enough */
	if ( argc < 3 ) {
		printf("Usage: ./a.out <N # of Random steps> <distribution name>\n");
		printf("\n   Supported distribution names:\n");
		printf("           NORMAL or GAUSSIAN\n");
		printf("           RAYLEIGH\n");
		printf("           LORENTZIAN\n");
		printf("           POISSONIAN\n");
		printf("\nPlease try again.\n");
		exit(0);
		}

	/* Get the value of number of random elements N */
        strcpy(line, argv[1]);	
	for( int q=0; q<strlen(line); q++ ) {
		if ( !isdigit(line[q]) ) {
			printf( "Entered input for number of random elements is not a number\n" );
			exit(0);
		}
	}
	N = atoi(line); 
        printf("\nNumber of random elements N %d\n", N);

	/* Get the distribution name */
        strcpy(line, argv[2]);

	/* Convert to capitals */
	for(i=0; i<strlen(line); i++)
	{
		if(line[i]>='A'&&line[i]<='Z')
		upper_line[i]=line[i];
		else
		upper_line[i]=line[i]+'A'-'a';
	}
	upper_line[i]='\0';

	/* Find which distribution to generate */
	if ( strcmp( argv[2], "NORMAL" ) == 0 ||
	          strcmp( upper_line, "GAUSSIAN" ) == 0 ) {
                        normal_distribution(N);
	}
	else if ( strcmp( upper_line, "RAYLEIGH" ) == 0 ){
			rayleigh_distribution(N);
	}
	else if ( strcmp( upper_line, "LORENTZIAN" ) == 0 ) {
                        lorentzian_distribution(N);
	}
	else if ( strcmp( upper_line, "POISSONIAN" ) == 0 ) {
                        printf("\nFunction for %s\n", argv[2]);
	}
	else {
                        printf("\nNot a supported distribution: %s\n", argv[2]);
			exit(0);
	}


}


/*
	'''''''''''''''''
	 End of Program
	.................
*/


