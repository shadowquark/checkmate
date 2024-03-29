#include "atlas_1807_06573.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1807_06573::initialize() {
  setAnalysisName("atlas_1807_06573");          
  setInformation(""
    "# atlas 1807.06573 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("etSR0;etSR300;etSR600;etSR1200;etSR2000;etSR3000;mtSR0;mtSR300;mtSR600;mtSR1200;mtSR2000;mtSR3000");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1807_06573::analyze() {
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
  
// Coded by yyFish #############################################################
	electronsMedium = filterPhaseSpace( electronsMedium , 65 , -2.47 , 2.47 , 1 );
	muonsCombined = filterPhaseSpace( muonsCombined , 65 , -2.4 , 2.4 );
	jets = filterPhaseSpace( jets , 65 , -2.5 , 2.5 , 1 );
	jets = overlapRemoval( jets , electronsMedium , 0.2 );
	jets = overlapRemoval( jets , muonsCombined , 0.2 );
	electronsMedium = overlapRemoval( electronsMedium , jets , 0.4 );
	muonsCombined = overlapRemoval( muonsCombined , jets , 0.4 );
	vector<Jet*> tjets;
	for ( auto i : jets )
		if ( checkTauTag( i , "loose" ) )
			tjets.push_back(i);
	if ( tjets.size() != 1 )	return;
	if ( electronsMedium.size() == 1 && !muonsCombined.size() )
	{
		if ( abs( tjets[0]->P4().DeltaPhi( electronsMedium[0]->P4() ) ) <= 2.7 )
			return;
		double dilepM = ( tjets[0]->P4() + electronsMedium[0]->P4() ).M();
		if ( dilepM > 3000 )
			countSignalEvent( "etSR3000" );
		else if ( dilepM > 2000 )
			countSignalEvent( "etSR2000" );
		else if ( dilepM > 1200 )
			countSignalEvent( "etSR1200" );
		else if ( dilepM > 600 )
			countSignalEvent( "etSR600" );
		else if ( dilepM > 300 )
			countSignalEvent( "etSR300" );
		else
			countSignalEvent( "etSR0" );
	}
	if ( muonsCombined.size() == 1 && !electronsMedium.size() )
	{
		if ( abs( tjets[0]->P4().DeltaPhi( muonsCombined[0]->P4() ) ) <= 2.7 )
			return;
		double dilepM = ( tjets[0]->P4() + muonsCombined[0]->P4() ).M();
		if ( dilepM > 3000 )
			countSignalEvent( "mtSR3000" );
		else if ( dilepM > 2000 )
			countSignalEvent( "mtSR2000" );
		else if ( dilepM > 1200 )
			countSignalEvent( "mtSR1200" );
		else if ( dilepM > 600 )
			countSignalEvent( "mtSR600" );
		else if ( dilepM > 300 )
			countSignalEvent( "mtSR300" );
		else
			countSignalEvent( "mtSR0" );
	}
// NOT Double Checked ##########################################################
}

void Atlas_1807_06573::finalize() {
  // Whatever should be done after the run goes here
}       
