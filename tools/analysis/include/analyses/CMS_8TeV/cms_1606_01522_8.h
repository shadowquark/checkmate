#ifndef CMS_1606_01522_8_H_
#define CMS_1606_01522_8_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1606_01522_8 : public AnalysisBase {
  public:
    Cms_1606_01522_8() : AnalysisBase()  {}               
    ~Cms_1606_01522_8() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
