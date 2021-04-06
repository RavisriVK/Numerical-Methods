/***************************************************
*Authors:Ravisri Valluri, Snehan
*Purpose:To solve a system of N equations and apply
*Date:21/09/2019
*Inputs:Data file out1_test0.csv from Interpolation
*Outputs:None
****************************************************/

/*******************
*Header files
********************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

/*this function converts the matrix a to an augmented L U matrix*/
/*n is the number of equations, a stores the coefficients of variables*/
void Decomposer(int n, float a[n][n])
{
	float factor;
	/*this loop goes through all rows but the first*/
	for(int i=0; i<(n-1); i++)
	{
		/*this loop performs the row operation on the matrix*/
		for(int j=i+1; j<n; j++)
		{
			/*factor stores values of the L matrix*/
			a[j][i]=(a[j][i])/(a[i][i]);
			factor=a[j][i];
			/*this loop generates values of U matrix*/
			for(int k=i+1; k<n; k++)
			{
				a[j][k]=a[j][k]-a[i][k]*factor;
			}
		}
	}
}

/*this function solves for the variables*/
/*n isn the number of equation, a stores coefficients, b stores constants and x stores the solution*/
void Substituter(int n, float a[n][n], float b[n], float x[n])
{
	/*forward substitution*/
	for(int i=1; i<n; i++)
	{
		/*this extends the row operation to the constants*/
		for(int j=0; j<i; j++)
		{
			b[i]-=a[i][j]*b[j];
		}
	}
	
	/*back substitution*/
	x[n-1]=b[n-1]/(a[n-1][n-1]);
	float sum;
	/*we solve for all variables by means of the (n-1)th variable*/
	for(int i=n-2; i>=0; i--)
	{
		sum=0;
		for(int j=i+1; j<n; j++)
		{
			sum+=a[i][j]*x[j];
		}
		x[i]=(b[i]-sum)/(a[i][i]);
	}
}

/*this function generates points between points given by x and y*/
/*m stores number of points, x, y store the absissa and ordinate, ts stores user input*/
void CubicSpline(int m, float x[m], float y[m], float ts)
{
	FILE *out;
	out=fopen("Interpol.txt","w");
	int n=m-1;
	float intervalue=0;
	float a[n], b[n], c[m], d[n], v[m], h[n];
	float A[m][m];
	/*Creating h matrix*/
	for(int i=0; i<n; i++)
	{
		h[i]=x[i+1]-x[i];
	}
	/*Creating A matrix*/
	for(int i=0; i<m; i++)
	for(int j=0; j<m; j++)
	A[i][j]=0;
	for(int i=1; i<n; i++)
	{
		A[i][i-1]=h[i-1];
		A[i][i]=2*h[i-1]+2*h[i];
		A[i][i+1]=h[i];
	}
	A[0][0]=1;
	A[n][n]=1;
	/*Creating v matrix*/
	for(int i=1; i<n; i++)
	{
		v[i]=3*((y[i+1]-y[i])/h[i]) - 3*((y[i]-y[i-1])/h[i-1]);
	}
	v[0]=0;
	v[n]=0;
	/*Solving for c*/
	Decomposer(m, A);
	Substituter(m, A, v, c);
	c[n]=0;
	/*Generating a, b, and d matrices*/
	for(int i=0; i<n; i++)
	{
		b[i]=(a[i+1]-a[i])/h[i] - (h[i]/3.0)*(2*c[i]+c[i+1]);
		d[i]=(c[i+1]-c[i])/(3.0*h[i]);
		a[i]=y[i];
	}
	/*Starting to interpolate...*/	
	for(int i=0; i<(n-1); i++)
	{
		/*Moving along x values till maximum value reached- required range*/
		if(ts>=x[i])
		intervalue= a[i]+(ts-x[i])*b[i]+c[i]*(pow((ts-x[i]),2)) + d[i]*(pow((ts-x[i]),3));
		/*Contingency if file is different*/
		if(isnan(intervalue)||intervalue<0||intervalue>1)
		intervalue=a[i];
	}
	printf("Interpolated value=%f\n>By cubic spline method<\n",intervalue);
}

/*******************
*Main function
*******************/
void main(int argc, char **argv)
{       
	/*Given equations for testing assignment*/
	float a[3][3]={{3,-0.1,-0.2},{0.1,7,-0.3},{0.3,-0.2,10}};
	float b[3]={7.85,-19.3,71.4};
	float x[3];
	printf("Given equations were:\n");
	for(int i=1; i<=3; i++)
	{
		printf("%.1fx(1) + %.1fx(2) + %.1fx(3) = %.2f\n", a[i-1][0],a[i-1][1],a[i-1][2],b[i-1]);
	}
	Decomposer(3,a);
	Substituter(3,a,b,x);
	printf("x1, x2, and x3 are:\n");
	for(int i=0;i<3;i++)
		printf("%f ",x[i]);
	printf("\n");
	/*Using to generate cubic splines*/
	float X[502], Y[502];
	FILE *pt;
	pt=fopen("out1_test0.csv", "r");
	for(int i=0; i<502; i++)
	{
		X[i]=i;
		fscanf(pt, "%f", &Y[i]);
	}
	if(argc<2)
	printf("\nUser has chosen not to try interpolation.\nFormat is: ./a.out #value\nValue must be in 0 to 501.");
	else
	CubicSpline(502, X, Y, atof(argv[1]));
}
