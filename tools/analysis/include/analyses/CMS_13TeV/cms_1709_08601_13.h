#ifndef CMS_1709_08601_13_H_
#define CMS_1709_08601_13_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1709_08601_13 : public AnalysisBase {
  public:
    Cms_1709_08601_13() : AnalysisBase()  {}               
    ~Cms_1709_08601_13() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
