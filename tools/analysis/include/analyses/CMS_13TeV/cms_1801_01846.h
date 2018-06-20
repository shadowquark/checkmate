#ifndef CMS_1801_01846_H_
#define CMS_1801_01846_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1801_01846 : public AnalysisBase {
  public:
    Cms_1801_01846() : AnalysisBase()  {}               
    ~Cms_1801_01846() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
