#ifndef CMS_1410_6679_MUMU_H_
#define CMS_1410_6679_MUMU_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1410_6679_mumu : public AnalysisBase {
  public:
    Cms_1410_6679_mumu() : AnalysisBase()  {}               
    ~Cms_1410_6679_mumu() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
