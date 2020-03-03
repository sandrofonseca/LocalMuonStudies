#ifndef LocalMuonStudies_CSCRPCLocalReco_h
#define LocalMuonStudies_CSCRPCLocalReco_h

//
// Package:    CSCRPCLocalReco
// Class:      CSCRPCLocalReco
//


// framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHit.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCChamber.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"


#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"

#include "LocalMuonStudies/CSCRPCLocalReco/plugins/CSCRPCBinder.h"


// use cout
#include <iostream>
#include <cmath>

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TTree.h"

// system include files
#include <memory>
#include <string>
#include <sstream>
#include <vector>

//
// class declaration
//

#ifndef debug
#define debug 1
#endif


using namespace std;
using namespace edm;


class CSCRPCLocalReco : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
	public:
		explicit CSCRPCLocalReco(const edm::ParameterSet&);
		~CSCRPCLocalReco();



	private:
                virtual void beginRun(const edm::Run& Run, const edm::EventSetup& iSetup);
		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;
		virtual void endRun(const edm::Run& Run, const edm::EventSetup& iSetup);
 		
		void initialization();
		void sampleCSCSegments(ESHandle<CSCGeometry>& theCSCGeometry);
		bool filterCSCSegment(const CSCSegment& sampleCSCSegment);
		bool filterSegmentEta(const CSCSegment& sampleCSCSegment);
		double deltaRforSegment(const GlobalPoint& globalPosition1, const GlobalVector& globalVector1, const GlobalPoint& globalPosition2, const GlobalVector& globalVector2);
		void findPeakTime(const CSCSegment& sampleCSCSegment);
                void build(ESHandle<RPCGeometry>& RPCGeometryEH, ESHandle<CSCGeometry>& CSCGeometryEH);
                 

		// ___________________________________________________________________________
		// coordinate ranges: phi[-180, 180] or [-pi, pi], theta[0, 90] or [0, pi/2]
		inline double range_phi_deg(double deg) {
			while (deg < -180.)
				deg += 360.;
			while (deg >= +180.)
				deg -= 360.;
			return deg;
		}

		inline double range_phi_rad(double rad) {
			while (rad < -M_PI)
				rad += 2. * M_PI;
			while (rad >= +M_PI)
				rad -= 2. * M_PI;
			return rad;
		}

		inline double range_theta_deg(double deg) {
			deg = fabs(deg);
			while (deg >= 180.)
				deg -= 180.;
			if (deg >= 180. / 2.)
				deg = 180. - deg;
			return deg;
		}

		inline double range_theta_rad(double rad) {
			rad = fabs(rad);
			while (rad >= M_PI)
				rad -= M_PI;
			if (rad >= M_PI / 2.)
				rad = M_PI - rad;
			return rad;
		}

		// ___________________________________________________________________________
		// radians, degrees
		inline double deg_to_rad(double deg) {
			constexpr double factor = M_PI / 180.;
			return deg * factor;
		}

		inline double rad_to_deg(double rad) {
			constexpr double factor = 180. / M_PI;
			return rad * factor;
		}

		// member variables
		bool dumpDigis;
		bool dumpRechits;
		bool dumpSegments;

                bool isSegmentMatchFilter;
                unsigned int SampleType;
                double deltaRTH;
                double ConeAngleX;
                double ConeAngleY;
                bool isEtaFilter;
                double MinSegmentEta;
                double MaxSegmentEta;
                int TrackType;
                double RangeStrips;
                double MaxD;

		EDGetTokenT<CSCRecHit2DCollection> rh_token;
		EDGetTokenT<CSCSegmentCollection> se_token;
		EDGetTokenT<RPCRecHitCollection> rpc_token;
		TTree *cscrpc;

               Handle<CSCSegmentCollection> pCSCSegments;

                // RPC Hits
                Handle<RPCRecHitCollection> pRPCrecHits;   

                ESHandle<CSCGeometry> theCSCGeometry;
                ESHandle<RPCGeometry> theRPCGeometry;


                map< CSCDetId, vector<RPCDetId> > CSC2RPCRolls;
                CSCRPCBinder CSC2RPCMap;

		int runNumber;
		unsigned int impactEventId;
		unsigned int impactRPCId;
		double impactCSCPeakTime2nd;
		double impactCSCPeakTimeAverage;
		double impactAngle;
		double impactDistanceZ;
		double impactStrip;
		double impactResidual;
		int impactClusterSize;
		int impactRPCBX;
		double impactGlobalPosition_X;
		double impactGlobalPosition_Y;
		double impactLocalPosition_X;
		double impactLocalPosition_Y;
                
		double RollSize;
		double StripLength;
		double StripWidth;
		double ConeRadiusX;
		double ConeRadiusY;

		double CSCPeakTime2nd;
		double CSCPeakTimeAverage;
		int CSCWireTimeBin;  
		int RPCBX;
		double deltaBXTime;

		RPCDetId impactRPCRollId;
		LocalVector impactDirection;
		LocalPoint impactPoint;

		bool isvalideRPCBX;
		bool anycoincidence;

		bool issampled;
		bool isSamplefilled;
		bool ischecked;



		vector<float> CSCGlobalPosition_X;
		vector<float> CSCGlobalPosition_Y;
		vector<float> CSCGlobalPosition_Z;

		vector<float> CSCGlobalPosition_Theta;
		vector<float> CSCGlobalPosition_Phi;

		vector<float> CSCGlobalPosition_Eta;

		vector<float> RPCGlobalPosition_X;
		vector<float> RPCGlobalPosition_Y;
		vector<float> RPCGlobalPosition_Z;

		vector<float> RPCGlobalPosition_Theta;
		vector<float> RPCGlobalPosition_Phi;
		vector<float> RPCGlobalPosition_Eta;

		// RPC nomenclature branches
		short rpcIDRegion, rpcIDRing, rpcIDStation,rpcIDSector,rpcIDLayer,rpcIDSubsection,rpcIDRoll,rpcChamberID;
		// CSC nomenclature branches
		short cscIDEndcap, cscIDRing, cscIDStation, cscIDChamber;











};
#endif
