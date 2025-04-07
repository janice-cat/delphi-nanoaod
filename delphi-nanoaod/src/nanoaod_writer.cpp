#include "nanoaod_writer.hpp"
#include "phdst.hpp"
#include <Math/PositionVector3D.h>
#include <Math/DisplacementVector3D.h>
#include <Math/SMatrix.h>

// Additional common block to extract jet releateted variables
extern "C" struct
{
    float thr;
    float obl;
    float sph;
    float apl;
} psjval_;

float &SPH = psjval_.sph;
float &APL = psjval_.apl;
float &THR = psjval_.thr;
float &OBL = psjval_.obl;

// MakeField with type deduction
template <typename T>
void MakeField(std::unique_ptr<RNTupleModel> &model, const std::string &name, const std::string &description, std::shared_ptr<T> &ptr)
{
    ptr = model->MakeField<T>({name, description});
}    


// Fill a vector with n elements using a function f
template <typename T, typename Func>
void fillVector(std::shared_ptr<std::vector<T>> &ptr, int i, int n, Func f)
{
    ptr->clear();
    std::generate_n(std::back_inserter(*ptr), n, [i, f]() mutable {
        return f(i++);
    });
}




NanoAODWriter::NanoAODWriter() : output_("nanoaod.root"), mc_(false) {};

NanoAODWriter::~NanoAODWriter() {};

NanoAODWriter *NanoAODWriter::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new NanoAODWriter();
    }
    return static_cast<NanoAODWriter *>(instance_);
};

void NanoAODWriter::setOutput(const std::filesystem::path &output)
{
    output_ = output;
};

void NanoAODWriter::setMC()
{
    mc_ = true;
};

void NanoAODWriter::user00()
{
    // std::cout << "NanoAODWriter::user00: Initialising" << std::endl;
    super::user00();

    std::unique_ptr<RNTupleModel> model = RNTupleModel::Create();

    defineEvent(model);
    definePart(model);
    defineVtx(model);

    if (sk::IFLJET > 0)
    {
        defineJet(model);
    }
    if (mc_ && sk::IFLSIM > 0)
    {
        defineSimPart(model);
        defineGenPart(model);
        defineSimVtx(model);
    }
    if (sk::IFLTRA > 0)
    {
        defineTrac(model);
    }
    if (sk::IFLMUO > 0)
    {
        defineMuid(model);    
    }
    if (sk::IFLELE > 0)
    {
        defineElid(model);
    }
    if (sk::IFLHAD > 0)
    {
        defineHadid(model);
    }   
    if (sk::IFLBTG > 0)
    {
        defineBtag(model);
    }
    writer_ = RNTupleWriter::Recreate(std::move(model), "Events", output_.string());
};

int NanoAODWriter::user01()
{
     return super::user01();
};

void NanoAODWriter::user02()
{
    super::user02();

    fillEvent();
    fillPart();
    fillVtx();
    if (sk::IFLJET > 0)
    {
        fillJet();
    }
    if (mc_ && sk::IFLSIM > 0)
    {
        fillSimPart();
        fillGenPart();
        fillSimVtx();
    }
    if (sk::IFLTRA > 0)
    {
        fillTrac();
    }
    if (sk::IFLMUO > 0)
    {
        fillMuid();
    }
    if (sk::IFLELE > 0)
    {
        fillElid();
    }
    if (sk::IFLBTG > 0)
    {
        fillBtag();
    }
    if (sk::IFLHAD > 0)
    {
        fillHadid();
    }

    writer_->Fill();
};

void NanoAODWriter::defineEvent(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Event_runNumber", "Run number", Event_runNumber_);
    MakeField(model, "Event_evtNumber", "Event number", Event_eventNumber_);
    MakeField(model, "Event_fillNumber", "Fill number", Event_fillNumber_);
    MakeField(model, "Event_date", "Date", Event_date_);
    MakeField(model, "Event_time", "Time", Event_time_);
    MakeField(model, "Event_magField", "Magnetic field", Event_magField_);
    MakeField(model, "Event_shortDstVersion", "Short DST version", Event_shortDstVersion_);
    MakeField(model, "Event_hadronTag", "T4 Hadron tag", Event_hadronT4_);
    MakeField(model, "Event_nChaMultT4", "Number of charged particles with T4", Event_nChaMultT4_);
    MakeField(model, "Event_nChaMult", "Number of charged particles", Event_nChaMult_);
    MakeField(model, "Event_nNeuMult", "Number of neutral particles", Event_nNeuMult_);
    MakeField(model, "Event_cmEnergy", "Center of mass energy", Event_cmEnergy_);
    MakeField(model, "Event_totalChaEnergy", "Total charged energy", Event_totalChaEnergy_);
    MakeField(model, "Event_totalEMEnergy", "Total electromagnetic energy", Event_totalEMEnergy_);
    MakeField(model, "Event_totalHadEnergy", "Total hadronic energy", Event_totalHadEnergy_);
    MakeField(model, "Event_DSTType", "DST type", Event_DSTType_);
}

