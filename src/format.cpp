#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hh{0};
    int mm{0};
    int ss{0};

    hh = static_cast<int>(seconds/60.0);
    mm = static_cast<int>(seconds/(60.0*60.0));
    ss = seconds % (60*60);

    string output = StringTime(hh) + ":" + StringTime(mm) + ":" + StringTime(ss);
    return output; 
}

// INPUT: int measuring seconds, minutes or hours
// OUTPUT: string with a 2-digit formatted time unit
string Format::StringTime(int time_unit) {
    if (time_unit == 0) {
        return "00";
    } else if (time_unit < 10) {
        return "0" + std::to_string(time_unit);
    } else {
        return std::to_string(time_unit).substr(0, 2);
    }

}