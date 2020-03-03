//
// Package:    CSCRPCLocalReco
// Class:      CSCRPCLocalReco
//


// include files
#include "LocalMuonStudies/CSCRPCLocalReco/plugins/CSCRPCLocalReco.h"

//
// constructors and destructor
//
CSCRPCLocalReco::CSCRPCLocalReco(const edm::ParameterSet& pset)
{
	dumpDigis  = pset.getUntrackedParameter<bool>("dumpDigis", true);
	dumpRechits  = pset.getUntrackedParameter<bool>("dumpRechits", true);
	dumpSegments  = pset.getUntrackedParameter<bool>("dumpSegments", true);
	isSegmentMatchFilter = pset.getUntrackedParameter<bool>("isSegmentMatchFilter");
	SampleType = pset.getUntrackedParameter<unsigned int>("SampleType");
	TrackType = pset.getUntrackedParameter<int>("TrackType");
	deltaRTH = pset.getUntrackedParameter<double>("deltaRTH");
	ConeAngleX = pset.getUntrackedParameter<double>("ConeAngleX");
	ConeAngleY = pset.getUntrackedParameter<double>("ConeAngleY");
	isEtaFilter = pset.getUntrackedParameter<bool>("isEtaFilter");
	MinSegmentEta = pset.getUntrackedParameter<double>("MinSegmentEta");
	MaxSegmentEta = pset.getUntrackedParameter<double>("MaxSegmentEta");
	RangeStrips = pset.getUntrackedParameter<double>("RangeStrips");
	MaxD = pset.getUntrackedParameter<double>("MaxD");

	rh_token = consumes<CSCRecHit2DCollection>(pset.getParameter<edm::InputTag>("cscRecHitTag"));
	se_token = consumes<CSCSegmentCollection>(pset.getParameter<edm::InputTag>("cscSegTag"));
	rpc_token = consumes<RPCRecHitCollection>(pset.getParameter<edm::InputTag>("rpcRecHitTag"));

	//now do what ever initialization is needed
	edm::Service<TFileService> fs;
	cscrpc = fs->make<TTree>("cscrpc","cscrpc");

	initialization();
}


CSCRPCLocalReco::~CSCRPCLocalReco()
{ }


//
// member functions
//

// called for each event
	void
CSCRPCLocalReco::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace std;

cout << "run: " << iEvent.id().run() << "   event: " << iEvent.id().event() << endl;
        cout << "--------------------------------------------------------------------------------" << endl;

        iEvent.getByToken(se_token, pCSCSegments);
        iEvent.getByToken(rpc_token, pRPCrecHits);

	runNumber = iEvent.run();
	impactEventId = iEvent.id().event();
        
      
        iSetup.get<MuonGeometryRecord>().get(theCSCGeometry);
        iSetup.get<MuonGeometryRecord>().get(theRPCGeometry); 
        
 
    
        CSC2RPCMap.initiate(theRPCGeometry, theCSCGeometry);
        CSC2RPCMap.checkBinding(theRPCGeometry, theCSCGeometry);
        
        build(theRPCGeometry, theCSCGeometry);


        sampleCSCSegments(theCSCGeometry);



	// ntuple filling
	cscrpc->Fill();
	//
}

///
void CSCRPCLocalReco::beginRun(const edm::Run& Run, const edm::EventSetup& iSetup) {



}

///
void CSCRPCLocalReco::endRun(const edm::Run& Run, const edm::EventSetup& iSetup) {



}


