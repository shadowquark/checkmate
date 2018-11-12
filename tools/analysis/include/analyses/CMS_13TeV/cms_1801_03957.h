#ifndef CMS_1801_03957_H_
#define CMS_1801_03957_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1801_03957 : public AnalysisBase {
  public:
    Cms_1801_03957() : AnalysisBase()  {}               
    ~Cms_1801_03957() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
