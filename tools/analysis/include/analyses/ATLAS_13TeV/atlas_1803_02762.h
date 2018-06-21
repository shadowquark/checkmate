#ifndef ATLAS_1803_02762_H_
#define ATLAS_1803_02762_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1803_02762 : public AnalysisBase {
  public:
    Atlas_1803_02762() : AnalysisBase()  {}               
    ~Atlas_1803_02762() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
