#ifndef CMS_1803_06292_H_
#define CMS_1803_06292_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1803_06292 : public AnalysisBase {
  public:
    Cms_1803_06292() : AnalysisBase()  {}               
    ~Cms_1803_06292() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