///////////////////////////
void CSCRPCLocalReco::build(edm::ESHandle<RPCGeometry>& RPCGeometryEH, edm::ESHandle<CSCGeometry>& CSCGeometryEH) {

	for (TrackingGeometry::DetContainer::const_iterator it = RPCGeometryEH->dets().begin(); it < RPCGeometryEH->dets().end();it++) {

		if (dynamic_cast<const RPCChamber*>(*it) != nullptr) {

			const RPCChamber* ch = dynamic_cast<const RPCChamber*>(*it);

			std::vector<const RPCRoll*> RPCRolls = (ch->rolls());

			for (auto & RPCRollIter  : RPCRolls){

				RPCDetId RPCRollId = RPCRollIter->id();
				RPCGeomServ rpcsrv(RPCRollId);


				int Region = RPCRollId.region();
				if(Region == 0)
					continue;
				// For RPC Region is +1/-1 for +Z/-Z, while for CSC endcap is 1/2 for +Z/-Z
				if(Region == -1)
					Region = 2;
				int Station = RPCRollId.station();
				if(Station == 4)
					continue;
				int Ring = RPCRollId.ring();
				if(Ring == 1)
					continue;
				if((Station == 2 || Station == 3) && Ring == 3)
					Ring = 2;
				// correspond CSC chamber??
				int Chamber = rpcsrv.segment();
				int Layer = 0;
				CSCDetId CSCIndex(Region, Station, Ring, Chamber, Layer);
				std::vector<RPCDetId> RPCRolls_temp;
				if(CSC2RPCRolls.find(CSCIndex) != CSC2RPCRolls.end())
					RPCRolls_temp = CSC2RPCRolls[CSCIndex];
				RPCRolls_temp.push_back(RPCRollId);
				CSC2RPCRolls[CSCIndex] = RPCRolls_temp;
				//meCollection[rpcRoll] = bookDetUnitSeg(rpcRoll, (*RPCRollIter)->nstrips(), (*RPCRollIter)->pitch(), (*RPCRollIter)->specificTopology().stripLength());
			}
		}
	}
}
///////////////////////////////////////////////////
void CSCRPCLocalReco::sampleCSCSegments(edm::ESHandle<CSCGeometry>& theCSCGeometry) {
	unsigned int nSegment = pCSCSegments->size();
	cout << "This event has " << nSegment << " CSCSegments" << endl;

	//Statistics->Fill(nSegment);

std::cout << " Geometry node for CSCGeom is  " << &(*theCSCGeometry) << std::endl;
   std::cout << " I have " << theCSCGeometry->dets().size() << " dets" << std::endl;
std::cout << " I have " << theCSCGeometry->layers().size() << " layers" << std::endl;
std::cout << " I have " << theCSCGeometry->chambers().size() << " chambers" << std::endl;
    


	std::map<CSCDetId, int> CSCSegmentsCounter;
	for(CSCSegmentCollection::const_iterator CSCSegIter = pCSCSegments->begin(); CSCSegIter != pCSCSegments->end(); CSCSegIter++)
		CSCSegmentsCounter[CSCSegIter->cscDetId()]++;
	for(CSCSegmentCollection::const_iterator CSCSegIter = pCSCSegments->begin(); CSCSegIter != pCSCSegments->end(); CSCSegIter++) {


		issampled = false;
		isSamplefilled = false;
		ischecked = false;
//		isEfficiencyfilled = false;
//https://github.com/cms-sw/cmssw/blob/CMSSW_11_1_DEVEL_X/SUSYBSMAnalysis/HSCP/plugins/MuonSegmentProducer.cc#L115-L118
//                CSCDetId CSCId  = (CSCDetId)(*CSCSegIter).cscDetId();
                // CSCDetId CSCId((*CSCSegIter).geographicalId().rawId());
		//CSCDetId CSCId((*CSCSegIter).cscDetId());
                CSCDetId CSCId = (CSCDetId)(*CSCSegIter).cscDetId();
                cout << " CSCId " << (*CSCSegIter) << std::endl;
//                std::cout << " Geometry node for CSCGeom is  " << &(*theCSCGeometry) << std::endl;
		int Endcap = CSCId.endcap();
		int Station = CSCId.station();
		int Ring = CSCId.ring();
		int Chamber = CSCId.chamber();
		int Layer = CSCId.layer();
		cout << "CSC endcap is " << Endcap << ", Station is " << Station << ", Ring is " << Ring << " Chamber is " << Chamber << ", Layer is " << Layer << endl;
		if(Station == 4)
			continue;
		if(Ring == 1 || Ring == 4)
			continue;

		if(isEtaFilter == true && !filterSegmentEta(*CSCSegIter))
			continue;
		// One chamber has only 1 segment to avoid electron shower segments, and require at least 2 segments for this event
		cout << "CSCSegmentsCounter: " << CSCSegmentsCounter[CSCId] << endl;

		if(CSCSegmentsCounter[CSCId] == 1) {
			//if(CSCSegmentsCounter[CSCId] == 1 && pCSCSegments->size() >= 2) {
			LocalPoint SegmentLocalPosition= CSCSegIter->localPosition();
			LocalVector SegmentLocalDirection=CSCSegIter->localDirection();
			cout << "CSC segment local position: " << SegmentLocalPosition << endl;
			cout << "CSC segment local direction: " << SegmentLocalDirection << endl;
//https://cmssdt.cern.ch/lxr/source/Alignment/MuonAlignmentAlgorithms/src/SegmentToTrackAssociator.cc#0161
                       // const GeomDet* CSCDet = theCSCGeometry->idToDet(CSCId);
                        CSCDetId chamberId(CSCId.endcap(), CSCId.station(), CSCId.ring(), CSCId.chamber(), 0);
			const GeomDet* CSCDet = theCSCGeometry->idToDet(chamberId);
  //                        const GeomDet* CSCDet ;
			cout << "=CSC Chamber ID=>" << chamberId <<  endl;
			if(dynamic_cast<const CSCChamber*>(CSCDet) != 0) {
				const CSCChamber* CSCCh = dynamic_cast<const CSCChamber*>(CSCDet);
				GlobalPoint SegmentGlobalPosition = CSCCh->toGlobal(SegmentLocalPosition);
				GlobalVector SegmentGlobalDirection = CSCCh->toGlobal(SegmentLocalDirection);
				cout << "CSC segment global position: " << SegmentGlobalPosition << endl;
				cout << "CSC segment global direction: " << SegmentGlobalDirection << endl;
                               
  			       CSCGlobalPosition_X.push_back(SegmentGlobalPosition.x());
                               CSCGlobalPosition_Y.push_back(SegmentGlobalPosition.y());
                               CSCGlobalPosition_Z.push_back(SegmentGlobalPosition.z());
                 
				// Find correspond RPC rolls
				if(CSC2RPCMap.nearbyRPCRolls(CSCId).size() != 0) {
					std::vector<RPCDetId> RPCRolls = CSC2RPCMap.nearbyRPCRolls(CSCId);
					for(std::vector<RPCDetId>::const_iterator RPCRollIter = RPCRolls.begin(); RPCRollIter != RPCRolls.end(); RPCRollIter++) {
						if(ischecked == true)
							continue;

						ConeRadiusX = 0.;
						ConeRadiusY = 0.;

						int RPCStation = RPCRollIter->station();
						int RPCRing = RPCRollIter->ring();
						int RPCRollNumber = RPCRollIter->roll();
						bool isSpecialRoll = false;
						if((RPCStation == 1 && RPCRing == 3 && (RPCRollNumber == 1 || RPCRollNumber == 2)) || (RPCStation == 2 && RPCRing == 3 && RPCRollNumber == 1)) {
							isSpecialRoll = true;
							if(debug) cout << "SpecialRoll: " << RPCStation << ", " << RPCRing << ", " << RPCRollNumber << endl;
						}
						//isSpecialRoll = false;
						bool passCSCFilter = filterCSCSegment(*CSCSegIter);
						if(isSegmentMatchFilter == true && !isSpecialRoll && !passCSCFilter) {
							if(debug) cout << "skip the filter for special roll: " << isSegmentMatchFilter << ", " << isSpecialRoll << ", " << passCSCFilter << endl;
							continue;
						}

						const GeomDetUnit* RPCDet = theRPCGeometry->idToDetUnit(*RPCRollIter);
						if(dynamic_cast<const RPCRoll*>(RPCDet) != 0) {
							const RPCRoll* sampleRPCRoll = dynamic_cast<const RPCRoll*>(RPCDet);
							const BoundPlane& sampleRPCRollSurface = sampleRPCRoll->surface();
							const StripTopology& sampleRPCRollTop = sampleRPCRoll->specificTopology();

							GlobalPoint test(1, 1, 1);
							LocalPoint test1 = sampleRPCRoll->toLocal(test);
							LocalPoint test2 = sampleRPCRollSurface.toLocal(test);
							if(debug) cout << "For test: RPC roll local position is :" << test1 << endl;
							if(debug) cout << "For test: RPC roll surface local position is :" << test2 << endl;

							LocalPoint SegmentRPCPosition = sampleRPCRoll->toLocal(SegmentGlobalPosition);
							LocalVector SegmentRPCDirection = sampleRPCRoll->toLocal(SegmentGlobalDirection);
							double X0 = SegmentRPCPosition.x();
							double Y0 = SegmentRPCPosition.y();
							double Z0 = SegmentRPCPosition.z();
							double dx = SegmentRPCDirection.x();
							double dy = SegmentRPCDirection.y();
							double dz = SegmentRPCDirection.z();
							//for(unsigned int iStrip= 1; iStrip <= RPCRo->nstrips(); iStrip++)
							//if(debug) cout << "Strip " << iStrip << " local position is " << RPCRo->centreOfStrip((int)iStrip) << endl;
							double X = X0 + (-Z0) * dx / dz;
							double Y = Y0 + (-Z0) * dy / dz;
							LocalPoint impactRPCPosition(X, Y, 0);
							double S = (SegmentRPCPosition - impactRPCPosition).mag();
							//ConeRadiusX = S * ConeAngleX / cos(((SegmentRPCDirection.theta() > M_PI/2)?(M_PI-SegmentRPCDirection.theta()):(SegmentRPCDirection.theta())));
							//ConeRadiusY = S * ConeAngleY / cos(((SegmentRPCDirection.theta() > M_PI/2)?(M_PI-SegmentRPCDirection.theta()):(SegmentRPCDirection.theta())));
							ConeRadiusX = S * ConeAngleX;
							ConeRadiusY = S * ConeAngleY;
							// For RE1 the special layerout doesn't suit the cone algorithm
							if(Station == 1)
								ConeRadiusX = 0;

							if(fabs(S) > MaxD) {
								if(debug) cout << "Extropolate distance is too long!" << endl;
								continue;
							}

							StripLength = sampleRPCRollTop.stripLength();
							StripWidth = sampleRPCRollTop.localPitch(impactRPCPosition);
							int NStrips = sampleRPCRollTop.nstrips();
							RollSize = (double)NStrips * StripWidth;

							if(fabs(impactRPCPosition.z()) <= 1 && fabs(impactRPCPosition.x()) <= (RollSize*0.5+ConeRadiusX) && fabs(impactRPCPosition.y()) <= (StripLength*0.5+ConeRadiusY)) {

								if(debug) cout << "CSC chamber: " << CSCId.rawId() << ", Endcap: " << Endcap << ", Station: " << Station << ", Ring: " << Ring << ", Chamber: " << Chamber << ", layer: " << Layer << ", RPCRing: " << RPCRing << ", " << "RPCRollNumber: " << RPCRollNumber << ", roll size: " << RollSize << ", strip length: " << StripLength << ", strip width: " << StripWidth << ", ConeRadiusX: " << ConeRadiusX <<  ", ConeRadiusY: " << ConeRadiusY << endl;

								impactRPCRollId = *RPCRollIter; 
								impactDirection = SegmentRPCDirection; 
								impactPoint = impactRPCPosition; 
								findPeakTime(*CSCSegIter);

								// For Ntuple
								impactDistanceZ = Z0;
								impactRPCId = RPCRollIter->rawId();
								impactCSCPeakTime2nd = CSCPeakTime2nd;
								impactCSCPeakTimeAverage = CSCPeakTimeAverage;
								impactAngle = ((SegmentRPCDirection.theta() > M_PI/2) ? (M_PI- SegmentRPCDirection.theta()) : SegmentRPCDirection.theta());
								//impactAngle = ((SegmentGlobalDirection.theta() > M_PI/2) ? (M_PI- SegmentGlobalDirection.theta()) : SegmentGlobalDirection.theta());
								impactStrip = sampleRPCRollTop.strip(impactRPCPosition); // If outside the strip topology, the StripNumber will be set to 0/N_strip
								impactLocalPosition_X = impactRPCPosition.x();
								impactLocalPosition_Y = impactRPCPosition.y();
								GlobalPoint impactPoint_Global = sampleRPCRoll->toGlobal(impactRPCPosition);
								impactGlobalPosition_X = impactPoint_Global.x();
								impactGlobalPosition_Y = impactPoint_Global.y();

								RPCGlobalPosition_X.push_back(impactPoint_Global.x());
                        					RPCGlobalPosition_Y.push_back(impactPoint_Global.y());
                        					RPCGlobalPosition_Z.push_back(impactPoint_Global.z());

								if(debug) cout << "impactRPCId is " << impactRPCId << ", impactCSCPeakTime2nd is " << impactCSCPeakTime2nd << ", impactAngle is " << impactAngle << ", impactStrip is " << impactStrip << ", impactLocalPosition_X is " << impactLocalPosition_X << ", impactLocalPosition_Y is " << impactLocalPosition_Y << ", impactGlobalPosition_X is " << impactGlobalPosition_X << ", impactGlobalPosition_Y is " << impactGlobalPosition_Y << endl;
								// check efficiency
								issampled = true;
								if(ischecked == false)
								//	EfficiencybyRPCrecHit();
								if(anycoincidence == true && ischecked == false) {
									ischecked = true;
									//  fillEfficiency();
								}
								// avoid efficiency is calculated for 2 times
							}
							else {
								if(debug) cout << "out of roll size" << endl;
							}
						}
					}
					if(issampled == true && ischecked == false) {
						if(debug) cout << "Could not find a Roll has recHit inside window." << endl;
						ischecked = true;
						//fillSample();
						//                     fillEfficiency();
					}
				}
			} 
		}
	    }
	}

