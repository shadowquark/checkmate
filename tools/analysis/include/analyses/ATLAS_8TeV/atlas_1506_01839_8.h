#ifndef ATLAS_1506_01839_8_H_
#define ATLAS_1506_01839_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1506_01839_8 : public AnalysisBase {
  public:
    Atlas_1506_01839_8() : AnalysisBase()  {}               
    ~Atlas_1506_01839_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
