
#ifndef TIME_H
#define TIME_H

#include <iostream>

std::vector<struct tm> config_time(std::vector<std::string> d);
struct tm add_days(struct tm tm, int days);


#endif