void CSCRPCLocalReco::findPeakTime(const CSCSegment& sampleCSCSegment) {

		std::vector<CSCRecHit2D> sampleCSCrecHitDGroup = sampleCSCSegment.specificRecHits();
		// Find the 1st and 2nd peak time
		double peakTime1st = 10000.;
		double peakTime2nd = 5000.;
		double peakTimesum = 0.;

		int WireTimeBinSum = 0;
		int averageWireTimeBin = 0;

		for(std::vector<CSCRecHit2D>::iterator CSCrecHitDIter = sampleCSCrecHitDGroup.begin(); CSCrecHitDIter != sampleCSCrecHitDGroup.end(); CSCrecHitDIter++) {
			double temppeakTime = CSCrecHitDIter->tpeak();
			peakTimesum += temppeakTime;
			if(temppeakTime < peakTime1st) {
				peakTime1st = temppeakTime;
				peakTime2nd = peakTime1st;
			}
			if(temppeakTime >= peakTime1st && temppeakTime < peakTime2nd)
				peakTime2nd = temppeakTime;

		}

		CSCPeakTime2nd = peakTime2nd;
		CSCPeakTimeAverage = peakTimesum / sampleCSCrecHitDGroup.size();
		if(debug) cout << "CSCPeakTime2nd is " << CSCPeakTime2nd << ", CSCPeakTimeAverage is " << CSCPeakTimeAverage << endl;

		//averageWireTimeBin = WireTimeBinSum / sampleCSCrecHitDGroup.size();
		//CSCWireTimeBin = averageWireTimeBin - 6;
		//if(debug) cout << "CSC average wire time bin is " << averageWireTimeBin << ", CSC wire BX is " << CSCWireTimeBin << endl;

	}

	/////////
