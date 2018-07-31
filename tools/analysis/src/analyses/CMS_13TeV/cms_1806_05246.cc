#include "cms_1806_05246.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1806_05246::initialize() {
  setAnalysisName("cms_1806_05246");          
  setInformation(""
    "# cms 1806.05246 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("2l;3l;4l");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1806_05246::analyze() {
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
	muonsCombined = filterIsolation( muonsCombined , 0 );
	electronsTight = filterIsolation( electronsLoose , 0 );
	muonsCombined = filterPhaseSpace( muonsCombined , 10 , -2.4 , 2.4 );
	electronsTight = filterPhaseSpace( electronsLoose , 10 , -2.5 , 2.5 );
	bool flag[2] = {};
	if ( muonsCombined.size() + electronsTight.size() < 3 )	return;
	if ( muonsCombined.size() + electronsTight.size() > 3 )	
		flag[0] = 1;
	if ( muonsCombined.size() + electronsTight.size() > 4 )	
		flag[1] = 1;
	if ( !flag[0] && muonsCombined.size() == 2 &&\
		muonsCombined[0]->Charge + muonsCombined[1]->Charge == 0 )
	{
		vector<double> lRank = { muonsCombined[0]->PT , muonsCombined[1]->PT ,\
					electronsTight[0]->PT };
		sort( lRank.begin() , lRank.end() ,\
			[]( double x , double y ){ return x > y; } );
		if ( lRank[0] <= 25 || lRank[1] <= 20 || lRank[2] <= 15 )	return;
		double tmpMll = ( muonsCombined[0]->P4() + muonsCombined[1]->P4() ).M();
		if ( muonsCombined[0]->Charge + electronsTight[0]->Charge == 0 &&\
			( muonsCombined[0]->P4() + electronsTight[0]->P4() ).M() < tmpMll )
			tmpMll = ( muonsCombined[0]->P4() + electronsTight[0]->P4() ).M();
		if ( muonsCombined[1]->Charge + electronsTight[0]->Charge == 0 &&\
			( muonsCombined[1]->P4() + electronsTight[0]->P4() ).M() < tmpMll )
			tmpMll = ( muonsCombined[1]->P4() + electronsTight[0]->P4() ).M();
		if ( tmpMll <= 12 )	return;
		if ( tmpMll >= 100 )	return;
		jets = filterPhaseSpace( jets , 30 , -4.7 , 4.7 );
		if ( jets.size() )	return;
		if ( missingET->PT <= 50 )	return;
		TLorentzVector dilep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		if ( abs( dilep.M() - 91.1876 ) <= 25 )	return;
		dilep += electronsTight[0]->P4();
		if ( abs( missingET->P4().DeltaPhi(dilep) ) <= 2.2 )	return;
		countSignalEvent( "3l" );
	}
	if ( !flag[1] && muonsCombined.size() == 4 )
	{
		int totCharge = 0;
		for ( auto i : muonsCombined )
			totCharge += i->Charge;
		if (totCharge)	return;
		if ( muonsCombined[0]->PT <= 25 || muonsCombined[1]->PT <= 15 )
			return;	
		TLorentzVector diZ , diX , l4 = muonsCombined[1]->P4() +\
						muonsCombined[2]->P4() + muonsCombined[3]->P4();
		for ( auto i : muonsCombined )
			if ( muonsCombined[0]->Charge + i->Charge == 0 )
			{
				diZ = muonsCombined[0]->P4() + i->P4();
				diX = l4 - i->P4();
				if ( diZ.M() <= 4 || diX.M() <= 4 )	return;
				break;
			}
		if ( abs( diZ.M() - 91.1876 ) > abs( diX.M() - 91.1876 ) )
		{
			TLorentzVector temp = diZ;
			diZ = diX;
			diX = temp;
		}
		if ( abs( diZ.M() - 91.1876 ) >= 15 )	return;
		if ( diX.M() <= 10 || diX.M() >= 50 )	return;
		if ( missingET->PT <= 35 || missingET->PT >= 100 )	return;
		if ( ( diX + diZ ).M() <= 140 )	return;
		countSignalEvent( "4l" );
	}
	if ( !flag[1] && muonsCombined.size() == 3 && electronsTight.size() == 1 )
	{
		int totCharge = electronsTight[0]->Charge;
		vector<double> lRank = { electronsTight[0]->PT };
		TLorentzVector diZ , diX , l4 = electronsTight[0]->P4();
		for ( auto i : muonsCombined )
		{
			l4 += i->P4();
			totCharge += i->Charge;
			lRank.push_back( i->PT );
			if ( muonsCombined[0]->Charge + i->Charge == 0 )
				diZ = muonsCombined[0]->P4() + i->P4();
		}
		diX = l4 - diZ;
		if (totCharge)	return;
		sort( lRank.begin() , lRank.end() ,\
			[]( double x , double y ){ return x > y; } );
		if ( lRank[0] <= 25 || lRank[1] <= 15 )	return;	
		if ( abs( diZ.M() - 91.1876 ) >= 15 )	return;
		if ( diX.M() <= 10 || diX.M() >= 50 )	return;
		if ( missingET->PT <= 35 || missingET->PT >= 100 )	return;
		if ( ( diX + diZ ).M() <= 140 )	return;
		if ( diX.M() <= 4 || diZ.M() <= 4 )	return;
		countSignalEvent( "4l" );
	}
	if ( !flag[1] && muonsCombined.size() == 2 && electronsTight.size() == 2 )
	{
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	return;
		if ( electronsTight[0]->Charge + electronsTight[1]->Charge )	return;
		TLorentzVector diZ = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		TLorentzVector diX = electronsTight[0]->P4() + electronsTight[1]->P4();
		if ( abs( diZ.M() - 91.1876 ) > abs( diX.M() - 91.1876 ) )
		{
			TLorentzVector temp = diZ;
			diZ = diX;
			diX = temp;
		}
		vector<double> lRank = { electronsTight[0]->PT , electronsTight[1]->PT ,\
					muonsCombined[0]->PT , muonsCombined[1]->PT };
		sort( lRank.begin() , lRank.end() ,\
			[]( double x , double y ){ return x > y; } );
		if ( lRank[0] <= 25 || lRank[1] <= 15 )	return;	
		if ( abs( diZ.M() - 91.1876 ) >= 15 )	return;
		if ( diX.M() <= 10 || diX.M() >= 50 )	return;
		if ( missingET->PT <= 35 || missingET->PT >= 100 )	return;
		if ( ( diX + diZ ).M() <= 140 )	return;
		if ( diX.M() <= 4 || diZ.M() <= 4 )	return;
		countSignalEvent( "4l" );
	}
// NOT Double Checked #########################################################
}

void Cms_1806_05246::finalize() {
  // Whatever should be done after the run goes here
}       
