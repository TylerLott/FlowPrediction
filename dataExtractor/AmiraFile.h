// AmiraFile, contains the mesh of the flow field for each timestep of the simulation
//

#pragma once

#include <iostream>

class AmiraFile {
  // class stuff here
  public:
    //constructor with filepath input
    AmiraFile(); 
    float* nextTimestep();
    float* generatePts();
    float* generateNextPts();

  private:
    int xDim, yDim, zDim;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    int numComp;
    int idx = 0;
    float* pData;


};

