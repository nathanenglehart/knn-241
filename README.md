# K Nearest Neighbors Classifier Algorithm

## Authors

Nathan Englehart, Xuhang Cao, Samuel Topper, Ishaq Kothari (Autumn 2021)

## Usage

To run this program on any unix-based system:

```bash
git clone https://github.com/nathanenglehart/knn-cpp-241
cd knn-cpp-241
make
```

The program is meant to be run as the below, where train and test are the paths to the train and test csv files.

```bash
./knn-cli [train] [test] [options...]
```

A help menu is avaiable by running:

```bash
./knn-cli -h
```

which displays:
```
K Nearest Neighbors Cli (2021 Dec 9, compiled Fri Jul 29 18:18:59 2022 18:19:01)

usage: ./knn-cli [train] [test] [options ..]    read in train csv and test csv files from filesystem
   or: ./knn-cli -h                             displays help menu

Arguments:
   -h     Displays help menu
   -v     Displays output in verbose mode
   -e     Runs algorithm using the Euclidean Distance formula
   -m     Runs algorithm using the Manhattan Distance formula
   -c     Runs algorithm using the Chebyshev Distance formula
```

To run this program in verbose mode, please run:

```bash
./knn-cli [train] [test] -v 
```

## Prerequisites

Before installing knn-cli, you must have installed gnuplot and boost on your computer. 

**Fedora**

```bash
sudo dnf install gnuplot
```

**Arch**

```bash
sudo pacman -S boost gnuplot
```

**Ubuntu**

```bash
sudo apt-get install libboost-all-dev gnuplot
```

**Mac**

```bash
brew install boost gnuplot
```

## Installation

To install this program on Linux or Mac please run the following.

```bash
git clone https://github.com/nathanenglehart/knn-cpp-241
cd knn-cpp-241
make
sudo cp knn-cli /usr/local/bin/knn-cli
sudo chmod 0755 /usr/local/bin/knn-cli
```

Now, the program can then be run from any location on your system, as in the below.

```bash
knn-cli [train] [test] [options...]
```

## Uninstall

To uninstall this program from your system, run the following.

```bash
sudo rm /usr/local/bin/knn-cli
```

## Notes

We are including the eigen3 linear algebra library folder within this program. 

## References

Barber, David. (2016). Bayesian Reasoning and Machine Learning. Cambridge University Press.
