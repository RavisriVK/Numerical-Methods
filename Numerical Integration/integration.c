/*******************************************************************

*Purpose: To perform Integration of a given Dataset by various methods namely Romberg's Method, Trapezoidal Rule, Midpoint Rule and Simpson's Rule and comparing them.
*Authors: Ravisri Kumara Valluri, Kevin Martin, J.Phani Jayanth
*Date: 09/09/2019
*Inputs: A file named "out1_test0.csv" containing the dataset to be intergrated.
*Outputs: The value of the Integral by Romberg's Method and values using other methods into three different files named "trap.txt","mid.txt" and "simp.txt" for different step sizes(∆x).

*********************************************************************/

/****************
*Header files
****************/
#include <stdio.h>
#include <math.h>

/***********************************************************
*This Function generates extra points for integration *through the Quadratic Interpolation Method
***********************************************************/
double f_gen(double a, double *arr)
{
	int i,j;		
	double l1,l2,l3,temp,err,p1,p2,p3,x;
	int n1,n2,n3;
	if((ceil(a)==a))return arr[(int)a];
	if(a-floor(a)>0.5)		
	{
		n3=(int)ceil(a)+1;
		n1=(int)floor(a);
		n2=(int)ceil(a);
	}
	else
	{
		n1=(int)floor(a)-1;
		n2=(int)floor(a);
		n3=(int)ceil(a);
	}
	l1=arr[n1];
	l2=arr[n2];
	l3=arr[n3];
	p1=((a-n3)*(a-n2))/2;
	p2=-((a-n1)*(a-n3));
	p3=((a-n1)*(a-n2))/2;
	temp=l1*p1+l2*p2+l3*p3;        
	return temp;
}

/****************************************************
Function for obtaining the value of integral by *Romberg's Method
*****************************************************/
double romberg(double (*f/* function to integrate */), double /*lower limit*/ a, double /*upper limit*/ b, 		       size_t max_steps, double /*desired accuracy*/ acc)
{
	double R1[max_steps], R2[max_steps]; //buffers
	double *Rp = &R1[0], *Rc = &R2[0];
	// Rp is the previous row, Rc is the current row
	double h = (b-a);        //step size
	Rp[0] = (f[(int)a] + f[(int)b])*h*.5; 
	// First Trapezoidal step
	for(size_t i = 1; i < max_steps; ++i)
	{
		h /= 2.;
		double c = 0;
		size_t ep = 1 << (i-1); //2^(n-1)
		for(size_t j = 1; j <= ep; ++j)
		{
			c += f_gen(a+(2*j-1)*h,f);
		}
		Rc[0] = h*c + .5*Rp[0]; //R(i,0)
		for(size_t j = 1; j <= i; ++j)
		{
			double n_k = pow(4, j);
			Rc[j] = (n_k*Rc[j-1] - Rp[j-1])/(n_k-1); //compute R(i,j)
		}

		if(i > 1 && fabs(Rp[i-1]-Rc[i]) < acc)
		{
			return Rc[i-1];
		}
	
	      //swap Rn and Rc as we only need the last row
		double *rt = Rp;
		Rp = Rc;
		Rc = rt;
	}
	return Rp[max_steps-1]; //return our best guess
}

/*****************************************************
*Function that generates the Integral Value 
*using Trapezoidal Rule
*****************************************************/
double trap(double *arr, double x)
{
	double integral=0;
	for(double i=0; i<501; i+=pow(10,x))
	{
		integral+=((f_gen(i+pow(10,x),arr)+f_gen(i,arr))*pow(10,x))/2;
	}
	return integral;
}

/***************************************************
*Function that generates the value of Integral
*using Midpoint Rule
***************************************************/
double midmethod(double *arr, double x)
{
	double integral=0;
	double del=pow(10,x);
	for(double i=0; i<501; i+=del/*0.0001*/)
	{
		integral+=f_gen(i+(del/2),arr)*del;
	}
	return integral;
}

/***************************************************
*Function that generates the Integral value 
*using Simpson's Method
***************************************************/
double simpsons(double *arr,double x)
{
	double integral=arr[0]+arr[501];
	double del=pow(10,x);
	int k=0;
	for(double i=1; i<501; i+=del)
	{	
		k++;
		integral+=f_gen(i,arr)*(k%2==0?2:4);
	}
	integral*=del/3.0;
	return integral;
}

/*****************
*Main Program
*****************/
void main()
{
	FILE *inp; // File pointer for Input
	inp=fopen("out1_test0.csv","r"); // Reading data from the file "out1_test0.csv"
	double arr[502];
	int i;
	double i1;
/*************************************************************
*This for loop gives the value of the integral obtained *by Romberg's Rule
*************************************************************/
	for(i=0;i<502;i++)
	{
		fscanf(inp,"%lf",&arr[i]);
	}
	double ans=romberg(arr,0,501,18,0.000001);
	printf("Romberg= %f\n",ans);	
	inp=fopen("trap.txt","w");
	for(i1=1.5;i1>=-4;i1-=0.5)fprintf(inp,"%f\t%f\n",i1,trap(arr,i1));/*Writing the Integral values   		obtained from trapezoidal Rule for various ∆x into the file "trap.txt" */
	
	inp=fopen("mid.txt","w");             
	for(i1=1.5;i1>=-4;i1-=0.5)fprintf(inp,"%f\t%f\n",i1,midmethod(arr,i1));     /*Writing the Integral 		values obtained from Midpoint Rule for various ∆x into the file "mid.txt" */

	inp=fopen("simp.txt","w");
	for(i1=1.5;i1>=-4;i1-=0.5)fprintf(inp,"%f\t%f\n",i1,simpsons(arr,i1));       /*Writing the Integral 		values obtained from Simpsons Method for various ∆x into the file "simp.txt" */
}

/***********************
*End of the Program
***********************/