void NanoAODWriter::fillEvent()
{
    *Event_runNumber_ = phdst::IIIRUN;
    *Event_eventNumber_ = phdst::IIIEVT;
    *Event_fillNumber_ = phdst::IIFILL;
    *Event_date_ = phdst::IIIDAT;
    *Event_time_ = phdst::IIITIM;
    *Event_magField_ = sk::BMAG;
    *Event_shortDstVersion_ = sk::ISVER;
    *Event_hadronT4_ = sk::IHAD4 > 0;
    *Event_nChaMultT4_ = sk::NCTR4;
    *Event_nChaMult_ = sk::NCTRK;
    *Event_nNeuMult_ = sk::NNTRK;
    *Event_cmEnergy_ = sk::ECMAS;
    *Event_totalChaEnergy_ = sk::ECHAR;
    *Event_totalEMEnergy_ = sk::EMNEU;
    *Event_totalHadEnergy_ = sk::EHNEU;
    *Event_DSTType_ = sk::CDTYPE();
}

void NanoAODWriter::definePart(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nPart", "Number of particles", nPart_);
    MakeField(model, "Part_vector", "Particle 4-vector", Part_vector_);
    MakeField(model, "Part_charge", "Particle charge", Part_charge_);
    MakeField(model, "Part_pdgId", "Particle PDG ID", Part_pdgId_);
    MakeField(model, "Part_massid", "Particle mass ID", Part_massid_);
    MakeField(model, "Part_jetnr", "jet index*1000 + hemisphere index * 100 + vtx code*10 vtx index", Part_jetnr_);
    MakeField(model, "Part_lock", "Particle lock", Part_lock_);

    if (mc_ && sk::IFLSIM > 0)
    {
        MakeField(model, "Part_simIdx", "Particle simulation index", Part_simIdx_);
        MakeField(model, "Part_originVtxIdx", "Particle origin vertex index", Part_originVtxIdx_);
        MakeField(model, "Part_decayVtxIdx", "Particle decay vertex index", Part_decayVtxIdx_);;
    }

}

void NanoAODWriter::fillPart()
{
    *nPart_ = sk::NVECP;
    fillVector(Part_vector_, sk::LVPART, sk::NVECP, [](int i)
                            { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector(Part_charge_, sk::LVPART, sk::NVECP, [](int i)
                        { return int(sk::VECP(7, i)); });
    fillVector(Part_pdgId_, sk::LVPART, sk::NVECP, [](int i)
                        { return int(sk::VECP(8, i)); });
    fillVector(Part_massid_, sk::LVPART, sk::NVECP, [](int i)
                    { return sk::VECP(9, i); });
    fillVector(Part_jetnr_, sk::LVPART, sk::NVECP, [](int i)
                        { return sk::VECP(10, i); });
    fillVector(Part_lock_, sk::LVPART, sk::NVECP, [](int i)
                    { return sk::LVLOCK(i); });

    if (mc_ && sk::IFLSIM > 0)
    {
        fillVector(Part_simIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAST(i) - 1; });
        fillVector(Part_originVtxIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAPV(1, i) - 1; });
        fillVector(Part_decayVtxIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAPV(2, i) - 1; });
    }

    if (sk::IFLTRA > 0)
    {
        fillVector(Part_tracIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return int(sk::VECP(7, i)) != 0 ? i - 1 : -1; });
    }

    if (sk::IFLMUO > 0)
    {
        fillVector(Part_muidIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::KMUID(3, i) != 0 ? i - 1 : -1; });
    }

    if (sk::IFLELE > 0)
    {
        fillVector(Part_elidIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::KELID(1, i) != 0 ? i - 1 : -1; });
    }
}

void NanoAODWriter::defineJet(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nJet", "Number of jets", nJet_);
    MakeField(model, "Jet_vector", "Jet 4-vector", Jet_vector_);
    MakeField(model, "Jet_charge", "Jet charge", Jet_charge_);

    if ((sk::IFLJET / 10) % 10 == 0)
    {
        MakeField(model, "Jet_oblatness", "Jet oblatness", Jet_oblatness_);
        MakeField(model, "Jet_thrust", "Jet thrust", Jet_thrust_);
        MakeField(model, "Jet_thrustVector", "Jet thrust vector", Jet_thrustVector_);
    }
    if ((sk::IFLJET / 100) % 10 == 0)
    {
        MakeField(model, "Jet_sphericity", "Jet sphericity", Jet_sphericity_);
        MakeField(model, "Jet_aplanarity", "Jet aplanarity", Jet_aplanarity_);
        MakeField(model, "Jet_sphericityVector", "Jet sphericity vector", Jet_sphericityVector_);
    }
}

