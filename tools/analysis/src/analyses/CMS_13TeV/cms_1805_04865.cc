#include "cms_1805_04865.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1805_04865::initialize() {
  setAnalysisName("cms_1805_04865");          
  setInformation(""
    "# cms 1805.04865 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("et;mt;2t");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1805_04865::analyze() {
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
  // std::vector<int> triFlags; triFlags.push_back(0); triFlags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, triFlags)        Same as above, but both the first and the third condition have to be fulfilled
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
	jets = filterPhaseSpace( jets , 18.5 , -2.3 , 2.3 );
	vector<Jet*> tjets;
	for ( auto i : jets )
		if ( checkTauTag( i , "tight" ) )
			tjets.push_back(i);
	if ( !tjets.size() )	return;
	electronsLoose = filterPhaseSpace( electronsLoose , 7 , -2.5 , 2.5 );
	muonsCombined = filterPhaseSpace( muonsCombined , 5 , -2.4 , 2.4 );
	bool triFlag[3] = {};
	if ( muonsCombined.size() < 2 )	return;
	if ( muonsCombined[0]->PT <= 24 )	triFlag[0] = 1;
	if ( muonsCombined[0]->PT <= 17 || muonsCombined[1]->PT <= 8 )
		triFlag[1] = 1;
	if ( muonsCombined.size() > 2 &&\
		( muonsCombined[0]->PT <= 12 || muonsCombined[1]->PT <= 10 ) )
		triFlag[2] = 1;
	if ( muonsCombined[0]->PT <= 18 )	return;
	if ( triFlag[1] && triFlag[2] && muonsCombined[0]->PT <= 25 )	return;
	if ( muonsCombined[1]->PT <= 9 )	return;
	if ( triFlag[0] && triFlag[1] && muonsCombined[1]->PT <= 11 )	return;
	if ( triFlag[0] && triFlag[1] )	
		muonsCombined = filterPhaseSpace( muonsCombined , 6 , -2.4 , 2.4 );
	bool flag[3] = {};
	if ( muonsCombined.size() == 3 && tjets.size() == 1 )
	{
		flag[1] = 1;
		int totCharge = 0;
		for ( auto i : muonsCombined )
			totCharge += i->Charge;
		if ( abs(totCharge) - 1 )	flag[1] = 0;	
		TLorentzVector dilep = muonsCombined[0]->P4() , ditau = tjets[0]->P4();
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge == 0 )
		{
			dilep += muonsCombined[1]->P4();
			ditau += muonsCombined[2]->P4();
			if ( muonsCombined[2]->Charge + tjets[0]->Charge )
				flag[1] = 0;
		} else
		{
			dilep += muonsCombined[2]->P4();
			ditau += muonsCombined[1]->P4();
			if ( muonsCombined[1]->Charge + tjets[0]->Charge )
				flag[1] = 0;
		}
		if ( ( dilep + ditau ).M() >= 120 )	flag[1] = 0;
		if ( ditau.M() >= dilep.M() )	flag[1] = 0;
		if ( dilep.M() < 14 || dilep.M() > 64 )	flag[1] = 0;
		muonsCombined = overlapRemoval( muonsCombined , 0.3 );
		if ( muonsCombined.size() != 3 )	flag[1] = 0;
	}
	if ( muonsCombined.size() == 2 && tjets.size() == 2 )
	{
		flag[2] = 1;
		if ( tjets[0]->Charge + tjets[1]->Charge )	flag[2] = 0;
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	flag[2] = 0;
		TLorentzVector dilep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		TLorentzVector ditau = tjets[0]->P4() + tjets[1]->P4();
		if ( ( dilep + ditau ).M() >= 130 )	flag[2] = 0;
		if ( ditau.M() >= dilep.M() )	flag[2] = 0;
		if ( dilep.M() < 14 || dilep.M() > 64 )	flag[2] = 0;
		muonsCombined = overlapRemoval( muonsCombined , 0.3 );
		muonsCombined = overlapRemoval( muonsCombined , tjets , 0.4 );
		if ( muonsCombined.size() != 2 )	flag[2] = 0;
	}
	if ( muonsCombined.size() == 2 && electronsLoose.size() && tjets.size() == 1 )
	{
		flag[0] = 1;
		if ( electronsLoose[0]->Charge + tjets[0]->Charge )	flag[0] = 0;
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	flag[0] = 0;
		TLorentzVector dilep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		TLorentzVector ditau = tjets[0]->P4() + electronsLoose[0]->P4();
		if ( ( dilep + ditau ).M() >= 120 )	flag[0] = 0;
		if ( ditau.M() >= dilep.M() )	flag[0] = 0;
		if ( dilep.M() < 14 || dilep.M() > 64 )	flag[0] = 0;
		muonsCombined = overlapRemoval( muonsCombined , 0.3 );
		muonsCombined = overlapRemoval( muonsCombined , electronsLoose , 0.3 );
		muonsCombined = overlapRemoval( muonsCombined , tjets , 0.4 );
		if ( muonsCombined.size() != 2 )	flag[0] = 0;
	}
	if ( flag[0] )	countSignalEvent( "et" );
	if ( flag[1] )	countSignalEvent( "mt" );
	if ( flag[2] )	countSignalEvent( "2t" );
// NOT Double Checked #########################################################
}

void Cms_1805_04865::finalize() {
  // Whatever should be done after the run goes here
}       
