#include "cms_1801_03957.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1801_03957::initialize() {
  setAnalysisName("cms_1801_03957");          
  setInformation(""
    "# cms 1801.03957 13TeV 35.9/fb\n"
  "");
  setLuminosity(35.9*units::INVFB);      
  bookSignalRegions("SR01;SR02;SR03;SR04;SR05;SR06;SR07;SR08;SR09;SR10;SR11;SR12;SR13;SR14;SR15;SR16;SR17;SR18;SR19;SR20;SR21;SR22;SR23;SR24;SR25;SR26;SR27;SR28;SR29;SR30;SR31;SR32;SR33;SR34;SR35;SR36;SR37;SR38;SR39;SR40;SR41;SR42;SR43;SR44;SR45;SR46;SR47;SR48;SR49;SR50;SR51;SR52;SR53;SR54;SR55;SR56;SR57;SR58");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1801_03957::analyze() {
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
	if ( missingET->PT < 50 )	return;
	int flag3l = 2;
	muonsCombined = filterPhaseSpace( muonsCombined , 0 , -2.5 , 2.5 );
	electronsTight = filterPhaseSpace( electronsTight , 0 , -2.5 , 2.5 );
	TLorentzVector dilep;
	if ( muonsCombined.size() == 2 && electronsTight.size() == 1 &&\
		! ( muonsCombined[0]->Charge + muonsCombined[1]->Charge ) )
	{
		flag3l = 0;
		dilep = muonsCombined[0]->P4() + muonsCombined[1]->P4();
	}
	if ( muonsCombined.size() == 1 && electronsTight.size() == 2 &&\
		! ( muonsCombined[0]->Charge + muonsCombined[1]->Charge ) )
	{
		flag3l = 1;
		dilep = electronsTight[0]->P4() + electronsTight[1]->P4();
	}
	if ( flag3l == 2 )	return;
	double Ht = 0 , Mt;
	if (flag3l)
		Mt = mT( muonsCombined[0]->P4() , missingET->P4() );
	else
		Mt = mT( electronsTight[0]->P4() , missingET->P4() );
	jets = filterPhaseSpace( jets , 30 , -2.5 , 2.5 );
	for ( auto i : jets )
		Ht += i->PT;
	if ( dilep.M() < 75 )
	{
		if( Mt < 100 )
		{
			if ( Ht < 200 )
			{
				if ( missingET->PT < 100 )
					countSignalEvent( "SR01" );
				else if ( missingET->PT < 150 )
					countSignalEvent( "SR02" );
				else if ( missingET->PT < 200 )
					countSignalEvent( "SR03" );
				else
					countSignalEvent( "SR04" );
			} else
				countSignalEvent( "SR12" );
		} else if ( Mt < 160 )
		{
			if ( Ht < 200 )
			{
				if ( missingET->PT < 100 )
					countSignalEvent( "SR05" );
				else if ( missingET->PT < 150 )
					countSignalEvent( "SR06" );
				else
					countSignalEvent( "SR07" );
			} else
				countSignalEvent( "SR13" );
		} else
		{
			if ( Ht < 200 )
			{
				if ( missingET->PT < 100 )
					countSignalEvent( "SR08" );
				else if ( missingET->PT < 150 )
					countSignalEvent( "SR09" );
				else if ( missingET->PT < 200 )
					countSignalEvent( "SR10" );
				else
					countSignalEvent( "SR11" );
			} else
				countSignalEvent( "SR14" );
		}
	} else if ( dilep.M() < 105 )
	{
		if ( Mt < 100 )
		{
			if ( missingET->PT < 100 )
			{
				if ( Ht < 100 )
					return;
				else if ( Ht < 200 )
					countSignalEvent( "SR27" );
				else
					countSignalEvent( "SR40" );
			} else if ( missingET->PT < 150 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR15" );
				else if ( Ht < 200 )
					countSignalEvent( "SR28" );
				else
					countSignalEvent( "SR40" );
			
			} else if ( missingET->PT < 200 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR16" );
				else if ( Ht < 200 )
					countSignalEvent( "SR29" );
				else
					countSignalEvent( "SR41" );
			} else if ( missingET->PT < 250 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR17" );
				else if ( Ht < 200 )
					countSignalEvent( "SR30" );
				else
					countSignalEvent( "SR41" );
			} else if ( missingET->PT < 350 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR18" );
				else if ( Ht < 200 )
					countSignalEvent( "SR31" );
				else
					countSignalEvent( "SR42" );
			} else
			{
				if ( Ht < 100 )
					countSignalEvent( "SR18" );
				else if ( Ht < 200 )
					countSignalEvent( "SR31" );
				else
					countSignalEvent( "SR43" );
			}
		} else if ( Mt < 160 )
		{
			if ( missingET->PT < 100 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR19" );
				else if ( Ht < 200 )
					countSignalEvent( "SR32" );
				else
					countSignalEvent( "SR44" );
			} else if ( missingET->PT < 150 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR20" );
				else if ( Ht < 200 )
					countSignalEvent( "SR33" );
				else
					countSignalEvent( "SR45" );
			
			} else if ( missingET->PT < 200 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR21" );
				else if ( Ht < 200 )
					countSignalEvent( "SR34" );
				else
					countSignalEvent( "SR46" );
			} else if ( missingET->PT < 250 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR22" );
				else if ( Ht < 200 )
					countSignalEvent( "SR35" );
				else
					countSignalEvent( "SR47" );
			} else if ( missingET->PT < 300 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR22" );
				else if ( Ht < 200 )
					countSignalEvent( "SR35" );
				else
					countSignalEvent( "SR48" );
			} else
			{
				if ( Ht < 100 )
					countSignalEvent( "SR22" );
				else if ( Ht < 200 )
					countSignalEvent( "SR35" );
				else
					countSignalEvent( "SR49" );
			}
		} else
		{
			if ( missingET->PT < 100 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR23" );
				else if ( Ht < 200 )
					countSignalEvent( "SR36" );
				else
					countSignalEvent( "SR50" );
			} else if ( missingET->PT < 150 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR24" );
				else if ( Ht < 200 )
					countSignalEvent( "SR37" );
				else
					countSignalEvent( "SR51" );
			
			} else if ( missingET->PT < 200 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR25" );
				else if ( Ht < 200 )
					countSignalEvent( "SR38" );
				else
					countSignalEvent( "SR52" );
			} else if ( missingET->PT < 250 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR26" );
				else if ( Ht < 200 )
					countSignalEvent( "SR39" );
				else
					countSignalEvent( "SR53" );
			} else if ( missingET->PT < 300 )
			{
				if ( Ht < 100 )
					countSignalEvent( "SR26" );
				else if ( Ht < 200 )
					countSignalEvent( "SR39" );
				else
					countSignalEvent( "SR54" );
			} else
			{
				if ( Ht < 100 )
					countSignalEvent( "SR26" );
				else if ( Ht < 200 )
					countSignalEvent( "SR39" );
				else
					countSignalEvent( "SR55" );
			}
		}
	} else
	{
		if ( Mt < 100 )
			countSignalEvent( "SR56" );
		else if ( Mt < 160 )
			countSignalEvent( "SR57" );
		else
			countSignalEvent( "SR58" );
	}	
// NOT Double Checked ##########################################################
}

void Cms_1801_03957::finalize() {
  // Whatever should be done after the run goes here
}       