void NanoAODWriter::fillJet()
{
    *nJet_ = sk::NJET;
    fillVector(Jet_vector_, sk::LVJET+1, sk::NJET, [](int i)
                                          { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector(Jet_charge_, sk::LVJET+1, sk::NJET, [](int i)
                                      { return int(sk::VECP(6, i)); });


    if ((sk::IFLJET / 10) % 10 == 0)
    {
        *Jet_thrust_ = THR;
        *Jet_oblatness_ = OBL;
        fillVector(Jet_thrustVector_, sk::LVTHRU, 3, [](int i)
                               { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });
    }

    if ((sk::IFLJET / 100) % 10 == 0)
    {
        *Jet_sphericity_ = SPH;
        *Jet_aplanarity_ = APL;
        fillVector(Jet_sphericityVector_, sk::LVSPHE, 3, [](int i)
                               { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });
    }
}

void NanoAODWriter::defineVtx(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nVtx", "Number of vertices", nVtx_);
    MakeField(model, "Vtx_firstOutIdx", "Vertex first outgoing particle index", Vtx_firstOutIdx_);
    MakeField(model, "Vtx_firstInIdx", "Vertex first incoming particle index", Vtx_firstInIdx_);
    MakeField(model, "Vtx_nOut", "Vertex number of outgoing particles", Vtx_nOut_);
    MakeField(model, "Vtx_ndf", "Vertex number of degrees of freedom", Vtx_ndf_);
    MakeField(model, "Vtx_mcode", "Vertex MC code", Vtx_mcode_);
    MakeField(model, "Vtx_position", "Vertex position", Vtx_position_);
    MakeField(model, "Vtx_chi2", "Vertex chi2", Vtx_chi2_);
    MakeField(model, "Vtx_errorMatrix", "Vertex error matrix", Vtx_errorMatrix_);
    MakeField(model, "Vtx_errorFlag", "Vertex error flag", Vtx_errorFlag_);
    MakeField(model, "Vtx_status", "Vertex status", Vtx_status_);
}

void NanoAODWriter::fillVtx()
{
    *nVtx_ = sk::NVTX;
    fillVector(Vtx_firstOutIdx_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(1, i); });
    fillVector(Vtx_firstInIdx_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(2, i); });
    fillVector(Vtx_nOut_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(3, i); });
    fillVector(Vtx_ndf_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(4, i); });
    fillVector(Vtx_mcode_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(5, i); });
    fillVector(Vtx_position_, 1, sk::NVTX, [](int i)
                          { return XYZPointF(sk::QVTX(6, i), sk::QVTX(7, i), sk::QVTX(8, i)); });
    fillVector(Vtx_chi2_, 1, sk::NVTX, [](int i)
                      { return sk::QVTX(9, i); });
    fillVector(Vtx_errorMatrix_, 1, sk::NVTX, [](int i)
                                         { return ROOT::Math::SMatrixSym3F(&sk::QVTX(10, i), 6); });
    fillVector(Vtx_errorFlag_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(16, i); });
    fillVector(Vtx_status_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(17, i); });
}

void NanoAODWriter::defineSimPart(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nSimPart", "Number of simulated particles", nSimPart_);
    MakeField(model, "SimPart_vector", "Simulated particle vector", SimPart_vector_);
    MakeField(model, "SimPart_pdgId", "Simulated particle PDG ID", SimPart_pdgId_);
    MakeField(model, "SimPart_partIdx", "Simulated particle index", SimPart_partIdx_);
    MakeField(model, "SimPart_genIdx", "Simulated particle generation index", SimPart_genIdx_);
    MakeField(model, "SimPart_originVtxIdx", "Simulated particle origin vertex index", SimPart_originVtxIdx_);
    MakeField(model, "SimPart_decayVtxIdx", "Simulated particle decay vertex index", SimPart_decayVtxIdx_);
}

void NanoAODWriter::fillSimPart()
{
    *nSimPart_ = sk::NVECMC;
    fillVector(SimPart_vector_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                            { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector(SimPart_pdgId_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return int(sk::VECP(6, i)); });
    fillVector(SimPart_partIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTPA(i) - 1; });
    fillVector(SimPart_genIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTLU(i) - 1; });
    fillVector(SimPart_originVtxIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTVX(1, i) - 1; });
    fillVector(SimPart_decayVtxIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTVX(2, i) - 1; });
}

void NanoAODWriter::defineGenPart(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nGenPart", "Number of generated particles", nGenPart_);
    MakeField(model, "GenPart_status", "Generated particle status", GenPart_status_);
    MakeField(model, "GenPart_pdgId", "Generated particle PDG ID", GenPart_pdgId_);
    MakeField(model, "GenPart_parentIdx", "Generated particle parent index", GenPart_parentIdx_);
    MakeField(model, "GenPart_firstChildIdx", "Generated particle first child index", GenPart_firstChildIdx_);
    MakeField(model, "GenPart_lastChildIdx", "Generated particle last child index", GenPart_lastChildIdx_);
    MakeField(model, "GenPart_vector", "Generated particle vector", GenPart_vector_);
    MakeField(model, "GenPart_vertex", "Generated particle vertex", GenPart_vertex_);
    MakeField(model, "GenPart_tau", "Generated particle lifetime", GenPart_tau_);
    MakeField(model, "GenPart_simIdx", "Generated particle simulation index", GenPart_simIdx_);
}

