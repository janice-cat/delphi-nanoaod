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

// Fill a vector with n elements using a function f
template <typename T>
void fillVector(std::shared_ptr<std::vector<T>> &ptr, int i, int n, std::function<T(int)> f)
{
    ptr->clear();
    std::generate_n(std::back_inserter(*ptr), n, [i, f]() mutable
                    { return f(i++); });
};

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
    Event_runNumber_ = model->MakeField<int>({"Event_runNumber", "Run number"});
    Event_eventNumber_ = model->MakeField<int>({"Event_evtNumber", "Event number"});
    Event_fillNumber_ = model->MakeField<int>({"Event_fillNumber", "Fill number"});
    Event_date_ = model->MakeField<int>({"Event_date", "Date"});
    Event_time_ = model->MakeField<int>({"Event_time", "Time"});
    Event_magField_ = model->MakeField<float>({"Event_magField", "Magnetic field"});
    Event_shortDstVersion_ = model->MakeField<int8_t>({"Event_shortDstVersion", "Short DST version"});
    Event_hadronT4_ = model->MakeField<bool>({"Event_hadronTag", "T4 Hadron tag"});
    Event_nChaMultT4_ = model->MakeField<int16_t>({"Event_nChaMultT4", "Number of charged particles with T4"});
    Event_nChaMult_ = model->MakeField<int16_t>({"Event_nChaMult", "Number of charged particles"});
    Event_nNeuMult_ = model->MakeField<int16_t>({"Event_nNeuMult", "Number of neutral particles"});
    Event_cmEnergy_ = model->MakeField<float>({"Event_cmEnergy", "Center of mass energy"});
    Event_totalChaEnergy_ = model->MakeField<float>({"Event_totalChaEnergy", "Total charged energy"});
    Event_totalEMEnergy_ = model->MakeField<float>({"Event_totalEMEnergy", "Total electromagnetic energy"});
    Event_totalHadEnergy_ = model->MakeField<float>({"Event_totalHadEnergy", "Total hadronic energy"});
    Event_DSTType_ = model->MakeField<std::string>({"Event_DSTType", "DST type"});
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
    nPart_ = model->MakeField<int16_t>({"nPart", "Number of particles"});
    Part_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"Part_vector", "Particle 4-vector"});
    Part_charge_ = model->MakeField<std::vector<int16_t>>({"Part_charge", "Particle charge"});
    Part_pdgId_ = model->MakeField<std::vector<int16_t>>({"Part_pdgId", "Particle PDG ID"});
    Part_massid_ = model->MakeField<std::vector<int>>({"Part_massid", "Particle mass ID"});
    Part_jetnr_ = model->MakeField<std::vector<int16_t>>({"Part_jetnr", "jet index*1000 + hemisphere index * 100 + vtx code*10 vtx index"});
    Part_lock_ = model->MakeField<std::vector<int>>({"Part_lock", "Particle lock"});

    if (mc_ && sk::IFLSIM > 0)
    {
        Part_simIdx_ = model->MakeField<std::vector<int16_t>>({"Part_simIdx", "Particle simulation index"});
        Part_originVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Part_originVtxIdx", "Particle origin vertex index"});
        Part_decayVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Part_decayVtxIdx", "Particle decay vertex index"});
    }

}

void NanoAODWriter::fillPart()
{
    *nPart_ = sk::NVECP;
    fillVector<XYZTVectorF>(Part_vector_, sk::LVPART, sk::NVECP, [](int i)
                            { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector<int16_t>(Part_charge_, sk::LVPART, sk::NVECP, [](int i)
                        { return int(sk::VECP(7, i)); });
    fillVector<int16_t>(Part_pdgId_, sk::LVPART, sk::NVECP, [](int i)
                        { return int(sk::VECP(8, i)); });
    fillVector<int>(Part_massid_, sk::LVPART, sk::NVECP, [](int i)
                    { return sk::VECP(9, i); });
    fillVector<int16_t>(Part_jetnr_, sk::LVPART, sk::NVECP, [](int i)
                        { return sk::VECP(10, i); });
    fillVector<int>(Part_lock_, sk::LVPART, sk::NVECP, [](int i)
                    { return sk::LVLOCK(i); });

    if (mc_ && sk::IFLSIM > 0)
    {
        fillVector<int16_t>(Part_simIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAST(i) - 1; });
        fillVector<int16_t>(Part_originVtxIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAPV(1, i) - 1; });
        fillVector<int16_t>(Part_decayVtxIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::IPAPV(2, i) - 1; });
    }

    if (sk::IFLTRA > 0)
    {
        fillVector<int16_t>(Part_tracIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return int(sk::VECP(7, i)) != 0 ? i - 1 : -1; });
    }

    if (sk::IFLMUO > 0)
    {
        fillVector<int16_t>(Part_muidIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::KMUID(3, i) != 0 ? i - 1 : -1; });
    }

    if (sk::IFLELE > 0)
    {
        fillVector<int16_t>(Part_elidIdx_, sk::LVPART, sk::NVECP, [](int i)
                            { return sk::KELID(1, i) != 0 ? i - 1 : -1; });
    }
}

