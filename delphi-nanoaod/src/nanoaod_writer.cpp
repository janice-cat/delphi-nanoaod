#include "nanoaod_writer.hpp"
#include "phdst.hpp"
#include <Math/PositionVector3D.h>
#include <Math/DisplacementVector3D.h>
#include <Math/SMatrix.h>

template <typename T>
void fillVector(std::shared_ptr<std::vector<T> > & ptr, int i, int n, std::function<T(int)> f)
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
    std::cout << "NanoAODWriter::user00: Initialising" << std::endl;
    super::user00();

    auto model = RNTupleModel::Create();
    Event_runNumber_ = model->MakeField<int>({"Event_runNumber", "Run number"});
    Event_evtNumber_ = model->MakeField<int>({"Event_evtNumber", "Event number"});
    Event_fillNumber_ = model->MakeField<int>({"Event_fillNumber", "Fill number"});
    Event_date_ = model->MakeField<int>({"Event_date", "Date"});
    Event_time_ = model->MakeField<int>({"Event_time", "Time"});
    Event_magField_ = model->MakeField<float>({"Event_magField", "Magnetic field"});
    Event_shortDstVersion_ = model->MakeField<int8_t>({"Event_shortDstVersion", "Short DST version"});
    Event_hadronTag_ = model->MakeField<int8_t>({"Event_hadronTag", "T4 Hadron tag"});
    Event_nChaMultT4_ = model->MakeField<int16_t>({"Event_nChaMultT4", "Number of charged particles with T4"});
    Event_nChaMult_ = model->MakeField<int16_t>({"Event_nChaMult", "Number of charged particles"});
    Event_nNeuMult_ = model->MakeField<int16_t>({"Event_nNeuMult", "Number of neutral particles"});
    Event_cmEnergy_ = model->MakeField<float>({"Event_cmEnergy", "Center of mass energy"});
    Event_totalChaEnergy_ = model->MakeField<float>({"Event_totalChaEnergy", "Total charged energy"});
    Event_totalEMEnergy_ = model->MakeField<float>({"Event_totalEMEnergy", "Total electromagnetic energy"});
    Event_totalHadEnergy_ = model->MakeField<float>({"Event_totalHadEnergy", "Total hadronic energy"});
    Event_DSTType_ = model->MakeField<std::string>({"Event_DSTType", "DST type"});

    nPart_ = model->MakeField<int16_t>({"nPart", "Number of particles"});
    Part_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"Part_vector", "Particle 4-vector"});
    Part_charge_ = model->MakeField<std::vector<int16_t>>({"Part_charge", "Particle charge"});
    Part_pdgId_ = model->MakeField<std::vector<int16_t>>({"Part_pdgId", "Particle PDG ID"});
    Part_massid_ = model->MakeField<std::vector<int>>({"Part_massid", "Particle mass ID"});
    Part_jetnr_ = model->MakeField<std::vector<int16_t>>({"Part_jetnr", "Particle jet number"});
    Part_lock_ = model->MakeField<std::vector<int>>({"Part_lock", "Particle lock"});
    if (mc_ && sk::IFLSIM > 0)
    {
        Part_simIdx_ = model->MakeField<std::vector<int16_t>>({"Part_simIdx", "Particle simulation index"});
        Part_originVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Part_originVtxIdx", "Particle origin vertex index"});
        Part_decayVtxIdx_ = model->MakeField<std::vector<int16_t>>({"Part_decayVtxIdx", "Particle decay vertex index"});
    }

    nJet_ = model->MakeField<int16_t>({"nJet", "Number of jets"});
    Jet_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"Jet_vector", "Jet Lorenz vector"});
    Jet_charge_ = model->MakeField<std::vector<int16_t>>({"Jet_charge", "Jet charge"});

    Thrust_vector_ = model->MakeField<std::vector<XYZVectorF>>({"Thrust_vector", "Thrust vector"});

    Sphericity_vector_ = model->MakeField<std::vector<XYZVectorF>>({"Sphericity_vector", "Sphericity vector"});

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

    if (mc_ && sk::IFLSIM > 0)
    {
        nSimPart_ = model->MakeField<int16_t>({"nSimPart", "Number of simulated particles"});
        SimPart_vector_ = model->MakeField<std::vector<XYZTVectorF>>({"SimPart_vector", "Simulated particle vector"});
        SimPart_pdgId_ = model->MakeField<std::vector<int16_t>>({"SimPart_pdgId", "Simulated particle PDG ID"});
        SimPart_partIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_partIdx", "Simulated particle index"});
        SimPart_genIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_genIdx", "Simulated particle generation index"});
        SimPart_originVtxIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_originVtxIdx", "Simulated particle origin vertex index"});
        SimPart_decayVtxIdx_ = model->MakeField<std::vector<int16_t>>({"SimPart_decayVtxIdx", "Simulated particle decay vertex index"});

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

        nSimVtx_ = model->MakeField<int16_t>({"nSimVtx", "Number of simulated vertices"});
        SimVtx_firstOutIdx_ = model->MakeField<std::vector<int16_t>>({"SimVtx_firstOutIdx", "Simulated vertex first outgoing particle index"});
        SimVtx_firstInIdx_ = model->MakeField<std::vector<int16_t>>({"SimVtx_firstInIdx", "Simulated vertex first incoming particle index"});
        SimVtx_nOut_ = model->MakeField<std::vector<int16_t>>({"SimVtx_nOut", "Simulated vertex number of outgoing particles"});
        SimVtx_mcode_ = model->MakeField<std::vector<int16_t>>({"SimVtx_mcode", "Simulated vertex MC code"});
        SimVtx_vertex_ = model->MakeField<std::vector<XYZPointF>>({"SimVtx_vertex", "Simulated vertex"});
        SimVtx_errorFlag_ = model->MakeField<std::vector<int16_t>>({"SimVtx_errorFlag", "Simulated vertex error flag"});
        SimVtx_status_ = model->MakeField<std::vector<int16_t>>({"SimVtx_status", "Simulated vertex status"});
    }

    writer_ = RNTupleWriter::Recreate(std::move(model), "Events", output_.string());
};