void NanoAODWriter::fillGenPart()
{
    *nGenPart_ = sk::NP;
    fillVector(GenPart_status_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 1); });
    fillVector(GenPart_pdgId_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 2); });
    fillVector(GenPart_parentIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 3); });
    fillVector(GenPart_firstChildIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 4); });
    fillVector(GenPart_lastChildIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 5); });
    fillVector(GenPart_vector_, 1, sk::NP, [](int i)
                            { return XYZTVectorF(sk::PP(i, 1), sk::PP(i, 2), sk::PP(i, 3), sk::PP(i, 4)); });
    fillVector(GenPart_vertex_, 1, sk::NP, [](int i)
                            { return XYZTVectorF(sk::VP(i, 1), sk::VP(i, 2), sk::VP(i, 3), sk::VP(i, 4)); });
    fillVector(GenPart_tau_, 1, sk::NP, [](int i)
                      { return sk::VP(i, 5); });
    fillVector(GenPart_simIdx_, 1, sk::NP, [](int i)
                        { return sk::ILUST(i) - 1; });
}

void NanoAODWriter::defineSimVtx(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "nSimVtx", "Number of simulated vertices", nSimVtx_);
    MakeField(model, "SimVtx_firstOutIdx", "Simulated vertex first outgoing particle index", SimVtx_firstOutIdx_);
    MakeField(model, "SimVtx_firstInIdx", "Simulated vertex first incoming particle index", SimVtx_firstInIdx_);
    MakeField(model, "SimVtx_nOut", "Simulated vertex number of outgoing particles", SimVtx_nOut_);
    MakeField(model, "SimVtx_mcode", "Simulated vertex MC code", SimVtx_mcode_);
    MakeField(model, "SimVtx_vertex", "Simulated vertex", SimVtx_vertex_);
    MakeField(model, "SimVtx_errorFlag", "Simulated vertex error flag", SimVtx_errorFlag_);
    MakeField(model, "SimVtx_status", "Simulated vertex status", SimVtx_status_);
}

void NanoAODWriter::fillSimVtx()
{
    *nSimVtx_ = sk::NVTXMC;
    fillVector(SimVtx_firstOutIdx_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(1, i) - 1; });
    fillVector(SimVtx_firstInIdx_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(2, i) - 1; });
    fillVector(SimVtx_nOut_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(3, i); });
    fillVector(SimVtx_mcode_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(4, i); });
    fillVector(SimVtx_vertex_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                          { return XYZPointF(sk::QVTX(1, i), sk::QVTX(2, i), sk::QVTX(3, i)); });
    fillVector(SimVtx_errorFlag_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(16, i); });
    fillVector(SimVtx_status_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(17, i); });
}

void NanoAODWriter::defineTrac(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Part_tracIdx", "Particle track index", Part_tracIdx_);
    MakeField(model, "Trac_originVtxIdx", "Track origin vertex index", Trac_originVtxIdx_);
    MakeField(model, "Trac_decayVtxIdx", "Track decay vertex index", Trac_decayVtxIdx_);
    MakeField(model, "Trac_impactParRPhi", "Track impact parameter RPhi", Trac_impactParRPhi_);
    MakeField(model, "Trac_impactParZ", "Track impact parameter Z", Trac_impactParZ_);
    MakeField(model, "Trac_thetaPerigee", "Track theta perigee", Trac_thetaPerigee_);
    MakeField(model, "Trac_phiPerigee", "Track phi perigee", Trac_phiPerigee_);
    MakeField(model, "Trac_curvaturePerigee", "Track curvature perigee", Trac_curvaturePerigee_);
    MakeField(model, "Trac_weightMatrix", "Track weight matrix", Trac_weightMatrix_);
    MakeField(model, "Trac_length", "Track length", Trac_length_);
    MakeField(model, "Trac_detectors", "Track detectors", Trac_detectors_);
    MakeField(model, "Trac_rFirstPoint", "Track first point radius", Trac_rFirstPoint_);
    MakeField(model, "Trac_zFirstPoint", "Track first point z", Trac_zFirstPoint_);
    MakeField(model, "Trac_chi2NoVD", "Track chi2 no vertex detector", Trac_chi2NoVD_);
    MakeField(model, "Trac_chi2VD", "Track chi2 vertex detector", Trac_chi2VD_);
    MakeField(model, "Trac_ndfNoVD", "Track ndf no vertex detector", Trac_ndfNoVD_);
    MakeField(model, "Trac_ndfVD", "Track ndf vertex detector", Trac_ndfVD_);
    MakeField(model, "Trac_nHitVDRPhi", "Track number of hits in vertex detector RPhi", Trac_nHitVDRPhi_);
    MakeField(model, "Trac_nHitVDZ", "Track number of hits in vertex detector Z", Trac_nHitVDZ_);
    MakeField(model, "Trac_resRPhiFirstPoint", "Track residual RPhi first point", Trac_resRPhiFirstPoint_);
    MakeField(model, "Trac_errorResRPhiFirstPoint", "Track error residual RPhi first point", Trac_errorResRPhiFirstPoint_);
    MakeField(model, "Trac_resZFirstPoint", "Track residual Z first point", Trac_resZFirstPoint_);
    MakeField(model, "Trac_errorResZFirstPoint", "Track error residual Z first point", Trac_errorResZFirstPoint_);
    MakeField(model, "Trac_impactParameterBeamSpotGeomSign", "Track impact parameter beam spot geometric sign", Trac_impactParameterBeamSpotGeomSign_);
    MakeField(model, "Trac_impactParameterZGeomSign", "Track impact parameter Z geometric sign", Trac_impactParameterZGeomSign_);
    MakeField(model, "Trac_impactParameterVertexGeomSign", "Track impact parameter vertex geometric sign", Trac_impactParameterVertexGeomSign_);
    MakeField(model, "Trac_chi2VDHits", "Chi2 vertex detector", Trac_chi2VDHits_);
}

