#include "atlas_1808_01899.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1808_01899::initialize() {
  setAnalysisName("atlas_1808_01899");          
  setInformation(""
    "# atlas 1808.01899 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SR1;SR2;SR3;SR4");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1808_01899::analyze() {
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
	if ( missingET->PT <= 60 )	return;
	electronsLoose = filterPhaseSpace( electronsLoose , 10 , -2.47 , 2.47 , 1 );
	muonsCombined = filterPhaseSpace( muonsCombined , 10 , -2.5 , 2.5 );
	jets = filterPhaseSpace( jets , 25 , -2.5 , 2.5 );
	jets = overlapRemoval( jets , electronsLoose , 0.2 );
	jets = overlapRemoval( jets , muonsCombined , 0.2 );
	if ( jets.size() )	return;
	if ( electronsLoose.size() + muonsCombined.size() - 4 )	return;
	if ( electronsLoose.size() == 2 &&\
		electronsLoose[0]->Charge + electronsLoose[1]->Charge )
		return;
	if ( muonsCombined.size() == 2 &&\
		muonsCombined[0]->Charge + muonsCombined[1]->Charge )
		return;
	if ( electronsLoose.size() == 1 || muonsCombined.size() == 1 )	return;
	int totCharge = 0;
	struct L4
	{
		int charge , flav;
		double pT;
		TLorentzVector p4;
	};
	vector<L4> l4;
	for ( auto i : electronsLoose )
	{
		totCharge += i->Charge;
		l4.push_back( L4{ i->Charge , 0 , i->PT , i->P4() } );
	}
	for ( auto i : muonsCombined )
	{
		totCharge += i->Charge;
		l4.push_back( L4{ i->Charge , 1 , i->PT , i->P4() } );
	}
	if (totCharge)	return;
	sort( l4.begin() , l4.end() ,\
		[]( L4 x , L4 y ){ return x.pT > y.pT; } );
	if ( l4[0].pT <= 30 )	return;
	TLorentzVector mXl;
	double dRmin = 10 , dRmax = 0;
	for ( auto i : l4 )
	{
		mXl += i.p4;
		for ( auto j : l4 )
		{
			if ( i.charge - j.charge && i.flav == j.flav )
			{
				if ( ( i.p4 + j.p4 ).M() <= 15)	return;
				if ( abs( ( i.p4 + j.p4 ).M() - 91.1876 ) <= 10 )	return;
			}
			if ( i.charge + j.charge && i.p4.DeltaR( j.p4 ) < dRmin )
				dRmin = i.p4.DeltaR( j.p4 );
			if ( i.charge + j.charge && i.p4.DeltaR( j.p4 ) > dRmax )
				dRmax = i.p4.DeltaR( j.p4 );
		}
	}
	if ( mXl.M() > 230 && l4[0].pT > 65 &&\
		0.16 <= dRmin && dRmin <= 1.21 && 0.27 <= dRmax && dRmax <= 2.03 )
		countSignalEvent( "SR1" );
	if ( mXl.M() > 270 && l4[0].pT > 45 &&\
		0.09 <= dRmin && dRmin <= 1.97 && 0.44 <= dRmax && dRmax <= 2.68 )
		countSignalEvent( "SR2" );
	if ( mXl.M() > 270 && l4[0].pT > 110 &&\
		0.39 <= dRmin && dRmin <= 2.22 && 0.55 <= dRmax && dRmax <= 2.90 )
		countSignalEvent( "SR3" );
	if ( mXl.M() > 370 && l4[0].pT > 160 &&\
		0.53 <= dRmin && dRmin <= 3.24 && 0.59 <= dRmax && dRmax <= 2.94 )
		countSignalEvent( "SR4" );
// NOT Double Checked ##########################################################
}

void Atlas_1808_01899::finalize() {
  // Whatever should be done after the run goes here
}       
