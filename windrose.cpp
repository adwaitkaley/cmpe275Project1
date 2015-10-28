#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "omp.h"
using namespace std;

//#define NUM_THREADS 4

int const NUM_SECTORS = 16;
int const NUM_SPEEDBINS = 7;

struct MesoData
{
 float wSpd;
 float wDirn;
};

int inline calcSpeedBin(float windSpd){

// Sector Ranges to match data values

  if(windSpd <= 0) return 0;
  else if (windSpd >0 && windSpd <=2) return 1;
  else if (windSpd >2 && windSpd <=4) return 2;
  else if (windSpd >4 && windSpd <=6) return 3;
  else if (windSpd >6 && windSpd <=8) return 4;
  else if (windSpd >8 && windSpd <=10) return 5;
  else return 6;
}

int inline calcDirectBin(float windDir)
{
  //cut into 16 sectors
   float sector=360/NUM_SECTORS;
   return windDir/sector;
}



void readMesoData(const char* argv,int wr[NUM_SECTORS][NUM_SPEEDBINS])
{

	// ifstream  datafile(argv);
	// double spd, dir;
	// char c;
	// //cout<<omp_get_thread_num();
	// for(;((datafile >> spd >> c >> dir) && (c == ','));)
	// {
	// 	int s=calcSpeedBin(spd);
	// 	int d=calcDirectBin(dir);
	//     //	#pragma omp atomic 
	//     {
	//     	wr[d][s]++;
	//     }
	// }


}

void displayArray(int wr[NUM_SECTORS][NUM_SPEEDBINS])
{
//	Display Array
	for(int i=0;i<NUM_SECTORS;i++)
	{
		for(int j=0;j<NUM_SPEEDBINS;j++)
			{
				cout<<wr[i][j]<<"\t";
			}
			cout<<endl;
	}

	// for(int i =0; i<NUM_SPEEDBINS*NUM_SECTORS;i++)
	// {
		
	// 		cout<<*wr[i]<<"\t";
	// 		if(i%NUM_SECTORS==0)
	// 			cout<<"\n";
	// }
}

/*void display3DArray(int wr[NUM_THREADS][NUM_SECTORS][NUM_SPEEDBINS])
{
	for(int k=0;k<NUM_THREADS;k++)
	{
		for(int i=0;i<NUM_SECTORS;i++)
		{
			for(int j=0;j<NUM_SPEEDBINS;j++)
				{
					cout<<wr[k][i][j]<<"\t";
				}
				cout<<endl;
		}
	}
}
*/

/*void displaySArray(int swr[16*7])
{
	//	Display Array
	for(int i=0;i<NUM_SECTORS;i++)
	{
		for(int j=0;j<NUM_SPEEDBINS;j++)
			{
				cout<<swr[i*16+j]<<"\t";
			}
			cout<<endl;
	}
}
*/

/*vector<MesoData> readMesoData(const char* argv)
{
	vector<MesoData> vecMesoData;  

	//reading from input file    
	ifstream  data(argv);

	if(data)
	{	
	 
	 string line,record;
	   
		while(getline(data,line,data.widen('\n')))
	  	{
			string row[2];
			stringstream ss;
			ss.str(line);
			int columncount=0;
			
			// reading data from each row		
			while(getline(ss,record,','))
				row[columncount++]=record;
			
			// Storing data in object 
			MesoData mesoData;
			istringstream (row[0])>>mesoData.wSpd;
			istringstream (row[1])>>mesoData.wDirn;
			
			// Storing entire Object in vector
			vecMesoData.push_back(mesoData);
			
	  	}
	}
	else
	{
		cout<<"File not Found"<<endl;
	}

	return vecMesoData;

}


void aggregateData(vector<MesoData> vecMesoData,int wr[NUM_SECTORS][NUM_SPEEDBINS])
{

	for(MesoData m: vecMesoData)
	{
		int s=calcSpeedBin(m.wSpd);
		int d=calcDirectBin(m.wDirn);
		
		if(s<NUM_SECTORS && d<NUM_SPEEDBINS)
			wr[d][s]++;
	}
 	
}
*/

int main(int argc, char* argv[])
{
   //Reading Arguments from cmd
	char* files[argc-1];
	for(int j=0;j<argc-1;j++)
 		files[j]=argv[j+1];

 	clock_t start=clock();
	

	//MultiDimensional Array Approach
	static int wr[NUM_SECTORS][NUM_SPEEDBINS]={0};
	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
 	{	 	
 		ifstream  datafile(files[j]);
		double spd, dir;
		char c;
		for(;((datafile >> spd >> c >> dir) && (c == ','));)
		{
			int s=calcSpeedBin(spd);
			int d=calcDirectBin(dir);
		    #pragma omp critical 
		    {
		    	wr[d][s]++;
		    }
		}
	}
	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	displayArray(wr);

	//3D Array Approach
	/*static int wr[NUM_THREADS][NUM_SECTORS][NUM_SPEEDBINS]={0};
	omp_set_num_threads(NUM_THREADS);

	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
 	{	 	
 		ifstream  datafile(files[j]);
		double spd, dir;
		char c;
		for(;((datafile >> spd >> c >> dir) && (c == ','));)
		{
			int s=calcSpeedBin(spd);
			int d=calcDirectBin(dir);
		    //	#pragma omp atomic 
		    {
		    	wr[omp_get_thread_num()][d][s]++;
		    }
		}
	}
	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	display3DArray(wr);*/



	/*//Single Dimensional Array Approach
	static int swr[NUM_SECTORS*NUM_SPEEDBINS]={0};
	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
	 	{	 	
	 		ifstream  datafile(files[j]);
			double spd, dir;
			char c;
			for(;((datafile >> spd >> c >> dir) && (c == ','));)
			{
				int s=calcSpeedBin(spd);
				int d=calcDirectBin(dir);
			  #pragma omp critical 
			    {
			    	swr[s*NUM_SECTORS+d]++;
			    }
			}
		}
	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	displaySArray(swr);		
	*/



	/*//Vector Approach
	static int wr[NUM_SECTORS][NUM_SPEEDBINS]={0};
	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
 	{
 		vector<MesoData> vecMesoData=readMesoData(files[j]);
		aggregateData(vecMesoData,wr);	
 	}
 	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	displayArray(wr);*/

	  
}
