#ifndef ATLAS_1902_05892_H_
#define ATLAS_1902_05892_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Atlas_1902_05892 : public AnalysisBase {
  public:
    Atlas_1902_05892() : AnalysisBase()  {}               
    ~Atlas_1902_05892() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
