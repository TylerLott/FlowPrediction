#include "AmiraFile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
  AmiraFile f;

  for(int j=0;j<1001;j++){
    vector<float> test = f.nextTimestep();
    int testSize = f.stepSize();
    cout << "length of each timestep array: " << test.size() << endl;
    cout << "first of step: " << test[52487] << endl;
    
    ofstream outfile;
    outfile.open("processedData/" + to_string(j) +".csv");
    for (int i=0;i<524288;i++){
      if(i % 1024 == 0){ 
        outfile << "\n";
      } else {
      outfile << ",";
      }
      outfile << test[i];
    }
    outfile.close();
  }

  vector<float> test2 = f.nextTimestep();

  cout << "first of step2: " << test2[0] << endl;
  f.saveTimestep();
  return 0;
}
