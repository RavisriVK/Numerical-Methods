#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define BufferLength 10
//#define Width 6
//#define Threshold 10
//#define Time 10000

double Modulus(double Value)
{
	if(Value>=0.0)
		return Value;
	else
		return (-Value);
}

int main(int argc, char **argv)
{
	/*Taking essential inputs*/
	int FilterWidth=(int)strtod(argv[1],NULL);
	int Threshold=(int)strtod(argv[2],NULL);
	int Time=(int)strtod(argv[3],NULL);
	
	/*Opening source data file and creating GNUPLOT file to plot filtered graph*/
	FILE *DataSource=fopen("CompleteData_fsc_ssc_10um.txt", "r");
	FILE *SmoothDataSource=popen("gnuplot -persistence", "w");
	fprintf(SmoothDataSource, "plot [0:%d][0:100] '-' w l\n",Time);
	
	int CircularBuffer[BufferLength][2]={0};
	int LastPeakCount=0;
	int CurrentPeakCount=0;
	int PeakPossibility=0;
	int CurrentPeakPossibility=0;
	int CurrentPeakCountPossibility=0;
	int Start=0;
	int MovingAverage;
	int PeakCount=0;
	int Count=0;
	int j;
	double BaseAverage=0.0;
	float Temp;
	for(int i=0; i<Time/FilterWidth; i++)
	{
		MovingAverage=0;
		for(j=0; j<FilterWidth; j++)
		{
			fscanf(DataSource, "%*f %f", &Temp);
			MovingAverage+=(int)Temp;
			Count++;
		}
		BaseAverage+=MovingAverage;
		MovingAverage/=FilterWidth;
		
		CircularBuffer[Start][0]=MovingAverage;
		CircularBuffer[Start][1]=Count-(FilterWidth/2);
		
		j=Start;
		PeakPossibility=0;
		for(int k=0; k<(BufferLength-1); k++)
		{
			if(PeakPossibility<CircularBuffer[j][0])
			{
				PeakPossibility=CircularBuffer[j][0];
				CurrentPeakCount=CircularBuffer[j][1];
			}
			if(j<(BufferLength-1))
			{
				j++;
			} else {
				j=0;
			}
		}

		if(((CurrentPeakCount-CurrentPeakCountPossibility)<35)&&(PeakPossibility<CurrentPeakPossibility)&&((PeakPossibility-(BaseAverage/Count))>Threshold))
		{
			if((CurrentPeakCount-LastPeakCount)>30)
			{
				LastPeakCount=CurrentPeakCountPossibility;
				printf("%d %d\n",CurrentPeakCountPossibility-10, CurrentPeakPossibility);
				PeakCount++;
			}
		} else {
			CurrentPeakPossibility=PeakPossibility;
			CurrentPeakCountPossibility=CurrentPeakCount;
		}
		
		if(Start<(BufferLength-1))
		{
			Start++;
		} else {
			Start=0;
		}
		
		fprintf(SmoothDataSource, "%d %d\n", i*FilterWidth-(FilterWidth/2), MovingAverage);
	}
	printf("\nNo. of peaks=%d\nBase average=%.1f\nCount=%d\n",PeakCount,BaseAverage/Time,Count);
	fprintf(SmoothDataSource, "\e");
	fflush(SmoothDataSource);
}



