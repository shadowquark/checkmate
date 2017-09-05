#include "cms_1410_6679_mumu2j_7.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1410_6679_mumu2j_7::initialize() {
  setAnalysisName("cms_1410_6679_mumu2j_7");
  setInformation(""
    "# cms_1410.6679 mumu2j 7TeV 5/fb\n"
  "");
  setLuminosity(5.0*units::INVFB);
  bookSignalRegions("VBFT;GFT;L");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1410_6679_mumu2j_7::analyze() {
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
	for ( int i = 0 ; i < muon15.size() ; ++ i )
		for ( int j = 0 ; j < tracks.size() ; ++ j )
			if ( tracks[j]->Particle == muon15[i]->Particle && ( tracks[j]->D0 > 2 || tracks[j]->DZ > 5 ) )
				muon15.erase( muon15.begin() + i );
	TLorentzVector dimuon[10];
	int muPair = 0;
	int mu1[10] , mu2[10];
	memset( mu1 , 0 , sizeof(mu1) );
	memset( mu2 , 0 , sizeof(mu2) );
	for ( int i = 0 ; i < muon15.size() ; ++ i )
		for ( int j = 0 ; j < muon15.size() ; ++ j )
			if ( muon15[j]->Charge + muon15[i]->Charge == 0 && muon15[i]->PT > 25 )
			{
				if ( muon15[j]->PT > 25 && i > j )	continue;
				mu1[muPair] = i;
				mu2[muPair] = j;
				dimuon[ muPair ++ ] = muon15[i]->P4() + muon15[j]->P4();
			}
	if ( !muPair )	return;
	vector<Jet*> jets30 = filterPhaseSpace( jets , 30 , -4.7 , 4.7 );
	bool flagJet40 = 0;
	for ( int i = 0 ; i < jets30.size() ; ++ i )
		if ( jets30[i]->PT > 40 )
			flagJet40 = 1;
	if ( jets30.size() < 2 || !flagJet40 )	return;
	if ( missingET->PT >= 40 )	return;
	int jetMaxn1 , jetMaxn2;
	double jetMax = 0;
	for ( int i = 0 ; i < jets30.size() ; ++ i )
		if ( jets30[i]->PT > jetMax )
		{
			jetMaxn1 = i;
			jetMax = jets30[i]->PT;
		}
	jetMax = 0;
	for ( int i = 0 ; i < jets30.size() ; ++ i )
		if ( jets30[i]->PT > jetMax && i != jetMaxn1 )
		{
			jetMaxn2 = i;
			jetMax = jets30[i]->PT;
		}
	bool flagDeltaEta = 0;
	if ( abs( jets30[jetMaxn1]->Eta - jets30[jetMaxn2]->Eta ) > 3.5 )	flagDeltaEta = 1;
	TLorentzVector dijet = jets30[jetMaxn1]->P4() + jets30[jetMaxn2]->P4();
	if ( flagDeltaEta && dijet.M() > 650 )
	{
		countSignalEvent( "VBFT" );
		return;
	}
	for ( int i = 0 ; i < muPair ; ++ i )
		if ( dimuon[i].M() > 50 && dijet.M() > 250 )
		{
			countSignalEvent( "GFT" );
			return;
		}
	countSignalEvent( "L" );
}

void Cms_1410_6679_mumu2j_7::finalize() {
  // Whatever should be done after the run goes here
}
