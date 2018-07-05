#include "atlas_1803_02762.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Atlas_1803_02762::initialize() {
  setAnalysisName("atlas_1803_02762");          
  setInformation(""
    "# atlas 1803.02762 13TeV 36.1/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("2SFa;2SFb;2SFc;2SFd;2SFe;2SFf;2SFg;2SFh;2SFi;2SFj;2SFk;2SFl;2SFm;2SFL;2SFT;2int;2high;2low;3a0J;3b0J;3c0J;3a1J;3b1J;3c1J;3Sa;3Sb;3Sc;3Sd;3Se");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1803_02762::analyze() {
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
	electronsLoose = filterPhaseSpace( electronsLoose , 10 , -2.47 , 2.47 );
	muonsCombined = filterPhaseSpace( muonsCombined , 10 , -2.4 , 2.4 );
	jets = filterPhaseSpace( jets , 20 , -2.4 , 2.4 );
	jets = overlapRemoval( jets , electronsLoose , 0.2 );
	electronsLoose = overlapRemoval( electronsLoose , jets , 0.4 );
	muonsCombined = overlapRemoval( muonsCombined , jets , 0.4 );
	if ( muonsCombined.size() + electronsLoose.size() != 2 &&\
		muonsCombined.size() + electronsLoose.size() != 3 )
		return;
	int llFlag = 0;
	double Mt2 , Mt = 13000;
	TLorentzVector diLep , l3;
	if ( muonsCombined.size() == 2 && !electronsLoose.size() )
	{
		llFlag = 1;
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	return;
		diLep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		Mt2 = mT2( muonsCombined[0]->P4() , muonsCombined[1]->P4() ,\
			missingET->P4().M() , missingET->P4() );
	}
	if ( electronsLoose.size() == 2 && !muonsCombined.size() )
	{
		llFlag = 2;
		if ( electronsLoose[0]->Charge + electronsLoose[1]->Charge )	return;
		diLep = electronsLoose[0]->P4() + electronsLoose[1]->P4();
		Mt2 = mT2( electronsLoose[0]->P4() , electronsLoose[1]->P4() ,\
			missingET->P4().M() , missingET->P4() );
	}
	if ( muonsCombined.size() == 3 )
	{
		llFlag = 3;
		TLorentzVector muons( 0 , 0 , 0 , 0 );
		for ( auto i : muonsCombined )
			muons += i->P4();
		for ( auto i : muonsCombined )
			for ( auto j : muonsCombined )
				if ( !( i->Charge + j->Charge ) &&\
					mT( muons - i->P4() - j->P4() , missingET->P4() ) < Mt )
				{
					Mt = mT( muons - i->P4() - j->P4() , missingET->P4() );
					diLep = i->P4() + j->P4();
					l3 = muons - i->P4() - j->P4();
				}
		
	}
	if ( muonsCombined.size() == 2 && electronsLoose.size() )
	{
		llFlag = 4;
		if ( muonsCombined[0]->Charge + muonsCombined[1]->Charge )	return;
		diLep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
		l3 = electronsLoose[0]->P4();
	}
	if ( muonsCombined.size() + electronsLoose.size() == 2 && !jets.size() )
	{
		if ( !llFlag )	return;
		if ( diLep.M() > 111 && Mt2 > 100 )	countSignalEvent( "2SFL" );
		if ( diLep.M() > 300 && Mt2 > 130 )	countSignalEvent( "2SFT" );
		if ( Mt2 < 100 )
			return;
		else if ( Mt2 < 150 )
		{
	 		if ( diLep.M() < 111 )	
				return;
			else if ( diLep.M() < 150 )
				countSignalEvent( "2SFa" );
			else if ( diLep.M() < 200 )
				countSignalEvent( "2SFb" );
			else if ( diLep.M() < 300 )
				countSignalEvent( "2SFc" );
			else
				countSignalEvent( "2SFd" );
		} else if ( Mt2 < 200 )
		{
	 		if ( diLep.M() < 111 )	
				return;
			else if ( diLep.M() < 150 )
				countSignalEvent( "2SFe" );
			else if ( diLep.M() < 200 )
				countSignalEvent( "2SFf" );
			else if ( diLep.M() < 300 )
				countSignalEvent( "2SFg" );
			else
				countSignalEvent( "2SFh" );
		} else if ( Mt2 < 300 )
		{
	 		if ( diLep.M() < 111 )	
				return;
			else if ( diLep.M() < 150 )
				countSignalEvent( "2SFi" );
			else if ( diLep.M() < 200 )
				countSignalEvent( "2SFj" );
			else if ( diLep.M() < 300 )
				countSignalEvent( "2SFk" );
			else
				countSignalEvent( "2SFl" );
		} else if ( diLep.M() > 111 )
			countSignalEvent( "2SFm" );
	}
	if ( muonsCombined.size() + electronsLoose.size() == 2 && jets.size() )
	{
		if ( !llFlag )	return;
		if ( jets.size() < 2 )	return;
		if ( diLep.M() <= 81 || diLep.M() >= 101 )	return;
		if ( missingET->PT <= 100 )	return;
		bool flags[4] = {};
		if ( jets.size() > 2 )	flags[2] = 1;
		if ( jets.size() == 2 || jets.size() > 5 )	flags[3] = 1;
		if ( diLep.M() < 86 || diLep.M() > 96 )	flags[3] = 1;
		TLorentzVector temp = diLep + missingET->P4();
		double	temp1 , temp2;
		int	temp1p , temp2p;
		temp1 = temp2 = 100;
		if ( jets.size() > 2 )
			for ( int i = 0 ; i < jets.size() ; ++ i )
				if ( abs( temp.DeltaPhi( jets[i]->P4() ) ) < temp2 )
				{
					if ( abs( temp.DeltaPhi( jets[i]->P4() ) ) < temp1 )
					{
						temp2 = temp1;
						temp2p = temp1p;
						temp1 = abs( temp.DeltaPhi( jets[i]->P4() ) );
						temp1p = i;
					} else
					{
						temp2 = abs( temp.DeltaPhi( jets[i]->P4() ) );
						temp2p = i;
					}
				}
		TLorentzVector tempJJ1 = jets[temp1p]->P4() + jets[temp2p]->P4();
		TLorentzVector tempJJ2 = jets[0]->P4() + jets[1]->P4();
		if ( tempJJ1.M() < 70 || tempJJ1.M() > 90 )	flags[3] = 1;
		if ( tempJJ2.M() < 70 || tempJJ2.M() > 90 )	flags[2] = 1;
		if ( tempJJ2.M() < 70 || tempJJ2.M() > 100 )	flags[0] = flags[1] = 1;
		if ( missingET->PT <= 150 )
			flags[0] = flags[1] = 1;
		else if ( missingET->PT <= 250 )
			flags[1] = 1;
		if ( diLep.Pt() <= 40 )
			flags[0] = flags[1] = flags[2] = flags[3] = 1;
		else if ( diLep.Pt() <= 60 )
			flags[0] = flags[1] = flags[2] = 1;
		else if ( diLep.Pt() <= 80 )
			flags[0] = flags[1] = 1;
		if ( tempJJ2.Pt() <= 100 || Mt2 <= 100)	flags[0] = flags[1] = 1;
		if ( jets[temp1p]->P4().DeltaR( jets[temp2p]->P4() ) >= 2.2 ) 
			flags[3] = 1;
		if ( jets[0]->P4().DeltaR( jets[1]->P4() ) >= 1.5 )
			flags[0] = flags[1] = 1;
		if ( llFlag == 1 &&\
			muonsCombined[0]->P4().DeltaR( muonsCombined[1]->P4() ) >= 1.8 )
			flags[0] = flags[1] = 1;
		if ( llFlag == 2 &&\
			electronsLoose[0]->P4().DeltaR( electronsLoose[1]->P4() ) >= 1.8 )
			flags[0] = flags[1] = 1;
		if ( abs( missingET->P4().DeltaPhi(diLep) ) >= 0.8 )	flags[2] = 1;
		if ( abs( missingET->P4().DeltaPhi(tempJJ1) ) >= 2.2 )	flags[3] = 1;
		if ( abs( missingET->P4().DeltaPhi(tempJJ2) ) >= 1.5 )	flags[2] = 1;
		if ( abs( missingET->P4().DeltaPhi(tempJJ2) ) <= 0.5 ||\
			abs( missingET->P4().DeltaPhi(tempJJ2) ) >= 3.0 )	
			flags[0] = flags[1] = 1;
		if ( missingET->PT / diLep.Pt() <= 0.6 || missingET->PT / diLep.Pt() >= 1.6 )
			flags[2] = 1;
		if ( missingET->PT / tempJJ2.Pt() >= 0.8 )	flags[2] = 1;
		TLorentzVector jetISR = - jets[temp1p]->P4() - jets[temp2p]->P4();
		for ( auto i : jets )
			jetISR += i->P4();
		if ( missingET->PT / jetISR.Pt() >= 0.8 ||\
			missingET->PT / jetISR.Pt() <= 0.4 )
			flags[3] = 1;
		if ( abs( missingET->P4().DeltaPhi(jetISR) ) <= 2.4 )
			flags[3] = 1;
		if ( abs( missingET->P4().DeltaPhi( jets[0]->P4() ) ) <= 2.6 )
			flags[3] = 1;
		if ( diLep.Eta() >= 1.6 )	flags[3] = 1;
		if ( jets.size() > 2 && jets[2]->PT <= 30 )	flags[3] = 1;
		if ( !flags[0] )	countSignalEvent( "2int" );
		if ( !flags[1] )	countSignalEvent( "2high" );
		if ( !flags[2] || !flags[3] )	countSignalEvent( "2low" );
	}
	if ( muonsCombined.size() + electronsLoose.size() == 3 )
	{
		if ( !llFlag )	return;
		if ( Mt <= 110 )	return;
		if ( diLep.M() < 81.2 )
		{
			if ( missingET->PT <= 130 )	return;
			if ( l3.Pt() < 20 )
				return;
			else if ( l3.Pt() < 30 )
				countSignalEvent( "3Sa" );
			else
				countSignalEvent( "3Sb" );
		} else if ( diLep.M() < 101.2 )
		{
			if ( jets.size() )
			{
				if ( missingET->PT <= 120 )
					return;
				else if ( missingET->PT < 200 )
				{
					if ( jets[0]->PT > 70 && ( diLep + l3 ).Pt() < 120 )
						countSignalEvent( "3a1J" );
				} else
				{
					if ( Mt < 160 )
						countSignalEvent( "3b1J" );
					else if ( l3.Pt() > 35 )
						countSignalEvent( "3c1J" );
				}
			} else
			{
				if ( missingET->PT < 60 )
					return;
				else if ( missingET->PT < 120 )
					countSignalEvent( "3a0J" );
				else if ( missingET->PT < 170 )
					countSignalEvent( "3b0J" );
				else
					countSignalEvent( "3c0J" );
			}
		} else
		{
			if ( missingET->PT <= 130 )	return;
			if ( l3.Pt() < 20 )
				return;
			else if ( l3.Pt() < 50 )
				countSignalEvent( "3Sc" );
			else if ( l3.Pt() < 80 )
				countSignalEvent( "3Sd" );
			else 
				countSignalEvent( "3Se" );
		}
	}
// NOT Double Checked #########################################################
}

void Atlas_1803_02762::finalize() {
  // Whatever should be done after the run goes here
}       
