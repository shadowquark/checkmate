#include "cms_1701_02032_8_2mu2t.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1701_02032_8_2mu2t::initialize() {
  setAnalysisName("cms_1701_02032_8_2mu2t");          
  setInformation(""
    "# cms 1701.02032 8TeV 19.7/fb 2mu2tau\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  bookSignalRegions("2e;emu;eh;muh;hh");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1701_02032_8_2mu2t::analyze() {
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
	muonsCombined = filterPhaseSpace( muonsCombined , 5 , -2.4 , 2.4 );
	if ( muonsCombined.size() < 2 )	return;
	int muon1 , muon2;
	double lMuon1 , lMuon2;
	muon1 = muon2 = 0;
	lMuon1 = lMuon2 = 0;
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( muonsCombined[i]->PT > lMuon1 )
		{
			lMuon1 = muonsCombined[i]->PT;
			muon1 = i;
		}
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( muonsCombined[i]->PT > lMuon2 && muonsCombined[i]->Charge != muonsCombined[muon1]->Charge )
		{
			lMuon2 = muonsCombined[i]->PT;
			muon2 = i;
		}
	if ( lMuon1 <= 17 )	return;
	if ( lMuon2 <= 8 )	return;
//	if ( abs( muonsCombined[lmuon1]->P4().M() - 125 ) < )
	if ( muonsCombined[muon1]->P4().DeltaR( muonsCombined[muon2]->P4() ) < 0.4 )	return;
	electrons = filterPhaseSpace( electrons , 7 , -2.5 , 2.5 );
	jets = filterPhaseSpace( jets , 15 , -2.3 , 2.3 );
	for ( int i = 0 ; i < jets.size() ; ++ i )
		if ( checkBTag( jets[i] ) )
			return;
	TLorentzVector l4T = muonsCombined[muon1]->P4() + muonsCombined[muon2]->P4();
	if ( l4T.M() < 14 || l4T.M() > 66 )	return;
	if ( electrons.size() == 2 && muonsCombined.size() == 2 && lMuon1 > 18 && lMuon2 > 9 )
		for ( int i = 0 ; i < electrons.size() ; ++ i )
			for ( int j = i + 1 ; j < electrons.size() ; ++ j )
				if ( electrons[i]->Charge != electrons[j]->Charge )
				{
					TLorentzVector l4t = electrons[i]->P4() + electrons[j]->P4();
					if ( abs( ( l4T + l4t ).M() - 125 ) >= 25 )	continue;
					if ( abs( l4T.M() - l4t.M() ) / l4T.M() >= 0.8 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( electrons[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( electrons[j]->P4() ) < 0.4 )	continue;
					if ( electrons[i]->P4().DeltaR( electrons[j]->P4() ) < 0.4 )	continue;
					countSignalEvent( "2e" );
				}
	if ( electrons.size() == 1 && muonsCombined.size() == 3 )
		for ( int i = 0 ; i < electrons.size() ; ++ i )
			for ( int j = 0 ; j < muonsCombined.size() ; ++ j )
				if ( electrons[i]->Charge != muonsCombined[j]->Charge && j != muon1 && j != muon2 )
				{
					TLorentzVector l4t = electrons[i]->P4() + muonsCombined[j]->P4();
					if ( abs( ( l4T + l4t ).M() - 125 ) >= 25 )	continue;
					if ( abs( l4T.M() - l4t.M() ) / l4T.M() >= 0.8 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( muonsCombined[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( muonsCombined[j]->P4() ) < 0.4 )	continue;
					if ( electrons[i]->P4().DeltaR( muonsCombined[j]->P4() ) < 0.4 )	continue;
					countSignalEvent( "emu" );
				}
	if ( electrons.size() == 1 && ! jets.size() && muonsCombined.size() == 2 && lMuon1 > 18 && lMuon2 > 9 )
	{
		bool ehFlag = 0;
		for ( int i = 0 ; i < electrons.size() ; ++ i )
		{
			if ( ehFlag )	break;
			for ( int j = 0 ; j < jets.size() ; ++ j )
			{
				if ( ehFlag )	break;
				if ( electrons[i]->Charge + jets[j]->Charge == 0 )
				{
					TLorentzVector l4t = electrons[i]->P4() + jets[j]->P4();
					if ( abs( ( l4T + l4t ).M() - 125 ) >= 25 )	continue;
					if ( abs( l4T.M() - l4t.M() ) / l4T.M() >= 0.8 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( electrons[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( electrons[i]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					countSignalEvent( "eh" );
					ehFlag = 1;
				}
			}
		}
	}
	if ( electrons.size() == 0 && muonsCombined.size() == 3 && ! jets.size() )
		for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
			for ( int j = 0 ; j < jets.size() ; ++ j )
				if ( muonsCombined[i]->Charge + jets[j]->Charge == 0 && i != muon1 && i != muon2 )
				{
					TLorentzVector l4t = muonsCombined[i]->P4() + jets[j]->P4();
					if ( abs( ( l4T + l4t ).M() - 125 ) >= 25 )	continue;
					if ( abs( l4T.M() - l4t.M() ) / l4T.M() >= 0.8 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( muonsCombined[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( muonsCombined[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[i]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					countSignalEvent( "muh" );
				}
	if ( electrons.size() == 0 && muonsCombined.size() == 2 && jets.size() > 1 && lMuon1 > 18 && lMuon2 > 9 )
		for ( int i = 0 ; i < jets.size() ; ++ i )
			for ( int j = 0 ; j < jets.size() ; ++ j )
				if ( jets[i]->Charge + jets[j]->Charge == 0 )
				{
					TLorentzVector l4t = jets[i]->P4() + jets[j]->P4();
					if ( abs( ( l4T + l4t ).M() - 125 ) >= 25 )	continue;
					if ( abs( l4T.M() - l4t.M() ) / l4T.M() >= 0.8 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( jets[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( jets[i]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon1]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( muonsCombined[muon2]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					if ( jets[i]->P4().DeltaR( jets[j]->P4() ) < 0.4 )	continue;
					countSignalEvent( "hh" );
				}
}

void Cms_1701_02032_8_2mu2t::finalize() {
  // Whatever should be done after the run goes here
}       
