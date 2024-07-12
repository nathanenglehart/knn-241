#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "includes/gnuplot_i.hpp" 
#include "includes/eigen3/Eigen/Dense"
#include "includes/utils.h"
#include "includes/knn.h"
#include "includes/kfcv.h"

/* Nathan Englehart, Xuhang Cao, Samuel Topper, Ishaq Kothari (Autumn 2021) */

void wait_for_key()
{
    
    /* Pauses program util keypress. */
	
	
    std::cout << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();

}

template<typename T> T load_csv(const std::string & sys_path)
{

  /* Returns csv file input as an Eigen matrix or vector. */

  std::ifstream in;
  in.open(sys_path);
  std::string line;
  std::vector<double> values;
  uint rows = 0;

  while(std::getline(in, line))
  {
      std::stringstream lineStream(line);
      std::string cell;

      while(std::getline(lineStream, cell, ','))
      {
          values.push_back(std::stod(cell));
      }

      rows = rows + 1;
  }

  return Eigen::Map<const Eigen::Matrix<typename T::Scalar, T::RowsAtCompileTime, T::ColsAtCompileTime, Eigen::RowMajor>>(values.data(), rows, values.size()/rows);

   /* based on code from https://stackoverflow.com/questions/34247057/how-to-read-csv-file-and-assign-to-eigen-matrix/39146048 */
}

void driver(std::string sys_path_test, std::string sys_path_train, double (*distance_function) (Eigen::VectorXd a, Eigen::VectorXd b, int length), bool verbose)
{

  /* Driver for a k nearest neighbors classifier. */

  Eigen::MatrixXd train = load_csv<Eigen::MatrixXd>(sys_path_train);
  Eigen::MatrixXd test = load_csv<Eigen::MatrixXd>(sys_path_test);

  if(verbose == true)
  {
      std::cout << "train data filepath: " << sys_path_train << "\n";
  }

  if(verbose == true)
  {
      std::cout << "test data filepath: " << sys_path_test << "\n\n";
  }

  // use 10 fold cross validation to find optimal K parameter

  std::vector<double> error;

  int num_folds = 10;
  
  // int num_k_values = (train.rows()/num_folds)*(num_folds-1);
  int num_k_values = 30;

  for(int i = 1; i < num_k_values; i+=2)
  {
  	if(verbose == true)
	{
  		printf("computing error for k=%d",i);
	}

	double result = kfcv(train,num_folds,&knn,i,*&distance_function);
	error.push_back(result);

	if(verbose == true)
	{
		printf(" -> %f\n", result);
  	}
  }

  double min_error = error.front();
  int optimal_param_value = 1;
  int curr_param_value = 1;

  std::vector<int> k_values;

  for(auto v : error)
  {

	k_values.push_back(curr_param_value);

	if(v < min_error)
	{
		min_error = v;
		optimal_param_value = curr_param_value;
	}

	curr_param_value += 2;
  }


  if(verbose == true)
  {
	printf("\nmin error: %f\n",min_error);
	printf("optimal k: %d\n\n",optimal_param_value);
  }

// show optimal k parameter

try
{
	Gnuplot g1("k");
	g1.set_xlabel("K");
	g1.set_ylabel("Misclassification Rate");
	//g1.set_title("error for different k values");
	g1.set_style("points pt '*'").plot_xy(k_values,error);
	//g1.cmd("set terminal png");
	//g1.cmd("set output 'figures/misclassification_rate_across_folds.png'");
	if(verbose == true)
  {

  	g1.showonscreen(); // window output
	wait_for_key();	
  }
	
	//g1.reset_plot();


} catch(GnuplotException ge)
{
	std::cout << "gnuplot error" << "\n";
	std::cout << ge.what() << std::endl;
}
  

  
 

  // run knn with optimal K parameter

  std::vector<int> predictions = knn(test, test.rows(), train, train.rows(), optimal_param_value, *&distance_function);

  if(verbose == true)
  {
    int count = 0;
    for(auto v : predictions)
    {
        std::cout << "vector " << count << " classification = " << v << "\n";
        count++;
    }
  }
}

