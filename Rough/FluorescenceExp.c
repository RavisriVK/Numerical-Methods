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
	/*====================================
	  Preparing program by creating files,
	  variables, and initializing buffers
	 ====================================*/
	
	/*Taking essential inputs*/
	int FilterWidth=(int)strtod(argv[1],NULL);
	int Threshold=(int)strtod(argv[2],NULL);
	int Time=(int)strtod(argv[3],NULL);
	
	/*Opening source data file and creating GNUPLOT file to plot filtered graph*/
	FILE *DataSource=fopen("CompleteData_fsc_ssc_10um.txt", "r");
	FILE *SmoothDataSource=popen("gnuplot -persistence", "w");
	fprintf(SmoothDataSource, "plot [0:%d][0:100] '-' w l\n",Time);
	FILE *Top=popen("gnuplot -persistence", "w");
	fprintf(Top, "plot [0:%d][2500:2850] '-' w l\n",Time);
	
	/*This buffer stores a selection of data values and their 'Counts' or time of occurrences*/
	int CircularBuffer[BufferLength][3]={0};
	int Start=0;
	
	/*These variables were created to ensure that a single peak isn't counted twice*/
	/*These variables store the Counts of the current peak detected and the last detected peak*/
	int LastPeakCount=0;
	int CurrentPeakCount=0;
	int VerificationCount=0;
	
	/*This stores the maximum value in the buffer at a given time to check if it is a peak*/
	int PeakPossibility=0;
	int PeakVerification=0;
	
	/*This circular buffer stores the peak values, SizePeaks stores memory of buffer used, PeakStart shows newest data*/
	int Peaks[100]={0};
	int SizePeaks=0;
	int PeakStart=0;
	float PeakAverage=0;
	float PeakVariance=0;
	
	/*This circular buffer stores times between cells, SizeCellTime stores memory of buffer used, CellTimeStart shows newest data */
	int CellTime[100]={0};
	int SizeCellTime=0;
	int CellTimeStart=0;
	float CellTimeAverage=0;
	float CellTimeVariance=0;
	int PreviousCellTime=0;
	
	/*This circular buffer stores full width half maxes, SizeFWHM stores memory of buffer used, FWHMStart shows newest data */
	int FWHM[100]={0};
	int SizeFWHM=0;
	int FWHMStart=0;
	float FWHMAverage=0;
	float FWHMVariance=0;
	int x1,x2,x3;
	int y1,y2,y3;
	float Xcm, Ycm;
	

	/*Here IsMiddle checks that the peak is found in the correct location*/
	int IsMiddle=0;
	
	/*These variables are self explanatory, though we remind that Count is a time equivalent*/
	double BaseSumSideScatter=0.0;
	double BaseSumForwardScatter=0.0;
	double BaseAverageSideScatter=0.0;
	double BaseAverageForwardScatter=0.0;
	int MovingAverageSideScatter;
	int MovingAverageForwardScatter;
	int CellCount=0;
	int PeakCount=0;
	int Count=0;
	
	/*Here these variables are used to determine the width of the peak*/
	int StartWidthCheck=0;
	int HalfWidth=0;
	
	/*j is a loop counter, ResetCounter is used to determine when to reset BaseSums to 0*/
	int j;
	int ResetCounter;
	
	/*TempScatter stores the values of the columns in data, Temp is simply used for convenience*/
	float TempSideScatter;
	float TempForwardScatter;
	float Temp;
	
	/*=======================
	  Beginning Of Procceses
	 =======================*/
	
	/*This loop signals start of the process*/
	/*Note that we use Time/FilterWidth as there is another loop running #FilterWidth times*/
	for(int i=0; i<Time/FilterWidth; i++)
	{
		
		
		/*==============================================================================
		  Here we are smoothening out the data by applying a moving average filter
		  In this loop, we also keep track of the Count, and the sum of elements so far
		 ===============================================================================*/
		MovingAverageSideScatter=0;
		MovingAverageForwardScatter=0;
		
		for(j=0; j<FilterWidth; j++)
		{
			fscanf(DataSource, "%f %f",&TempForwardScatter,&TempSideScatter);
			MovingAverageSideScatter+=(int)TempSideScatter;
			MovingAverageForwardScatter+=(int)TempForwardScatter;
			Count++;
		}
		BaseSumForwardScatter+=MovingAverageForwardScatter;
		BaseSumSideScatter+=MovingAverageSideScatter;
		BaseAverageSideScatter=BaseSumSideScatter/Count;
		BaseAverageForwardScatter=BaseSumForwardScatter/Count;
		MovingAverageSideScatter/=FilterWidth;
		MovingAverageForwardScatter/=FilterWidth;
		
		/*In the circular buffer, we store the filtered data, and the corresponding Counts*/
		CircularBuffer[Start][0]=MovingAverageSideScatter;
		CircularBuffer[Start][1]=Count;
		CircularBuffer[Start][2]=MovingAverageForwardScatter;
		
		/*We initialize j as Start for the beginning of the circular buffer*/
		/*PeakPossibility is set as 0 to find the maximum in the buffer and IsMiddle is 0*/
		j=Start;
		PeakPossibility=0;
		PeakVerification=0;
		IsMiddle=0;
		
		/*==============================================================================
		 This loop travels through the buffer to find the maximum and the count there
		 ===============================================================================*/
		for(int k=0; k<(BufferLength); k++)
		{
			/*Checking for maximum*/
			if(PeakPossibility<CircularBuffer[j][0])
			{
				PeakPossibility=CircularBuffer[j][0];
				CurrentPeakCount=CircularBuffer[j][1];
				/*Checking if max was found in middle of buffer-it should give the exact peak location*/
				if(k==(BufferLength/2))
				{
					IsMiddle=1;
					x1=CircularBuffer[((j+1)<BufferLength)?(j+1):(j+1-BufferLength)][1];
					x2=CircularBuffer[((j+2)<BufferLength)?(j+2):(j+2-BufferLength)][1];
					x3=CircularBuffer[((j+3)<BufferLength)?(j+3):(j+3-BufferLength)][1];
					y1=CircularBuffer[((j+1)<BufferLength)?(j+1):(j+1-BufferLength)][0];
					y2=CircularBuffer[((j+2)<BufferLength)?(j+2):(j+2-BufferLength)][0];
					y3=CircularBuffer[((j+3)<BufferLength)?(j+3):(j+3-BufferLength)][0];
					Xcm=(x1+x2+x3)/3.0;
					Ycm=(y1+y2+y3)/3.0;
					
				} else
				{
					IsMiddle=0;
				}
			}
			
			if(PeakVerification<CircularBuffer[j][2])
			{
				PeakVerification=CircularBuffer[j][2];
				VerificationCount=CircularBuffer[j][1];
			}
			
			/*Here we are going through the Circular Buffer*/
			if(j<(BufferLength-1))
			{
				j++;
			} else {
				j=0;
			}
		}
		
		/*===================================================================================================
		 Now we are checking if maximum found is a peak and if such a peak is a cell
		 Height of peak from base should be above threshold, peaks should be spaced at least 30 count apart
		 ===================================================================================================*/
		if((PeakPossibility-(BaseSumSideScatter/(Count)))>Threshold)
		{
			if(((CurrentPeakCount-LastPeakCount)>30)&&(IsMiddle==1))
			{
				/*If a peak was found we now take it as the last known peak*/
				LastPeakCount=CurrentPeakCount;
				/*printf("\n%d %d %f\n",CurrentPeakCount-(FilterWidth/2), PeakPossibility, BaseSumSideScatter/Count); //Debugging */
				PeakCount++;
				
				/*We are adding the peak to the Peaks buffer*/
				Peaks[PeakStart]=PeakPossibility;
				if(SizePeaks<99)
					SizePeaks++;
				if(PeakStart<99)
					PeakStart++;
				else
					PeakStart=0;
				
				/*Finding the FWHM*/
				Temp=((float)BaseSumSideScatter)/Count;
				FWHM[FWHMStart]=(int)((Temp-PeakPossibility)*((CurrentPeakCount-Xcm)/(PeakPossibility-Ycm)));
				printf("%d %f %f\n",FWHM[FWHMStart],Xcm,Ycm);
				if(SizeFWHM<99)
					SizeFWHM++;
				if(FWHMStart<99)
					FWHMStart++;
				else
					FWHMStart=0;
				
				/*Checking if the peak is a cell*/
				if(Modulus((PeakVerification-(BaseSumForwardScatter/(Count))))>Threshold)
				{
					/*printf("+\n"); //Debugging*/
					
					CellCount++;
					
					/*Adding the cell time interval to the CellTime buffer*/
					CellTime[CellTimeStart]=CurrentPeakCount-PreviousCellTime;
					PreviousCellTime=CurrentPeakCount;
					if(SizeCellTime<99)
						SizeCellTime++;
					if(CellTimeStart<99)
						CellTimeStart++;
					else
						CellTimeStart=0;
					
				}
			}
		}
		
		/*========================================================================
		 Now we are preparing the buffer to overwrite oldest data with newer data
		 =========================================================================*/
		if(Start<(BufferLength-1))
		{
			Start++;
		} else {
			Start=0;
		}
		
		/*Printing the smoothened data in a graph*/
		fprintf(SmoothDataSource, "%d %d\n", i*FilterWidth+(FilterWidth/2), MovingAverageSideScatter);
		fprintf(Top, "%d %d\n", i*FilterWidth+(FilterWidth/2), MovingAverageForwardScatter);

	}
	for(int k=0; k<SizePeaks; k++)
	{
		PeakAverage+=Peaks[k];
		PeakVariance+=pow(Peaks[k],2);
	}
	PeakAverage=PeakAverage/SizePeaks;
	PeakVariance=(PeakVariance/SizePeaks)-pow(PeakAverage,2);
	
	for(int k=1; k<SizeCellTime; k++)
	{
		CellTimeAverage+=CellTime[k];
		CellTimeVariance+=pow(CellTime[k],2);
	}
	CellTimeAverage=CellTimeAverage/(SizeCellTime-1);
	CellTimeVariance=(CellTimeVariance/(SizeCellTime-1))-pow(CellTimeAverage,2);
	
	for(int k=0; k<SizeFWHM; k++)
	{
		FWHMAverage+=FWHM[k];
		FWHMVariance+=pow(FWHM[k],2);
	}
	FWHMAverage=FWHMAverage/(SizeFWHM);
	FWHMVariance=(FWHMVariance/(SizeFWHM))-pow(FWHMAverage,2);
	
	printf("\nNumber of peaks detected in side scatter=%d\nNo. of cells=%d\n",PeakCount,CellCount);
	printf("\nAverage Peak Height from X-axis=%f\nPeak Height Standard Deviation=%f\n",PeakAverage,sqrt(PeakVariance));
	printf("\nAverage Cell Time Distance=%f\nStandard Deviation of Cell Time Intervals=%f\n",CellTimeAverage,sqrt(CellTimeVariance));
	printf("\nAverage FWHM=%f\nStandard Deviation of FWHMs=%f\n",FWHMAverage,sqrt(FWHMVariance));
	
	/*Ensuring that graph can be viewed during program runtime*/
	fprintf(SmoothDataSource, "\e");
	fflush(SmoothDataSource);
	fprintf(Top, "\e");
	fflush(Top);
}