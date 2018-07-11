#include "cms_1709_08908.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1709_08908::initialize() {
  setAnalysisName("cms_1709_08908");          
  setInformation(""
    "# cms 1709.08908 13TeV 35.9/fb ll + jets\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("SRA100;SRA150;SRA250;SRB100;SRB150;SRB250;SRC100;SRC150");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1709_08908::analyze() {
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
  

// code by yyFish ###################################################
	muonsCombined = filterPhaseSpace( muonsCombined , 5 , -2.4 , 2.4 );
	if ( muonsCombined.size() < 2 )	return;
	if ( muonsCombined[0]->PT <= 17 )	return;
	if ( muonsCombined[1]->PT <= 8 )	return;
	jets = filterPhaseSpace( jets , 35 , -2.4 , 2.4 );
	jets = overlapRemoval( jets , muonsCombined , 0.4 );
	if ( jets.size() < 2 )	return;
	vector<Muon*> muons = filterPhaseSpace\
				( muonsCombined , 5 , -1.4 , 1.4 );
	vector<Muon*> muonsTemp1 = filterPhaseSpace\
					( muonsCombined , 5 , 1.6 , 2.4 );
	vector<Muon*> muonsTemp2 = filterPhaseSpace\
					( muonsCombined , 5 , -2.4 , -1.6 );
	muons.insert( muons.end() , muonsTemp1.begin() , muonsTemp1.end() );
	muons.insert( muons.end() , muonsTemp2.begin() , muonsTemp2.end() );
	sort( muons.begin() , muons.end() ,\
		[]( Muon *x , Muon *y ){ return x->PT > y->PT; } );
	muons = overlapRemoval( muons , 0.1 );
	if ( muons.size() < 2 )	return;
	if ( muons[0]->Charge + muons[1]->Charge )	return;
	if ( muons[0]->PT <= 25 )	return;
	if ( muons[1]->PT <= 20 )	return;
	TLorentzVector dimuon = muons[0]->P4() + muons[1]->P4();
	if ( dimuon.Pt() <= 86 )	return;
	if ( dimuon.Pt() >= 96 )	return;
	if ( missingET->PT <= 100 )	return;
	if ( abs( missingET->P4().DeltaPhi( jets[0]->P4() ) ) < 0.4 )	return;
	if ( abs( missingET->P4().DeltaPhi( jets[1]->P4() ) ) < 0.4 )	return;
	if ( muons.size() > 2 )
		for ( int i = 2 ; i < muons.size() ; ++ i )
			if ( ( muons[i]->P4().DeltaR( muons[0]->P4() ) < 0.4 ||\
				muons[i]->P4().DeltaR( muons[1]->P4() ) < 0.4 )\
				&& muons[i]->PT > 10 )
				return;
	if ( mT2( muons[0]->P4() , muons[1]->P4() , 0 ) <= 80 )
		return;
	double Ht = 0;
	for ( int i = 0 ; i < jets.size() ; ++ i )	Ht += jets[i]->PT;
	if ( jets.size() > 5 )
	{
		if ( missingET->PT > 150 )
			countSignalEvent( "SRC150" );
		else
			countSignalEvent( "SRC100" );
		return;
	}
	if ( Ht <= 500 )	return;
	if ( jets.size() > 3 )
	{
		if ( missingET->PT > 250 )
			countSignalEvent( "SRB250" );
		else if ( missingET->PT > 150 )
			countSignalEvent( "SRB150" );
		else
			countSignalEvent( "SRB100" );
		return;
	}
	if ( missingET->PT > 250 )
		countSignalEvent( "SRA250" );
	else if ( missingET->PT > 150 )
		countSignalEvent( "SRA150" );
	else
		countSignalEvent( "SRA100" );
// ##################################################################
}

void Cms_1709_08908::finalize() {
  // Whatever should be done after the run goes here
}       