int main(int argc, char **argv)
{

  /* Default parameters are K = 1, distance function is set to Euclidean Distance, and verbose is set to false. */

  auto distance_function = &EuclideanDistance;
  bool verbose = false;

  if(argc == 1)
  {
    std::cout << "No arguments supplied.\n";
    std::cout << "Usage: ./knn-cli [train] [test] [options ..]\n";
    std::cout << "More info with: \"./knn-cli -h\"\n";
    return 1;
  }

  int counter = 1;

  while(counter < argc)
  {
    if(argv[counter][0] == '-' && argv[counter][1] == 'h') //&& argv[counter][2] == '\0'
    {
      std::cout << "K Nearest Neighbors Cli (2021 Dec 9, compiled " << __TIMESTAMP__ << " " << __TIME__ << ")\n\n";
      std::cout << "usage: ./knn-cli [train] [test] [options ..]    read in train csv and test csv files from filesystem\n";
      std::cout << "   or: ./knn-cli -h                             displays help menu\n\n";
      std::cout << "Arguments:\n";
      std::cout << "   -h     Displays help menu\n";
      std::cout << "   -v     Displays output in verbose mode\n";
      std::cout << "   -e     Runs algorithm using the Euclidean Distance formula\n";
      std::cout << "   -m     Runs algorithm using the Manhattan Distance formula\n";
      std::cout << "   -c     Runs algorithm using the Chebyshev Distance formula\n";
      return 0;
    } else if(counter == 1 && !(valid_filepath(argv[1])))
    {
      std::cout << "Invalid filepath: " << argv[1] << "\n";
      std::cout << "Usage: ./knn-cli [train] [test] [options ..]\n";
      return 1;
    } else if(counter == 2 && !(valid_filepath(argv[1])))
    {
      std::cout << "Invalid filepath: " << argv[1] << "\n";
      std::cout << "Usage: ./knn-cli [train] [test] [options ..]\n";
      return 1;
    } else if(counter == 1 && (valid_filepath(argv[1])))
    {
      //std::cout << "Train filepath: " << argv[1] << "\n";
    } else if(counter == 2 && (valid_filepath(argv[2])))
    {
      //std::cout << "Test filepath: " << argv[2] << "\n";
    } else if(counter >= 3)
    {

      if(argv[counter][0] == '-' && argv[counter][1] == 'v' && argv[counter][2] == '\0')
      {
        verbose = true;
      } else if(argv[counter][0] == '-' && argv[counter][1] == 'e' && argv[counter][2] == '\0')
      {
        distance_function = &EuclideanDistance;
      } else if(argv[counter][0] == '-' && argv[counter][1] == 'm' && argv[counter][2] == '\0')
      {
        distance_function = &ManhattanDistance;
      } else if(argv[counter][0] == '-' && argv[counter][1] == 'c' && argv[counter][2] == '\0')
      {
        distance_function = &ChebyshevDistance;
      } else
      {
        std::cout << "Unknown option argument: " << argv[counter] << "\n";
        std::cout << "More info with: \"./knn-cli -h\"\n";
        return 1;
      }
    } else
    {
      std::cout << "Unknown option argument: " << argv[counter] << "\n";
      std::cout << "More info with: \"./knn-cli -h\"\n";
      return 1;
    }

    counter = counter + 1;
  }

  driver(argv[2],argv[1],distance_function,verbose);

  /* [Iris-virginica] => 0 [Iris-versicolor] => 1 [Iris-setosa] => 2 */

  //driver("./data/iristest.csv", "./data/iris.csv", 11, &ManhattanDistance, true);
  //driver("./data/iristest.csv", "./data/iris.csv", 11, &ChebyshevDistance, true);
  //driver("./data/iristest.csv", "./data/iris.csv", 11, &EuclideanDistance, true);

  //driver("./data/S1test.csv", "./data/S1train.csv", 131, &EuclideanDistance, true);
  //driver("./data/S2test.csv", "./data/S2train.csv", 85, &EuclideanDistance, true);


  return 0;
}
