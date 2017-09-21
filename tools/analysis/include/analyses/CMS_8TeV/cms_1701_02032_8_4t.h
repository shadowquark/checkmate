#ifndef CMS_1701_02032_8_4T_H_
#define CMS_1701_02032_8_4T_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1701_02032_8_4t : public AnalysisBase {
  public:
    Cms_1701_02032_8_4t() : AnalysisBase()  {}               
    ~Cms_1701_02032_8_4t() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
