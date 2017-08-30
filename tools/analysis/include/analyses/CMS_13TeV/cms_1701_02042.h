#ifndef CMS_1701_02042_H_
#define CMS_1701_02042_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1701_02042 : public AnalysisBase {
  public:
    Cms_1701_02042() : AnalysisBase()  {}               
    ~Cms_1701_02042() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
