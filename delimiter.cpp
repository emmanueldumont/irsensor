#include <iostream>
#include <stdexcept>
#include <string>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define SIZE_BUFFER 128
#define NUMBER_IRSENSOR 4
#define DIFF_TIMER 3

const char lut_ID[NUMBER_IRSENSOR] = {'2', '3', '4', '9'};


typedef struct irSensor
{
	char id;
	double lastTime;
	int occurence;

}t_irSensor;

t_irSensor setup[NUMBER_IRSENSOR];

enum Zones { CORRIDOR, ENTRY, DESK_WINDOW, SHELVES, LAST_ZONE };


time_t start;
double seconds_since_start;


// compare time and update setup's data
void compareDate(double time, char rcvID) {
	
	int cpt = 0;
	
	
	do {
		if( rcvID == setup[cpt].id) {
		
				// Too long ? new !
			if( (time-setup[cpt].lastTime) > DIFF_TIMER) {
				setup[cpt].lastTime = time;
				setup[cpt].occurence = 1;
			}else{		// Short time ? Add !
				setup[cpt].lastTime = time;
				setup[cpt].occurence += 1;
			}
		}else {  // Update the other
			if( ((time-setup[cpt].lastTime) > DIFF_TIMER) && (setup[cpt].occurence!=0) )
				setup[cpt].occurence = 0;
		}
		
		cpt++;
		
	}while(cpt < NUMBER_IRSENSOR);
		
}


// Filter data; return the filtered zone
void filter(double time, char * buffer){

	char * ptrSubstring = NULL;

	// Who is speaking ?
	ptrSubstring = strstr(buffer,"from 1.1.");
	
	if(ptrSubstring != NULL)	{
		compareDate(time, ptrSubstring[9]);
	}
	
	
	// Conclude on zone
	cout <<"\r";
	int temp = 3;
	for(int cpt = 0; cpt < NUMBER_IRSENSOR; cpt++ ){
		cout << "   ";
		cout << "IR" << setup[cpt].id << " " << (temp<setup[cpt].occurence)? "0" : "1";
	}
	std::cout.flush();
}



// Exec the function
void exec(const char* cmd) {

    char * buffer = NULL;
    
    buffer = (char *) malloc(SIZE_BUFFER * sizeof(char));
    
    if(buffer !=NULL) {
    
		  memset(buffer,0,SIZE_BUFFER);
		  
		  FILE* pipe = popen(cmd, "r");
		  if (!pipe) throw std::runtime_error("popen() failed!");
		  try {
		      while (!feof(pipe)) {
		          if (fgets(buffer, SIZE_BUFFER, pipe) != NULL){
		          		
		        		seconds_since_start = difftime( time(0), start);
		            filter(seconds_since_start, buffer);
		            
		            //std::cout << " " <<seconds_since_start << " " << buffer;
		            
		          }
		      }
		  } catch (...) {
		  		cout << "Out of order !"<< endl;
		      pclose(pipe);
		      free(buffer);
		      throw;
		  }
		  pclose(pipe);
    }
}


int main()
{
	
	start = time(0);
	
	for(int i = 0; i < NUMBER_IRSENSOR ; i++){
		
		setup[i].id = lut_ID[i];
		setup[i].lastTime = difftime( time(0), start);
		setup[i].occurence = 0;
	}
	
	exec("groupsocketlisten ip:127.0.0.1");
	
	return 0;
}
