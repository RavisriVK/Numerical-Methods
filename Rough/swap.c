#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

void shuffler(int N, int p, char *BitStream, int BitStore[2][p]) {
    srand(time(0));
    int SwapMap[N-1];
    int temp;
    for(int i=N-1; i>0; i--)
    {
        SwapMap[i-1]=rand()%(i+1);
        temp=BitStream[i];
        BitStream[i]=BitStream[SwapMap[i-1]];
        BitStream[SwapMap[i-1]]=temp;
    }
    
    for(int i=0; i<p; i++)
    {
        BitStore[0][i]=i;
        BitStore[1][i]=BitStream[i];
    }
    for(int i=0; i<(N-1); i++)
    {
        for(int j=0; j<p; j++)
        {
            if(BitStore[0][j]==SwapMap[i])
            {
                BitStore[0][j]=(i+1);
            } else if(BitStore[0][j]==(i+1))
            {
                BitStore[0][j]=SwapMap[i];
            }
        }
        temp=BitStream[SwapMap[i]];
        BitStream[SwapMap[i]]=BitStream[i+1];
        BitStream[i+1]=temp;
    }

int main()
{
    int RandomSelection[2][4];
    char BitStream[]={1,2,3,4,5,6,7};
    shuffler(7, 4, BitStream, RandomSelection);
}
