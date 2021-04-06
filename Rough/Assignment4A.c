#include<stdio.h>
#include<math.h>

void Decomposer(int n, float a[n][n])
{
	float factor;
	for(int i=0; i<(n-1); i++)
	{
		for(int j=i+1; j<n; j++)
		{
			a[j][i]=(a[j][i])/(a[i][i]);
			factor=a[j][i];
			for(int k=i+1; k<n; k++)
			{
				a[j][k]=a[j][k]-a[i][k]*factor;
			}
		}
	}
}

void Substituter(int n, float a[n][n], float b[n], float x[n])
{
	/*forward substitution*/
	for(int i=1; i<n; i++)
	{
		for(int j=0; j<i; j++)
		{
			b[i]-=a[i][j]*b[j];
		}
	}
	
	/*back substitution*/
	x[n-1]=b[n-1]/(a[n-1][n-1]);
	float sum;
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

void CubicSpline(int m, float x[m], float y[m], float xin)
{
	int n=m-1;
	float temp=0;
	float a[n], b[n], c[m], d[n], v[m], h[n];
	float A[m][m];
	v[0]=0;
	v[n]=0;
	/*Creating h matrix*/
	for(int i=1; i<m; i++)
	{
		h[i-1]=x[i]-x[i-1];
	}
	/*Creating A matrix*/
	for(int i=1; i<n; i++)
	{
		A[i][i-1]=h[i-1];
		A[i][i]=2*(h[i-1]+h[i]);
		A[i][i+1]=h[i];
		for(int j=0; j<m; j++)
		{
			if((j>i+1)||(j<i-1))
			A[i][j]=0;
		}
	}
	A[0][0]=1;
	A[n][n]=1;
	for(int i=1; i<m; i++)
	{
		A[0][i]=0;
		A[n][i-1]=0;
	}
	/*Creating v matrix*/
	for(int i=1; i<n; i++)
	{
		v[i]=3*((y[i+1]-y[i])/h[i]+(y[i]-y[i-1])/h[i-1]);
	}
	/*Solving for c*/
	Decomposer(m, A);
	Substituter(m, A, v, c);
	c[n]=0;
	/*Generating a, b, and d matrices*/
	for(int i=0; i<n; i++)
	{
		b[i]=(a[i+1]-a[i])/h[i] - (h[i]*(2*c[i]+c[i+1]))/3.0;
		d[i]=(c[i+1]-c[i])/(3.0*h[i]);
		a[i]=y[i];
	}
	/*Starting to interpolate...*/
	for(int i=0; i<n; i++)
	{
        if(xin>=x[i])
            temp=a[i]+b[i]*(xin-x[i])+c[i]*pow(xin-x[i], 2.0)+d[i]*pow(xin-x[i], 3.0);
        if(isnan(temp)||temp<0||temp>1)
            temp=a[i];
	}
    printf("Interpolated value=%f",temp);
}

void main(int argc, char **argv)
{
    /*Solving given set of equations*/
	float a[3][3]={{3,-0.1,-0.2},{0.1,7,-0.3},{0.3,-0.2,10}};   //stores coefficients
	float b[3]={7.85,-19.3,71.4};
	float x[3];
	Decomposer(3,a);
	Substituter(3,a,b,x);
    printf("Given equations were:\n");
    for(int i=0; i<3; i++)
           printf("%f*x(1) + %f*x(2) + %f*x(3)= %f\n",a[i][0], a[i][1], a[i][2], b[i]);
	printf("Solutions for x1, x2, and x3 are, by LU decomposition:\n");
	for(int i=0;i<3;i++)
	  printf("%lf\n",x[i]);
	printf("\n\n");
    /*Using LU decomposition for cubic spline interpolation*/
    float X[502], Y[502];
    FILE *pt;
    pt=fopen("out1_test0.csv", "r");
    for(int i=0; i<502; i++)
    {
        X[i]=i;
        fscanf(pt, "%f", &Y[i]);
    }
    if(argc>=2)
        CubicSpline(502, X, Y, atof(argv[1]));
    else
        printf("User has chosen not to try cubic interpolation.\nCommand line format is: ./a.out #value\nValue is from 0 to 501.");
}
