/************************************************************************
Authors:Ravisri Kumara Valluri, Kevin Martin, Phani Jayanth
Date:4/9/2019
Purpose:To use interpolation methods for a set of points
Inputs:None
Outputs:Values of output of real and interpolated data at specific values
*************************************************************************/

/************
Header files
*************/

#include<stdio.h>
#include<math.h>

/*This function generates Newtons divided difference formula recursively for a term*/

float newtonian(float *a, float *b, int n, float x) //a and b store input info
{
	float p=1;
	if(n==2)
	{
		return  ((*(b+1)-*b)/(*(a+1)-*a))*(x-*a);
// P(2)=f(x(0))+ f[x(0),x(1)](x-x(0)), and f[x(0),x(1)] is slope
	} else {
		for(int i=0; i<(n-1); i++)
		{
			p*=(x-*(a+i));
		}
		return ((newtonian(a+1, b+1, n-1, x)-newtonian(a, b, n-1, x))/(*(a+n-1)-*a))*p;
//This formula would be P(n)=P(n-1)+f[x(0),x(1),...,x(n)](x-x(0))(x-x(1))...(x-x(n-1))
	}
}

void main()
{
	float x[]={1.0,1.1,1.2,1.3}; //X values
	float y[]={tan(1),tan(1.1),tan(1.2),tan(1.3)}; //Y values
	float checker=0; //Runs through all X values from 1.00 to 1.39
	float error=0; //Stores error
	float poly=0, comp=0; //comp stores Lagrange coefficient for a term, poly stores interpolated value 
	printf("Lagrange method");
	printf("Real                Interpolated\n");
	for(int k=0; k<40; k++) //Running through all elements
	{
		poly=0; //Setting up for a run
		checker=1.0+k*0.01; //Pushing X forward
		for(int i=0; i<4; i++)
		{
			comp=1;
			for(int j=0; j<4; j++) 
			{
				if(j!=i)
				comp*=(checker-x[j])/(x[i]-x[j]);
				//Obtaining the Lagrange coefficient
			}
			poly+=y[i]*comp;
			//Summing up terms
		}
		if(k==15||k==35)
		printf("%f %f - %f %f\n", checker, tan(checker), checker, poly);
		//printing result
		error+=(poly-tan(checker))*(poly-tan(checker));
	}
	printf("Standard deviation=%f\n",sqrt(error/40));
	error=0;
	float j=y[0];
	printf("\n\nDivided differences method for limited points:\n");
	printf("Real                Interpolated\n");
	for(int i=0; i<40; i++)
	{
		for(int k=2; k<=4; k++)
		{
			//Adding the newtonian difference terms
			j+=newtonian(x, y, k, (1+0.01*i));
		}
		error+=(j-tan(1+0.01*i))*(j-tan(1+0.01*i));
		if(i==15||i==35)		
		printf("%f %f - %f %f\n",1+0.01*i,tan(1+0.01*i),1+0.01*i, j);
		j=y[0];
	}
	printf("Standard deviation=%f\n",sqrt(error/40));

}

/***************
*End of program!
****************/
