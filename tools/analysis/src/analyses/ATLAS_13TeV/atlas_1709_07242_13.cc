#include "atlas_1709_07242_13.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1709_07242_13::initialize() {
  setAnalysisName("atlas_1709_07242_13");          
  setInformation(""
    "# atlas 1709.07242 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("lhb;lhnb;hhb;hhnb");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1709_07242_13::analyze() {
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
	electronsTight = filterPhaseSpace( electronsTight , 15 , -2.47 , 2.47 , 1 );
	muonsCombined = filterPhaseSpace( muonsCombined , 7 , -2.5 , 2.5 );
	jets = filterPhaseSpace( jets , 20 , -2.5 , 2.5 , 1 );
	vector<Jet*> tauHadl , tauHadh;
	for ( int i = 0 ; i < jets.size() ; ++ i )
		if ( abs( jets[i]->Charge ) == 1 && checkTauTag( jets[i] , "medium" ) )
		{
			if ( jets[i]->PT > 25 )	tauHadl.push_back( jets[i] );
			if ( jets[i]->PT > 65 )	tauHadh.push_back( jets[i] );
		}
	jets = overlapRemoval( jets , tauHadl , 0.2 );
	jets = overlapRemoval( jets , electronsTight , 0.4 );
	jets = overlapRemoval( jets , muonsCombined , 0.4 );
	tauHadl = overlapRemoval( tauHadl , electronsTight , 0.2 );
	tauHadh = overlapRemoval( tauHadh , electronsTight , 0.2 );
	tauHadl = overlapRemoval( tauHadl , muonsCombined , 0.2 );
	tauHadh = overlapRemoval( tauHadh , muonsCombined, 0.2 );
	electronsTight = overlapRemoval( electronsTight , muonsCombined , 0.2 );
	if ( ! tauHadl.size() )	return;
	int selLep = 0;
	for ( int i = 0 ; i < electronsTight.size() ; ++ i )
		if ( electronsTight[i]->PT > 30 )
			selLep ++;
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( muonsCombined[i]->PT > 30 )
			selLep ++;
	if ( selLep > 1 )	return;
	bool bFlag = 0;
	for ( int i = 0 ; i < jets.size() ; ++ i )
		if ( checkBTag( jets[i] ) )
			bFlag = 1;
	if (selLep)
	{
		for ( int i = 0 ; i < tauHadl.size() ; ++ i )
			if ( abs( tauHadl[i]->Eta ) < 2.3 )
			{
				if ( electronsTight.size() &&\
					tauHadl[i]->Charge + electronsTight[0]->Charge == 0 )
				{
					TLorentzVector pTl = electronsTight[0]->P4();
					if ( pTl.DeltaPhi( tauHadl[i]->P4() ) <= 2.4 )	return;
					if ( mT( pTl , missingET->P4() ) >= 40 )	return;
					if (bFlag)
						countSignalEvent( "lhb" );
					else
						countSignalEvent( "lhnb" );
				}
				if ( muonsCombined.size() &&\
					tauHadl[i]->Charge + muonsCombined[0]->Charge == 0 &&\
					abs( muonsCombined[0]->Eta ) < 2.4 )
				{
					TLorentzVector pTl = muonsCombined[0]->P4();
					if ( pTl.DeltaPhi( tauHadl[i]->P4() ) <= 2.4 )	return;
					if ( mT( pTl , missingET->P4() ) >= 40 )	return;
					if (bFlag)
						countSignalEvent( "lhb" );
					else
						countSignalEvent( "lhnb" );
				}
			}
	} else
	{
		
		if ( tauHadh.size() < 2 )	return;
		if ( tauHadh[0]->Charge + tauHadh[1]->Charge )	return;
		if ( tauHadh[0]->P4().DeltaPhi( tauHadh[1]->P4() ) <= 2.7 )	return;
		if (bFlag)
			countSignalEvent( "hhb" );
		else
			countSignalEvent( "hhnb" );
	}
}

void Atlas_1709_07242_13::finalize() {
  // Whatever should be done after the run goes here
}       
