#ifndef ATLAS_1708_09266_H_
#define ATLAS_1708_09266_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1708_09266 : public AnalysisBase {
  public:
    Atlas_1708_09266() : AnalysisBase()  {}               
    ~Atlas_1708_09266() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
