/*
 * AnalysisHandlerCMS_8TeV.h
 *
 *  Created on: Feb 23, 2015
 *      Author: daschm
 */

#ifndef ANALYSISHANDLERCMS_8TEV_H_
#define ANALYSISHANDLERCMS_8TEV_H_

#include <math.h>


#include "Global.h"

#include "AnalysisHandler.h"

#include "cms_1301_4698_WW.h"
#include "cms_1303_2985.h"
#include "cms_1405_7570.h"
#include "cms_1408_3583.h"
#include "cms_1502_06031.h"
#include "cms_1504_03198.h"
#include "cms_smp_12_006.h"
#include "cms_sus_12_019.h"
#include "cms_sus_13_016.h"
#include "cms_exo_14_014.h"
#include "cms_1410_6679_mumu.h"
#include "cms_1410_6679_mumu2j.h"
#include "cms_1606_01522_8.h"
#include "cms_1506_00424_8.h"
#include "cms_1701_02032_8_4t.h"
#include "cms_1701_02032_8_2mu2b.h"
#include "cms_1701_02032_8_2mu2t.h"
//@extraheader@


class AnalysisHandlerCMS_8TeV : public AnalysisHandler {
    //! Function pointer for 2dim tagging efficiencies
    typedef double (*Eff_Fun_Ptr2)(double x,
                                   double y);

    //! Function pointer for 3dim tagging efficiencies
    typedef double (*Eff_Fun_Ptr3)(double x,
                                   double y,
                                   double z);
public:
    //! Standard Constructor
    AnalysisHandlerCMS_8TeV();

    //! Standard Destructor
    ~AnalysisHandlerCMS_8TeV();

protected:
    //! CMS specific initialisation (currently empty)
    void initialize();

    //! CMS specific finalisation (currently empty)
    void finalize();

    //! booking of CMS analyses
    void bookAnalysis(std::string analysisName,
                      Tag_Map whichTags,
                      Param_Map eventParameters);

    //! Applies CMS reconstructions on electrons, muons, photons, jets
    void postProcessParticles();

    //! links CMS specific particle vectors to CMS analyses
    void linkObjects();

private:
    //! Apply CMS photon identification efficiencies
    void identifyPhotons();
    //! Apply CMS electron identification efficiencies
    void identifyElectrons();
    //! Apply CNS muon identification efficiencies
    void identifyMuons();
    //! Apply CNS b tagging efficiencies
    void tagBJets();
    //! Apply CMS tau tagging efficiencies
    void tagTauJets();

    //! ATLAS photon identification efficiency, medium
    static double photonEffMedium(double pt,
                                  double eta);

    //! ATLAS electron reconstruction efficiency
    static double electronRecEff(double pt,
                                 double eta);

    //! ATLAS electron identification efficiency, medium
    static double electronIDEffMedium(double pt,
                                    double eta);
    //! ATLAS electron identification efficiency, tight / medium
    static double electronIDEffTightOverMedium(double pt,
                                             double eta);

    //! ATLAS muon identification efficiency, 'combined plus SA'
    static double muonEffCombPlus(double phi,
                                  double eta);
    //! ATLAS muon identification efficiency, 'combined' / 'combined plus SA'
    static double muonEffCombOverCombPlus(double phi,
                                          double eta);


    //! CMS tautagging signal efficiency, loose, 1 prong
    static double tauSigEffSingleLoose(double pt,
                                       double eta);
    //! CMS tautagging signal efficiency, medium, 1 prong
    static double tauSigEffSingleMedium(double pt,
                                        double eta);
    //! CMS tautagging signal efficiency, tight, 1 prong
    static double tauSigEffSingleTight(double pt,
                                       double eta);
    //! CMS tautagging signal efficiency, loose, >1 prong
    static double tauSigEffMultiLoose(double pt,
                                      double eta);
    //! CMS tautagging signal efficiency, medium, >1 prong
    static double tauSigEffMultiMedium(double pt,
                                       double eta);
    //! CMS tautagging signal efficiency, tight, >1 prong
    static double tauSigEffMultiTight(double pt,
                                      double eta);
    //! CMS tautagging background efficiency, loose, 1 prong
    static double tauBkgEffSingleLoose(double pt,
                                       double eta);
    //! CMS tautagging background efficiency, medium, 1 prong
    static double tauBkgEffSingleMedium(double pt,
                                        double eta);
    //! CMS tautagging background efficiency, tight, 1 prong
    static double tauBkgEffSingleTight(double pt,
                                       double eta);
    //! CMS tautagging background efficiency, loose, >1 prong
    static double tauBkgEffMultiLoose(double pt,
                                      double eta);
    //! CMS tautagging background efficiency, medium, >1 prong
    static double tauBkgEffMultiMedium(double pt,
                                       double eta);
    //! CMS tautagging background efficiency, tight, >1 prong
    static double tauBkgEffMultiTight(double pt,
                                      double eta);

    //! CMS btagging signal efficiency (eff in percent!)
    static double bSigEff(double pt,
                          double eta,
                          double eff);
    //! CMS btagging background c jet efficiency
    static double bBkgCJetEff(double pt,
                              double eta,
                              double eff);
    //! CMS btagging background light jet efficiency
    static double bBkgLJetEff(double pt,
                              double eta,
                              double eff);
    //! CMS btagging background c jet rejection
    static double bBkgCJetRej(double eff);
    //! CMS btagging background light jet efficiency
    static double bBkgLJetRej(double eff);

    //! DR(jet, track) for tracks to be associated to tau candidate
    static const double DR_TAU_TRACK;
    //! ptmin for tracks to be associated to tau candidate
    static const double PTMIN_TAU_TRACK;
    //! DR(jet, true_tau) for taus to be matched to jet candidate
    static const double DR_TAU_TRUTH;

    //! maximum eta for truth taus to be considered for matching
    static const double ETAMAX_TAU_TRUTH;
    //! ptmin for truth taus to be considered for matching
    static const double PTMIN_TAU_TRUTH;
         //! DR(jet, b) for b quarks to be associated to jet candidate
    static const double DR_B_TRUTH;
    //! maximum eta for truth b quarks to be considered for matching
    static const double ETAMAX_B_TRUTH;
    //! ptmin for truth bs to be considered for matching
    static const double PTMIN_B_TRUTH;

    //! ATLAS muon detector map, detector types
    enum muonDetector {cscsm, cscla, bee, endsm, endla, trans, feet, barov, barsm, barla};
    //! ATLAS muon detector map, type for each eta and phi position
    static const muonDetector detMap[53][21];
    //! ATLAS muon detector map, eta positions in array
    static const double etaProj[21];
    //! ATLAS muon detector map, phi positions in array
    static const double phiProj[53];

    //! list of loose reconstructed CMS electrons
    std::vector<Electron*> electronsLoose;
    //! list of medium reconstructed ATLAS electrons
    std::vector<Electron*> electronsMedium;
    //! list of tight reconstructed ATLAS electrons
    std::vector<Electron*> electronsTight;
    //! list of loose reconstructed CMS muons
    std::vector<Muon*> muonsLoose;
    //! list of 'combined' reconstructed CMS muons
    std::vector<Muon*> muonsCombined;
    //! list of 'combined + SA' reconstructed CMS muons
    std::vector<Muon*> muonsCombinedPlus;
    //! list of 'loose' reconstructed CMS photons
    std::vector<Photon*> photonsLoose;
    //! list of 'medium' reconstructed CMS photons
    std::vector<Photon*> photonsMedium;

};

#endif /* ANALYSISHANDLERCMS_8TEV_H_ */
