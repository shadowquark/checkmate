#include "cms_1606_01522_8.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1606_01522_8::initialize() {
  setAnalysisName("cms_1606_01522_8");          
  setInformation(""
    "# cms 1606.01522 8TeV 19.4/fb\n"
  "");
  setLuminosity(19.4*units::INVFB);      
  bookSignalRegions("0;15;45;85;125;165");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1606_01522_8::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

	missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
	electronsTight = filterPhaseSpace( electronsTight , 10 , -2.5 , 2.5 );
	muonsCombined = filterPhaseSpace( muonsCombined , 10 , -2.4 , 2.4 );
	if ( electronsTight.size() != 1 || muonsCombined.size() != 1 || electronsTight[0]->Charge != - muonsCombined[0]->Charge )	return;
	Electron *ee = electronsTight[0];
	Muon *mm = muonsCombined[0];
	if ( ee->PT <= 20 && mm->PT <= 20 )	return;
	TLorentzVector dilep = ee->P4() + mm->P4();
	if ( dilep.M() <= 12 )	return;
	if ( dilep.Pt() <= 30 )	return;
	TLorentzVector dilepT( dilep.Px() , dilep.Py() , 0 , dilep.E() );
	if ( mT( dilepT , missingET->P4() ) < 60 )	return;
	double pEtA , pEtC , nEt;
	pEtA = nEt = missingET->PT;
	bool flagE , flagM;
	flagE = flagM = 0;
	if ( abs( missingET->Phi - ee->Phi ) < M_PI / 2 )	flagE = 1;
	if ( abs( missingET->Phi - mm->Phi ) < M_PI / 2 )	flagM = 1;
	double dRe2 = pow( missingET->Phi - ee->Phi , 2 ) + pow( missingET->Eta - ee->Eta , 2 );
	double dRm2 = pow( missingET->Phi - mm->Phi , 2 ) + pow( missingET->Eta - mm->Eta , 2 );
	if ( flagE && dRe2 <= dRm2 )
		pEtA = missingET->P4().Pt( ee->P4().Vect() );
	if ( flagM && dRm2 <= dRe2 )
		pEtA = missingET->P4().Pt( ee->P4().Vect() );
	TLorentzVector sumCPt( 0 , 0 , 0 , 0 );
	for ( int i = 0 ; i < tracks.size() ; ++ i )
		sumCPt += tracks[i]->P4();
	pEtC = sqrt( pow( sumCPt.Px() , 2 ) + pow( sumCPt.Py() , 2 ) );
	sumCPt.SetPxPyPzE( - sumCPt.Px() , - sumCPt.Py() , 0 , pEtC );
	flagE = flagM = 0;
	if ( abs( sumCPt.Phi() - ee->Phi ) < M_PI / 2 )	flagE = 1;
	if ( abs( sumCPt.Phi() - mm->Phi ) < M_PI / 2 )	flagE = 1;
	dRe2 = pow( sumCPt.Phi() - ee->Phi , 2 ) + pow( sumCPt.Eta() - ee->Eta , 2 );
	dRm2 = pow( sumCPt.Phi() - mm->Phi , 2 ) + pow( sumCPt.Eta() - mm->Eta , 2 );
	if ( flagE && dRe2 <= dRm2 )
		pEtC = sumCPt.Pt( ee->P4().Vect() );
	if ( flagM && dRm2 <= dRe2 )
		pEtC = sumCPt.Pt( ee->P4().Vect() );
	if ( nEt <= 20 || pEtA <= 20 || pEtC <= 20 )	return;
	for ( int i = 0 ; i < jets.size() ; ++ i )
		if ( checkBTag( jets[i] ) )	return;
	TLorentzVector pTH = dilepT + missingET->P4();
	if ( pTH.P() < 15 )
	{
		countSignalEvent( "0" );
		return;
	}
	if ( pTH.P() < 45 )
	{
		countSignalEvent( "15" );
		return;
	}
	if ( pTH.P() < 85 )
	{
		countSignalEvent( "45" );
		return;
	}
	if ( pTH.P() < 125 )
	{
		countSignalEvent( "85" );
		return;
	}
	if ( pTH.P() < 165 )
	{
		countSignalEvent( "125" );
		return;
	}
	countSignalEvent( "165" );
}

void Cms_1606_01522_8::finalize() {
  // Whatever should be done after the run goes here
}       
