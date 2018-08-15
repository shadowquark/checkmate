#ifndef CMS_1808_03684_H_
#define CMS_1808_03684_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1808_03684 : public AnalysisBase {
  public:
    Cms_1808_03684() : AnalysisBase()  {}               
    ~Cms_1808_03684() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
