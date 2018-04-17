#include "cms_1506_00424_8.h"
// AUTHOR: shadowquark
//  EMAIL: shadowquark@outlook.com
void Cms_1506_00424_8::initialize() {
  setAnalysisName("cms_1506_00424_8");
  setInformation(""
    "# cms 1506.00424 8TeV 20.7/fb\n"
  "");
  setLuminosity(20.7*units::INVFB);
  bookSignalRegions("4mu");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1506_00424_8::analyze() {
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
	muonsCombined = filterPhaseSpace( muonsCombined , 8 , -2.4 , 2.4 );
	muonsCombined = filterIsolation(muonsCombined);
	if ( muonsCombined.size() < 4 )	return;
	bool flagMuon17 = 0;
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		if ( muonsCombined[i]->PT > 17 && abs( muonsCombined[i]->Eta ) < 0.9 )
			flagMuon17 = 1;
	if ( !flagMuon17 )	return;
	TLorentzVector dimuon[10];
	int muPair = 0;
	int mu1[10] , mu2[10];
	memset( mu1 , 0 , sizeof(mu1) );
	memset( mu2 , 0 , sizeof(mu2) );
	for ( int i = 0 ; i < muonsCombined.size() ; ++ i )
		for ( int j = i ; j < muonsCombined.size() ; ++ j )
			if ( muonsCombined[i]->Charge + muonsCombined[j]->Charge == 0 )
			{
				TLorentzVector tmpDimuon = muonsCombined[i]->P4() + muonsCombined[j]->P4();;
				if ( tmpDimuon.M() < 5 || muonsCombined[i]->P4().DeltaR( muonsCombined[j]->P4() ) < 0.01 )
				{
					mu1[muPair] = i;
					mu2[muPair] = j;
					dimuon[ muPair ++ ] = tmpDimuon;
				}
			}
	if ( abs( dimuon[0].M() - dimuon[1].M() ) >= 0.13 + 0.065 / 2 * ( dimuon[0].M() + dimuon[1].M() ) )	return;
	bool flagDimuon[10];
	double deltaZ;
	bool flagZ = 0;
	double diM1 , diM2;
	memset( flagDimuon , 0 , sizeof(flagDimuon) );
	for ( int i = 0 ; i < tracks.size() ; ++ i )
		for ( int j = 0 ; j < tracks.size() ; ++ j )
			for ( int k = 0 ; k < muPair ; ++ k ) 
				if ( tracks[i]->Particle == muonsCombined[ mu1[k] ]->Particle 
					&& tracks[j]->Particle == muonsCombined[ mu2[k] ]->Particle )
				{
					double dis2 = pow( tracks[i]->XOuter - tracks[j]->XOuter , 2 );
					dis2 += pow( tracks[i]->YOuter - tracks[j]->YOuter , 2 );
					dis2 += pow( tracks[i]->ZOuter - tracks[j]->ZOuter , 2 );
					if ( sqrt(dis2) <= 0.5 )
					{
						flagDimuon[i] = 1;
						if ( flagZ )
						{
							deltaZ = - tracks[i]->Z - tracks[j]->Z;
							diM2 = dimuon[i].M();
						}
						else
						{
							deltaZ = tracks[i]->Z + tracks[j]->Z;
							flagZ = 1;
							diM1 = dimuon[i].M();
						}
					}
				}
	int muPairs = 0;
	for ( int i = 0 ; i < muPair ; ++ i )
		if ( flagDimuon[i] )
			muPairs ++;
	if ( muPairs != 2 )	return;
	if ( abs( deltaZ / 2 ) >= 1 )	return;
	if ( abs( diM1 - diM2 ) >= 0.13 + 0.065 * ( diM1 + diM2 ) / 2 )	return;
	countSignalEvent( "4mu" );
}

void Cms_1506_00424_8::finalize() {
  // Whatever should be done after the run goes here
}
