#ifndef WriteData_h
#define WriteData_h
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "curl.h"
using namespace std;

int write_data(void* ptr, int size, int nmemb, FILE* stream);
struct MemoryStruct {
	char* memory;
	size_t size;
};
void* myrealloc(void* ptr, size_t size);
int write_data2(void* ptr, size_t size, size_t nmemb, void* data);
string getTimeinSeconds(string Time);

#endif // !WriteData_h