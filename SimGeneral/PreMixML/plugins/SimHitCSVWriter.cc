#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include <fstream>

class SimHitCSVWriter : public edm::one::EDAnalyzer<> {
public:
  explicit SimHitCSVWriter(const edm::ParameterSet&);
  ~SimHitCSVWriter() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

private:
  edm::EDGetTokenT<CrossingFrame<PSimHit>> token_;
  std::ofstream outfile_;
};

SimHitCSVWriter::SimHitCSVWriter(const edm::ParameterSet& iConfig) {
  token_ = consumes<CrossingFrame<PSimHit>>(iConfig.getParameter<edm::InputTag>("inputTag"));
  outfile_.open(iConfig.getUntrackedParameter<std::string>("outputFile", "simhits.csv"));
  outfile_ << "event,detUnitId,entryX,entryY,entryZ,exitX,exitY,exitZ,timeOfFlight,energyLoss,particleType\n";
}

SimHitCSVWriter::~SimHitCSVWriter() {
  if (outfile_.is_open()) outfile_.close();
}

void SimHitCSVWriter::analyze(const edm::Event& iEvent, const edm::EventSetup&) {
  edm::Handle<CrossingFrame<PSimHit>> handle;
  iEvent.getByToken(token_, handle);

  if (!handle.isValid()) {
    edm::LogWarning("SimHitCSVWriter") << "PSimHit CrossingFrame not found!";
    return;
  }

  MixCollection<PSimHit> simHits(handle.product());

  for (const auto& hit : simHits) {
    outfile_ << iEvent.id().event() << ","
             << hit.detUnitId() << ","
             << hit.entryPoint().x() << "," << hit.entryPoint().y() << "," << hit.entryPoint().z() << ","
             << hit.exitPoint().x() << "," << hit.exitPoint().y() << "," << hit.exitPoint().z() << ","
             << hit.timeOfFlight() << ","
             << hit.energyLoss() << ","
             << hit.particleType()
             << "\n";
  }
}

void SimHitCSVWriter::endJob() {
  outfile_.flush();
}

DEFINE_FWK_MODULE(SimHitCSVWriter);
