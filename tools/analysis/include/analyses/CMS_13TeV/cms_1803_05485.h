#ifndef CMS_1803_05485_H_
#define CMS_1803_05485_H_
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
#include "AnalysisBase.h"

class Cms_1803_05485 : public AnalysisBase {
  public:
    Cms_1803_05485() : AnalysisBase()  {}               
    ~Cms_1803_05485() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