void NanoAODWriter::defineJet(std::unique_ptr<RNTupleModel> &model)
{
    nJet_ = model->MakeField<int16_t>({"nJet", "Number of jets"});
    Jet_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"Jet_vector", "Jet 4-vector"});
    Jet_charge_ = model->MakeField<std::vector<int16_t>>({"Jet_charge", "Jet charge"});

    if ((sk::IFLJET / 10) % 10 == 0)
    {
        Jet_oblatness_ = model->MakeField<float>({"Jet_oblatness", "Jet oblatness"});
        Jet_thrust_ = model->MakeField<float>({"Jet_thrust", "Jet thrust"});
        Jet_thrustVector_ = model->MakeField<std::vector<XYZVectorF>>({"Jet_thrustVector", "Jet thrust vector"});
    }
    if ((sk::IFLJET / 100) % 10 == 0)
    {
        Jet_sphericity_ = model->MakeField<float>({"Jet_sphericity", "Jet sphericity"});
        Jet_aplanarity_ = model->MakeField<float>({"Jet_aplanarity", "Jet aplanarity"});
        Jet_sphericityVector_ = model->MakeField<std::vector<XYZVectorF>>({"Jet_sphericityVector", "Jet sphericity vector"});
    }
}

void NanoAODWriter::fillJet()
{
    *nJet_ = sk::NJET;
    fillVector<XYZTVectorF>(Jet_vector_, sk::LVJET+1, sk::NJET, [](int i)
                                          { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector<int16_t>(Jet_charge_, sk::LVJET+1, sk::NJET, [](int i)
                                      { return int(sk::VECP(6, i)); });


    if ((sk::IFLJET / 10) % 10 == 0)
    {
        *Jet_thrust_ = THR;
        *Jet_oblatness_ = OBL;
        fillVector<XYZVectorF>(Jet_thrustVector_, sk::LVTHRU, 3, [](int i)
                               { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });
    }

    if ((sk::IFLJET / 100) % 10 == 0)
    {
        *Jet_sphericity_ = SPH;
        *Jet_aplanarity_ = APL;
        fillVector<XYZVectorF>(Jet_sphericityVector_, sk::LVSPHE, 3, [](int i)
                               { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });
    }
}

void NanoAODWriter::defineVtx(std::unique_ptr<RNTupleModel> &model)
{
    nVtx_ = model->MakeField<int16_t>({"nVtx", "Number of vertices"});
    Vtx_firstOutIdx_ = model->MakeField<std::vector<int16_t>>({"Vtx_firstOutIdx", "Vertex first outgoing particle index"});
    Vtx_firstInIdx_ = model->MakeField<std::vector<int16_t>>({"Vtx_firstInIdx", "Vertex first incoming particle index"});
    Vtx_nOut_ = model->MakeField<std::vector<int16_t>>({"Vtx_nOut", "Vertex number of outgoing particles"});
    Vtx_ndf_ = model->MakeField<std::vector<int16_t>>({"Vtx_ndf", "Vertex number of degrees of freedom"});
    Vtx_mcode_ = model->MakeField<std::vector<int16_t>>({"Vtx_mcode", "Vertex MC code"});
    Vtx_position_ = model->MakeField<std::vector<XYZPointF>>({"Vtx_position", "Vertex position"});
    Vtx_chi2_ = model->MakeField<std::vector<float>>({"Vtx_chi2", "Vertex chi2"});
    Vtx_errorMatrix_ = model->MakeField<std::vector<ROOT::Math::SMatrixSym3F>>({"Vtx_errorMatrix", "Vertex error matrix"});
    Vtx_errorFlag_ = model->MakeField<std::vector<int16_t>>({"Vtx_errorFlag", "Vertex error flag"});
    Vtx_status_ = model->MakeField<std::vector<int16_t>>({"Vtx_status", "Vertex status"});
}

void NanoAODWriter::fillVtx()
{
    *nVtx_ = sk::NVTX;
    fillVector<int16_t>(Vtx_firstOutIdx_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(1, i); });
    fillVector<int16_t>(Vtx_firstInIdx_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(2, i); });
    fillVector<int16_t>(Vtx_nOut_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(3, i); });
    fillVector<int16_t>(Vtx_ndf_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(4, i); });
    fillVector<int16_t>(Vtx_mcode_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(5, i); });
    fillVector<XYZPointF>(Vtx_position_, 1, sk::NVTX, [](int i)
                          { return XYZPointF(sk::QVTX(6, i), sk::QVTX(7, i), sk::QVTX(8, i)); });
    fillVector<float>(Vtx_chi2_, 1, sk::NVTX, [](int i)
                      { return sk::QVTX(9, i); });
    fillVector<ROOT::Math::SMatrixSym3F>(Vtx_errorMatrix_, 1, sk::NVTX, [](int i)
                                         { return ROOT::Math::SMatrixSym3F(&sk::QVTX(10, i), 6); });
    fillVector<int16_t>(Vtx_errorFlag_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(16, i); });
    fillVector<int16_t>(Vtx_status_, 1, sk::NVTX, [](int i)
                        { return sk::KVTX(17, i); });
}

