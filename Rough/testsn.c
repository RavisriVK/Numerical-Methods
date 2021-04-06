#include<stdio.h>


void main()
{


double rawfsc,rawssc,filteredssc,filteredfsc,filteredtime;
int filterflag=0;
int timecount=-1;	
FILE *fptr,*fptr2,*fptr3;
fptr=fopen("CompleteData_fsc_ssc_10um.txt","r");//////////////////////////////////////////////////////////check for test file
fptr2=fopen("tester.txt","w");	
fptr3=fopen("tester2.txt","w");
int i=-1;

while(!feof(fptr))
{
timecount++;
i++;	
if(filterflag<7)
{	
fscanf(fptr,"%lf %lf",&rawfsc,&rawssc);
filteredfsc+=rawfsc;
filteredssc+=rawssc;
filteredtime+=timecount;
filterflag++;
continue;
}
if(filterflag==7)
{
filteredfsc=filteredfsc*(1.0/7.0);
filteredssc=filteredssc*(1.0/7.0);
filteredtime=filteredtime*(1.0/7.0);
filterflag=0;
fprintf(fptr2,"%lf %lf\n",filteredtime,filteredfsc);
fprintf(fptr3,"%d %lf\n",4+(7*i),filteredssc);	
filteredfsc=0;
filteredssc=0;
filteredtime=0;

}
}
}