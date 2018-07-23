#ifndef CMS_1807_02048_H_
#define CMS_1807_02048_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1807_02048 : public AnalysisBase {
  public:
    Cms_1807_02048() : AnalysisBase()  {}               
    ~Cms_1807_02048() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
