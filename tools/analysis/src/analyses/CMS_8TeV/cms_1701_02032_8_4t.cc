#include "cms_1701_02032_8_4t.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1701_02032_8_4t::initialize() {
  setAnalysisName("cms_1701_02032_8_4t");          
  setInformation(""
    "# cms 1701.02032 8TeV 19.7/fb 4tauH\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  bookSignalRegions("mts50;mtg50");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1701_02032_8_4t::analyze() {
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
	muonsCombined = filterIsolation(muonsCombined);
	vector<Muon*> muons25 = filterPhaseSpace( muonsCombined , 25 , -2.1 , 2.1 );
	if ( !muons25.size() )	return;
	electrons = filterPhaseSpace( electrons , 7 , -2.5 , 2.5 );
	vector<Muon*> muons5 = filterPhaseSpace( muonsCombined , 5 , -2.4 , 2.4 );
	vector<Jet*> jets10 = filterPhaseSpace( jets , 10 , -2.3 , 2.3 );
/*	for ( int i = 0 ; i < jets10.size() ; ++ i )
		for ( int j = jets10[i]->Constituents.LowerBound() ; j <= jets10[i]->Constituents.GetLast() ; ++ j )
		{
			if ( jets10[i]->Constituents[j]->IsA() == GenParticle::Class() )
			{
			}
//			fprintf( fout , "%d\t%d\n" , jets10[i]->Constituents.LowerBound() , jets10[i]->Constituents.GetLast() );
//			fprintf( fout , "%lf, %d\n" , jets10[i]->PT , jets10[i]->Charge );
//			fprintf( fout , "%s\n" , jets10[i]->Constituents[0]->ClassName() );
//			double	tmpPT = jets10[i]->PT;
//			int	tmpCharge = jets10[i]->Charge;
//			fprintf( fout , "%s\n" , jets10[i]->Constituents[j]->ClassName() );
			if ( ! strcmp( jets10[i]->Constituents[j]->ClassName() , "Muon" ) )
			{
			}
		}*/
	for ( int trigger = 0 ; trigger < muons25.size() ; ++ trigger )
	{
		bool isoflag = 1;
		for ( int i = 0 ; i < electrons.size() ; ++ i )
			if ( electrons[i]->P4().DeltaR( muons25[trigger]->P4() ) < 0.4 )
			{
				isoflag = 0;
				break;
			}
		if ( !isoflag )	continue;
		for ( int i = 0 ; i < muons5.size() ; ++ i )
			if ( muons5[i]->P4().DeltaR( muons25[trigger]->P4() ) < 0.5 )
			{
				isoflag = 0;
				break;
			}
		if ( !isoflag )	continue;
		for ( int i = 0 ; i < jets10.size() ; ++ i )
			if ( jets10[i]->P4().DeltaR( muons25[trigger]->P4() ) < 0.4 )
			{
				isoflag = 0;
				break;
			}
		if ( !isoflag )	continue;
		for ( int i = 0 ; i < muons5.size() ; ++ i )
			for ( int j = 0 ; j < jets10.size() ; ++ j )
				if ( muons5[i]->Charge != muons25[trigger]->Charge && muons5[i]->Charge * jets10[j]->Charge < 0 )
				{
					if ( jets10[j]->PT <= 20 )	continue;
					TLorentzVector tauMX = muons5[i]->P4() + jets10[j]->P4();
					bool mTbin = mT( muons25[trigger]->P4() , missingET->P4() ) <= 50;
					if ( tauMX.M() >= 4 && mTbin )	countSignalEvent( "mts50" );
					if ( tauMX.M() >= 4 && !mTbin )	countSignalEvent( "mtl50" );
				} 
	}
}

void Cms_1701_02032_8_4t::finalize() {
  // Whatever should be done after the run goes here
}       
