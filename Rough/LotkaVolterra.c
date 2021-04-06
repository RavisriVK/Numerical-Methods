/**********************************************************
 *Authors: Ravisri Valluri, Snehan
 *Purpose:To generate bifurcation plot for a logistic map
 *Date:25/09/2019
 *Inputs:None
 *Outputs:None
 *********************************************************/

/*Header files*/
#include<stdio.h>
#include<stdlib.h>

void LotkaVolterraRungaKutta(double, double, double, double, double, double);
void LotkaVolterraEuler(double, double, double, double, double, double);

/*************************************************************
 *Main function
 *Command line arguments may be of form:
 *1) ./a.out (executes for set example values by Euler method)
 *2) ./a.out <Method> #r #a #b #m #H_initial #P_initial
 *Valid options for method are:
 *1)'e' or 'E' for Euler method
 *2)'r' or 'R' for RungaKutta method
 ************************************************************/

int main(int argc, char **argv)
{
	/*Default option*/
	if(argc==1)
	{
		LotkaVolterraEuler(0.9, 1.05, 1.15, 1.2, 3.5, 4.0);
	} else if(argc==8)
	{
		/*Checking if user wants Euler or RungaKutta*/
		if(argv[1][0]=='e'||argv[1][0]=='E')
		{
			/*Input users values into function by strtod*/
			LotkaVolterraEuler(strtod(argv[2],NULL), strtod(argv[3],NULL), strtod(argv[4],NULL),strtod(argv[5],NULL),strtod(argv[6],NULL),strtod(argv[7],NULL)) ;
		} else if(argv[1][0]=='r'||argv[1][0]=='R') 
		{
			/*Input users values into function by strtod*/
			LotkaVolterraRungaKutta(strtod(argv[2],NULL), strtod(argv[3],NULL), strtod(argv[4],NULL),strtod(argv[5],NULL),strtod(argv[6],NULL),strtod(argv[7],NULL)) ;
		} else {
			printf("Invalid option.\n");
		}
	} else 
	{
		/*No option matched*/
		printf("Invalid number of arguments.\n");
	}
}

void LotkaVolterraRungaKutta(double r, double a, double b, double m, double H_initial, double P_initial)
{
    /*No of points generated*/
    int n=5000;

    /*For predator vs prey model*/
    double H1[n];
    double P1[n];
    
    /*For population vs time*/
    double H2[n];
    double P2[n];
    
    double time_step=0.15;
    
    /*Stores centre difference values of H and P*/
    double H1_centre;
    double P1_centre;
    double H2_centre;
    double P2_centre;
    
    /*Initializing*/
    H1[0]=H_initial;
    P1[0]=P_initial;
    H2[0]=H_initial;
    P2[0]=P_initial;
    
    /*We finally mastered GNUPLOT!*/
    FILE *PredvsPrey = popen("gnuplot -persistent", "w");
    FILE *PopvsTime = popen("gnuplot -persistent", "w");
    
    FILE *pt1=fopen("pt1.txt","w");
    FILE *pt2=fopen("pt2.txt","w");
    
    fprintf(PredvsPrey, "set title 'Predator vs Prey (RungaKutta)' \n set xlabel 'Prey' \n set ylabel 'Predator'\n ");
    fprintf(PopvsTime, "set title 'Population vs Time (RungaKutta)' \n set xlabel 'Time' \n set ylabel 'Population'\n ");
    
    fprintf(PredvsPrey, "plot '-' with d ls 1\n");
    //fprintf(PopvsTime, "plot '-' using 1:2 with d ls 1 title 'Prey', '-' using 1:3 with d ls 4 title 'Predator' \n");
    
    /*The iterative method begins here*/
    for(int j=1;j<n;j++)
    {

	/*This finds next step of H1 & P1 by centre difference method*/
        H1_centre=H1[j-1] + 0.5*time_step*(r*H1[j-1] - a*H1[j-1]*P1[j-1]);
        P1_centre=P1[j-1] + 0.5*time_step*(m*H1[j-1]*P1[j-1] - b*P1[j-1]);
        H1[j] =H1[j-1] + time_step*(r*H1_centre - a*H1_centre*P1_centre);
        P1[j] =P1[j-1] + time_step*(m*H1_centre*P1_centre - b*P1_centre);

	/*This finds next step of H2 & P2 by centre difference method*/
        H2_centre=H2[j-1] + 0.5*0.5*time_step*(r*H2[j-1] - a*H2[j-1]*P2[j-1]);
        P2_centre=P2[j-1] + 0.5*0.5*time_step*(m*H2[j-1]*P2[j-1] - b*P2[j-1]);
        H2[j] =H2[j-1] + 0.5*time_step*(r*H2_centre - a*H2_centre*P2_centre);
        P2[j] =P2[j-1] + 0.5*time_step*(m*H2_centre*P2_centre - b*P2_centre);

	/*Sending to gnuplot files*/
        fprintf(PredvsPrey, "%lf %lf\n", H1[j], P1[j]);
        fprintf(pt1, "%d %lf\n", j, H2[j]);
        fprintf(pt2, "%d %lf\n", j, P2[j]);
    }
    /*Starting to execute*/
    fprintf(PredvsPrey, "e\n");
    //fprintf(PopvsTime, "e\n");
    fprintf(PopvsTime, "plot 'pt1.txt' with l ls 1 title 'Prey', 'pt2.txt' with l ls 21 title 'Predator' \n");
    
	/*Ensures gnuplot runs while program runs*/
	fflush(PredvsPrey);
	fflush(PopvsTime);
}

