#ifndef CMS_1410_6679_MUMU_7_H_
#define CMS_1410_6679_MUMU_7_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1410_6679_mumu_7 : public AnalysisBase {
  public:
    Cms_1410_6679_mumu_7() : AnalysisBase()  {}               
    ~Cms_1410_6679_mumu_7() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