void NanoAODWriter::defineSimPart(std::unique_ptr<RNTupleModel> &model)
{
    nSimPart_ = model->MakeField<int16_t>({"nSimPart", "Number of simulated particles"});
    SimPart_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"SimPart_vector", "Simulated particle vector"});
    SimPart_pdgId_ = model->MakeField<std::vector<int16_t>>({"SimPart_pdgId", "Simulated particle PDG ID"});
    SimPart_partIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_partIdx", "Simulated particle index"});
    SimPart_genIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_genIdx", "Simulated particle generation index"});
    SimPart_originVtxIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_originVtxIdx", "Simulated particle origin vertex index"});
    SimPart_decayVtxIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_decayVtxIdx", "Simulated particle decay vertex index"});
}

void NanoAODWriter::fillSimPart()
{
    *nSimPart_ = sk::NVECMC;
    fillVector<XYZTVectorF>(SimPart_vector_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                            { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector<int16_t>(SimPart_pdgId_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return int(sk::VECP(6, i)); });
    fillVector<int16_t>(SimPart_partIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTPA(i) - 1; });
    fillVector<int16_t>(SimPart_genIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTLU(i) - 1; });
    fillVector<int16_t>(SimPart_originVtxIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTVX(1, i) - 1; });
    fillVector<int16_t>(SimPart_decayVtxIdx_, sk::MTRACK + sk::LVPART, sk::NVECMC, [](int i)
                        { return sk::ISTVX(2, i) - 1; });
}

void NanoAODWriter::defineGenPart(std::unique_ptr<RNTupleModel> &model)
{
    nGenPart_ = model->MakeField<int16_t>({"nGenPart", "Number of generated particles"});
    GenPart_status_ = model->MakeField<std::vector<int16_t>>({"GenPart_status", "Generated particle status"});
    GenPart_pdgId_ = model->MakeField<std::vector<int16_t>>({"GenPart_pdgId", "Generated particle PDG ID"});
    GenPart_parentIdx_ = model->MakeField<std::vector<int16_t>>({"GenPart_parentIdx", "Generated particle parent index"});
    GenPart_firstChildIdx_ = model->MakeField<std::vector<int16_t>>({"GenPart_firstChildIdx", "Generated particle first child index"});
    GenPart_lastChildIdx_ = model->MakeField<std::vector<int16_t>>({"GenPart_lastChildIdx", "Generated particle last child index"});
    GenPart_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"GenPart_vector", "Generated particle vector"});
    GenPart_vertex_ = model->MakeField<std::vector<XYZTVectorF>>({"GenPart_vertex", "Generated particle vertex"});
    GenPart_tau_ = model->MakeField<std::vector<float>>({"GenPart_tau", "Generated particle lifetime"});
    GenPart_simIdx_ = model->MakeField<std::vector<int16_t>>({"GenPart_simIdx", "Generated particle simulation index"});
}