void LotkaVolterraEuler(double r, double a, double b, double m, double H_initial, double P_initial)
{
    /*No of points generated*/
    int n=30000;
    double H1[n];
    double P1[n];
    
    /*For predator vs prey model*/
    double H2[n];
    double P2[n];
    
    double time_step=0.01;
    
    /*Initializing*/
    H1[0]=H_initial;
    P1[0]=P_initial;
    H2[0]=H_initial;
    P2[0]=P_initial;
    
    /*File pipe to gnuplot*/
    FILE *PredvsPrey = popen("gnuplot -persistent", "w");
    FILE *PopvsTime = popen("gnuplot -persistent", "w");
    FILE *pt1=fopen("pt1.txt","w");
    FILE *pt2=fopen("pt2.txt","w");
    
    /*Giving commands for each graph*/
    fprintf(PredvsPrey, "set title 'Predator vs Prey (Euler)' \n set xlabel 'Prey' \n set ylabel 'Predator'\n ");
    fprintf(PopvsTime, "set title 'Population vs Time (Euler)' \n set xlabel 'Time' \n set ylabel 'Population'\n ");
    
    fprintf(PredvsPrey, "plot '-' with l ls 1\n");
    //fprintf(PopvsTime, "plot '-' using 1:2 with d ls 1 title 'Prey', '-' using 1:3 with d ls 4 title 'Predator' \n");
    
    /*Iterative method starts*/
    for(int j=1;j<n;j++)
    {
    	/*Less acurate first derivative approximation*/
    	/*Finding points for predator vs prey*/
        H1[j] =H1[j-1] + time_step*(r*H1[j-1] - a*H1[j-1]*P1[j-1]);
        P1[j] =P1[j-1] + time_step*(m*H1[j-1]*P1[j-1] - b*P1[j-1]);
        
        /*Finding points for population vs time*/
        H2[j] =H2[j-1] + 0.5*time_step*(r*H2[j-1] - a*H2[j-1]*P2[j-1]);
        P2[j] =P2[j-1] + 0.5*time_step*(m*H2[j-1]*P2[j-1] - b*P2[j-1]);
        
        fprintf(PredvsPrey, "%lf %lf\n", H1[j], P1[j]);
        fprintf(pt1, "%d %lf\n", j, H2[j]);
        fprintf(pt2, "%d %lf\n", j, P2[j]);
    }
    /*Starting to execute*/
    fprintf(PredvsPrey, "e\n");
    //fprintf(PopvsTime, "e\n");
    fprintf(PopvsTime, "plot 'pt1.txt' with d ls 1 title 'Prey', 'pt2.txt' with d ls 21 title 'Predator' \n");
    /*Ensures gnuplot runs while program runs*/
    fflush(PredvsPrey);
    fflush(PopvsTime);
    
}

 