bool CSCRPCLocalReco::filterCSCSegment(const CSCSegment& sampleCSCSegment) {

		bool isTrackSegment = false;
		CSCDetId sampleCSCId = sampleCSCSegment.cscDetId();
		const CSCChamber* sampleCSCChamber = theCSCGeometry->chamber(sampleCSCId);
		LocalVector localsampleVector = sampleCSCSegment.localDirection();
		LocalPoint localsamplePosition = sampleCSCSegment.localPosition();
		GlobalVector globalsampleVector = sampleCSCChamber->toGlobal(localsampleVector);
		GlobalPoint globalsamplePosition = sampleCSCChamber->toGlobal(localsamplePosition);
		int sampleCSCStation = sampleCSCId.station();
		int sampleCSCzEndcap = sampleCSCId.zendcap();

		for(CSCSegmentCollection::const_iterator CSCSegIter = pCSCSegments->begin(); CSCSegIter != pCSCSegments->end(); CSCSegIter++) {
			CSCDetId iterCSCId = CSCSegIter->cscDetId();
			int iterCSCStation = iterCSCId.station();
			int iterCSCzEndcap = iterCSCId.zendcap();
			if((abs(sampleCSCStation-iterCSCStation) == 0) || (sampleCSCzEndcap != iterCSCzEndcap))
				continue;

			if(debug) cout << "sampleCSCSegment:" << sampleCSCStation << ", " << sampleCSCzEndcap << ". CSCSegIter:" << iterCSCStation << ", " << iterCSCzEndcap << endl;

			const CSCChamber* iterCSCChamber = theCSCGeometry->chamber(iterCSCId);
			LocalVector localIterVector = CSCSegIter->localDirection();
			LocalPoint localIterPosition = CSCSegIter->localPosition();
			GlobalVector globalIterVector = iterCSCChamber->toGlobal(localIterVector);
			GlobalPoint globalIterPosition = iterCSCChamber->toGlobal(localIterPosition);

			double deltaR1 = deltaRforSegment(globalsamplePosition, globalsampleVector, globalIterPosition, globalIterVector);
			double deltaR2 = deltaRforSegment(globalIterPosition, globalIterVector, globalsamplePosition, globalsampleVector);
			if(deltaR1 <= deltaRTH || deltaR2 <= deltaRTH)
				isTrackSegment = true;
		}
		if(isTrackSegment == true) {
			if(debug) cout << "Pass Segment filter. successfully." << endl;
		}
		else {
			if(debug) cout << "Pass Segment filter unsuccessfully." << endl;
		}

		return isTrackSegment;
	}
