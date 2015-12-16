#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <stdlib.h>     
#include <time.h>

int R_update(int current,int behind,int infront,int t,int i){
	int val=0;
	if (current==0){	
		if ((behind==1 && infront==0) || (behind==1 && infront==1)){
			val=1;
		}
	}
	if (current==1){
		if ((behind==0 && infront==1) || (behind==1 && infront==1)){
			val=1;
		}
	}
	return val;
};

float crude_vel(int before,int after){
	float velocity=0.0;
	if (before==1 && after==0){velocity=1.0;}
	return velocity;
};

int main(){
	const int scale=100000;
	const int steps=100;
	const float prop=0.52;	
	float ave_velocity=0.0;
	float cars=0.0;	

	std::vector<int> space (scale,0);   	
	std::vector<int> space_next (scale,0);
	
	srand(time(NULL)); 

	for(int i=0;i<scale;i++){
		double random_num = ((double) rand() / (RAND_MAX));
		if (random_num<prop){
			space[i]=1;
			cars++;
		}
	}
	
	std::ofstream myfile;
  	myfile.open ("raw_output.txt");
	
	for (int i=0;i<scale;i++){
		myfile << space[i] << "\t";
	}
	myfile << std::endl;
	
	for(int t=0;t<steps;t++){
		for(int i=0;i<scale;i++){
			if(i!=0 && i!=scale-1){
				if (space[i]==1){
					if(space[i+1]==0){
						space_next[i+1]==1;
					}
				}
				space_next[i]=R_update(space[i],space[i-1],space[i+1],t,i);
			}
			else{
				if(i==0){
					if(space[scale-1]==1){
						space_next[i]=1;
					}	
					else{
						space_next[i]=0;
					}
				}
				if(i==scale-1){
					if(space[0]==1){
						space_next[i]=1;
					}
					else{
						space_next[i]=0;
					}
				}
			}
			myfile << space_next[i] << "\t";	
		}		
		myfile << std::endl;
		
		for(int j=0;j<scale;j++){
			ave_velocity+=crude_vel(space[j],space_next[j])/(static_cast<float>(steps)*cars);
			space[j]=space_next[j];
		}
	}	
	
  	myfile.close();
	std::cout << "Average velocity: " << ave_velocity << "\n"; 
	
	return 0;
}
