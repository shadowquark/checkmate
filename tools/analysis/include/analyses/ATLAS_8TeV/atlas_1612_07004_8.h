#ifndef ATLAS_1612_07004_8_H_
#define ATLAS_1612_07004_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1612_07004_8 : public AnalysisBase {
  public:
    Atlas_1612_07004_8() : AnalysisBase()  {}               
    ~Atlas_1612_07004_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
