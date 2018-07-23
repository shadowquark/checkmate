#ifndef CMS_1806_05264_H_
#define CMS_1806_05264_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1806_05264 : public AnalysisBase {
  public:
    Cms_1806_05264() : AnalysisBase()  {}               
    ~Cms_1806_05264() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
