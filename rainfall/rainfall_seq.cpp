#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

#include "simulator.hpp"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 6) {
    cout << "WRONG INPUT FORMAT!! Syntax: ./rainfall <P> <M> <A> <N> "
            "<elevation_file>"
         << endl;
    cout << "P = # of parallel threads to use" << endl;
    cout << "M = # of simulation time steps" << endl;
    cout << "A = absorption rate (specified as a floating point number)"
         << endl;
    cout << "N = dimension of the landscape" << endl;
    cout << "elevation_file = input file that specifies the elevation of each "
            "point"
         << endl;
    return EXIT_FAILURE;
  }
  // parse args from input
  const int timeSteps = atoi(argv[2]);
  const float absorbRate = stof(argv[3]);
  const int N = atoi(argv[4]);
  const string elevationFile = argv[5];

  ifstream file(elevationFile.c_str());
  if (file.fail()) {
    cout << "Cannot open: " << elevationFile << endl;
    return EXIT_FAILURE;
  }
  Simulator* simulator = new Simulator(timeSteps, absorbRate, N, file);

  simulator->simulate();
  simulator->printResult();
  delete simulator;
  return EXIT_SUCCESS;
}
