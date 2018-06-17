#ifndef ATLAS_1712_02304_H_
#define ATLAS_1712_02304_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1712_02304 : public AnalysisBase {
  public:
    Atlas_1712_02304() : AnalysisBase()  {}               
    ~Atlas_1712_02304() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
