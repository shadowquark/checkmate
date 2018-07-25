#include "cms_1807_02048.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1807_02048::initialize() {
  setAnalysisName("cms_1807_02048");          
  setInformation(""
    "# cms 1807.02048 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("SR1;SR2;SR3;mt;et");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1807_02048::analyze() {
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
	bool flag[3] = {};
	if ( filterPhaseSpace( muonsCombined , 10 , -2.4 , 2.4 ).size() )
		flag[0] = 1;
	if ( filterPhaseSpace( muonsCombined , 15 , -2.4 , 2.4 ).size() > 1 )
		flag[1] = 1;
	if ( filterPhaseSpace( muonsCombined , 20 , -2.4 , 2.4 ).size() )
		flag[2] = 1;
	if ( filterPhaseSpace( electronsTight , 10 , -2.5 , 2.5 ).size() )
		flag[1] = 1;
	if ( filterPhaseSpace( electronsTight , 15 , -2.5 , 2.5 ).size() > 1 )
		flag[0] = 1;
	if ( filterPhaseSpace( electronsTight , 20 , -2.5 , 2.5 ).size() )
		flag[2] = 1;
	if ( flag[0] && flag[1] && flag[2] )	return;
	muonsCombined = filterPhaseSpace( muonsCombined , 25 , -2.4 , 2.4 );
	electronsTight = filterPhaseSpace( electronsTight , 26 , -2.1 , 2.1 );
	if ( muonsCombined.size() + electronsTight.size() > 1 )	return;
	if ( muonsCombined.size() )	flag[0] = flag[2] = 1;
	if ( electronsTight.size() )	flag[1] = flag[2] = 1;
	vector<Jet*> tjets , njets;
	for ( auto i : jets )
		if ( checkTauTag( i , "tight" ) )
			tjets.push_back(i);
		else
			njets.push_back(i);
	tjets = filterPhaseSpace( tjets , 20 , -2.3 , 2.3 );
	if ( !tjets.size() )	return;
	if ( tjets.size() > 2 )	return;
	if ( tjets.size() == 2 )	flag[0] = flag[1] = 1;
	if ( tjets.size() == 1 )	flag[2] = 1;
	if ( muonsCombined.size() + electronsTight.size() + tjets.size() != 2 )
		return;
	njets = filterPhaseSpace( njets , 20 , -2.4 , 2.4 );
	njets = overlapRemoval( njets , electronsTight , 0.4 );
	njets = overlapRemoval( njets , muonsCombined , 0.4 );
	njets = overlapRemoval( njets , tjets , 0.4 );
	if ( njets.size() )	flag[0] = flag[1] = 1;
	if ( !flag[0] &&\
		abs( electronsTight[0]->P4().DeltaPhi( tjets[0]->P4() ) ) >= 1.5 )
		flag[0] = 1;
	if ( !flag[1] &&\
		abs( muonsCombined[0]->P4().DeltaPhi( tjets[0]->P4() ) ) >= 1.5 )
		flag[1] = 1;
	if ( !flag[2] &&\
		abs( tjets[0]->P4().DeltaPhi( tjets[1]->P4() ) ) >= 1.5 )
		flag[2] = 1;
	if ( !flag[0] &&\
		abs( electronsTight[0]->P4().Eta() - tjets[0]->P4().Eta() ) >= 2 )
		flag[0] = 1;
	if ( !flag[1] &&\
		abs( muonsCombined[0]->P4().Eta() - tjets[0]->P4().Eta() ) >= 2 )
		flag[1] = 1;
	if ( !flag[0] && electronsTight[0]->P4().DeltaR( tjets[0]->P4() ) >= 3.5 )
		flag[0] = 1;
	if ( !flag[1] && muonsCombined[0]->P4().DeltaR( tjets[0]->P4() ) >= 3.5 )
		flag[1] = 1;
	if ( !flag[0] && ( electronsTight[0]->P4() + tjets[0]->P4() ).M() <= 50 )
		flag[0] = 1;
	if ( !flag[1] && ( muonsCombined[0]->P4() + tjets[0]->P4() ).M() <= 50 )
		flag[1] = 1;
	if ( missingET->PT <= 120 )	flag[0] = flag[1] = 1;
	if ( missingET->PT <= 50 )	flag[2] = 1;
	if ( !flag[0] )
	{
		double mTl1 = mT( electronsTight[0]->P4() , missingET->P4() );
		if ( mTl1 < 20 || ( 60 < mTl1 && mTl1 <= 120 ) )
			flag[0] = 1;
		if ( mTl1 + mT( tjets[0]->P4() , missingET->P4() ) <= 50 )
			flag[0] = 1;
		TVector3 lep1 = electronsTight[0]->P4().Vect();
		TVector3 lep2 = tjets[0]->P4().Vect();
		TVector3 lepm = missingET->P4().Vect();
		double Dl1l2 = ( lepm - ( lep1 + lep2 ) * 0.85 ) *\
				( lep1.Unit() + lep2.Unit() ).Unit();
		if ( Dl1l2 <= -500 )	flag[0] = 1;
		if ( mT2( electronsTight[0]->P4() , tjets[0]->P4() , 0 ) <= 100 )
			flag[0] = 1;
	}
	if ( !flag[1] )
	{
		double mTl1 = mT( muonsCombined[0]->P4() , missingET->P4() );
		if ( mTl1 < 20 || ( 60 < mTl1 && mTl1 <= 120 ) )
			flag[1] = 1;
		if ( mTl1 + mT( tjets[0]->P4() , missingET->P4() ) <= 50 )
			flag[1] = 1;
		TVector3 lep1 = muonsCombined[0]->P4().Vect();
		TVector3 lep2 = tjets[0]->P4().Vect();
		TVector3 lepm = missingET->P4().Vect();
		double Dl1l2 = ( lepm - ( lep1 + lep2 ) * 0.85 ) *\
				( lep1.Unit() + lep2.Unit() ).Unit();
		if ( Dl1l2 <= -500 )	flag[1] = 1;
		if ( mT2( muonsCombined[0]->P4() , tjets[0]->P4() , 0 ) <= 100 )
			flag[1] = 1;
	}
	if ( !flag[0] )	countSignalEvent( "et" );
	if ( !flag[1] )	countSignalEvent( "mt" );
	if ( flag[2] )	return;
	double mT2tt = mT2( tjets[0]->P4() , tjets[1]->P4() , 0 );
	double mTtt = mT( tjets[0]->P4() , missingET->P4() ) +\
			mT( tjets[0]->P4() , missingET->P4() );
	if ( mT2tt > 90 )
		countSignalEvent( "SR1" );
	else if ( mT2tt <= 40 )
		return;
	else if ( mTtt > 350 )
		countSignalEvent( "SR2" );
	else if ( mTtt > 300 )
		countSignalEvent( "SR3" );
// NOT Double Checked #########################################################
}

void Cms_1807_02048::finalize() {
  // Whatever should be done after the run goes here
}       
