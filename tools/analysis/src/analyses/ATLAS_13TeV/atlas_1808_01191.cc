#include "atlas_1808_01191.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1808_01191::initialize() {
  setAnalysisName("atlas_1808_01191");          
  setInformation(""
    "# atlas 1808.01191 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SR2l1;SR4l1;SR4l2");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1808_01191::analyze() {
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
	vector<Muon*> mu2l = filterPhaseSpace( muonsCombined , 7 , -2.4 , 2.4 );
	vector<Muon*> mu4l = filterPhaseSpace( muonsCombined , 5 , -2.7 , 2.7 );
	vector<Electron*> e4l = filterPhaseSpace( electronsLoose , 7 , -2.47 , 2.47 );
	struct L4
	{
		int charge;
		bool flav;
		TLorentzVector p4;
	};
	vector<L4> l4;
	for ( auto i : mu4l )
		l4.push_back( L4{ i->Charge , 1 , i->P4() } );
	for ( auto i : e4l )
		l4.push_back( L4{ i->Charge , 0 , i->P4() } );
	sort( l4.begin() , l4.end() ,\
		[]( L4 x , L4 y ){ return x.p4.Pt() > y.p4.Pt(); } );
	jets = filterPhaseSpace( jets , 20 , -2.5 , 2.5 );
	bool flag[2] = {};
	if ( mu2l.size() == 2 )	
	{
		if ( mu2l[0]->PT <= 30 || mu2l[1]->PT <= 20 )	flag[1] = 1;
		if ( mu2l[0]->P4().DeltaR( mu2l[1]->P4() ) >= 1.8 )	flag[1] = 1;
		if ( missingET->PT <= 175 )	flag[1] = 1;
		TLorentzVector dilep = mu2l[0]->P4() + mu2l[1]->P4();
		if ( dilep.M() <= 76 || dilep.M() >= 106 )	flag[1] = 1;
		if ( abs( dilep.DeltaPhi( missingET->P4() ) ) <= 2.7 )	flag[1] = 1;
		double Ht = mu2l[0]->PT + mu2l[1]->PT;
		TLorentzVector missjet = missingET->P4();
		for ( auto i : jets )
		{
			missjet += i->P4();
			Ht += i->PT;
		}
		if ( missingET->PT / Ht <= 0.33 )	flag[1] = 1;
		if ( abs( dilep.Pt() - missjet.Pt() ) / dilep.Pt() >= 0.2 )	flag[1] = 1;
		if ( !flag[1] && sqrt( pow( sqrt( pow( 91.1876 , 2 ) + dilep.Perp2() ) +\
						sqrt( pow( 91.1876 , 2 ) + missingET->P4().Perp2() ) , 2 )\
					- ( dilep + missingET->P4() ).Vect().Mag2() ) > 250 )
			countSignalEvent( "SR2l1" );
	}
	if ( l4.size() != 4 )	return;
	if ( l4[0].p4.Pt() <= 20 || l4[1].p4.Pt() <= 15 || l4[2].p4.Pt() <= 10 )
		flag[0] = 1;
	double mZ1 , mZ2;
	mZ1 = mZ2 = 13000;
	TLorentzVector diZ1 , diZ2;
	for ( auto i : l4 )
		for ( auto j : l4 )
		{
			if ( i.charge + j.charge || i.flav == j.flav )
				continue;
			double temp = abs( ( i.p4 + j.p4 ).M() - 91.1876 );
			if ( temp < mZ1 )
			{
				mZ1 = temp;
				diZ1 = i.p4 + j.p4;
				mZ2 = temp;
				diZ2 = diZ1;
			} else if ( temp < mZ2 )
			{
				mZ2 = temp;
				diZ2 = i.p4 + j.p4;
			}
		}
	if ( diZ1.M() >= 106 || diZ1.M() <= 50 || diZ2.M() <= 50 || diZ2.M() >= 115 )
		flag[0] = 1;
	if ( !flag[0] && ( diZ1 + diZ2 ).M() > 400 )
		countSignalEvent( "SR4l2" );
	else if ( !flag[0] && ( diZ1 + diZ2 ).M() > 220 )
		countSignalEvent( "SR4l1" );
// NOT Double Checked ##########################################################
}

void Atlas_1808_01191::finalize() {
  // Whatever should be done after the run goes here
}       