void NanoAODWriter::fillTrac()
{
    Trac_originVtxIdx_->clear();
    Trac_decayVtxIdx_->clear();
    Trac_impactParRPhi_->clear();
    Trac_impactParZ_->clear();
    Trac_thetaPerigee_->clear();
    Trac_phiPerigee_->clear();
    Trac_curvaturePerigee_->clear();
    Trac_weightMatrix_->clear();
    Trac_length_->clear();
    Trac_detectors_->clear();
    Trac_rFirstPoint_->clear();
    Trac_zFirstPoint_->clear();
    Trac_chi2NoVD_->clear();
    Trac_chi2VD_->clear();
    Trac_ndfNoVD_->clear();
    Trac_ndfVD_->clear();
    Trac_nHitVDRPhi_->clear();
    Trac_nHitVDZ_->clear();
    Trac_resRPhiFirstPoint_->clear();
    Trac_errorResRPhiFirstPoint_->clear();
    Trac_resZFirstPoint_->clear();
    Trac_errorResZFirstPoint_->clear();
    Trac_impactParameterBeamSpotGeomSign_->clear();
    Trac_impactParameterZGeomSign_->clear();
    Trac_impactParameterVertexGeomSign_->clear();
    Trac_chi2VDHits_->clear();

    for (int i = sk::LVPART; i <= sk::NVECP; i++)
    {
        if (int(sk::VECP(7, i)) != 0)
        {
            Trac_originVtxIdx_-> push_back(sk::KTRAC(1, i) - 1);
            Trac_decayVtxIdx_-> push_back(sk::KTRAC(2, i) - 1);
            Trac_impactParRPhi_-> push_back(sk::QTRAC(3, i));
            Trac_impactParZ_-> push_back(sk::QTRAC(4, i));
            Trac_thetaPerigee_-> push_back(sk::QTRAC(5, i));
            Trac_phiPerigee_-> push_back(sk::QTRAC(6, i));
            Trac_curvaturePerigee_-> push_back(sk::QTRAC(7, i));
            Trac_weightMatrix_-> push_back(ROOT::Math::SMatrixSym5F(&sk::QTRAC(8, i), 15));
            Trac_length_-> push_back(sk::QTRAC(24, i));
            Trac_detectors_-> push_back(sk::KTRAC(25, i));
            Trac_rFirstPoint_-> push_back(sk::QTRAC(26, i));
            Trac_zFirstPoint_-> push_back(sk::QTRAC(27, i));
            Trac_chi2NoVD_-> push_back(sk::QTRAC(28, i));
            Trac_chi2VD_-> push_back(sk::QTRAC(29, i));
            Trac_ndfNoVD_-> push_back(sk::KTRAC(30, i));
            Trac_ndfVD_-> push_back(sk::KTRAC(31, i));
            Trac_nHitVDRPhi_-> push_back(sk::KTRAC(32, i));
            Trac_nHitVDZ_-> push_back(sk::KTRAC(33, i));
            Trac_resRPhiFirstPoint_-> push_back(sk::QTRAC(34, i));
            Trac_errorResRPhiFirstPoint_-> push_back(sk::QTRAC(35, i));
            Trac_resZFirstPoint_-> push_back(sk::QTRAC(36, i));
            Trac_errorResZFirstPoint_-> push_back(sk::QTRAC(37, i));
            Trac_impactParameterBeamSpotGeomSign_-> push_back(sk::QTRAC(38, i));
            Trac_impactParameterZGeomSign_-> push_back(sk::QTRAC(39, i));
            Trac_impactParameterVertexGeomSign_-> push_back(sk::QTRAC(40, i));
            Trac_chi2VDHits_-> push_back(sk::QTRAC(42, i));
        }
    }
}

