#ifndef ATLAS_1807_06573_H_
#define ATLAS_1807_06573_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1807_06573 : public AnalysisBase {
  public:
    Atlas_1807_06573() : AnalysisBase()  {}               
    ~Atlas_1807_06573() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