void NanoAODWriter::fillGenPart()
{
    *nGenPart_ = sk::NP;
    fillVector<int16_t>(GenPart_status_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 1); });
    fillVector<int16_t>(GenPart_pdgId_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 2); });
    fillVector<int16_t>(GenPart_parentIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 3); });
    fillVector<int16_t>(GenPart_firstChildIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 4); });
    fillVector<int16_t>(GenPart_lastChildIdx_, 1, sk::NP, [](int i)
                        { return sk::KP(i, 5); });
    fillVector<XYZTVectorF>(GenPart_vector_, 1, sk::NP, [](int i)
                            { return XYZTVectorF(sk::PP(i, 1), sk::PP(i, 2), sk::PP(i, 3), sk::PP(i, 4)); });
    fillVector<XYZTVectorF>(GenPart_vertex_, 1, sk::NP, [](int i)
                            { return XYZTVectorF(sk::VP(i, 1), sk::VP(i, 2), sk::VP(i, 3), sk::VP(i, 4)); });
    fillVector<float>(GenPart_tau_, 1, sk::NP, [](int i)
                      { return sk::VP(i, 5); });
    fillVector<int16_t>(GenPart_simIdx_, 1, sk::NP, [](int i)
                        { return sk::ILUST(i) - 1; });
}

void NanoAODWriter::defineSimVtx(std::unique_ptr<RNTupleModel> &model)
{
    nSimVtx_ = model->MakeField<int16_t>({"nSimVtx", "Number of simulated vertices"});
    SimVtx_firstOutIdx_ = model->MakeField<std::vector<int16_t>>({"SimVtx_firstOutIdx", "Simulated vertex first outgoing particle index"});
    SimVtx_firstInIdx_ = model->MakeField<std::vector<int16_t>>({"SimVtx_firstInIdx", "Simulated vertex first incoming particle index"});
    SimVtx_nOut_ = model->MakeField<std::vector<int16_t>>({"SimVtx_nOut", "Simulated vertex number of outgoing particles"});
    SimVtx_mcode_ = model->MakeField<std::vector<int16_t>>({"SimVtx_mcode", "Simulated vertex MC code"});
    SimVtx_vertex_ = model->MakeField<std::vector<XYZPointF>>({"SimVtx_vertex", "Simulated vertex"});
    SimVtx_errorFlag_ = model->MakeField<std::vector<int16_t>>({"SimVtx_errorFlag", "Simulated vertex error flag"});
    SimVtx_status_ = model->MakeField<std::vector<int16_t>>({"SimVtx_status", "Simulated vertex status"});
}

void NanoAODWriter::fillSimVtx()
{
    *nSimVtx_ = sk::NVTXMC;
    fillVector<int16_t>(SimVtx_firstOutIdx_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(1, i) - 1; });
    fillVector<int16_t>(SimVtx_firstInIdx_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(2, i) - 1; });
    fillVector<int16_t>(SimVtx_nOut_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(3, i); });
    fillVector<int16_t>(SimVtx_mcode_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(4, i); });
    fillVector<XYZPointF>(SimVtx_vertex_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                          { return XYZPointF(sk::QVTX(1, i), sk::QVTX(2, i), sk::QVTX(3, i)); });
    fillVector<int16_t>(SimVtx_errorFlag_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(16, i); });
    fillVector<int16_t>(SimVtx_status_, sk::NVTXMX + 1, sk::NVTXMC, [](int i)
                        { return sk::KVTX(17, i); });
}