void NanoAODWriter::defineMuid(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Part_muidIdx", "Particle muon ID index", Part_muidIdx_);
    MakeField(model, "Muid_tag", "Muon ID tag", Muid_tag_);
    MakeField(model, "Muid_looseChi2", "Muon ID loose chi2", Muid_looseChi2_);
    MakeField(model, "Muid_hitPattern", "Muon ID hit pattern", Muid_hitPattern_);
}

void NanoAODWriter::fillMuid()
{
    Muid_tag_->clear();
    Muid_looseChi2_->clear();
    Muid_hitPattern_->clear();
    for (int i = sk::LVPART; i <= sk::NVECP; i++)
    {
        if (sk::KMUID(3, i) != 0)
        {
            Muid_tag_->push_back(sk::KMUID(1, i));
            Muid_looseChi2_->push_back(sk::QMUID(2, i));
            Muid_hitPattern_->push_back(sk::KMUID(3, i));
        }
    }
}

void NanoAODWriter::defineElid(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Part_elidIdx", "Particle electron ID index", Part_elidIdx_);
    MakeField(model, "Elid_tag", "Electron ID tag", Elid_tag_);
    MakeField(model, "Elid_gammaConversion", "Electron ID gamma conversion", Elid_gammaConversion_);
    MakeField(model, "Elid_px", "Best electron px estimation", Elid_px_);
    MakeField(model, "Elid_py", "Best electron py estimation", Elid_py_);
    MakeField(model, "Elid_pz", "Best electron pz estimation", Elid_pz_);
}

void NanoAODWriter::fillElid()
{
    Elid_tag_->clear();
    Elid_gammaConversion_->clear();
    Elid_px_->clear();
    Elid_py_->clear();
    Elid_pz_->clear();
    for (int i = sk::LVPART; i <= sk::NVECP; i++)
    {
        if (sk::KELID(1, i) != 0)
        {
            Elid_tag_->push_back(sk::KELID(1, i));
            Elid_gammaConversion_->push_back(sk::KELID(2, i));
            Elid_px_->push_back(sk::QELID(3, i));
            Elid_py_->push_back(sk::QELID(4, i));
            Elid_pz_->push_back(sk::QELID(5, i));
        }
    }       
}

void NanoAODWriter::defineBtag(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Btag_probNegIP", "B-tagging probability for tracks with negative impact parameter, hemisphere 1", Btag_probNegIP_);
    MakeField(model, "Btag_probPosIP", "B-tagging probability for tracks with negative impact parameter, hemisphere 2", Btag_probPosIP_);
    MakeField(model, "Btag_probAllIP", "B-tagging probability for tracks with negative impact parameter, whole event", Btag_probAllIP_);
    MakeField(model, "Btag_thrustVector", "B-tagging probability for tracks with positive impact parameter, hemisphere 1", Btag_thrustVector_);
}

void NanoAODWriter::fillBtag()
{
    fillVector(Btag_probNegIP_, 1, 3, [](int i)
                      { return sk::QBTPRN(i); });
    fillVector(Btag_probPosIP_, 1, 3, [](int i)
                      { return sk::QBTPRP(i); });
    fillVector(Btag_probAllIP_, 1, 3, [](int i)
                      { return sk::QBTPRS(i); });

    *Btag_thrustVector_ = XYZVectorF(sk::QBTTHR(1), sk::QBTTHR(2), sk::QBTTHR(3));
}   

