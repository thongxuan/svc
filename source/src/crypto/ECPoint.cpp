#include "EC_point.h"

EC_point::EC_point(){
	mpz_init(this->x);
	mpz_init(this->y);
	this->inf = false;
}

EC_point::EC_point(const mpz_t* x, const mpz_t* y){
	mpz_init_set(this->x, *x);
	mpz_init_set(this->y, *y);
	this->inf = false;
}

EC_point::EC_point(char* x_str, char* y_str){
	mpz_init_set_str(this->x, x_str, 10);
	mpz_init_set_str(this->y, y_str, 10);
	this->inf = false;
}

EC_point::~EC_point(){
}
