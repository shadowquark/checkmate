#ifndef ATLAS_1805_09299_H_
#define ATLAS_1805_09299_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1805_09299 : public AnalysisBase {
  public:
    Atlas_1805_09299() : AnalysisBase()  {}               
    ~Atlas_1805_09299() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
