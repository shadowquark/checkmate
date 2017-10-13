#ifndef ATLAS_1709_07242_13_H_
#define ATLAS_1709_07242_13_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1709_07242_13 : public AnalysisBase {
  public:
    Atlas_1709_07242_13() : AnalysisBase()  {}               
    ~Atlas_1709_07242_13() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
