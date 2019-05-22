#include "atlas_1902_05892.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1902_05892::initialize() {
  setAnalysisName("atlas_1902_05892");          
  setInformation(""
    "# ATLAS 1902.05892 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("4mu");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1902_05892::analyze() {
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
  
	filterIsolation( muonsCombined , 0 );
	filterIsolation( muonsCombined , 1 );
	filterPhaseSpace( muonsCombined , 5 , -2.7 , 2.7 );
	if ( muonsCombined.size() < 4 )	return;
	if ( muonsCombined[0]->PT <= 20 )	return;
	if ( muonsCombined[1]->PT <= 15 )	return;
	if ( muonsCombined[2]->PT <= 10 )	return;
	int charge = 0;
	for ( auto i : muonsCombined )
		charge += i->Charge;
	if (charge)	return;
	double diffllz = 91.1876;
	TLorentzVector lep1 , lep2 , lep3 , lep4;
	if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge == 0 &&
		abs( ( muonsCombined[0]->P4() + muonsCombined[1]->P4() ).M() - 91.1876 )\
			< diffllz )
	{
		if ( muonsCombined[0]->Charge > 0 )
		{
			lep1 = muonsCombined[0]->P4();
			lep2 = muonsCombined[1]->P4();
		} else
		{
			lep1 = muonsCombined[1]->P4();
			lep2 = muonsCombined[0]->P4();
		}
		if ( muonsCombined[2]->Charge > 0 )
		{
			lep3 = muonsCombined[2]->P4();
			lep4 = muonsCombined[3]->P4();
		} else
		{
			lep3 = muonsCombined[3]->P4();
			lep4 = muonsCombined[2]->P4();
		}
		diffllz = abs( ( muonsCombined[0]->P4() + muonsCombined[1]->P4() ).M()\
				- 91.1876 );
	}
	if ( muonsCombined[0]->Charge + muonsCombined[2]->Charge == 0 &&
		abs( ( muonsCombined[0]->P4() + muonsCombined[2]->P4() ).M() - 91.1876 )\
			< diffllz )
	{
		if ( muonsCombined[0]->Charge > 0 )
		{
			lep1 = muonsCombined[0]->P4();
			lep2 = muonsCombined[2]->P4();
		} else
		{
			lep1 = muonsCombined[2]->P4();
			lep2 = muonsCombined[0]->P4();
		}
		if ( muonsCombined[1]->Charge > 0 )
		{
			lep3 = muonsCombined[1]->P4();
			lep4 = muonsCombined[3]->P4();
		} else
		{
			lep3 = muonsCombined[3]->P4();
			lep4 = muonsCombined[1]->P4();
		}
		diffllz = abs( ( muonsCombined[0]->P4() + muonsCombined[2]->P4() ).M()\
				- 91.1876 );
	}
	if ( muonsCombined[0]->Charge + muonsCombined[3]->Charge == 0 &&
		abs( ( muonsCombined[0]->P4() + muonsCombined[3]->P4() ).M() - 91.1876 )\
			< diffllz )
	{
		if ( muonsCombined[0]->Charge > 0 )
		{
			lep1 = muonsCombined[0]->P4();
			lep2 = muonsCombined[3]->P4();
		} else
		{
			lep1 = muonsCombined[3]->P4();
			lep2 = muonsCombined[0]->P4();
		}
		if ( muonsCombined[1]->Charge > 0 )
		{
			lep3 = muonsCombined[1]->P4();
			lep4 = muonsCombined[2]->P4();
		} else
		{
			lep3 = muonsCombined[2]->P4();
			lep4 = muonsCombined[1]->P4();
		}
		diffllz = abs( ( muonsCombined[0]->P4() + muonsCombined[3]->P4() ).M()\
				- 91.1876 );
	}
	if ( lep1.DeltaR(lep2) < 0.2 )	return;
	if ( lep1.DeltaR(lep3) < 0.1 )	return;
	if ( lep1.DeltaR(lep4) < 0.1 )	return;
	if ( lep2.DeltaR(lep3) < 0.1 )	return;
	if ( lep2.DeltaR(lep4) < 0.1 )	return;
	if ( lep3.DeltaR(lep4) < 0.2 )	return; 
	TLorentzVector dilep12 = lep1 + lep2;
	TLorentzVector dilep34 = lep3 + lep4;
	TLorentzVector l4 = dilep12 + dilep34;
	if ( l4.M() < 70 )	return;
	if ( l4.M() > 1200 )	return;
	if ( ( lep1 + lep4 ).M() < 5 || ( lep2 + lep3 ).M() < 5 )	return;
	if ( dilep12.M() < 50 || dilep12.M() > 106 )	return;
	double fm4l;
	if ( lep4.M() < 100 )
		fm4l = 5;
	else if ( lep4.M() < 110 )
		fm4l = 5 + 0.7 * ( lep4.M() - 100 );
	else if ( lep4.M() < 140 )
		fm4l = 12;
	else if ( lep4.M() < 190 )
		fm4l = 12 + 0.76 * ( lep4.M() - 140 );
	else
		fm4l = 50;
	if ( dilep34.M() < fm4l || dilep34.M() > 115 )	return; 
	countSignalEvent( "4mu" );
}

void Atlas_1902_05892::finalize() {
  // Whatever should be done after the run goes here
} 

