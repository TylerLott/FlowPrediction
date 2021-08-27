#include "AmiraFile.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>

const char* FindAndJump(const char* buffer, const char* SearchString){
  const char* FoundLoc = strstr(buffer, SearchString);
  if (FoundLoc) return FoundLoc + strlen(SearchString);
  return buffer;
}


AmiraFile::AmiraFile() {
  // This constructor opens the amirafile and reads all data into memory
  //
  // Open file and check that it is found
  const char* fileName = "0000.am";
  FILE* fp = fopen(fileName, "rb");
  if (!fp) {
    printf("Could not find %s\n", fileName);
  }
  printf("Found and opened file %s\n", fileName);
  char buffer[2048];
  fread(buffer, sizeof(char), 2047, fp);
  buffer[2047] = '\0';

  // print out error if not a AmiraMesh file
  // Should probably throw an error so the function stops
  if (!strstr(buffer, "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1")) {
    printf("Not a proper AmiraMesh file.\n");
    fclose(fp);
  }

  // get dimensions of the mesh
  sscanf(FindAndJump(buffer, "define Lattice"), "%d %d %d", &xDim, &yDim, &zDim);
  printf("Found mesh dimensions -- %d %d %d\n", xDim, yDim, zDim);
  // get size of the bounds of mesh
  sscanf(FindAndJump(buffer, "BoundingBox"), "%g %g %g %g %g %g", &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
  printf("Found mesh boundaries -- %g %g %g %g %g %g\n", xmin, xmax, ymin, ymax, zmin, zmax);
  // is the mesh uniform
  const bool bIsUniform = (strstr(buffer, "CoordType \"uniform\"") != NULL);
  // get number of components for each vector

  if (strstr(buffer, "Lattice { float Data }")) {
    numComp = 1;
  } else {
    sscanf(FindAndJump(buffer, "Lattice { float["), "%d", &numComp);
  }
  printf("Found number of components -- %d\n", numComp);

  //Sanity check
  if (xDim <= 0 || yDim <= 0 || zDim <= 0
      || xmin > xmax || ymin > ymax || zmin > zmax
      || !bIsUniform || numComp <= 0)
  {
    printf("Something went wrong\n");
    fclose(fp);
  }
  
  // find the start of the actual data
  const long idxStartData = strstr(buffer, "# Data section follows") - buffer;
  if (idxStartData > 0) {
    fseek(fp, idxStartData, SEEK_SET);
    fgets(buffer, 2047, fp);
    fgets(buffer, 2047, fp);

    // determine how much to read and read it
    const size_t NumToRead = xDim * yDim * zDim * numComp;
    pData = new float[NumToRead];
    const size_t ActRead = fread((void*)pData, sizeof(float), NumToRead, fp);
    printf("num to read -- %ld\n", NumToRead);
    printf("act to read -- %ld\n", ActRead);
    if (NumToRead != ActRead) {
      delete[] pData;
      fclose(fp);
      printf("Something went wrong\n");
    }
  }
}

std::vector<float> AmiraFile::nextTimestep(){
  //initialize array of timestep data
  std::vector<float> stepData;
  printf("size of arr - in amirafile: %d\n", xDim*yDim*numComp);
  printf("xdim - in amirafile: %d\n", xDim);
  // index of timestep start 
  int k = idx * xDim * yDim * numComp;
  if(k < numComp * xDim * yDim * zDim){
    for(int i=0; i<xDim;i++){
      for(int j=0;j<yDim;j++){
        for(int c=0;c<numComp;c++){
          stepData.push_back(pData[((idx*yDim +i) * xDim + j) * numComp+c]);
        }
      }
    }
  }
  idx++;
  return stepData;
}
void AmiraFile::saveTimestep(){
  // save here
  int i = 0;
}
int AmiraFile::stepSize(){
  return xDim*yDim*numComp;
}