void NanoAODWriter::defineTrac(std::unique_ptr<RNTupleModel> &model)
{
    Part_tracIdx_ = model->MakeField<std::vector<int16_t>>({"Part_tracIdx", "Particle track index"});
    Trac_originVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Trac_originVtxIdx", "Track origin vertex index"});
    Trac_decayVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Trac_decayVtxIdx", "Track decay vertex index"});
    Trac_impactParRPhi_ = model->MakeField<std::vector<float>>({"Trac_impactParRPhi", "Track impact parameter RPhi"});
    Trac_impactParZ_ = model->MakeField<std::vector<float>>({"Trac_impactParZ", "Track impact parameter Z"});
    Trac_thetaPerigee_ = model->MakeField<std::vector<float>>({"Trac_thetaPerigee", "Track theta perigee"});
    Trac_phiPerigee_ = model->MakeField<std::vector<float>>({"Trac_phiPerigee", "Track phi perigee"});
    Trac_curvaturePerigee_ = model->MakeField<std::vector<float>>({"Trac_curvaturePerigee", "Track curvature perigee"});
    Trac_weightMatrix_ = model->MakeField<std::vector<ROOT::Math::SMatrixSym5F>>({"Trac_weightMatrix", "Track weight matrix"});
    Trac_length_ = model->MakeField<std::vector<float>>({"Trac_length", "Track length"});
    Trac_detectors_ = model->MakeField<std::vector<int16_t>>({"Trac_detectors", "Track detectors"});
    Trac_rFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_rFirstPoint", "Track first point radius"});
    Trac_zFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_zFirstPoint", "Track first point z"});
    Trac_chi2NoVD_ = model->MakeField<std::vector<float>>({"Trac_chi2NoVD", "Track chi2 no vertex detector"});
    Trac_chi2VD_ = model->MakeField<std::vector<float>>({"Trac_chi2VD", "Track chi2 vertex detector"});
    Trac_ndfNoVD_ = model->MakeField<std::vector<int16_t>>({"Trac_ndfNoVD", "Track ndf no vertex detector"});
    Trac_ndfVD_ = model->MakeField<std::vector<int16_t>>({"Trac_ndfVD", "Track ndf vertex detector"});
    Trac_nHitVDRPhi_ = model->MakeField<std::vector<int16_t>>({"Trac_nHitVDRPhi", "Track number of hits in vertex detector RPhi"});
    Trac_nHitVDZ_ = model->MakeField<std::vector<int16_t>>({"Trac_nHitVDZ", "Track number of hits in vertex detector Z"});
    Trac_resRPhiFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_resRPhiFirstPoint", "Track residual RPhi first point"});
    Trac_errorResRPhiFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_errorResRPhiFirstPoint", "Track error residual RPhi first point"});
    Trac_resZFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_resZFirstPoint", "Track residual Z first point"});
    Trac_errorResZFirstPoint_ = model->MakeField<std::vector<float>>({"Trac_errorResZFirstPoint", "Track error residual Z first point"});
    Trac_impactParameterBeamSpotGeomSign_ = model->MakeField<std::vector<float>>({"Trac_impactParameterBeamSpotGeomSign", "Track impact parameter beam spot geometric sign"});
    Trac_impactParameterZGeomSign_ = model->MakeField<std::vector<float>>({"Trac_impactParameterZGeomSign", "Track impact parameter Z geometric sign"});
    Trac_impactParameterVertexGeomSign_ = model->MakeField<std::vector<float>>({"Trac_impactParameterVertexGeomSign", "Track impact parameter vertex geometric sign"});
    // Trac_energyError_ = model->MakeField<std::vector<float>>({"Trac_energyError", "Track energy error"});
    Trac_chi2VDHits_ = model->MakeField<std::vector<float>>({"Trac_chi2VDHits", "Chi2 vertex detector"});
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
    // Trac_energyError_->clear();
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
            // Trac_energyError_-> push_back(sk::QTRAC(41, i));
            Trac_chi2VDHits_-> push_back(sk::QTRAC(42, i));
        }
    }
}

void NanoAODWriter::defineMuid(std::unique_ptr<RNTupleModel> &model)
{
    Part_muidIdx_ = model->MakeField<std::vector<int16_t>>({"Part_muidIdx", "Particle muon ID index"});
    Muid_tag_ = model->MakeField<std::vector<int>>({"Muid_tag", "Muon ID tag"});
    Muid_looseChi2_ = model->MakeField<std::vector<float>>({"Muid_looseChi2", "Muon ID loose chi2"});
    Muid_hitPattern_ = model->MakeField<std::vector<int16_t>>({"Muid_hitPattern", "Muon ID hit pattern"});
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
    Part_elidIdx_ = model->MakeField<std::vector<int16_t>>({"Part_elidIdx", "Particle electron ID index"});
    Elid_tag_ = model->MakeField<std::vector<int>>({"Elec_tag", "Electron ID tag"});
    Elid_gammaConversion_ = model->MakeField<std::vector<int16_t>>({"Elec_gammaConversion", "Electron ID gamma conversion"});
    Elid_px_ = model->MakeField<std::vector<float>>({"Elec_px", "Best electron px estimation"});
    Elid_py_ = model->MakeField<std::vector<float>>({"Elec_py", "Best electron py estimation"});
    Elid_pz_ = model->MakeField<std::vector<float>>({"Elec_pz", "Best electron pz estimation"});
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
    Btag_probNegIP_ = model->MakeField<std::vector<float>>({"Btag_probNegIP", "B-tagging probability for tracks with negative impact parameter, hemisphere 1"}); 
    Btag_probPosIP_ = model->MakeField<std::vector<float>>({"Btag_probPosIP", "B-tagging probability for tracks with negative impact parameter, hemisphere 2"});
    Btag_probAllIP_ = model->MakeField<std::vector<float>>({"Btag_probAllIP", "B-tagging probability for tracks with negative impact parameter, whole event"});
    Btag_thrustVector_ = model->MakeField<XYZVectorF>({"Btag_thrustVector", "B-tagging probability for tracks with positive impact parameter, hemisphere 1"});
}

