/*************************************************************************************
*Authors:Ravisri Valluri, Snehan
*Purpose:To predict area of person based on height and weight from linear regression
*Method:By solving three equations-
		n*a0    + sumw*a1  + sumh*a2  = sumy 
	        sumh*a0 + sumwh*a1 + sumh2*a2 = sumyh
	 	sumw*a0 + sumw2*a1 + sumwh*a2 = sumyw
		-obtained by minimizing standard deviation with respect to a0, a1, a2
		Area = a0 + a1*w + a2*h
*Date:21/09/2019
*Inputs:None
*Outputs:None
*************************************************************************************/

/*******************
*Header files
********************/

#include<stdio.h>
#include<math.h>

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

/*******************
*Main function
*******************/
void main() {
	/*Storing heights and weights*/
	int height[10]={182, 180, 179, 187, 189, 194, 195, 193, 200};
	int weight[10]={74, 88, 94, 78, 84, 98, 76, 86, 96};
	float area[10]={1.92, 2.11, 2.15, 2.02, 2.09, 2.31, 2.02, 2.16, 2.31};
	/*These variables store the sums of:*/
	/*Height, Weight, Height*Weight, Height*Height, Weight*Weight, Area, Height*Area, Weight*Area*/
	int l = 0, m = 0, rv = 0, o = 0, p = 0;
	float r = 0, s = 0, t = 0;
	for (int i = 0; i < 9; i++)
	{
		l += (height[i]);
		m += (weight[i]);
		rv += (height[i]) * (weight[i]);
		o += (height[i]) * (height[i]);
		p += (weight[i]) * (weight[i]);
		r += (area[i]);
		s += (height[i]) * (area[i]);
		t += (weight[i]) * (area[i]);
 	}
	float a[3][3] = {{9,l,m},{l,o,rv},{m,rv,p}};
	float b[3] = {r,s,t};
	/*Solving equations to obtain equation of area*/
	float x[3];
	Decomposer(3, a);
	Substituter(3, a, b, x);
	/*Equation for area*/
	printf("Equation for area is:\n");
	printf("Area=%f + %f*<height> + %f*<weight>\n", x[0], x[1], x[2]);
	/*Printing area for given case*/
	float ans =x[0]+x[1]*187+x[2]*78;//pow(M_E,x[0])* pow(187, x[1]) * pow(78, x[2]);
	printf("The area of a person of height 187cm and weight 78kg by linear regression is:\n");
	printf("%f\n", ans);
}
