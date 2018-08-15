#include "cms_1808_03684.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1808_03684::initialize() {
  setAnalysisName("cms_1808_03684");          
  setInformation(""
    "# cms 1808.03684 13TeV 77.3/fb\n"
  "");
  setLuminosity(77.3*units::INVFB);      
  bookSignalRegions("SR1;SR2;SR3;SR4");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1808_03684::analyze() {
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
	muonsCombined = filterPhaseSpace( muonsCombined , 5 , -2.4 , 2.4 );
	if ( muonsCombined.size() < 4 )	return;
	if ( muonsCombined[0]->PT <= 20 )	return;
	if ( muonsCombined[1]->PT <= 10 )	return;
	muonsCombined = overlapRemoval( muonsCombined , 0.02 );
	double minZ1 = 13000 , maxZ2 = 0;
	TLorentzVector diZ1 , diZ2;
	Muon *m1 , *m2 , *m3 , *m4;
	for ( auto i : muonsCombined )
		for ( auto j : muonsCombined )
		{
			double diM = ( i->P4() + j->P4() ).M();
			if ( diM <= 4 || diM >= 120 )	continue;
			double tmpZ = abs( 91.1876 - diM );
			if ( i->Charge + j->Charge == 0 && tmpZ < minZ1 )
			{
				diZ1 = i->P4() + j->P4();
				minZ1 = tmpZ;
				m1 = i;
				m2 = j;
			} 
		}
	for ( auto i : muonsCombined )
		for ( auto j : muonsCombined )
		{
			if ( i != m1 && i != m2 && j != m1 && j != m2 )	continue;
			double diM = ( i->P4() + j->P4() ).M();
			if ( diM <= 4 || diM >= 120 )	continue;
			if ( i->Charge + j->Charge == 0 && i->PT + j->PT > maxZ2 )
			{
				diZ2 = i->P4() + j->P4();
				maxZ2 = i->PT + j->PT;
				m3 = i;
				m4 = j;
			}
		}	
	if ( diZ1.M() <= 12 || diZ2.M() <= 4 )	return;
	if ( m1->Charge + m3->Charge == 0 && ( m1->P4() + m3->P4() ).M() <= 4 )	return;
	if ( m1->Charge + m4->Charge == 0 && ( m1->P4() + m4->P4() ).M() <= 4 )	return;
	if ( m2->Charge + m3->Charge == 0 && ( m2->P4() + m3->P4() ).M() <= 4 )	return;
	if ( m2->Charge + m4->Charge == 0 && ( m2->P4() + m4->P4() ).M() <= 4 )	return;
	if ( ( diZ1 + diZ2 ).M() <= 80 || ( diZ1 + diZ2 ).M() >= 100 )	return;
// NOT Double Checked ##########################################################
}

void Cms_1808_03684::finalize() {
  // Whatever should be done after the run goes here
}       
