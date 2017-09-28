#ifndef CMS_1306_1126_7_H_
#define CMS_1306_1126_7_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1306_1126_7 : public AnalysisBase {
  public:
    Cms_1306_1126_7() : AnalysisBase()  {}               
    ~Cms_1306_1126_7() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
