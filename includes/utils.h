#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "eigen3/Eigen/Dense"

/* Nathan Englehart, Xuhang Cao, Samuel Topper, Ishaq Kothari (Autumn 2021) */

bool is_digits(const std::string &);
bool valid_filepath(const std::string &);
template <typename T> T * to_array(std::vector<T>, int);
std::vector<int> argpartition(std::vector<double>, int);

#endif
