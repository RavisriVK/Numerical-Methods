#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void Solver(int n, float a[n][n], b[n], float x[n])
{
	float factor=0;
	for(int i=0; i<(n-1); i++)
	{
		for(int j=i+1; j<n; j++)
		{
			factor=a[j][i]/a[i][i];
			a[j][i]=factor;
			for(int k=i+1; k<n; k++)
			{
				a[j][k]=a[j][k]-a[i][k]*factor;
			}
		}
	}
	
	for(int i=1; i<n; i++)
	{
		for(int j=0; j<i; j++)
		{
			b[i]-=a[i][j]*b[j];
		}
	}
	
	x[n-1]=b[n-1]/a[n-1][n-1];
	float sum;
	for(int i=n-2; i>=0; i--)
	{
		sum=0;
		for(int j=i+1; j<n; j++)
		{
			sum+=a[i][j]*x[j];
		}
		x[i]=(b[i]-sum)/a[i][i];
	}
}



int main()
{

}
