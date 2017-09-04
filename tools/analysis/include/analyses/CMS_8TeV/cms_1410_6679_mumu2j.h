#ifndef CMS_1410_6679_MUMU2J_H_
#define CMS_1410_6679_MUMU2J_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1410_6679_mumu2j : public AnalysisBase {
  public:
    Cms_1410_6679_mumu2j() : AnalysisBase()  {}               
    ~Cms_1410_6679_mumu2j() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
