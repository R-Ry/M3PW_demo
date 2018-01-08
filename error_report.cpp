#include "error_report.hpp"

#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int log_number = 0;
static std::ofstream error_out;
static char filename[32];

void log_error(const char *message){
	if(log_number == 0){
		log_number = time(NULL);
		sprintf(filename, "error_log%d.txt", log_number);
	}
	if(!message){
		return;
	}
	error_out.open(filename, std::ios::app);
	
	time_t rawtime;
	struct tm* timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	error_out << "[" << asctime(timeinfo) << "] " << message << "\n\r";
	
	error_out.close();
}