#ifndef ATLAS_1808_01899_H_
#define ATLAS_1808_01899_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1808_01899 : public AnalysisBase {
  public:
    Atlas_1808_01899() : AnalysisBase()  {}               
    ~Atlas_1808_01899() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
