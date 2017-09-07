#ifndef CMS_1506_00424_8_H_
#define CMS_1506_00424_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1506_00424_8 : public AnalysisBase {
  public:
    Cms_1506_00424_8() : AnalysisBase()  {}               
    ~Cms_1506_00424_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
