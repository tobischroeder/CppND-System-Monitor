#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) 
{ 
    long seconds{0}, minutes{0}, hours{0}, remainder{0};

    remainder = time / 60;
    seconds = time % 60;

    remainder = remainder / 60;
    minutes = remainder % 60;

    hours = remainder % 60;

    string output = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    return output;
    
 }