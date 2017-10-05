#ifndef CMS_1507_03268_8_H_
#define CMS_1507_03268_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1507_03268_8 : public AnalysisBase {
  public:
    Cms_1507_03268_8() : AnalysisBase()  {}               
    ~Cms_1507_03268_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
