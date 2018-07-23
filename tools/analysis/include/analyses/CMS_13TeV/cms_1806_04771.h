#ifndef CMS_1806_04771_H_
#define CMS_1806_04771_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1806_04771 : public AnalysisBase {
  public:
    Cms_1806_04771() : AnalysisBase()  {}               
    ~Cms_1806_04771() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
