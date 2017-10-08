#include "cms_1507_03268_8.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1507_03268_8::initialize() {
  setAnalysisName("cms_1507_03268_8");          
  setInformation(""
    "# cms 1507.03268 8TeV 19.4/fb ll\n"
  "");
  setLuminosity(19.4*units::INVFB);      
  bookSignalRegions("0jDf;0jSf;1jDf;1jSf");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1507_03268_8::analyze() {
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
	jets = filterPhaseSpace( jets , 30 , -4.7 , 4.7 );
	if ( jets.size() > 1 )	return;
	for ( int i = 0 ; i < jets.size() ; ++ i )
		if ( checkBTag( jets[0] ) )
			return;
	electronsTight = filterPhaseSpace( electronsTight , 10 , -2.5 , 2.5 );
	muonsCombined = filterPhaseSpace( muonsCombined , 10 , -2.4 , 2.4 );
	if ( electronsTight.size() + muonsCombined.size() != 2 )	return;
	electronsTight = filterPhaseSpace( electronsTight , 20 , -2.5 , 2.5 );
	muonsCombined = filterPhaseSpace( muonsCombined , 20 , -2.4 , 2.4 );
	if ( electronsTight.size() + muonsCombined.size() != 2 )	return;
	TLorentzVector llV;
	int llP;
	if ( electronsTight.size() == 2 )
	{
		if ( electronsTight[0]->Charge == electronsTight[1]->Charge )	return;
		llV = electronsTight[0]->P4() + electronsTight[1]->P4();
		llP = 1;
	}
	if ( muonsCombined.size() == 2 )
	{
		if ( muonsCombined[0]->Charge == muonsCombined[1]->Charge )	return;
		llV = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		llP = 2;
	}
	if ( muonsCombined.size() == 1 && electronsTight.size() == 1 )
	{
		if ( electronsTight[0]->Charge == muonsCombined[0]->Charge )	return;
		llV = muonsCombined[0]->P4() + electronsTight[0]->P4();
		llP = 0;
	}
	double pEt = missingET->PT , tmpAng = M_PI / 2;
	bool tmplepFlag = 0;
	for ( int i = 0 ; i < electronsTight.size() ; ++ i )
		if ( abs( electronsTight[i]->P4().DeltaPhi( missingET->P4() ) ) < tmpAng )
		{
			tmpAng = abs( electronsTight[i]->P4().DeltaPhi( missingET->P4() ) );
			tmplepFlag = 1;
		}
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( abs( muonsCombined[i]->P4().DeltaPhi( missingET->P4() ) ) < tmpAng )
		{
			tmpAng = abs( muonsCombined[i]->P4().DeltaPhi( missingET->P4() ) );
			tmplepFlag = 1;
		}
	if (tmplepFlag)	pEt *= cos(tmpAng);
	if ( pEt <= 20 )	return;
	TLorentzVector trackET;
	pEt = 0;
	tmplepFlag = 0;
	tmpAng = M_PI / 2;
	for ( int i = 0 ; i < tracks.size() ; ++ i )
	{
		pEt += tracks[i]->PT;
		trackET += tracks[i]->P4();
	}
	trackET.SetPxPyPzE( - trackET.Px() , - trackET.Py() , - trackET.Pz() , 8000 - trackET.E() );
	for ( int i = 0 ; i < electronsTight.size() ; ++ i )
		if ( abs( electronsTight[i]->P4().DeltaPhi(trackET) ) < tmpAng )
		{
			tmpAng = abs( electronsTight[i]->P4().DeltaPhi(trackET) );
			tmplepFlag = 1;
		}
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( abs( muonsCombined[i]->P4().DeltaPhi(trackET) ) < tmpAng )
		{
			tmpAng = abs( muonsCombined[i]->P4().DeltaPhi(trackET) );
			tmplepFlag = 1;
		}
	if (tmplepFlag)	pEt *= cos(tmpAng);
	if ( pEt <= 20 )	return;
	if ( llV.M() < 12 )	return;
	if ( llV.Pt() <= 45 && llP )	return;
	if ( llV.Pt() <= 30 && !llP )	return;
	if ( abs( llV.M() - 91.1876 ) <= 15 && llP )	return;
	if ( ! jets.size() && !llP )	countSignalEvent( "0jDf" );
	if ( ! jets.size() && llP )	countSignalEvent( "0jSf" );
	if ( jets.size() && !llP )	countSignalEvent( "1jDf" );
	if ( jets.size() && llP )	countSignalEvent( "1jSf" );
}

void Cms_1507_03268_8::finalize() {
  // Whatever should be done after the run goes here
}       
