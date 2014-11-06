/*
 *  $Id: DmpAlgBgoMip.h, 2014-09-28 15:45:05 DAMPE $
 *  Author(s):
 *    yong(zhzhy@mail.ustc.edu.cn) 28/09/2014
*/
#ifndef DmpAlgBgoDacor_H
#define DmpAlgBgoDacor_H

#include "DmpVAlg.h"
#include "DmpEvtBgoDac.h"
#include "DmpEvtBgoRaw.h"
#include "DmpEvtHeader.h"
#include <map>
class DmpEvtHeader;
class DmpEvtBgoRaw;
class DmpEvtBgoDac;

class DmpAlgBgoDacor : public DmpVAlg{
/*
 *  DmpAlgBgoDacor
 *
 */
public:
  DmpAlgBgoDacor();
  ~DmpAlgBgoDacor();

  //void Set(const std::string &type,const std::string &value);
  // if you need to set some options for your algorithm at run time. Overload Set()
  bool Initialize();
  bool ProcessThisEvent();    // only for algorithm
  bool Finalize();
  bool GetDacPar();
  double DACcorrected(const short &gid,const double &adc);

private:
  DmpEvtHeader          *fEvtHeader;
  DmpEvtBgoRaw          *fBgoRaw;
  DmpEvtBgoDac          *fBgoDac;
  std::map<short,short>  fGidOrder; // key is global dynode ID
};

#endif
