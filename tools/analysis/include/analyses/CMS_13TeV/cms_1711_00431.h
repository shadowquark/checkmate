#ifndef CMS_1711_00431_H_
#define CMS_1711_00431_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1711_00431 : public AnalysisBase {
  public:
    Cms_1711_00431() : AnalysisBase()  {}               
    ~Cms_1711_00431() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
