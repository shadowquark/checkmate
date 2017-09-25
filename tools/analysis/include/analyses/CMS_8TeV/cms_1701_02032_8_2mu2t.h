#ifndef CMS_1701_02032_8_2MU2T_H_
#define CMS_1701_02032_8_2MU2T_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1701_02032_8_2mu2t : public AnalysisBase {
  public:
    Cms_1701_02032_8_2mu2t() : AnalysisBase()  {}               
    ~Cms_1701_02032_8_2mu2t() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
