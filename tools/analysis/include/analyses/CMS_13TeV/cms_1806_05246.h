#ifndef CMS_1806_05246_H_
#define CMS_1806_05246_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1806_05246 : public AnalysisBase {
  public:
    Cms_1806_05246() : AnalysisBase()  {}               
    ~Cms_1806_05246() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
