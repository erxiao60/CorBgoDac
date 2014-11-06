/*
 *  $Id: DmpAlgBgoMip.h, 2014-09-28 15:45:05 DAMPE $
 *  Author(s):
 *    yong(zhzhy@mail.ustc.edu.cn) 28/09/2014
*/
#include "DmpAlgBgoDacor.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "DmpDataBuffer.h"
#include "DmpBgoBase.h"
#include "TMath.h"
#include "DmpCore.h"
//-------------------------------------------------------------------
DmpAlgBgoDacor::DmpAlgBgoDacor()
 :DmpVAlg("BgoDacor"),
	fEvtHeader(0),
	fBgoRaw(0),
	fBgoDac(0)
{
        bool prepareDacPar=GetDacPar();
	if(!prepareDacPar){
	  std::cout<<"Error:Can not read DacPar!"<<std::endl;
	  }
}

//-------------------------------------------------------------------
DmpAlgBgoDacor::~DmpAlgBgoDacor(){
}

bool DmpAlgBgoDacor::GetDacPar(){
  TFile *fDacPar=new TFile("./DAC/DacPar.root");
  TTree *Dactree=(TTree*)fDacPar->Get("Calibration/Bgo");
  TBranch *b_fBgoDac;
  Dactree->SetBranchAddress("Dac",&fBgoDac,&b_fBgoDac);
  Dactree->GetEntry(0);
  //prepare parameters
short nDys=(short)fBgoDac->GlobalDynodeID.size();
   for(short i=0;i<nDys;++i){

    fGidOrder.insert(std::make_pair(fBgoDac->GlobalDynodeID[i],i));
  }
//  delete Dactree;
//  delete fDacPar;
//  delete b_fBgoDac;
  //usage: QdcCoe[fGidOrder[gid]];//Quadratic Coefficients
  //       Slope[...],Cst[...] are same.
  return true;
} 


//-------------------------------------------------------------------
bool DmpAlgBgoDacor ::Initialize(){
  //gRootIOSvc->Set("Output/FileName","./"+gRootIOSvc->GetInputFileName()+"_DACcor.root");
  // read input data
  fEvtHeader= new DmpEvtHeader();
  fBgoRaw=new DmpEvtBgoRaw();
  gDataBuffer->LinkRootFile("Event/Rdc/EventHeader",fEvtHeader);
  gDataBuffer->LinkRootFile("Event/Rdc/Bgo",fBgoRaw);
  
  fEvtHeader = dynamic_cast<DmpEvtHeader*>(gDataBuffer->ReadObject("Event/Rdc/EventHeader"));
  fBgoRaw = dynamic_cast<DmpEvtBgoRaw*>(gDataBuffer->ReadObject("Event/Rdc/Bgo"));
  
  gDataBuffer->RegisterObject("Event/Dacor/EventHeader",fEvtHeader,"DmpEvtHeader");
  gDataBuffer->RegisterObject("Event/Dacor/Bgo",fBgoRaw,"DmpEvtBgoRaw");

  return true;
}

//-------------------------------------------------------------------
bool  DmpAlgBgoDacor::ProcessThisEvent(){
  short nSignal = fBgoRaw->fGlobalDynodeID.size();
//std::cout<<"XXXXXXXXXXXXXXXX"<<nSignal<<std::endl;
  short gid = 0;
  double adc =0.;
  for (short i=0;i<nSignal;++i){
    gid=fBgoRaw->fGlobalDynodeID[i];
//std::cout<<i<<"  !!!!!!!!!!!!!!!!!!!!!!!!!!"<<DmpBgoBase::GetLayerID(gid)<<" "<<DmpBgoBase::GetBarID(gid)<<" "<<DmpBgoBase::GetSideID(gid)<<" "<<DmpBgoBase::GetDynodeID(gid)<<std::endl;
    adc=fBgoRaw->fADC[i]; 
    double cor=DACcorrected(gid,adc);
    fBgoRaw->fADC[i]=cor;   
  }
  return true;
}
//-------------------------------------------------------------------
double DmpAlgBgoDacor::DACcorrected(const short &gid,const double &adc){
  int n=(int)fGidOrder[gid];
  double a=fBgoDac->QdcCoe[n];
  double b=fBgoDac->Slope[n];
  double c=fBgoDac->Cst[n]-adc;
  double squareroot=b*b-4*a*c;
  if(squareroot<0){
    DmpLogWarning<<"Layer="<<DmpBgoBase::GetLayerID(gid)<<" Bar="<<DmpBgoBase::GetBarID(gid)<<" Side="<<DmpBgoBase::GetSideID(gid)<<" Event="<<gCore->GetCurrentEventID()<<" b*b-4*a*c="<<squareroot<<DmpLogEndl; 
    return 0.;
  }
  double x= (TMath::Sqrt(squareroot)-b)/2/a;
  double y= x*b+fBgoDac->Cst[n];
  return y;
}
//-------------------------------------------------------------------
bool DmpAlgBgoDacor::Finalize(){
//  	delete fEvtHeader;
//	delete fBgoDac;
//	delete fBgoRaw;
      	return true;
}

