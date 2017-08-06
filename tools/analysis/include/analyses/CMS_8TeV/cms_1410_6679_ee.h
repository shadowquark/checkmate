#ifndef CMS_1410_6679_EE_H_
#define CMS_1410_6679_EE_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1410_6679_ee : public AnalysisBase {
  public:
    Cms_1410_6679_ee() : AnalysisBase()  {}               
    ~Cms_1410_6679_ee() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