/////////////////
double CSCRPCLocalReco::deltaRforSegment(const GlobalPoint& globalPosition1, const GlobalVector& globalVector1, const GlobalPoint& globalPosition2, const GlobalVector& globalVector2) {

		GlobalVector VectorPoint2Point = (GlobalVector)(globalPosition2 - globalPosition1);
		double deltaR1 = sqrt((globalVector2.phi() - globalVector1.phi()).value() * (globalVector2.phi() - globalVector1.phi()).value() + (globalVector2.theta() - globalVector1.theta()) * (globalVector2.theta() - globalVector1.theta()));
		double deltaR2 = sqrt((VectorPoint2Point.phi() - globalVector1.phi()).value() * (VectorPoint2Point.phi() - globalVector1.phi()).value() + (VectorPoint2Point.theta() - globalVector1.theta()) * (VectorPoint2Point.theta() - globalVector1.theta()));
		if(debug) cout << "deltaR1: " << deltaR1 << ", deltaR2: " << deltaR2 << ". globalVector1: " << globalVector1.phi().value() << ", " << globalVector1.theta() << ". globalVector2: " << globalVector2.phi().value() << ", " << globalVector2.theta() << endl;
		return (deltaR1 > deltaR2) ? deltaR1 : deltaR2;
	}

bool CSCRPCLocalReco::filterSegmentEta(const CSCSegment& sampleCSCSegment) {

		bool isinsideEtaRange = false;
		CSCDetId sampleCSCId = sampleCSCSegment.cscDetId();
		const CSCChamber* sampleCSCChamber = theCSCGeometry->chamber(sampleCSCId);
		LocalVector localsampleVector = sampleCSCSegment.localDirection();
		GlobalVector globalsampleVector = sampleCSCChamber->toGlobal(localsampleVector);
		double sampleSegmentEta = globalsampleVector.eta();
		if((fabs(sampleSegmentEta) >= MinSegmentEta) && (fabs(sampleSegmentEta) <= MaxSegmentEta))
			isinsideEtaRange = true;

		if(isinsideEtaRange == true) {
			if(debug) cout << "Pass Eta filter. successfully." << endl;
		}
		else {
			if(debug) cout << "Pass Eta filter unsuccessfully." << endl;
		}

		return isinsideEtaRange;
	}

	////


	////////////////////////
	// called once each job, before event loop
