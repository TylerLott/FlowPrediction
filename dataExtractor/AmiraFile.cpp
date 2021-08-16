#include "AmiraFile.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


const char* FindAndJump(const char* buffer, const char* SearchString){
  const char* FoundLoc = strstr(buffer, SearchString);
  if (FoundLoc) return FoundLoc + strlen(SearchString);
  return buffer;
}


AmiraFile::AmiraFile() {
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

  const long idxStartData = strstr(buffer, "# Data section follows") - buffer;
  if (idxStartData > 0) {
    fseek(fp, idxStartData, SEEK_SET);
    fgets(buffer, 2047, fp);
    fgets(buffer, 2047, fp);

    if (pData) {
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
      for (int i = 0; i < 40; i++) {
        printf("%f ", pData[i]);
      }
      printf("%f\n", pData[10]);
      fclose(fp);
    }
  }
}

float* AmiraFile::nextTimestep(){
  return new float[90];
}
