#ifndef CMS_1709_08908_H_
#define CMS_1709_08908_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1709_08908 : public AnalysisBase {
  public:
    Cms_1709_08908() : AnalysisBase()  {}               
    ~Cms_1709_08908() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
