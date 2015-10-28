#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "omp.h"
using namespace std;


int const NUM_SECTORS = 16;
int const NUM_SPEEDBINS = 7;
static int wr[NUM_SECTORS][NUM_SPEEDBINS]={0};

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




void displayArray()
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

}


vector<MesoData> readMesoData(const char* argv)
{
	vector<MesoData> vecMesoData;  

	//reading from input file    
	ifstream  data(argv);

	if(data)
	{	
	 
	 string line,record;
	   
		while(getline(data,line,data.widen('\n')))
	  	{
			string row[5];
			stringstream ss;
			ss.str(line);
			int columncount=0;
			
			// reading data from each row		
			while(getline(ss,record,','))
				row[columncount++]=record;
			
			// Storing data in object 
			MesoData mesoData;
			istringstream (row[1])>>mesoData.wSpd;
			istringstream (row[2])>>mesoData.wDirn;
			
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


void aggregateData(vector<MesoData> vecMesoData)
{

	for(MesoData m: vecMesoData)
	{
		int s=calcSpeedBin(m.wSpd);
		int d=calcDirectBin(m.wDirn);
		
		if(s<NUM_SECTORS && d<NUM_SPEEDBINS)
			wr[d][s]++;
	}
 	
}


int main(int argc, char* argv[])
{
   //Reading Arguments from cmd
	char* files[argc-1];
	for(int j=0;j<argc-1;j++)
 		files[j]=argv[j+1];

 	clock_t start=clock();
	
	//Vector Approach
	
	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
 	{
 		vector<MesoData> vecMesoData=readMesoData(files[j]);
	
		#pragma omp critical
		{ aggregateData(vecMesoData);}	
 	}
 	
 	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	displayArray();

	  
}
