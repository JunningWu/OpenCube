#ifndef _TIME_CLASS_H_
#define _TIME_CLASS_H_

#include <iostream>
using namespace std;

static bool operator >(const struct timespec a, const struct timespec b){
	if (a.tv_sec > b.tv_sec) {
		return true;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec) {
		return true;
	}
	return false;
}

static bool operator <(const struct timespec a, const struct timespec b){
	if (a.tv_sec < b.tv_sec) {
		return true;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) {
		return true;
	}
	return false;
}

static bool operator >=(const struct timespec a, const struct timespec b){
	if (a.tv_sec < b.tv_sec) {
		return false;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) {
		return false;
	}
	return true;
}

static bool operator <=(const struct timespec a, const struct timespec b){
	if (a.tv_sec > b.tv_sec) {
		return false;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec) {
		return false;
	}
	return true;
}

static bool operator ==(const struct timespec a, const struct timespec b){
	if (a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec) {
		return true;
	}
	return false;
}

static bool operator !=(const struct timespec a, const struct timespec b){
	if (a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec) {
		return false;
	}
	return true;
}

static struct timespec operator -(const struct timespec a, const struct timespec b){
	struct timespec c;
	if (a < b) {
		cout << "timespec: negative time" << endl;
	}
	if (a.tv_nsec < b.tv_nsec) {
		c.tv_sec = a.tv_sec - b.tv_sec - 1;
		c.tv_nsec = (a.tv_nsec + (long)1e9) - b.tv_nsec;
	}
	else {
		c.tv_sec = a.tv_sec - b.tv_sec;
		c.tv_nsec = a.tv_nsec - b.tv_nsec;
	}
	return c;
}

static struct timespec operator +(const struct timespec a, const struct timespec b){
	struct timespec c;
	c.tv_sec = a.tv_sec + b.tv_sec;
	c.tv_nsec = a.tv_nsec + b.tv_nsec;
	if( c.tv_nsec >= 1e9){
		c.tv_nsec -= (long)1e9;
		c.tv_sec ++;
	}
	return c;
}

#endif
