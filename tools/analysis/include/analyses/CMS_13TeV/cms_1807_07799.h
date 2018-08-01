#ifndef CMS_1807_07799_H_
#define CMS_1807_07799_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1807_07799 : public AnalysisBase {
  public:
    Cms_1807_07799() : AnalysisBase()  {}               
    ~Cms_1807_07799() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
