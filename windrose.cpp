#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "omp.h"
using namespace std;

int const NUM_SECTORS = 16;
int const NUM_SPEEDBINS = 7;

//construct windrose aggregated matrix
// struct MesoData
// {
//  float wSpd;
//  float wDirn;
// };

int inline calcSpeedBin(float windSpd){
// Sector Ranges 
  if(windSpd <= 0) return 0;
  else if (windSpd >0 && windSpd <=2) return 1;
  else if (windSpd >1 && windSpd <=3) return 2;
  else if (windSpd >3 && windSpd <=5) return 3;
  else if (windSpd >5 && windSpd <=7) return 4;
  else if (windSpd >7 && windSpd <=9) return 5;
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

int main(int argc, char* argv[])
{
	static int wr[NUM_SECTORS][NUM_SPEEDBINS]={0};
	//clock_t start=clock();
	char* files[argc-1];
	for(int j=0;j<argc-1;j++)
 		files[j]=argv[j+1];

 	// for(int j=0;j<argc-1;j++)
 	// 	cout<<files[j];
 	clock_t start=clock();
	#pragma omp parallel for
	for(int j=0;j<argc-1;j++)
 	{	//printf("%s %d\n",files[j],omp_get_thread_num());
 		//readMesoData(files[j],wr);
 		ifstream  datafile(files[j]);
		double spd, dir;
		char c;
		//cout<<omp_get_thread_num();
		for(;((datafile >> spd >> c >> dir) && (c == ','));)
		//while((datafile >> spd >> c >> dir) && (c == ','))
		{
			int s=calcSpeedBin(spd);
			int d=calcDirectBin(dir);
		    //	#pragma omp atomic 
		    {
		    	wr[d][s]++;
		    }
		}
	}
	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
  	displayArray(wr);  
}
