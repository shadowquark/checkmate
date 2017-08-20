#include "cms_1410_6679_mumu.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1410_6679_mumu::initialize() {
  setAnalysisName("cms_1410_6679_mumu");          
  setInformation(""
    "# cms_1410_6679_mumu_8TeV_19.7\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  bookSignalRegions("TBB;TBO;TBE;TOO;TOE;TEE;LBB;LBO;LBE;LOO;LOE;LEE");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1410_6679_mumu::analyze() {
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
	vector<Muon*> muon15 = filterPhaseSpace( muonsCombined , 15 , -2.1 , 2.1 );
	if ( muon15.size() < 2 )	return;
	bool flag24 = 0;
	for ( int i = 0 ; i < muon15.size() ; ++ i )
	{
		if ( muon15[i]->PT > 24 )
			flag24 = 1; 
		for ( int j = 0 ; j < tracks.size() ; ++ j )
			if ( tracks[j]->Particle == muon15[i]->Particle && ( tracks[j]->D0 > 2 || tracks[j]->DZ > 5 ) )
				return;
	}
	if ( !flag24 )	return;
	FILE	*fout = fopen( "~/code/checkmate/log.log" , "w" );
	if ( muon15.size() != 2 )	fprintf( fout , "#muonsERROR\n" );
	TLorentzVector dimuon = muon15[0]->P4() + muon15[1]->P4();
	int label0 , label1 , label2;
	label0 = label1 = label2 = 0;
	for ( int i = 0 ; i < muon15.size() ; ++ i )
	{
		if ( abs( muon15[i]->Eta ) < 0.8 )
		{
			label0 ++;
		} else
		{
			if ( abs( muon15[i]->Eta ) < 1.6 )
				label1 ++;
			else
				label2 ++;
		}
	}
	if ( label0 == 2 )
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TBB" );
		else
			countSignalEvent( "LBB" );
		return;
	}
	if ( label0 == 1 && label1 == 1 )	
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TBO" );
		else
			countSignalEvent( "LBO" );
		return;
	}
	if ( label0 == 1 && label2 == 1 )	
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TBE" );
		else
			countSignalEvent( "LBE" );
		return;
	}
	if ( label1 == 2 )	
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TOO" );
		else
			countSignalEvent( "LOO" );
		return;
	}
	if ( label1 == 1 && label2 == 1 )	
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TOE" );
		else
			countSignalEvent( "LOE" );
		return;
	}
	if ( label2 == 2 )	
	{
		if ( dimuon.Pt() > 10 )
			countSignalEvent( "TEE" );
		else
			countSignalEvent( "LEE" );
		return;
	}
}

void Cms_1410_6679_mumu::finalize() {
  // Whatever should be done after the run goes here
}       
