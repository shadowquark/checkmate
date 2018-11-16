#ifndef CMS_1704_03366_H_
#define CMS_1704_03366_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1704_03366 : public AnalysisBase {
  public:
    Cms_1704_03366() : AnalysisBase()  {}               
    ~Cms_1704_03366() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