void NanoAODWriter::defineHadid(std::unique_ptr<RNTupleModel> &model)
{
    Haid_sign_ = model->MakeField<std::vector<int>>({"Haid_sign", "Used for combined tag"});
    Haid_kaonDedx_ = model->MakeField<std::vector<int>>({"Haid_kaonDedx", "Kaon signature with DEDX"});
    Haid_protonDedx_ = model->MakeField<std::vector<int>>({"Haid_protonDedx", "Proton signature with DEDX"});
    Haid_kaonRich_ = model->MakeField<std::vector<int>>({"Haid_kaonRich", "Kaon signature with RICH"});
    Haid_protonRich_ = model->MakeField<std::vector<int>>({"Haid_protonRich", "Proton signature with RICH"});
    Haid_pionRich_= model->MakeField<std::vector<int>>({"Haid_pionRich", "Pion singnature with RICH"});    
    Haid_kaonCombined_ = model->MakeField<std::vector<float>>({"Haid_kaonCombined", "Kaon signature with combined tag"});    
    Haid_protonCombined_ = model->MakeField<std::vector<float>>({"Haid_protonCombined", "Proton signature with combined tag"});
    Haid_richQuality_ = model->MakeField<std::vector<int>>({"Haid_richQuality", "RICH quality status"});

    Haidn_pionTag_ = model->MakeField<std::vector<int8_t>>({"Haidn_pionTag", "Pion tag"});
    Haidn_kaonTag_ = model->MakeField<std::vector<int8_t>>({"Haidn_kaonTag", "Kaon tag"});
    Haidn_protonTag_ = model->MakeField<std::vector<int8_t>>({"Haidn_protonTag", "Proton tag"});
    Haidn_heavyTag_ = model->MakeField<std::vector<int8_t>>({"Haidn_heavyTag", "Heavy particle tag"});
    Haidn_pionTrackSelection_ = model->MakeField<std::vector<int8_t>>({"Haidn_pionTrackSelection", "Pion track selection"});
    Haidn_kaonTrackSelection_ = model->MakeField<std::vector<int8_t>>({"Haidn_kaonTrackSelection", "Kaon track selection"});   
    Haidn_protonTrackSelection_ = model->MakeField<std::vector<int8_t>>({"Haidn_protonTrackSelection", "Proton track selection"}); 
    Haidn_heavyTrackSelection_ = model->MakeField<std::vector<int8_t>>({"Haidn_heavyTrackSelection", "Heavy particle track selection"});   

    Haidr_pionTag_ = model->MakeField<std::vector<int8_t>>({"Haidr_pionTag", "Pion tag"});
    Haidr_kaonTag_ = model->MakeField<std::vector<int8_t>>({"Haidr_kaonTag", "Kaon tag"});
    Haidr_protonTag_ = model->MakeField<std::vector<int8_t>>({"Haidr_protonTag", "Proton tag"});
    Haidr_heavyTag_ = model->MakeField<std::vector<int8_t>>({"Haidr_heavyTag", "Heavy particle tag"});
    Haidr_electronTag_ = model->MakeField<std::vector<int8_t>>({"Haidr_electronTag", "Electron tag"});
    Haidr_selectionFlag_ = model->MakeField<std::vector<int8_t>>({"Haidr_selectionFlag", "Selection flag"});

    Haide_pionTag_ = model->MakeField<std::vector<int8_t>>({"Haide_pionTag", "Pion tag"});
    Haide_kaonTag_ = model->MakeField<std::vector<int8_t>>({"Haide_kaonTag", "Kaon tag"});
    Haide_protonTag_ = model->MakeField<std::vector<int8_t>>({"Haide_protonTag", "Proton tag"});
    Haide_heavyTag_ = model->MakeField<std::vector<int8_t>>({"Haide_heavyTag", "Heavy particle tag"});
    Haide_electronTag_ = model->MakeField<std::vector<int8_t>>({"Haide_electronTag", "Electron tag"});
    Haide_selectionFlag_ = model->MakeField<std::vector<int8_t>>({"Haide_selectionFlag", "Selection flag"});

    Haidc_pionTag_ = model->MakeField<std::vector<int8_t>>({"Haidc_pionTag", "Pion tag"});
    Haidc_kaonTag_ = model->MakeField<std::vector<int8_t>>({"Haidc_kaonTag", "Kaon tag"});
    Haidc_protonTag_ = model->MakeField<std::vector<int8_t>>({"Haidc_protonTag", "Proton tag"});
    Haidc_heavyTag_ = model->MakeField<std::vector<int8_t>>({"Haidc_heavyTag", "Heavy particle tag"});
    Haidc_electronTag_ = model->MakeField<std::vector<int8_t>>({"Haidc_electronTag", "Electron tag"});
    Haidc_selectionFlag_ = model->MakeField<std::vector<int8_t>>({"Haidc_selectionFlag", "Selection flag"});

    Dedx_value_ = model->MakeField<std::vector<float>>({"Dedx_value", "Dedx value (1 fopr mips)"});
    Dedx_width_ = model->MakeField<std::vector<float>>({"Dedx_width", "Dedx Landau width"});
    Dedx_nrWires_ = model->MakeField<std::vector<int16_t>>({"Dedx_nrWires", "Dedx number of wires"});
    Dedx_gapWires_ = model->MakeField<std::vector<float>>({"Dedx_gapWires", "Mean distance between wires"});
    Dedx_error_ = model->MakeField<std::vector<float>>({"Dedx_error", "Dedx value error"});
    Dedx_valueVD_ = model->MakeField<std::vector<float>>({"Dedx_valueVD", "Dedx VD value"});
    Dedx_nrVDHits_ = model->MakeField<std::vector<int16_t>>({"Dedx_nrVDHits", "Number of VD hits"});

    Rich_theg_ = model->MakeField<std::vector<float>>({"Rich_theg", "Cherenkov angle gas radiator"});
    Rich_sigg_ = model->MakeField<std::vector<float>>({"Rich_sigg", "Sigma of Cherenvov angle gas radiator"});
    Rich_nphg_ = model->MakeField<std::vector<int16_t>>({"Rich_nphg", "Observerd number of photons gas radiator"});
    Rich_nepg_ = model->MakeField<std::vector<float>>({"Rich_nepg", "Expected number of photons gas radiator"});
    Rich_flagg_ = model->MakeField<std::vector<int16_t>>({"Rich_flagg", "Flag gas radiator"});
    Rich_thel_ = model->MakeField<std::vector<float>>({"Rich_thel", "Cherenkov angle liquid radiator"});
    Rich_sigl_ = model->MakeField<std::vector<float>>({"Rich_sigl", "Sigma of Cherenkov angle liquid radiator"});
    Rich_nphl_ = model->MakeField<std::vector<int16_t>>({"Rich_nphl", "Observerd number of photons liquid radiator"});
    Rich_nepl_ = model->MakeField<std::vector<float>>({"Rich_nepl", "Expected number of photons liquid radiator"});
    Rich_flagl_ = model->MakeField<std::vector<int16_t>>({"Rich_flagl", "Flag liquid radiator"});

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

void NanoAODWriter::fillBtag()
{
    fillVector<float>(Btag_probNegIP_, 1, 3, [](int i)
                      { return sk::QBTPRN(i); });
    fillVector<float>(Btag_probPosIP_, 1, 3, [](int i)
                      { return sk::QBTPRP(i); });
    fillVector<float>(Btag_probAllIP_, 1, 3, [](int i)
                      { return sk::QBTPRS(i); });

    *Btag_thrustVector_ = XYZVectorF(sk::QBTTHR(1), sk::QBTTHR(2), sk::QBTTHR(3));
}   

void NanoAODWriter::user99()
{
    // std::cout << "NanoAODWriter::user99: Finalising" << std::endl;
    writer_.reset();
};
