/******************************************************************************************
*Purpose: To use different methods of interpolation for a given data set and calculate their standard deviations
*Authors: Ravisri Valluri, Kevin Martin, J Phani Jayanth
*Date: 4/09/2019
*Inputs: None
*Outputs: Standard deviations using linear, quadratic and cubic interpolation and 3 txt files datal,dataq and datac containing the interpolated points
*******************************************************************************************/

#include<stdio.h>
#include<math.h>
void main()
{
	FILE *dat;//file pointer for input
	FILE *out;//file pointer for output
	int i,j;
	double a[502];		
	double l1,l2,l3,l4,temp,err,p1,p2,p3,p4,x;
	dat=fopen("out1_test0.csv","r");
	out=fopen("datal.txt","w");
	for(i=0;i<502;i++)
	{	
		fscanf(dat,"%lf",&a[i]);
	}
//Linear*********************************
	for(i=0;i<50;i++)	
	{
		l1=a[i*10];
		l2=a[(i+1)*10];
		fprintf(out,"%.14lf\n",l1);
		p1=(l2-l1)/10;
		for(j=0;j<9;j++)
		{
			temp=l1+(j+1)*p1;
			fprintf(out,"%.14lf\n",temp);
			err+=pow(temp-a[i*10+j+1],2);
		}
	}
	fprintf(out,"%.14lf\n",l2);
	err=sqrt(err/450);
	printf("Linear error= %f\n",err);
	err=0;
//Quadratic******************************
	out=fopen("dataq.txt","w");
	for(i=0;i<50;i+=2)
	{
		l1=a[i*10];
		l2=a[(i+1)*10];
		l3=a[(i+2)*10];
		fprintf(out,"%.14lf\n",l1);
		for(j=0;j<18;j++)
		{
			if(j==9){fprintf(out,"%.14lf\n",l2);continue;}
			x=(i*10+j+2);
			p1=((x-(i+1)*10)*(x-(i+2)*10))/200;
			p2=-((x-i*10)*(x-(i+2)*10))/100;
			p3=((x-i*10)*(x-(i+1)*10))/200;
			temp=l1*p1+l2*p2+l3*p3;
			fprintf(out,"%.14lf\n",temp);
			err+=pow(temp-a[i*10+j+1],2);
		}
	}
	fprintf(out,"%.14lf\n",l3);
	err=sqrt(err/450);
	printf("Quadratic error= %f\n",err);
	err=0;
//Cubic***********************************
	out=fopen("datac.txt","w");
	for(i=0;i<50;i+=3)
	{
		l1=a[i*10];
		l2=a[(i+1)*10];
		l3=a[(i+2)*10];
		l4=a[(i+3)*10];
		
		fprintf(out,"%.14lf\n",l1);
		for(j=0;j<27;j++)
		{
			if(j==9){fprintf(out,"%.14lf\n",l2);continue;}
			if(j==18){fprintf(out,"%.14lf\n",l3);continue;}
			x=(i*10+j+2);
			p1=-((x-(i+1)*10)*(x-(i+2)*10)*(x-(i+3)*10))/6000;
			p2=((x-i*10)*(x-(i+2)*10)*(x-(i+3)*10))/2000;
			p3=-((x-i*10)*(x-(i+1)*10)*(x-(i+3)*10))/2000;
			p4=((x-i*10)*(x-(i+1)*10)*(x-(i+2)*10))/6000;
			temp=l1*p1+l2*p2+l3*p3+l4*p4;
			fprintf(out,"%.14lf\n",temp);
			err+=pow(temp-a[i*10+j+1],2);
		}
	}
	fprintf(out,"%.14lf\n",l4);
	err=sqrt(err/450);
	printf("Cubic error= %f\n",err);
	err=0;
}
