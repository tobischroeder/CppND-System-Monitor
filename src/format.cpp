#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) 
{ 
    int seconds{0}, minutes{0}, hours{0};

    hours = time / 3600;
    minutes = (time % 3600) / 60;
    seconds = time % 60;

    char x[10];

    sprintf(x,"%02d:%02d:%02d", hours, minutes, seconds);

    string output = x;

    return output;
    
 }