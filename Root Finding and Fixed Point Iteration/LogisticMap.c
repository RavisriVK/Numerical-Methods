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

