#ifndef ATLAS_1808_01191_H_
#define ATLAS_1808_01191_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1808_01191 : public AnalysisBase {
  public:
    Atlas_1808_01191() : AnalysisBase()  {}               
    ~Atlas_1808_01191() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