void CSCRPCLocalReco::beginJob() {
			//CSC
			cscrpc->Branch("CSCGlobalPosition_X",&CSCGlobalPosition_X);
			cscrpc->Branch("CSCGlobalPosition_Y",&CSCGlobalPosition_Y);
			cscrpc->Branch("CSCGlobalPosition_Z",&CSCGlobalPosition_Z);

			cscrpc->Branch("CSCGlobalPosition_Phi",&CSCGlobalPosition_Phi);
			cscrpc->Branch("CSCGlobalPosition_Theta",&CSCGlobalPosition_Theta);
			cscrpc->Branch("CSCGlobalPosition_Eta",&CSCGlobalPosition_Eta);

			// CSC nomenclature branches
			cscrpc->Branch("cscIDEndcap",&cscIDEndcap,"cscIDEndcap/S");
			cscrpc->Branch("cscIDRing",&cscIDRing,"cscIDRing/S");
			cscrpc->Branch("cscIDStation",&cscIDStation,"cscIDStation/S");
			cscrpc->Branch("cscIDChamber",&cscIDChamber,"cscIDChamber/S");

			//RPC
			cscrpc->Branch("RPCGlobalPosition_X",&RPCGlobalPosition_X);
			cscrpc->Branch("RPCGlobalPosition_Y",&RPCGlobalPosition_Y);
			cscrpc->Branch("RPCGlobalPosition_Z",&RPCGlobalPosition_Z);

			cscrpc->Branch("RPCGlobalPosition_Phi",&RPCGlobalPosition_Phi);
			cscrpc->Branch("RPCGlobalPosition_Theta",&RPCGlobalPosition_Theta);
			cscrpc->Branch("RPCGlobalPosition_Eta",&RPCGlobalPosition_Eta);

			// RPC nomenclature branches
			cscrpc->Branch("rpcIDRegion",&rpcIDRegion,"rpcIDRegion/S");
			cscrpc->Branch("rpcIDRing",&rpcIDRing,"rpcIDRing/S");
			cscrpc->Branch("rpcIDStation",&rpcIDStation,"rpcIDStation/S");  
			cscrpc->Branch("rpcIDSector",&rpcIDSector,"rpcIDSector/S");
			cscrpc->Branch("rpcIDLayer",&rpcIDLayer,"rpcIDLayer/S");
			cscrpc->Branch("rpcIDSubsection",&rpcIDSubsection,"rpcIDSubsection/S");
			cscrpc->Branch("rpcIDRoll",&rpcIDRoll,"rpcIDRoll/S");
			cscrpc->Branch("rpcChamberID",&rpcChamberID,"rpcChamberID/S");
			///////


		}

void CSCRPCLocalReco::initialization() {

			CSCGlobalPosition_X.clear();
			CSCGlobalPosition_Y.clear();
			CSCGlobalPosition_Z.clear();

			CSCGlobalPosition_Theta.clear();
			CSCGlobalPosition_Phi.clear();

			CSCGlobalPosition_Eta.clear();

			RPCGlobalPosition_X.clear();
			RPCGlobalPosition_Y.clear();
			RPCGlobalPosition_Z.clear();

			RPCGlobalPosition_Theta.clear();
			RPCGlobalPosition_Phi.clear();

			// RPC nomenclature branches
			rpcIDRegion = 0, rpcIDRing = 0, rpcIDStation = 0,rpcIDSector = 0,rpcIDLayer = 0,rpcIDSubsection = 0,rpcIDRoll = 0,rpcChamberID = 0;
			// CSC nomenclature branches
			cscIDEndcap = 0, cscIDRing = 0, cscIDStation = 0, cscIDChamber = 0;




		}

// called once each job, after event loop
void CSCRPCLocalReco::endJob()
		{
		}

///////////////////////////////////
//define this as a plug-in
DEFINE_FWK_MODULE(CSCRPCLocalReco);