void NanoAODWriter::defineHadid(std::unique_ptr<RNTupleModel> &model)
{
    MakeField(model, "Haid_sign", "Used for combined tag", Haid_sign_);
    MakeField(model, "Haid_kaonDedx", "Kaon signature with DEDX", Haid_kaonDedx_);
    MakeField(model, "Haid_protonDedx", "Proton signature with DEDX", Haid_protonDedx_);
    MakeField(model, "Haid_kaonRich", "Kaon signature with RICH", Haid_kaonRich_);
    MakeField(model, "Haid_protonRich", "Proton signature with RICH", Haid_protonRich_);
    MakeField(model, "Haid_pionRich", "Pion signature with RICH", Haid_pionRich_);
    MakeField(model, "Haid_kaonCombined", "Kaon signature with combined tag", Haid_kaonCombined_);
    MakeField(model, "Haid_protonCombined", "Proton signature with combined tag", Haid_protonCombined_);
    MakeField(model, "Haid_richQuality", "RICH quality status", Haid_richQuality_);

    MakeField(model, "Haidn_pionTag", "Pion tag", Haidn_pionTag_);
    MakeField(model, "Haidn_kaonTag", "Kaon tag", Haidn_kaonTag_);
    MakeField(model, "Haidn_protonTag", "Proton tag", Haidn_protonTag_);
    MakeField(model, "Haidn_heavyTag", "Heavy particle tag", Haidn_heavyTag_);
    MakeField(model, "Haidn_pionTrackSelection", "Pion track selection", Haidn_pionTrackSelection_);
    MakeField(model, "Haidn_kaonTrackSelection", "Kaon track selection", Haidn_kaonTrackSelection_);
    MakeField(model, "Haidn_protonTrackSelection", "Proton track selection", Haidn_protonTrackSelection_);
    MakeField(model, "Haidn_heavyTrackSelection", "Heavy particle track selection", Haidn_heavyTrackSelection_);

    MakeField(model, "Haidr_pionTag", "Pion tag", Haidr_pionTag_);
    MakeField(model, "Haidr_kaonTag", "Kaon tag", Haidr_kaonTag_);
    MakeField(model, "Haidr_protonTag", "Proton tag", Haidr_protonTag_);
    MakeField(model, "Haidr_heavyTag", "Heavy particle tag", Haidr_heavyTag_);
    MakeField(model, "Haidr_electronTag", "Electron tag", Haidr_electronTag_);
    MakeField(model, "Haidr_selectionFlag", "Selection flag", Haidr_selectionFlag_);

    MakeField(model, "Haide_pionTag", "Pion tag", Haide_pionTag_);
    MakeField(model, "Haide_kaonTag", "Kaon tag", Haide_kaonTag_);
    MakeField(model, "Haide_protonTag", "Proton tag", Haide_protonTag_);
    MakeField(model, "Haide_heavyTag", "Heavy particle tag", Haide_heavyTag_);
    MakeField(model, "Haide_electronTag", "Electron tag", Haide_electronTag_);
    MakeField(model, "Haide_selectionFlag", "Selection flag", Haide_selectionFlag_);

    MakeField(model, "Haidc_pionTag", "Pion tag", Haidc_pionTag_);
    MakeField(model, "Haidc_kaonTag", "Kaon tag", Haidc_kaonTag_);
    MakeField(model, "Haidc_protonTag", "Proton tag", Haidc_protonTag_);
    MakeField(model, "Haidc_heavyTag", "Heavy particle tag", Haidc_heavyTag_);
    MakeField(model, "Haidc_electronTag", "Electron tag", Haidc_electronTag_);
    MakeField(model, "Haidc_selectionFlag", "Selection flag", Haidc_selectionFlag_);

    MakeField(model, "Dedx_value", "Dedx value (1 for mips)", Dedx_value_);
    MakeField(model, "Dedx_width", "Dedx Landau width", Dedx_width_);
    MakeField(model, "Dedx_nrWires", "Dedx number of wires", Dedx_nrWires_);
    MakeField(model, "Dedx_gapWires", "Mean distance between wires", Dedx_gapWires_);
    MakeField(model, "Dedx_error", "Dedx value error", Dedx_error_);
    MakeField(model, "Dedx_valueVD", "Dedx VD value", Dedx_valueVD_);
    MakeField(model, "Dedx_nrVDHits", "Number of VD hits", Dedx_nrVDHits_);

    MakeField(model, "Rich_theg", "Cherenkov angle gas radiator", Rich_theg_);
    MakeField(model, "Rich_sigg", "Sigma of Cherenkov angle gas radiator", Rich_sigg_);
    MakeField(model, "Rich_nphg", "Observed number of photons gas radiator", Rich_nphg_);
    MakeField(model, "Rich_nepg", "Expected number of photons gas radiator", Rich_nepg_);
    MakeField(model, "Rich_flagg", "Flag gas radiator", Rich_flagg_);
    MakeField(model, "Rich_thel", "Cherenkov angle liquid radiator", Rich_thel_);
    MakeField(model, "Rich_sigl", "Sigma of Cherenkov angle liquid radiator", Rich_sigl_);
    MakeField(model, "Rich_nphl", "Observed number of photons liquid radiator", Rich_nphl_);
    MakeField(model, "Rich_nepl", "Expected number of photons liquid radiator", Rich_nepl_);
    MakeField(model, "Rich_flagl", "Flag liquid radiator", Rich_flagl_);
}

