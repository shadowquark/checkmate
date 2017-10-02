#ifndef ATLAS_1508_04735_8_H_
#define ATLAS_1508_04735_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1508_04735_8 : public AnalysisBase {
  public:
    Atlas_1508_04735_8() : AnalysisBase()  {}               
    ~Atlas_1508_04735_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
