// AmiraFile, contains the mesh of the flow field for each timestep of the simulation

#pragma once

#include <iostream>
#include <vector>

class AmiraFile {
  // class stuff here
  public:
    //constructor with filepath input
    AmiraFile(); 
    std::vector<float> nextTimestep();
    void saveTimestep();
    int stepSize();
    int xDim, yDim, zDim, numComp;

  private:
    float xmin, xmax, ymin, ymax, zmin, zmax;
    int idx = 0;
    float* pData;
    

};

