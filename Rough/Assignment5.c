/**********************************************************
 *Authors: Ravisri Valluri, Snehan
 *Purpose:To generate bifurcation plot for a logistic map
 *Date:25/09/2019
 *Inputs:None
 *Outputs:Text file "BifurcationPlot.txt"
 *********************************************************/

/*Header files*/
#include<stdio.h>
#include<stdlib.h>

/*These are the values of r for which acceptor's characteristics change*/
#define BRANCH1 1.0
#define BRANCH2 2.8

/*Functions list*/
void LogisticMap(double, double, int);
void LotkaVolterra(double, double, double, double, double, double);

/*****************************************************
 *Main function
 *Command line arguments may be of form:
 *1) ./a.out (executes for set example values)
 *2) ./a.out #x_beginning #r_change #max_iterations
 ****************************************************/

int main(int argc, char **argv)
{
    FILE *points;
    points=fopen("BifurcationPlot.txt","w");
    /*r is growth rate & x depicts population*/
    double r_change;
    double x_beginning;
    int max_iterations;
    /*Checking if user wants to customize*/
    if(argc==4)
    {
        x_beginning=strtod(argv[1],NULL);
        r_change=strtod(argv[2], NULL);
        max_iterations=(int)atoi(argv[3]);
    } else {
        x_beginning=0.5;
        r_change=0.005;
        max_iterations=150;
    }
    LogisticMap(x_beginning, r_change, max_iterations);
    LotkaVolterra(0.9, 1.0, 1.1, 1.2, 2.5, 3.0);
}

/*This function generates and prints logistic map plot of population vs growth rate*/
void LogisticMap(double x_beginning, double r_change, int max_iterations)
{
    /*Piping commands to gnuplot*/
    FILE *gnuplot;
    gnuplot=popen("gnuplot -persistence", "w");
    fprintf(gnuplot, "set title 'Population vs. Growth Rate' \n set xlabel 'Growth Rate' \n set ylabel 'Population'\n ");
    FILE *points;
    points=fopen("BifurcationPlot.txt","w");
    /*r is growth rate & x depicts population*/
    double r_beginning=0.0;
    double r_final=4.0;
    double r_current;
    double x_current;
    /*10000 is a large enough number of iterations to come to steady state*/
    int steady_state=10000;
    /*From growth rate values of r from 0 to 1, attractor is always 0*/
    for(r_current=r_beginning ; r_current<=BRANCH1 ; r_current+=r_change)
    {
        fprintf(points, "%f %f\n", r_current, 0.0);
    }
    /*From growth rate of r from 1 to 2.8, there is only one attractor- 1-(1/r) */
    for(/*I simply abhor while loops*/ ; r_current<=BRANCH2 ; r_current+=r_change)
    {
        fprintf(points, "%f %f\n", r_current, 1.0-(1.0/r_current));
    }
    /*From here on out, there will defintely be multiple attractors*/
    for( ; r_current<=r_final; r_current+=r_change )
    {
        x_current=x_beginning;
        /*Coming to steady state*/
        for(int i=0; i<steady_state; i++)
        {
            x_current=r_current*x_current*(1.0-x_current);
        }
        /*We assume each point beyond steady state is an attractor*/
        for(int i=0; i<max_iterations; i++)
        {
            x_current=r_current*x_current*(1.0-x_current);
            fprintf(points, "%f %f\n", r_current, x_current);
        }
    }
    fprintf(gnuplot, "plot 'BifurcationPlot.txt' w d \n");
    fflush(gnuplot);
}

/*This plots differential equations:
 *H'=rH-aHP
 *P'=bHP-mP */
