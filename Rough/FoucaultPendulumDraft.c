/*=========================================================================================================================================
 *Authors: Ravisri Valluri, Snehan
 *Date: 23/10/2019
 *Purpose:
 -To solve and graph the differentials associated with a foucault pendulum numerically by Euler's, Heun's, Runge-Kutta's and RK45 methods
 -The differential equations are:
 -1) x''= -(Ψ^2)x + 2(Ω)y'
 -2) y''= -(Ψ^2)y - 2(Ω)x'
 *Inputs: Only command line inputs
 -Possible input formats are:
 -1) ./a.out #Method #CoriolisCoefficient #NaturalFrequency
 -2) ./a.out #Method #CoriolisCoefficient #NaturalFrequency #InitialXVelocity #InitialYVelocity
 -3) ./a.out #Method #CoriolisCoefficient #NaturalFrequency #InitialXVelocity #InitialYVelocity #TimeStep #Iterations
 -Available methods are
 -i) E or e for Euler's
 -ii) H or h for Heun's
 -iii) R or r for Runge Kutta's
 -iv) F or f for RK45 (Fehlberg's)
 *Outputs:GNUPLOT graphs
 ==========================================================================================================================================*/

/*==============
 *Header Files
 ==============*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*=====================
 *Function Declarations
 ======================*/

void RangeKuttaFourStage(double, double, double, double, double, int);
void RK45Fehlberg(double, double, double, double, double, int);
void Euler(double, double, double, double, double, int);
void HeunsTwoStage(double, double, double, double, double, int);

/*=============
 *Main function
 ==============*/

int main(int argc, char **argv)
{
	/*Creating variables to store inputs*/
	double CoriolisCoefficient, Frequency, Vx, Vy, TimeStep;
	int Iterations;
	char Option=argv[1][0];
    
	/*Checking that number of inputs is correct*/
	if(argc==8)
    {
        CoriolisCoefficient=strtod(argv[2], NULL);
        Frequency=strtod(argv[3], NULL);
		Vx=strtod(argv[4],NULL), Vy=strtod(argv[5],NULL);
        TimeStep=strtod(argv[6],NULL);
        Iterations=(int)atoi(argv[7]);
	} else if(argc==4)
	{
		CoriolisCoefficient=strtod(argv[2], NULL);
        Frequency=strtod(argv[3], NULL);
		Vx=3, Vy=4;
        TimeStep=0.0001;
        Iterations=100000;
	} else if(argc==6)
	{
		CoriolisCoefficient=strtod(argv[2], NULL);
        Frequency=strtod(argv[3], NULL);
        Vx=strtod(argv[4],NULL), Vy=strtod(argv[5],NULL);
		TimeStep=0.0001;
        Iterations=100000;
	} else
	{
		/*Displaying possible formats*/
		printf("\nPossible input formats are:\n");
		printf("1) ./a.out #Method #CoriolisCoefficient #NaturalFrequency\n");
		printf("2) ./a.out #Method #CoriolisCoefficient #NaturalFrequency #InitialXVelocity #InitialYVelocity\n");
		printf("3) ./a.out #Method #CoriolisCoefficient #NaturalFrequency #InitialXVelocity #InitialYVelocity #TimeStep #Iterations\n");
		printf("Available methods are:\ni)E/e for Euler's\nii)H/h for Heun's\niii)R/r for Runge Kutta's\niv)F/f for RK45 (Fehlberg's)\n");
		printf("Please choose one.\n\n");
		return 0;
	}
	
	/*Printing graph by chosen method*/
	switch(Option)
	{
		case 'h':
		case 'H': HeunsTwoStage(CoriolisCoefficient, Frequency, Vx, Vy, TimeStep, Iterations); break;
		case 'e':
		case 'E': Euler(CoriolisCoefficient, Frequency, Vx, Vy, TimeStep, Iterations); break;
		case 'r':
		case 'R': RangeKuttaFourStage(CoriolisCoefficient, Frequency, Vx, Vy, TimeStep, Iterations); break;
		case 'f':
		case 'F': RK45Fehlberg(CoriolisCoefficient, Frequency, Vx, Vy, TimeStep, Iterations); break;
		default: printf("No such method available.\n"); break;
	}
}

