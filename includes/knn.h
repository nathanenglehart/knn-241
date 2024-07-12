#ifndef KNN_H
#define KNN_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "eigen3/Eigen/Dense"
#include "utils.h"

/* Nathan Englehart, Xuhang Cao, Samuel Topper, Ishaq Kothari (Autumn 2021) */

double EuclideanDistance(Eigen::VectorXd a, Eigen::VectorXd b, int length);
double ManhattanDistance(Eigen::VectorXd a, Eigen::VectorXd b, int length);
double ChebyshevDistance(Eigen::VectorXd a, Eigen::VectorXd b, int length);
std::vector<double> distances(Eigen::VectorXd vector, int vector_length, Eigen::MatrixXd X, int X_size, double (*distance_function) (Eigen::VectorXd a, Eigen::VectorXd b, int length));
int plurality_class(std::vector<int> &);
std::vector<int> knn(Eigen::MatrixXd M1, int M1_size, Eigen::MatrixXd M2, int M2_size, int K, double (*distance_function) (Eigen::VectorXd a, Eigen::VectorXd b, int length));


#endif