void NanoAODWriter::fillHadid()
{
    Haid_sign_->clear();
    Haid_kaonDedx_->clear();
    Haid_protonDedx_->clear();
    Haid_kaonRich_->clear();
    Haid_protonRich_->clear();
    Haid_pionRich_->clear();
    Haid_kaonCombined_->clear();
    Haid_protonCombined_->clear();
    Haid_richQuality_->clear();

    Haidn_pionTag_->clear();
    Haidn_kaonTag_->clear();
    Haidn_protonTag_->clear();
    Haidn_heavyTag_->clear();
    Haidn_pionTrackSelection_->clear();
    Haidn_kaonTrackSelection_->clear();
    Haidn_protonTrackSelection_->clear();
    Haidn_heavyTrackSelection_->clear();

    Haidr_pionTag_->clear();
    Haidr_kaonTag_->clear();
    Haidr_protonTag_->clear();
    Haidr_heavyTag_->clear();
    Haidr_electronTag_->clear();
    Haidr_selectionFlag_->clear();

    Haide_pionTag_->clear();
    Haide_kaonTag_->clear();
    Haide_protonTag_->clear();
    Haide_heavyTag_->clear();
    Haide_electronTag_->clear();
    Haide_selectionFlag_->clear();

    Haidc_pionTag_->clear();
    Haidc_kaonTag_->clear();
    Haidc_protonTag_->clear();
    Haidc_heavyTag_->clear();
    Haidc_electronTag_->clear();
    Haidc_selectionFlag_->clear();

    Dedx_value_->clear();
    Dedx_width_->clear();
    Dedx_nrWires_->clear();
    Dedx_gapWires_->clear();
    Dedx_error_->clear();
    Dedx_valueVD_->clear();
    Dedx_nrVDHits_->clear();

    Rich_theg_->clear();
    Rich_sigg_->clear();
    Rich_nphg_->clear();
    Rich_nepg_->clear();
    Rich_flagg_->clear();
    Rich_thel_->clear();
    Rich_sigl_->clear();
    Rich_nphl_->clear();
    Rich_nepl_->clear();
    Rich_flagl_->clear();

    for (int i = sk::LVPART; i<= sk::NVECP; ++i)
    {
        if (int(sk::VECP(7, i)) != 0)
        {
            Haid_sign_->push_back(sk::KHAID(1, i));
            Haid_kaonDedx_->push_back(sk::KHAID(2, i));
            Haid_protonDedx_->push_back(sk::KHAID(3, i));
            Haid_kaonRich_->push_back(sk::KHAID(4, i));
            Haid_protonRich_->push_back(sk::KHAID(5, i));
            Haid_pionRich_->push_back(sk::KHAID(6, i));
            Haid_kaonCombined_->push_back(sk::QHAID(7, i));
            Haid_protonCombined_->push_back(sk::QHAID(8, i));
            Haid_richQuality_->push_back(sk::KHAID(9, i));

            Haidn_pionTag_->push_back(sk::KHAIDN(1, i));
            Haidn_kaonTag_->push_back(sk::KHAIDN(2, i));
            Haidn_protonTag_->push_back(sk::KHAIDN(3, i));
            Haidn_heavyTag_->push_back(sk::KHAIDN(4, i));
            Haidn_pionTrackSelection_->push_back(sk::KHAIDT(1, i));
            Haidn_kaonTrackSelection_->push_back(sk::KHAIDT(2, i));
            Haidn_protonTrackSelection_->push_back(sk::KHAIDT(3, i));
            Haidn_heavyTrackSelection_->push_back(sk::KHAIDT(4, i));

            Haidr_pionTag_->push_back(sk::KHAIDR(1, i));
            Haidr_kaonTag_->push_back(sk::KHAIDR(2, i));
            Haidr_protonTag_->push_back(sk::KHAIDR(3, i));
            Haidr_heavyTag_->push_back(sk::KHAIDR(4, i));
            Haidr_electronTag_->push_back(sk::KHAIDR(5, i));
            Haidr_selectionFlag_->push_back(sk::KHAIDR(6, i));

            Haide_pionTag_->push_back(sk::KHAIDE(1, i));
            Haide_kaonTag_->push_back(sk::KHAIDE(2, i));
            Haide_protonTag_->push_back(sk::KHAIDE(3, i));
            Haide_heavyTag_->push_back(sk::KHAIDE(4, i));
            Haide_electronTag_->push_back(sk::KHAIDE(5, i));
            Haide_selectionFlag_->push_back(sk::KHAIDE(6, i));

            Haidc_pionTag_->push_back(sk::KHAIDC(1, i));
            Haidc_kaonTag_->push_back(sk::KHAIDC(2, i));
            Haidc_protonTag_->push_back(sk::KHAIDC(3, i));
            Haidc_heavyTag_->push_back(sk::KHAIDC(4, i));
            Haidc_electronTag_->push_back(sk::KHAIDC(5, i));
            Haidc_selectionFlag_->push_back(sk::KHAIDC(6, i));

            Dedx_value_->push_back(sk::QDEDX(1, i));
            Dedx_width_->push_back(sk::QDEDX(2, i));
            Dedx_nrWires_->push_back(sk::KDEDX(3, i));
            Dedx_gapWires_->push_back(sk::QDEDX(4, i));
            Dedx_error_->push_back(sk::QDEDX(5, i));
            Dedx_valueVD_->push_back(sk::QDEDX(6, i));
            Dedx_nrVDHits_->push_back(sk::KDEDX(7, i));

            Rich_theg_->push_back(sk::THEG(i));
            Rich_sigg_->push_back(sk::SIGG(i));
            Rich_nphg_->push_back(sk::NPHG(i));
            Rich_nepg_->push_back(sk::NEPG(i));
            Rich_flagg_->push_back(sk::FLAGG(i));
            Rich_thel_->push_back(sk::THEL(i));
            Rich_sigl_->push_back(sk::SIGL(i));
            Rich_nphl_->push_back(sk::NPHL(i));
            Rich_nepl_->push_back(sk::NEPL(i));
            Rich_flagl_->push_back(sk::FLAGL(i));
        }
    }
}

void NanoAODWriter::user99()
{
    // std::cout << "NanoAODWriter::user99: Finalising" << std::endl;
    writer_.reset();
};
