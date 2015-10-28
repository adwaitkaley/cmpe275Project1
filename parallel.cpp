#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include "omp.h"
using namespace std;

int const NUM_SECTORS = 16;
int const NUM_SPEEDBINS = 7;
int const OMP_NUM_THREADS= stoi(getenv ("OMP_NUM_THREADS"));

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


void displayArray(int wr[][NUM_SECTORS*NUM_SPEEDBINS]){

	/*for (int t = 0; t < OMP_NUM_THREADS; t++)
	*/		for(int i=0;i<NUM_SPEEDBINS;i++)
		{
			for(int j=0;j<NUM_SECTORS;j++)
				{
					//cout<<wr[t][i][j]<<"\t";
					//wr[0][i][j]=wr[0][i][j]+wr[1][i][j];
					wr[0][NUM_SPEEDBINS*i+j]=wr[0][NUM_SPEEDBINS*i+j]+wr[1][NUM_SPEEDBINS*i+j];
				}
				//cout<<endl;
		}
	/*}*/
cout<<"-------------------------------------------------------------"<<endl;
cout<<"-------------------------------------------------------------"<<endl;
	for(int i=0;i<NUM_SPEEDBINS;i++){
		for(int j=0;j<NUM_SECTORS;j++){
				cout<<wr[0][NUM_SPEEDBINS*i+j]<<"\t";
		}
		cout<<endl;	
	}
	cout<<"-------------------------------------------------------------"<<endl;
	for(int i=0;i<NUM_SPEEDBINS;i++){
		for(int j=0;j<NUM_SECTORS;j++){
				cout<<wr[1][NUM_SPEEDBINS*i+j]<<"\t";
		}
		cout<<endl;	
	}
}

int main(int argc, char* argv[])
{
	  	
  	clock_t start=clock();
	int wr[OMP_NUM_THREADS][NUM_SECTORS * NUM_SPEEDBINS]={0};
	memset(wr,0,sizeof(int)*NUM_SECTORS * NUM_SPEEDBINS*OMP_NUM_THREADS);



	//read list of files
	char* files[argc-1];
	for(int j=0;j<argc-1;j++)
 		files[j]=argv[j+1];

// 	#pragma omp parallel
 	displayArray(wr);  

 	ifstream *data[argc-1];
	#pragma omp parallel for
 	for(int i=0;i<argc-1;i++){
 		data[i]= new ifstream(files[i]);
 	}
    #pragma omp barrier
 	
 	double spd=0.0, dir=0.0;
	char c='\0';
 	

	#pragma omp parallel for private(spd,dir,c) collapse(2)
	for (int t = 0; t < OMP_NUM_THREADS; t++){
		#pragma omp private(w[t])
			for(int j=0;j<argc-1;j++){

		 		cout<<omp_get_thread_num();
		 		//ifstream  datafile(files[j]);		
				//cout<<omp_get_thread_num();
				#pragma omp critical (file)
				{
				if(!data[j]->is_open()){
					cout<<"Error opening : "<<files[j];
				}else
				while((*data[j] >> spd >> c >> dir) && (c == ','))
				{
					int s=calcSpeedBin(spd);
					int d=calcDirectBin(dir);
				    
				    	wr[t][NUM_SECTORS*s+d]=wr[t][NUM_SECTORS*s+d]+1;  
				}
			}
			}
				
	}
	#pragma omp barrier
	
	//close all files	
	#pragma omp parallel for 	
 	for(int i=0;i<argc-1;i++){
 		data[i]->close();
 	}

	printf("\nTime taken: %fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);

  	//displayArray(wr);  
	int w[NUM_SECTORS * NUM_SPEEDBINS]={0};
	//#pragma omp parallel for collapse(2)
  		for(int i=0;i<NUM_SPEEDBINS;i++)
		{
			for(int j=0;j<NUM_SECTORS;j++)
				{
					//cout<<wr[t][i][j]<<"\t";
					//wr[0][i][j]=wr[0][i][j]+wr[1][i][j];
	//				#pragma omp critical
					w[NUM_SPEEDBINS*i+j]=
					wr[0][NUM_SPEEDBINS*i+j]+wr[1][NUM_SPEEDBINS*i+j]
					+wr[2][NUM_SPEEDBINS*i+j];+wr[3][NUM_SPEEDBINS*i+j];
					/*+wr[4][NUM_SPEEDBINS*i+j]+wr[5][NUM_SPEEDBINS*i+j]
					+wr[6][NUM_SPEEDBINS*i+j]+wr[7][NUM_SPEEDBINS*i+j];*/
				}
				//cout<<endl;
		}
	/*}*/
		#pragma omp barrier
cout<<"-------------------------------------------------------------"<<endl;
cout<<"-------------------------------------------------------------"<<endl;
	for(int i=0;i<NUM_SPEEDBINS;i++){
		for(int j=0;j<NUM_SECTORS;j++){
				cout<<w[NUM_SPEEDBINS*i+j]<<"\t";
		}
		cout<<endl;	
	}
	
}
