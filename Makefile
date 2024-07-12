TARGETS=knn-cli
CXX=g++ -std=c++14 
INC=-I./includes

all: $(TARGETS)

knn-cli: utils.o knn.o kfcv.o main.o
	$(CXX) $(INC) utils.o knn.o kfcv.o main.o -o knn-cli -lboost_iostreams -lboost_system -lboost_filesystem

kfcv.o: includes/kfcv.h kfcv.cpp
	$(CXX) $(INC) -c kfcv.cpp

knn.o: utils.o includes/knn.h knn.cpp
	$(CXX) $(INC) -c knn.cpp

utils.o: includes/utils.h utils.cpp
	$(CXX) $(INC) -c utils.cpp

clean:
	rm -rf $(TARGETS) *.o *.gch