int NanoAODWriter::user01()
{
    std::cout << "NanoAODWriter::user01: Processing pilot " << phdst::NEVENT + 1 << std::endl;
    return super::user01();
};

void NanoAODWriter::user02()
{
    std::cout << "NanoAODWriter::user02: Processing event " << phdst::NEVENT + 1 << std::endl;
    super::user02();
    *Event_runNumber_ = phdst::IIIRUN;
    *Event_evtNumber_ = phdst::IIIEVT;
    *Event_fillNumber_ = phdst::IIFILL;
    *Event_date_ = phdst::IIIDAT;
    *Event_time_ = phdst::IIITIM;
    *Event_magField_ = sk::BMAG;
    *Event_shortDstVersion_ = sk::ISVER;
    *Event_hadronTag_ = sk::IHAD4;
    *Event_nChaMultT4_ = sk::NCTR4;
    *Event_nChaMult_ = sk::NCTRK;
    *Event_nNeuMult_ = sk::NNTRK;
    *Event_cmEnergy_ = sk::ECMAS;
    *Event_totalChaEnergy_ = sk::ECHAR;
    *Event_totalEMEnergy_ = sk::EMNEU;
    *Event_totalHadEnergy_ = sk::EHNEU;
    *Event_DSTType_ = sk::CDTYPE();

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

    *nJet_ = sk::NJET;
    fillVector<XYZTVectorF>(Jet_vector_, sk::LVJET, sk::NJET, [](int i)
                                          { return XYZTVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i), sk::VECP(4, i)); });
    fillVector<int16_t>(Jet_charge_, sk::LVJET, sk::NJET, [](int i)
                                      { return int(sk::VECP(6, i)); });

    fillVector<XYZVectorF>(Thrust_vector_, sk::LVTHRU, 3, [](int i)
                                            { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });

    fillVector<XYZVectorF>(Sphericity_vector_, sk::LVSPHE, 3, [](int i)
                                                { return XYZVectorF(sk::VECP(1, i), sk::VECP(2, i), sk::VECP(3, i)); });

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

    if (mc_ && sk::IFLSIM > 0)
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

    writer_->Fill();
};

void NanoAODWriter::user99()
{
    std::cout << "NanoAODWriter::user99: Finalising" << std::endl;
    writer_.reset();
};
