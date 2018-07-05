#include "cms_1803_06292.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1803_06292::initialize() {
  setAnalysisName("cms_1803_06292");          
  setInformation(""
    "# cms 1803.06292 13TeV 36/fb\n"
  "");
  setLuminosity(36.0*units::INVFB);      
  bookSignalRegions("ee120;ee400;ee600;ee900;ee1300;ee1800;mm120;mm400;mm600;mm900;mm1300;mm1800");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1803_06292::analyze() {
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
  
// Coded by yyFish ############################################################
	muonsCombined = filterIsolation( muonsCombined , 0 );
	electronsTight = filterIsolation( electronsTight , 0 );
	muonsCombined = filterPhaseSpace( muonsCombined , 53 , -2.4 , 2.4 );
	electronsTight = filterPhaseSpace( electronsTight , 35 , -2.5 , 2.5 , 1 );
	if ( muonsCombined.size() < 2 && electronsTight.size() < 2 )	return;
	if ( muonsCombined.size() > 1 )
	{
		double tempPT = 0 , ml2;
		for ( auto i : muonsCombined )
			for ( auto j : muonsCombined )
			{
				if ( i->Charge + j->Charge )	continue;
				if ( ( i->P4() + j->P4() ).Pt() > tempPT )
				{
					tempPT = ( i->P4() + j->P4() ).Pt();
					ml2 = ( i->P4() + j->P4() ).M();
				}
			}
		if ( ml2 > 1800 )
			countSignalEvent( "mm1800" );
		else if ( ml2 > 1300 )
			countSignalEvent( "mm1300" );
		else if ( ml2 > 900 )
			countSignalEvent( "mm900" );
		else if ( ml2 > 600 )
			countSignalEvent( "mm600" );
		else if ( ml2 > 400 )
			countSignalEvent( "mm400" );
		else if ( ml2 > 120 )
			countSignalEvent( "mm120" );
	}
	if ( electronsTight.size() == 2 &&\
		electronsTight[0]->PT + electronsTight[1]->PT > 800 )
	{
		if ( abs( electronsTight[0]->Eta ) > 1.57 &&\
			abs( electronsTight[1]->Eta ) < 1.57 )
			return;
		double ml2 = ( electronsTight[0]->P4() + electronsTight[1]->P4() ).M();
		if ( ml2 > 1800 )
			countSignalEvent( "ee1800" );
		else if ( ml2 > 1300 )
			countSignalEvent( "ee1300" );
		else if ( ml2 > 900 )
			countSignalEvent( "ee900" );
		else if ( ml2 > 600 )
			countSignalEvent( "ee600" );
		else if ( ml2 > 400 )
			countSignalEvent( "ee400" );
		else if ( ml2 > 120 )
			countSignalEvent( "ee120" );
	}
// NOT Double Checked #########################################################
}

void Cms_1803_06292::finalize() {
  // Whatever should be done after the run goes here
}       
