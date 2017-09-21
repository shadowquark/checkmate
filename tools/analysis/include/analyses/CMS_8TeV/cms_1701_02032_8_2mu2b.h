#ifndef CMS_1701_02032_8_2MU2B_H_
#define CMS_1701_02032_8_2MU2B_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1701_02032_8_2mu2b : public AnalysisBase {
  public:
    Cms_1701_02032_8_2mu2b() : AnalysisBase()  {}               
    ~Cms_1701_02032_8_2mu2b() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
