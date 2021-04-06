//EE1103 - Assignment 5(b) - Lotka-Volterra Model of Predator-Prey
//Done by Group 12 - Ch Sarvani (ee18b120), Abhijeet Ajithkumar (ee18b121), Akilesh Kannan (ee18b122)

//Commands to used in terminal:
/*
gcc lotkavolterra_Grp12.c -o lvg12
./lvg12 1 2 3 4 0.1 0.2
*/

//Disclaimer - the numbers used in the above command can be changed according to user preference.

#include <stdio.h>
#include <stdlib.h>

void solvelvde(double a, double b, double c, double d, double x0, double y0) //function to solve the DEs using Euler's method
{
	//for generating predator vs. prey plot
	double *x = malloc(10000 * sizeof(double));
	double *y = malloc(10000 * sizeof(double));

	//for generating time series plot of predator and prey
	double *x1 = malloc(10000 * sizeof(double));
	double *y1 = malloc(10000 * sizeof(double)); 


	*(x) = x0;
	*(y) = y0;

	*(x1) = x0;
	*(y1) = y0;

	FILE *gnuplotpvp = popen("gnuplot -persistent", "w");
	FILE *gnuplottime = popen("gnuplot -persistent", "w");
	
	FILE *lvtimedata = fopen("lvtimedata.txt", "w"); //storing the data in a file named lvtimedata.txt

	fprintf(gnuplotpvp, "set title 'predator vs. prey graph' \n set xlabel 'prey' \n set ylabel 'predator'\n ");
	fprintf(gnuplottime, "set title 'time dependence of predator and prey' \n set xlabel 'time' \n set ylabel 'predator (or) prey'\n ");

	fprintf(gnuplotpvp, "plot '-' with d ls 1 title 'predator-prey plot'\n");

	for(int j=1;j<10000;j++)
	{
		//solving the DEs with step size of 0.01 secs for plot of predator vs prey
		*(x+j) = (a*(*(x+j-1)) - b*(*(x+j-1))*(*(y+j-1)))*0.01 + *(x+j-1);
		*(y+j) = (d*(*(x+j-1))*(*(y+j-1)) - c*(*(y+j-1)))*0.01 + *(y+j-1);

		//solving the DEs with step size of 0.005 secs for time dependence plot of predator and prey
		*(x1+j) = (a*(*(x1+j-1)) - b*(*(x1+j-1))*(*(y1+j-1)))*0.005 + *(x1+j-1);
		*(y1+j) = (d*(*(x1+j-1))*(*(y1+j-1)) - c*(*(y1+j-1)))*0.005 + *(y1+j-1);

		//plotting the data on gnuplot
		fprintf(gnuplotpvp, "%lf %lf\n", *(x+j), *(y+j));
		fprintf(lvtimedata, "%d %lf %lf\n", j, *(x1+j), *(y1+j));
	}

	fprintf(gnuplotpvp, "e\n");
	fprintf(gnuplottime, "plot 'lvtimedata.txt' using 1:2 w d ls 2 title 'prey', 'lvtimedata.txt' using 1:3 w d ls 4 title 'predator'\n");

	//making the gnuplot window open without terminating the C program
	fflush(gnuplottime);
	fflush(gnuplotpvp);

	fclose(lvtimedata);

	free(x);
	free(y);
	free(x1);
	free(y1);
}

int main(int argc, char* argv[])
{
	double a,b,c,d,x0,y0; //a,b,c,d are the constants involved in the equation (α,β,ɣ,δ); x0 and y0 are the initial conditons of prey and predator respectively

	if(argc!=7)
	{
		printf("Example usage: %s 1 2 3 4 5 6 - the last two numbers indicate the initial values of number of prey and predator respectively\n", argv[0]);
		exit(0);
	}

	//assigning values to a,b,c,d,x0,y0 from input
	a = strtod(argv[1],NULL);
	b = strtod(argv[2],NULL);
	c = strtod(argv[3],NULL);
	d = strtod(argv[4],NULL);
	x0 = strtod(argv[5],NULL);
	y0 = strtod(argv[6],NULL);

	solvelvde(a,b,c,d,x0,y0); //function call

	return 0;
}