/*void LotkaVolterra(double r, double a, double b, double m, double H_initial, double P_initial)
{
   // FILE *LVdata1;
   // LVdata1=fopen("LotkaVolterraData1.txt","w");
   // FILE *LVdata2;
   // LVdata2=fopen("LotkaVolterraData2.txt","w");
    FILE *PredvsPrey;
    PredvsPrey=popen("gnuplot -persistence","w");
    FILE *PopvsTime;
    PopvsTime=popen("gnuplot -persistence","w");
    fprintf(PredvsPrey, "set title 'Predator vs Prey' \n set xlabel 'Prey' \n set ylabel 'Predator'\n ");
    fprintf(PopvsTime, "set title 'Population vs Time' \n set xlabel 'Time' \n set ylabel 'Population'\n ");
    
    fprintf(PredvsPrey, "plot '-' with d ls 1 \n");
    fprintf(PopvsTime, "plot '-' using 1:2 with d ls 1 , '-' using 1:3 with d ls 4 \n");
    int n=10000;
    double H1[n];
    double P1[n];
    double H2[n];
    double P2[n];
    H1[0]=H_initial;
    H2[0]=H_initial;
    P1[0]=P_initial;
    P2[0]=P_initial;
    for(int i=1; i<n; i++)
    {
        P1[i]=P1[i-1]+0.01*(b*H1[i-1]*P1[i-1]-m*P1[i-1]);
        H1[i]=H1[i]+0.01*(r*H1[i-1]-a*H1[i-1]*P1[i-1]);
        P2[i]=P2[i]+0.005*(b*H2[i-1]*P2[i-1]-m*P2[i-1]);
        H2[i]=H2[i]+0.005*(r*H2[i-1]-a*H2[i-1]*P2[i-1]);
        fprintf(PredvsPrey,"%f %f\n", H1[i],P1[i]);
        fprintf(PopvsTime,"%d %f %f\n", i, H2[i], P2[i]);
    }
    fprintf(PredvsPrey, "set title 'Predator vs. Prey' \n set xlabel 'Prey' \n set ylabel 'Predator' \n" );
    fprintf(pred_prey, "plot 'LotkaVolterraData1.txt' w d \n");
    fprintf(time_dep, "set title 'Population vs. Time' \nset xlabel 'Time'\nset ylabel 'Population' \n");
    fprintf(time_dep, "plot 'LotkaVolterraData2.txt' using 1:2 w d ls 2 title 'Prey', 'LotkaVolterraData.txt' using 1:3 w d ls 4 title 'Predator'\n");
    fprintf(PredvsPrey, "e\n");
    fprintf(PopvsTime, "e\n");
    fflush(PredvsPrey);
    fflush(PopvsTime);
    
} */
void LotkaVolterra(double r, double a, double b, double m, double H_initial, double P_initial) //function to solve the DEs using Euler's method
{
    int n=5000;
    //for generating predator vs. prey plot
    double H1[n];
    double P1[n];
    
    //for generating time series plot of predator and prey
    double H2[n];
    double P2[n];
    
    double time_step=0.15;
    double H1_centre;
    double P1_centre;
    double H2_centre;
    double P2_centre;
    
    H1[0]=H_initial;
    P1[0]=P_initial;
    H2[0]=H_initial;
    P2[0]=P_initial;
    
    FILE *PredvsPrey = popen("gnuplot -persistent", "w");
    FILE *PopvsTime = popen("gnuplot -persistent", "w");
    
    fprintf(PredvsPrey, "set title 'Predator vs Prey' \n set xlabel 'Prey' \n set ylabel 'Predator'\n ");
    fprintf(PopvsTime, "set title 'Population vs Time' \n set xlabel 'Time' \n set ylabel 'Population'\n ");
    
    fprintf(PredvsPrey, "plot '-' with d ls 1\n");
    fprintf(PopvsTime, "plot '-' using 1:2 with d ls 1 title 'Prey', '-' using 1:3 with d ls 4 title 'Predator' \n");
    
    for(int j=1;j<n;j++)
    {
        //solving the DEs with step size of 0.01 secs for plot of predator vs prey
        //H1[j] =H1[j-1] + time_step*(r*H1[j-1] - a*H1[j-1]*P1[j-1]);
        //P1[j] =P1[j-1] + time_step*(m*H1[j-1]*P1[j-1] - b*P1[j-1]);
        H1_centre=H1[j-1] + 0.5*time_step*(r*H1[j-1] - a*H1[j-1]*P1[j-1]);
        P1_centre=P1[j-1] + 0.5*time_step*(m*H1[j-1]*P1[j-1] - b*P1[j-1]);
        H1[j] =H1[j-1] + time_step*(r*H1_centre - a*H1_centre*P1_centre);
        P1[j] =P1[j-1] + time_step*(m*H1_centre*P1_centre - b*P1_centre);
        //solving the DEs with step size of 0.005 secs for time dependence plot of predator and prey
        //H2[j] =H2[j-1] + 0.5*time_step*(r*H2[j-1] - a*H2[j-1]*P2[j-1]);
        //P2[j] =P2[j-1] + 0.5*time_step*(m*H2[j-1]*P2[j-1] - b*P2[j-1]);
        H2_centre=H2[j-1] + 0.5*0.5*time_step*(r*H2[j-1] - a*H2[j-1]*P2[j-1]);
        P2_centre=P2[j-1] + 0.5*0.5*time_step*(m*H2[j-1]*P2[j-1] - b*P2[j-1]);
        H2[j] =H2[j-1] + 0.5*time_step*(r*H2_centre - a*H2_centre*P2_centre);
        P2[j] =P2[j-1] + 0.5*time_step*(m*H2_centre*P2_centre - b*P2_centre);
        //plotting the data on gnuplot
        fprintf(PredvsPrey, "%lf %lf\n", H1[j], P1[j]);
        fprintf(PopvsTime, "%d %lf %lf\n", j, H2[j], P2[j]);
    }
    
    fprintf(PredvsPrey, "e\n");
    fprintf(PopvsTime, "e\n");
    
    //making the gnuplot window open without terminating the C program
    fflush(PredvsPrey);
    fflush(PopvsTime);}
