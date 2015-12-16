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
	int var_steps=100;
	float var_prop=0.4;
	int max_scale=10000;

	int k=0;
	std::vector<float> ave_velocity (max_scale,0.0);
	
	#pragma omp for
	for(int var_scale=10;var_scale<max_scale;var_scale+=50){	
		int scale=var_scale;
		int steps=var_steps;
		float prop=var_prop;	
		float cars=0.0;	

		std::vector<int> space (scale,0);   	
		std::vector<int> space_next (scale,0);
		
		srand(time(NULL)); 

		#pragma omp for
		for(int i=0;i<scale;i++){
			double random_num = ((double) rand() / (RAND_MAX));
			if (random_num<prop){
				space[i]=1;
				cars++;
			}
		}
	
		for(int t=0;t<steps;t++){
			#pragma omp for 
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
			}		
			
			for(int j=0;j<scale;j++){
				ave_velocity[k]+=crude_vel(space[j],space_next[j])/(static_cast<float>(steps)*cars);
				space[j]=space_next[j];
			}
		}	
	
		std::cout << "Average velocity: " << ave_velocity[k] << "\n"; 
		k++;
	}	

	std::ofstream myfile;
	myfile.open ("raw_output.txt");
	
	for (int j=0;j<max_scale;j++){
		myfile << (static_cast<float>(j))*50.0 + 10.0 << "\t" << ave_velocity[j] << std::endl;
	}
	
	myfile.close();

	return 0;
}
