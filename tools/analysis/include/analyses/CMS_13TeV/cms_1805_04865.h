#ifndef CMS_1805_04865_H_
#define CMS_1805_04865_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1805_04865 : public AnalysisBase {
  public:
    Cms_1805_04865() : AnalysisBase()  {}               
    ~Cms_1805_04865() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
