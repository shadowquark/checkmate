#include "cms_1801_01846.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1801_01846::initialize() {
  setAnalysisName("cms_1801_01846");          
  setInformation(""
    "# cms 1801.01846 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("EW125ll1;EW125ll2;EW125ll3;EW125ll4;EW200ll1;EW200ll2;EW200ll3;EW200ll4;EW250ll1;EW250ll2;EW250ll3;EW250ll4;tt125ll1;tt125ll2;tt125ll3;tt200ll1;tt200ll2;tt200ll3;tt300ll1;tt300ll2;tt300ll3");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1801_01846::analyze() {
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
// code by yyFish #############################################################
	if ( missingET->PT <= 125 )	return;
	jets = filterPhaseSpace( jets , 25 , -2.4 , 2.4 );
	if ( !jets.size() )	return;
	double Ht = 0;
	for ( auto jet : jets )
		Ht += jet->PT;
	if ( Ht <= 100 )	return;
	if ( missingET->PT / Ht < 0.6 || missingET->PT / Ht > 1.4 )	return;
	muonsCombined = filterIsolation( muonsCombined , 0 );
	muonsCombined = filterIsolation( muonsCombined , 1 );
	muonsCombined = filterPhaseSpace( muonsCombined , 3.5 , -2.4 , 2.4 );
	if ( muonsCombined.size() < 2 )	return;
	if ( muonsCombined[0]->PT <= 5 )	return;
	if ( muonsCombined[0]->PT >= 30 )	return;
	if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	return;
	TLorentzVector dimuon = muonsCombined[0]->P4() + muonsCombined[1]->P4();
	if ( dimuon.Pt() <= 3 )	return;
	if ( dimuon.M() < 4 )	return;
	double rescale[2];
	rescale[0] = ( muonsCombined[1]->P4().Py() * missingET->P4().Px() -\
			muonsCombined[1]->P4().Px() * missingET->P4().Py() ) /\
			( muonsCombined[0]->P4().Px() * muonsCombined[1]->P4().Py() -\
				muonsCombined[0]->P4().Py() * muonsCombined[1]->P4().Px() );
	rescale[1] = ( muonsCombined[0]->P4().Py() * missingET->P4().Px() -\
			muonsCombined[0]->P4().Px() * missingET->P4().Py() ) /\
			( muonsCombined[1]->P4().Px() * muonsCombined[0]->P4().Py() -\
				muonsCombined[1]->P4().Py() * muonsCombined[0]->P4().Px() );
	double diTauMass = ( ( 1 + rescale[0] ) * muonsCombined[0]->P4() +\
				( 1 + rescale[1] ) * muonsCombined[1]->P4() ).M();
	if ( 0 < diTauMass && diTauMass < 160 )	return;
	if ( missingET->PT < 200 )
	{
		if ( muonsCombined[1]->PT <= 5 )	return;
		if ( muonsCombined[0]->PT < 12 )
			countSignalEvent( "tt125ll1" );
		else if ( muonsCombined[0]->PT < 20 )
			countSignalEvent( "tt125ll2" );
		else if ( muonsCombined[0]->PT < 30 )
			countSignalEvent( "tt125ll3" );
		if ( mT( muonsCombined[0]->P4() , missingET->P4() ) >= 70 ||\
			mT( muonsCombined[1]->P4() , missingET->P4() ) >= 70 )
			return;
		if ( dimuon.M() < 9 )
			countSignalEvent( "EW125l1" );
		else if ( dimuon.M() < 10.5 )
			return;
		else if ( dimuon.M() < 20 )
			countSignalEvent( "EW125l2" );
		else if ( dimuon.M() < 30 )
			countSignalEvent( "EW125l3" );
		else if ( dimuon.M() < 50 )
			countSignalEvent( "EW125l4" );
	} else
	{
		if ( missingET->PT < 300 )
		{
			if ( muonsCombined[1]->PT <= 5 )	return;
			if ( muonsCombined[0]->PT < 12 )
				countSignalEvent( "tt200ll1" );
			else if ( muonsCombined[0]->PT < 20 )
				countSignalEvent( "tt200ll2" );
			else if ( muonsCombined[0]->PT < 30 )
				countSignalEvent( "tt200ll3" );
		} else
		{
			if ( muonsCombined[0]->PT < 12 )
				countSignalEvent( "tt300ll1" );
			else if ( muonsCombined[0]->PT < 20 )
				countSignalEvent( "tt300ll2" );
			else if ( muonsCombined[0]->PT < 30 )
				countSignalEvent( "tt300ll3" );
		}
		if ( mT( muonsCombined[0]->P4() , missingET->P4() ) >= 70 ||\
			mT( muonsCombined[1]->P4() , missingET->P4() ) >= 70 ||\
			muonsCombined[1]->PT <= 5 )
			return;
		if ( missingET->PT < 250 )
		{
			if ( dimuon.M() < 9 )
				countSignalEvent( "EW200l1" );
			else if ( dimuon.M() < 10.5 )
				return;
			else if ( dimuon.M() < 20 )
				countSignalEvent( "EW200l2" );
			else if ( dimuon.M() < 30 )
				countSignalEvent( "EW200l3" );
			else if ( dimuon.M() < 50 )
				countSignalEvent( "EW200l4" );
		} else
		{
			if ( dimuon.M() < 9 )
				countSignalEvent( "EW250l1" );
			else if ( dimuon.M() < 10.5 )
				return;
			else if ( dimuon.M() < 20 )
				countSignalEvent( "EW250l2" );
			else if ( dimuon.M() < 30 )
				countSignalEvent( "EW250l3" );
			else if ( dimuon.M() < 50 )
				countSignalEvent( "EW250l4" );
		}
	}
	
// NOT double checked #########################################################   
}

void Cms_1801_01846::finalize() {
  // Whatever should be done after the run goes here
}       
