#ifndef RAND_HPP
#define RAND_HPP

#include <cstdlib>

/*random number generator*/
static bool random_number_initialized = false;
static int random_seed;
static int random_number(){
	if(random_number_initialized == false){
		random_seed = ((long)(new int)) % 32667;
		srand(random_seed);
		random_number_initialized = true;
	}
	return rand();
}

#endif