/*===================
 *Function prototypes
 ====================*/

//./a.out 0.073 4 3 4 0.000411 105000
/*This function graphs the equations by Euler's method*/
void Euler(double CoriolisCoefficient, double Frequency, double Vx, double Vy, double TimeStep, int Iterations)
{
	/*State Vector stores x-coordinate, x-velocity, y-coordinate, y-velocity*/
	/*Function Vector stores the differentiated elements of State Vector*/
    double StateVector[4]={0.0,Vx,4.0,Vy};
    double FunctionVector[4]={Vx, 2*CoriolisCoefficient*Vy, Vy, (-2)*CoriolisCoefficient*Vx};
    
	/*Creating GNUPLOT file and preparing to plot*/
    FILE *EulerGNUPLOT=popen("gnuplot -persistence","w");
    fprintf(EulerGNUPLOT,"set title 'Euler Method'\nset xlabel 'X-coordinate\nset ylabel 'Y-coordinate'\n");
    fprintf(EulerGNUPLOT,"plot '-' w l\n");
    fprintf(EulerGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
    
	/*This loop increments state step by step*/
    for(int i=0; i<Iterations; i++)
    {
		/*Here using y(new)=y(old) + y'(old)*TimeStep */
        StateVector[0]=StateVector[0]+FunctionVector[0]*TimeStep;
        StateVector[1]=StateVector[1]+FunctionVector[1]*TimeStep;
        StateVector[2]=StateVector[2]+FunctionVector[2]*TimeStep;
        StateVector[3]=StateVector[3]+FunctionVector[3]*TimeStep;
        
		/*Printing only selected points to avoid saturation*/
		if(i%50==0)
			fprintf(EulerGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
        
		/*Bringing FunctionVector to new state*/
		FunctionVector[0]=StateVector[1];
        FunctionVector[1]=2*CoriolisCoefficient*StateVector[3]-pow(Frequency,2.0)*StateVector[0];
        FunctionVector[2]=StateVector[3];
        FunctionVector[3]=(-2)*CoriolisCoefficient*StateVector[1]-pow(Frequency,2.0)*StateVector[2];
    }
	/*Enabling run-time viewing of graph*/
    fprintf(EulerGNUPLOT, "\e");
    fflush(EulerGNUPLOT);
}

/*This function graphs the equations by Heun's method*/
void HeunsTwoStage(double CoriolisCoefficient, double Frequency, double Vx, double Vy, double TimeStep, int Iterations)
{
	/*State Vector stores x-coordinate, x-velocity, y-coordinate, y-velocity - this is the state*/
	/*Function Vector stores the differentiated elements of State Vector*/
	/*Shadow Vector locates the state at an incremented time to improve accuracy of approximation*/
	/*Step Vector finds differentiated Shadow Vector matrix*/
    double StateVector[4]={0.0,Vx,0.0,Vy};
    double FunctionVector[4]={Vx, 2*CoriolisCoefficient*Vy, Vy, (-2)*CoriolisCoefficient*Vx};
    double StepVector[4], ShadowVariable[4];
	
	/*Creating GNUPLOT file and preparing to plot*/
    FILE *HeunsGNUPLOT=popen("gnuplot -persistence","w");
    fprintf(HeunsGNUPLOT,"set title 'Heuns Method'\nset xlabel 'X-coordinate\nset ylabel 'Y-coordinate'\n");
    fprintf(HeunsGNUPLOT,"plot '-' w l\n");
    fprintf(HeunsGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
    
	/*This loop increments state step by step*/
    for(int i=0; i<Iterations; i++)
    {
		/*Here Shadow Variable finds state at an incremented time and Step Vector finds differentiated form*/
        ShadowVariable[0]=StateVector[0]+FunctionVector[0]*(TimeStep/1.0);
        ShadowVariable[1]=StateVector[1]+FunctionVector[1]*(TimeStep/1.0);
        ShadowVariable[2]=StateVector[2]+FunctionVector[2]*(TimeStep/1.0);
        ShadowVariable[3]=StateVector[3]+FunctionVector[3]*(TimeStep/1.0);
        
		StepVector[0]=ShadowVariable[1];
        StepVector[1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[2]=ShadowVariable[3];
        StepVector[3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here using y(new)= y(old) + (0.5*TimeStep)*(y'(old)+y'(incremented)) */
        StateVector[0]=StateVector[0]+(TimeStep/2.0)*(StepVector[0]+FunctionVector[0]);
        StateVector[1]=StateVector[1]+(TimeStep/2.0)*(StepVector[1]+FunctionVector[1]);
        StateVector[2]=StateVector[2]+(TimeStep/2.0)*(StepVector[2]+FunctionVector[2]);
        StateVector[3]=StateVector[3]+(TimeStep/2.0)*(StepVector[3]+FunctionVector[3]);
        
		/*Printing only selected points to avoid saturation*/
        if(i%50==0)
			fprintf(HeunsGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
        
		/*Bringing FunctionVector to new state*/
        FunctionVector[0]=StateVector[1];
        FunctionVector[1]=2*CoriolisCoefficient*StateVector[3]-pow(Frequency,2.0)*StateVector[0];
        FunctionVector[2]=StateVector[3];
        FunctionVector[3]=(-2)*CoriolisCoefficient*StateVector[1]-pow(Frequency,2.0)*StateVector[2];

    }
	/*Enabling run-time viewing of graph*/
    fprintf(HeunsGNUPLOT, "\e");
    fflush(HeunsGNUPLOT);
}

/*This function graphs the equations by Runge-Kutta's method*/
void RangeKuttaFourStage(double CoriolisCoefficient, double Frequency, double Vx, double Vy, double TimeStep, int Iterations)
{
	/*State Vector stores x-coordinate, x-velocity, y-coordinate, y-velocity - this is the state*/
	/*Function Vector stores the differentiated elements of State Vector*/
	/*Shadow Vector locates the state at an incremented time to improve accuracy of approximation*/
	/*Step Vectors find differentiated Shadow Vector matrices- or incremented Function Vectors*/
    double StateVector[4]={0.0,Vx,0.0,Vy};
    double FunctionVector[4]={Vx, 2*CoriolisCoefficient*Vy, Vy, (-2)*CoriolisCoefficient*Vx};
    double StepVector1[4], StepVector2[4], StepVector3[4], StateVector3[4], ShadowVariable[4];
    
	/*Creating GNUPLOT file and preparing to plot*/
	FILE *RangeKuttaGNUPLOT=popen("gnuplot -persistence","w");
    fprintf(RangeKuttaGNUPLOT,"set title 'Range Kutta 4 Stage'\nset xlabel 'X-coordinate\nset ylabel 'Y-coordinate'\n");
    fprintf(RangeKuttaGNUPLOT,"plot '-' w l\n");
    fprintf(RangeKuttaGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
    
	/*This loop increments state step by step*/
    for(int i=0; i<Iterations; i++)
    {
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+FunctionVector[0]*(TimeStep/2.0);
        ShadowVariable[1]=StateVector[1]+FunctionVector[1]*(TimeStep/2.0);
        ShadowVariable[2]=StateVector[2]+FunctionVector[2]*(TimeStep/2.0);
        ShadowVariable[3]=StateVector[3]+FunctionVector[3]*(TimeStep/2.0);
		
        StepVector1[0]=ShadowVariable[1];
        StepVector1[1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector1[2]=ShadowVariable[3];
        StepVector1[3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+StepVector1[0]*(TimeStep/2.0);
        ShadowVariable[1]=StateVector[1]+StepVector1[1]*(TimeStep/2.0);
        ShadowVariable[2]=StateVector[2]+StepVector1[2]*(TimeStep/2.0);
        ShadowVariable[3]=StateVector[3]+StepVector1[3]*(TimeStep/2.0);
		
        StepVector2[0]=ShadowVariable[1];
        StepVector2[1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector2[2]=ShadowVariable[3];
        StepVector2[3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+StepVector2[0]*(TimeStep/1.0);
        ShadowVariable[1]=StateVector[1]+StepVector2[1]*(TimeStep/1.0);
        ShadowVariable[2]=StateVector[2]+StepVector2[2]*(TimeStep/1.0);
        ShadowVariable[3]=StateVector[3]+StepVector2[3]*(TimeStep/1.0);
		
        StepVector3[0]=ShadowVariable[1];
        StepVector3[1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector3[2]=ShadowVariable[3];
        StepVector3[3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here using y(new)= y(old) + (TimeStep/6)*(y'(old)+2y'(inc1)+2y'(inc2)+y'(inc3)) */
        StateVector[0]=StateVector[0]+(TimeStep/6.0)*(FunctionVector[0]+2*StepVector1[0]+2*StepVector2[0]+StepVector3[0]);
        StateVector[1]=StateVector[1]+(TimeStep/6.0)*(FunctionVector[1]+2*StepVector1[1]+2*StepVector2[1]+StepVector3[1]);
        StateVector[2]=StateVector[2]+(TimeStep/6.0)*(FunctionVector[2]+2*StepVector1[2]+2*StepVector2[2]+StepVector3[2]);
        StateVector[3]=StateVector[3]+(TimeStep/6.0)*(FunctionVector[3]+2*StepVector1[3]+2*StepVector2[3]+StepVector3[3]);
        
        /*Printing only selected points to avoid saturation*/
		if(i%50==0)
			fprintf(RangeKuttaGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
        
		/*Bringing FunctionVector to new state*/
        FunctionVector[0]=StateVector[1];
        FunctionVector[1]=2*CoriolisCoefficient*StateVector[3]-pow(Frequency,2.0)*StateVector[0];
        FunctionVector[2]=StateVector[3];
        FunctionVector[3]=(-2)*CoriolisCoefficient*StateVector[1]-pow(Frequency,2.0)*StateVector[2];

    }
	/*Enabling run-time viewing of graph*/
    fprintf(RangeKuttaGNUPLOT, "\e");
    fflush(RangeKuttaGNUPLOT);
}

/*This function graphs the equations by RK45 method*/
void RK45Fehlberg(double CoriolisCoefficient, double Frequency, double Vx, double Vy, double TimeStep, int Iterations)
{
    /*State Vector stores x-coordinate, x-velocity, y-coordinate, y-velocity - this is the state*/
	/*Function Vector stores the differentiated elements of State Vector*/
	/*Shadow Vector locates the state at an incremented time to improve accuracy of approximation*/
	/*Step Vectors find differentiated Shadow Vector matrices- or incremented Function Vectors*/
	double StateVector[4]={0.0,Vx,0.0,Vy};
    double FunctionVector[4]={Vx, 2*CoriolisCoefficient*Vy, Vy, (-2)*CoriolisCoefficient*Vx};
    double StepVector[5][4], ShadowVariable[4];
    
	/*Creating GNUPLOT file and preparing to plot*/
	FILE *RangeKuttaGNUPLOT=popen("gnuplot -persistence","w");
    fprintf(RangeKuttaGNUPLOT,"set title 'RK45-Fehlberg Method'\nset xlabel 'X-coordinate\nset ylabel 'Y-coordinate'\n");
    fprintf(RangeKuttaGNUPLOT,"plot '-' w l\n");
    fprintf(RangeKuttaGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
    
	/*This loop increments state step by step*/
    for(int i=0; i<Iterations; i++)
    {
		/*In this loop, all the coefficients used were developed by Cash and Karp*/
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+FunctionVector[0]*(TimeStep*(0.2));
        ShadowVariable[1]=StateVector[1]+FunctionVector[1]*(TimeStep*(0.2));
        ShadowVariable[2]=StateVector[2]+FunctionVector[2]*(TimeStep*(0.2));
        ShadowVariable[3]=StateVector[3]+FunctionVector[3]*(TimeStep*(0.2));
		
        StepVector[0][0]=ShadowVariable[1];
        StepVector[0][1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[0][2]=ShadowVariable[3];
        StepVector[0][3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+StepVector[0][0]*(TimeStep*(9.0/40.0))+FunctionVector[0]*(TimeStep*(3.0/40.0));
        ShadowVariable[1]=StateVector[1]+StepVector[0][1]*(TimeStep*(9.0/40.0))+FunctionVector[1]*(TimeStep*(3.0/40.0));
        ShadowVariable[2]=StateVector[2]+StepVector[0][2]*(TimeStep*(9.0/40.0))+FunctionVector[2]*(TimeStep*(3.0/40.0));
        ShadowVariable[3]=StateVector[3]+StepVector[0][3]*(TimeStep*(9.0/40.0))+FunctionVector[3]*(TimeStep*(3.0/40.0));
        StepVector[1][0]=ShadowVariable[1];
		
        StepVector[1][1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[1][2]=ShadowVariable[3];
        StepVector[1][3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
		/*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/
        ShadowVariable[0]=StateVector[0]+StepVector[1][0]*(TimeStep*(1.2))-StepVector[0][0]*(TimeStep*(0.9))+FunctionVector[0]*(TimeStep*(0.3));
        ShadowVariable[1]=StateVector[1]+StepVector[1][1]*(TimeStep*(1.2))-StepVector[0][1]*(TimeStep*(0.9))+FunctionVector[1]*(TimeStep*(0.3));
        ShadowVariable[2]=StateVector[2]+StepVector[1][2]*(TimeStep*(1.2))-StepVector[0][2]*(TimeStep*(0.9))+FunctionVector[2]*(TimeStep*(0.3));
        ShadowVariable[3]=StateVector[3]+StepVector[1][3]*(TimeStep*(1.2))-StepVector[0][3]*(TimeStep*(0.9))+FunctionVector[3]*(TimeStep*(0.3));
        
		StepVector[2][0]=ShadowVariable[1];
        StepVector[2][1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[2][2]=ShadowVariable[3];
        StepVector[2][3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
       /*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/ ShadowVariable[0]=StateVector[0]+StepVector[2][0]*(TimeStep*(35.0/27.0))-StepVector[1][0]*(TimeStep*(70.0/27.0))+StepVector[0][0]*(TimeStep*(2.5))-FunctionVector[0]*(TimeStep*(11.0/54.0));
        ShadowVariable[1]=StateVector[1]+StepVector[2][1]*(TimeStep*(35.0/27.0))-StepVector[1][1]*(TimeStep*(70.0/27.0))+StepVector[0][1]*(TimeStep*(2.5))-FunctionVector[1]*(TimeStep*(11.0/54.0));
        ShadowVariable[2]=StateVector[2]+StepVector[2][2]*(TimeStep*(35.0/27.0))-StepVector[1][2]*(TimeStep*(70.0/27.0))+StepVector[0][2]*(TimeStep*(2.5))-FunctionVector[2]*(TimeStep*(11.0/54.0));
        ShadowVariable[3]=StateVector[3]+StepVector[2][3]*(TimeStep*(35.0/27.0))-StepVector[1][3]*(TimeStep*(70.0/27.0))+StepVector[0][3]*(TimeStep*(2.5))-FunctionVector[3]*(TimeStep*(11.0/54.0));
		
        StepVector[3][0]=ShadowVariable[1];
        StepVector[3][1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[3][2]=ShadowVariable[3];
        StepVector[3][3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
       /*Here Shadow Variable finds state at an incremented time and Step Vectors find incremented Function Vectors*/ ShadowVariable[0]=StateVector[0]+StepVector[3][0]*(TimeStep*(253.0/4096.0))+StepVector[2][0]*(TimeStep*(44275.0/110592.0))+StepVector[1][0]*(TimeStep*(575.0/13824.0))+StepVector[0][0]*(TimeStep*(175.0/512.0))+FunctionVector[0]*(TimeStep*(1631.0/55296.0));
        ShadowVariable[1]=StateVector[1]+StepVector[3][1]*(TimeStep*(253.0/4096.0))+StepVector[2][1]*(TimeStep*(44275.0/110592.0))+StepVector[1][1]*(TimeStep*(575.0/13824.0))+StepVector[0][1]*(TimeStep*(175.0/512.0))+FunctionVector[1]*(TimeStep*(1631.0/55296.0));
        ShadowVariable[2]=StateVector[2]+StepVector[3][2]*(TimeStep*(253.0/4096.0))+StepVector[2][2]*(TimeStep*(44275.0/110592.0))+StepVector[1][2]*(TimeStep*(575.0/13824.0))+StepVector[0][2]*(TimeStep*(175.0/512.0))+FunctionVector[2]*(TimeStep*(1631.0/55296.0));
        ShadowVariable[3]=StateVector[3]+StepVector[3][3]*(TimeStep*(253.0/4096.0))+StepVector[2][3]*(TimeStep*(44275.0/110592.0))+StepVector[1][3]*(TimeStep*(575.0/13824.0))+StepVector[0][3]*(TimeStep*(175.0/512.0))+FunctionVector[3]*(TimeStep*(1631.0/55296.0));
        
		StepVector[4][0]=ShadowVariable[1];
        StepVector[4][1]=2*CoriolisCoefficient*ShadowVariable[3]-pow(Frequency,2.0)*ShadowVariable[0];
        StepVector[4][2]=ShadowVariable[3];
        StepVector[4][3]=(-2)*CoriolisCoefficient*ShadowVariable[1]-pow(Frequency,2.0)*ShadowVariable[2];
        
       /*These coefficients were developed by Cash and Karp and are used for finding new state*/
		StateVector[0]=StateVector[0]+(TimeStep)*(FunctionVector[0]*(2825.0/27648.0)+StepVector[1][0]*(18575.0/48384.0)+StepVector[2][0]*(13525.0/55296.0)+StepVector[3][0]*(277.0/14336.0)+StepVector[4][0]*(0.25));
        StateVector[1]=StateVector[1]+(TimeStep)*(FunctionVector[1]*(2825.0/27648.0)+StepVector[1][1]*(18575.0/48384.0)+StepVector[2][1]*(13525.0/55296.0)+StepVector[3][1]*(277.0/14336.0)+StepVector[4][1]*(0.25));
        StateVector[2]=StateVector[2]+(TimeStep)*(FunctionVector[2]*(2825.0/27648.0)+StepVector[1][2]*(18575.0/48384.0)+StepVector[2][2]*(13525.0/55296.0)+StepVector[3][2]*(277.0/14336.0)+StepVector[4][2]*(0.25));
        StateVector[3]=StateVector[3]+(TimeStep)*(FunctionVector[3]*(2825.0/27648.0)+StepVector[1][3]*(18575.0/48384.0)+StepVector[2][3]*(13525.0/55296.0)+StepVector[3][3]*(277.0/14336.0)+StepVector[4][3]*(0.25));
        
		/*Printing selected points to avoid saturation*/
        if(i%50==0)
			fprintf(RangeKuttaGNUPLOT,"%f %f\n",StateVector[0], StateVector[2]);
		
		/*Bringing FunctionVector to new state*/
		FunctionVector[0]=StateVector[1];
		FunctionVector[1]=2*CoriolisCoefficient*StateVector[3]-pow(Frequency,2.0)*StateVector[0];
		FunctionVector[2]=StateVector[3];
		FunctionVector[3]=(-2)*CoriolisCoefficient*StateVector[1]-pow(Frequency,2.0)*StateVector[2];

	}
	/*Enabling run-time viewing of graph*/
	fprintf(RangeKuttaGNUPLOT, "\e");
	fflush(RangeKuttaGNUPLOT);
}
