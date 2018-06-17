#include "cms_1711_00431.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1711_00431::initialize() {
  setAnalysisName("cms_1711_00431");          
  setInformation(""
    "# cms 1711.00431 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("mPT100;mPT125;mPT150;mPT175;mPT200;mPT250;mPT300;mPT350;mPT400;mPT500");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1711_00431::analyze() {
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
// code by yyFish ##############################################################
	electronsTight = filterIsolation( electronsTight , 0 );
	muonsCombined = filterIsolation( muonsCombined , 0 );
	electronsTight = filterPhaseSpace( electronsTight , 20 , -2.5 , 2.5 );
	muonsCombined = filterPhaseSpace( muonsCombined , 20 , -2.4 , 2.4 );
	if ( muonsCombined.size() + electronsTight.size() != 2 )	return;
	TLorentzVector dilepton;
	if ( muonsCombined.size() == 1 )
		return;
	else if ( electronsTight.size() == 2 )
	{
		if ( electronsTight[0]->PT <= 25 )	return;
		if ( electronsTight[0]->Charge + electronsTight[1]->Charge )
			return;
		if ( electronsTight[0]->P4().DeltaR( electronsTight[1]->P4() )\
			>= 1.8 )
			return;
		dilepton = electronsTight[0]->P4() + electronsTight[1]->P4();
	} else
	{
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )
			return;
		if ( muonsCombined[0]->P4().DeltaR( muonsCombined[1]->P4() )\
			>= 1.8 )
			return;
		dilepton = muonsCombined[0]->P4() + muonsCombined[1]->P4();
	}
	if ( abs( dilepton.M() - 91.1876 ) >= 15 )	return;
	if ( dilepton.Pt() <= 60 )	return;
	jets = filterPhaseSpace( jets , 18 , -2.4 , 2.4 );
	for ( int i = 0 ; i < jets.size() ; ++ i )
	{
		if ( checkTauTag( jets[i] , "loose" ) )	return;
//		if ( jets[i]->PT > 20 && checkBTag( jets[i] ) )	return;
	}
	jets = filterPhaseSpace( jets , 30 , -2.4 , 2.4 );
	if ( jets.size() > 1 )	return;
	if ( jets.size() &&\
		abs( jets[0]->P4().DeltaPhi( missingET->P4() ) ) <= 0.5 )
		return;	
	if ( missingET->PT <= 100 )	return;
	if ( abs( 1 - missingET->PT / dilepton.Pt() ) >= 0.4 )	return;
	if ( abs( dilepton.DeltaPhi( missingET->P4() ) ) <= 2.6 )	return;
	if ( missingET->PT < 125 )
		countSignalEvent( "mPT100" );
	else if ( missingET->PT < 150 )
		countSignalEvent( "mPT125" );
	else if ( missingET->PT < 175 )
		countSignalEvent( "mPT150" );
	else if ( missingET->PT < 200 )
		countSignalEvent( "mPT175" );
	else if ( missingET->PT < 250 )
		countSignalEvent( "mPT200" );
	else if ( missingET->PT < 300 )
		countSignalEvent( "mPT250" );
	else if ( missingET->PT < 350 )
		countSignalEvent( "mPT300" );
	else if ( missingET->PT < 400 )
		countSignalEvent( "mPT350" );
	else if ( missingET->PT < 500 )
		countSignalEvent( "mPT400" );
	else
		countSignalEvent( "mPT500" );
//##############################################################################
}

void Cms_1711_00431::finalize() {
  // Whatever should be done after the run goes here
}       
