/*===========================================================================================================================
 *Authors:Ravisri Kumara Valluri, Snehan
 *Purpose:To find hamming distance between two arrays and to estimate offset by finding where the hamming distance is minimum
 *Date:09/10/2019
 *Inputs:Only command line input of format: ./a.out #Number_of_Bits #P_Fraction_Recieved #Q_Error_Fraction
 *Outputs:Estimated offset, graph of hamming distance vs offset
 ============================================================================================================================*/

/*=================
 *Header files
 =================*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*==================
 *Functions used
 ==================*/

/*This function chooses p random elements from the bitstream by Fisher-Yates algorithm*/
void ShuffleAndSelect(int N, int p, char *BitStream, int BitStore[2][p]) {
    /*Seeding rand function*/
    srand(time(0));
    
    /*Swap map stores the steps involved in the shuffle and temp is involved in a swap*/
    int SwapMap[N-1];
    int temp;
    
    /*This loop swaps the last considered element with a randomly chosen previous element*/
    for(int i=N-1; i>0; i--)
    {
        /*SwapMap stores info that ith element was swapped to a randomly generated location*/
        SwapMap[i-1]=rand()%(i+1);
        /*Performing swap*/
        temp=BitStream[i];
        BitStream[i]=BitStream[SwapMap[i-1]];
        BitStream[SwapMap[i-1]]=temp;
    }

    /*This loop simply allocates p elements of shuffled array to BitStore*/
    for(int i=0; i<p; i++)
    {
        BitStore[1][i]=BitStream[i];
        /*Storing shuffled array locations*/
        BitStore[0][i]=i;
    }
    
    /*As the BitStream array was shuffled locations were lost-now retrieving them*/
    for(int i=0; i<(N-1); i++)
    {
        for(int j=0; j<p; j++)
        {
            /*Tracing paths of locations of different elements*/
            if(BitStore[0][j]==SwapMap[i])
            {
                BitStore[0][j]=(i+1);
            } else if(BitStore[0][j]==(i+1))
            {
                BitStore[0][j]=SwapMap[i];
            }
        }
        /*Bringing BitStream back to order*/
        temp=BitStream[SwapMap[i]];
        BitStream[SwapMap[i]]=BitStream[i+1];
        BitStream[i+1]=temp;
    }
}

/*==================
 *Main function
 ==================*/

int main(int argc, char **argv)
{
    /*Seeding rand*/
    srand(time(0));
    
    /*Preparing to plot on gnuplot*/
    FILE *gnuplot=popen("gnuplot -persistence", "w");
    fprintf(gnuplot, "set title 'Hamming Distance vs Offset'\n");
    fprintf(gnuplot, "set xlabel 'Offset'\nset ylabel 'Hamming Distance'\nplot '-' w l\n");
    
    /*Obtaining command line inputs*/
	int N=(int) atoi(argv[1]);
	double P=atof(argv[2]);
	double Q=atof(argv[3]);
    
    /*Generating offset*/
	int M=(int) ((1.0*N)/(rand()%(N/10)));
	
    /*Allocating memory to bit stream*/
	char *BitStream= malloc(N*sizeof(char));
	
    /*Storing random bits- 0 or 1*/
    for(int i=0; i<N; i++)
	{
        BitStream[i]=rand()%2;
	}
    
    /*Creating an array for recieved bits and memory locations*/
    int p=(int)(P*N);
    int BitStore[2][p];
    
    /*Choosing p random elements*/
    char *OffsetBitStream=BitStream+M;
    ShuffleAndSelect(N-2*M, p, OffsetBitStream, BitStore);

    /*We find the maximum location to estimate the offsets we may comfortably use*/
    int MaxLocation=BitStore[0][0];
    
    /*Flipping bits with probability Q*/
    for(int i=0; i<p; i++)
    {
        if((((rand()*1.0)/RAND_MAX ))<=Q)
        {
            if(BitStore[1][i]==0)
                BitStore[1][i]=1;
            else
                BitStore[1][i]=0;
        }
        if(MaxLocation<BitStore[0][i])
        MaxLocation=BitStore[0][i];
    }
    
    /*The following variables are self explanatory*/
    int MaxOffset=N-MaxLocation-1;
    printf("MaxLocation=%d\nMaxOffset=%d\n",MaxLocation,MaxOffset);
    int MinHammingDistance;
    int HammingDistance;
    int OffsetLocation;
    
    for(int i=0; i<MaxOffset; i++)
    {
        HammingDistance=0;
        for(int j=0; j<p; j++)
        {
            if(BitStore[1][j]!=BitStream[i+BitStore[0][j]])
                HammingDistance++;
        }
        if(i==0)
        {
            MinHammingDistance=HammingDistance;
            OffsetLocation=0;
        }
        if(MinHammingDistance>HammingDistance)
        {
            MinHammingDistance=HammingDistance;
            OffsetLocation=i;
        }
        fprintf(gnuplot,"%d %d\n", i, HammingDistance);
    }
    printf("Offset=%d\nCalculated=%d\n",M, OffsetLocation);
    fprintf(gnuplot, "\e");
    printf("\n");
    return 0;
}

/*=================
 *End of Program
 =================*/
