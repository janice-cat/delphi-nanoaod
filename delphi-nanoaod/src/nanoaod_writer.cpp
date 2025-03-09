#include "nanoaod_writer.hpp"
#include "phdst.hpp"

template <typename T>
std::shared_ptr<std::vector<T>> makeVector(int i, int n, std::function<T(int)> f)
{
    auto v = std::make_shared<std::vector<T>>();
    std::generate_n(std::back_inserter(*v), n, [i, f]() mutable
                    { return f(i++); });
    return v;
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

    auto model = RNTupleModel::Create();
    // Event_nrun_ = model->MakeField<int>("Event_nrun", "Run number");
    // Event_ievt_ = model->MakeField<int>("Event_ievt", "Event number");
    // Event_fill_ = model->MakeField<int>("Event_fill", "Fill number");
    // Event_magField_ = model->MakeField<float>("Event_magField", "Magnetic field");
    // Event_dstVersion_ = model->MakeField<int>("Event_dstVersion", "DST version");
    // Event_hadronTag_ = model->MakeField<int>("Event_hadronTag", "T4 Hadron tag");
    // Event_nChaMultT4_ = model->MakeField<int>("Event_nChaMultT4", "Number of charged particles with T4");
    // Event_nChaMult_ = model->MakeField<int>("Event_nChaMult", "Number of charged particles");
    // Event_nNeuMult_ = model->MakeField<int>("Event_nNeuMult", "Number of neutral particles");
    // Event_cmEnergy_ = model->MakeField<float>("Event_cmEnergy", "Center of mass energy");
    // Event_totalChaEnergy_ = model->MakeField<float>("Event_totalChaEnergy", "Total charged energy");
    // Event_totalEMEnergy_ = model->MakeField<float>("Event_totalEMEnergy", "Total electromagnetic energy");
    // Event_totalHadEnergy_ = model->MakeField<float>("Event_totalHadEnergy", "Total hadronic energy");
    // Event_DSTType_ = model->MakeField<std::string>("Event_DSTType", "DST type");

    Event_nrun_ = model->MakeField<int>("Event_nrun");
    Event_ievt_ = model->MakeField<int>("Event_ievt");
    Event_fill_ = model->MakeField<int>("Event_fill");
    Event_magField_ = model->MakeField<float>("Event_magField");
    Event_dstVersion_ = model->MakeField<int>("Event_dstVersion");
    Event_hadronTag_ = model->MakeField<int>("Event_hadronTag");
    Event_nChaMultT4_ = model->MakeField<int>("Event_nChaMultT4");
    Event_nChaMult_ = model->MakeField<int>("Event_nChaMult");
    Event_nNeuMult_ = model->MakeField<int>("Event_nNeuMult");
    Event_cmEnergy_ = model->MakeField<float>("Event_cmEnergy");
    Event_totalChaEnergy_ = model->MakeField<float>("Event_totalChaEnergy");
    Event_totalEMEnergy_ = model->MakeField<float>("Event_totalEMEnergy");
    Event_totalHadEnergy_ = model->MakeField<float>("Event_totalHadEnergy");
    Event_DSTType_ = model->MakeField<std::string>("Event_DSTType");

    // nPart_ = model->MakeField<int>("nPart", "Number of particles");
    // Part_px_ = model->MakeField<std::vector<float>>("Part_px", "Particle x-momentum");
    // Part_py_ = model->MakeField<std::vector<float>>("Part_py", "Particle y-momentum");
    // Part_pz_ = model->MakeField<std::vector<float>>("Part_pz", "Particle z-momentum");
    // Part_energy_ = model->MakeField<std::vector<float>>("Part_energy", "Particle energy");
    // Part_mass_ = model->MakeField<std::vector<float>>("Part_mass", "Particle mass");
    // Part_p_ = model->MakeField<std::vector<float>>("Part_p", "Particle momentum");
    // Part_charge_ = model->MakeField<std::vector<float>>("Part_charge", "Particle charge");
    // Part_pdgId_ = model->MakeField<std::vector<float>>("Part_pdgId", "Particle PDG ID");
    // Part_massid_ = model->MakeField<std::vector<int>>("Part_massid", "Particle mass ID");
    // Part_jetnr_ = model->MakeField<std::vector<int>>("Part_jetnr", "Particle jet number");
    // Part_lock_ = model->MakeField<std::vector<int>>("Part_lock", "Particle lock");
    // if (mc_)
    // {
    //     Part_simIdx_ = model->MakeField<std::vector<int>>("Part_simIdx", "Particle simulation index");
    //     Part_originVtxIdx_ = model->MakeField<std::vector<int>>("Part_originVtxIdx", "Particle origin vertex index");
    //     Part_decayVtxIdx_ = model->MakeField<std::vector<int>>("Part_decayVtxIdx", "Particle decay vertex index");
    // }

    // nJet_ = model->MakeField<int>("nJet", "Number of jets");
    // Jet_px_ = model->MakeField<std::vector<float>>("Jet_px", "Jet x-momentum");
    // Jet_py_ = model->MakeField<std::vector<float>>("Jet_py", "Jet y-momentum");
    // Jet_pz_ = model->MakeField<std::vector<float>>("Jet_pz", "Jet z-momentum");
    // Jet_energy_ = model->MakeField<std::vector<float>>("Jet_energy", "Jet energy");
    // Jet_mass_ = model->MakeField<std::vector<float>>("Jet_mass", "Jet mass");
    // Jet_p_ = model->MakeField<std::vector<float>>("Jet_p", "Jet momentum");
    // Jet_charge_ = model->MakeField<std::vector<float>>("Jet_charge", "Jet charge");

    // Thrust_x_ = model->MakeField<std::vector<float>>("Thrust_x", "Thrust x-component");
    // Thrust_y_ = model->MakeField<std::vector<float>>("Thrust_y", "Thrust y-component");
    // Thrust_z_ = model->MakeField<std::vector<float>>("Thrust_z", "Thrust z-component");
    // Thrust_norm_ = model->MakeField<std::vector<float>>("Thrust_norm", "Thrust norm");

    // Sphericity_x_ = model->MakeField<std::vector<float>>("Sphericity_x", "Sphericity x-component");
    // Sphericity_y_ = model->MakeField<std::vector<float>>("Sphericity_y", "Sphericity y-component");
    // Sphericity_z_ = model->MakeField<std::vector<float>>("Sphericity_z", "Sphericity z-component");
    // Sphericity_norm_ = model->MakeField<std::vector<float>>("Sphericity_norm", "Sphericity norm");

    // nVtx_ = model->MakeField<int>("nVtx", "Number of vertices");
    // Vtx_firstOutIdx_ = model->MakeField<std::vector<int>>("Vtx_firstOutIdx", "Vertex first outgoing particle index");
    // Vtx_firstInIdx_ = model->MakeField<std::vector<int>>("Vtx_firstInIdx", "Vertex first incoming particle index");
    // Vtx_nOut_ = model->MakeField<std::vector<int>>("Vtx_nOut", "Vertex number of outgoing particles");
    // Vtx_ndf_ = model->MakeField<std::vector<int>>("Vtx_ndf", "Vertex number of degrees of freedom");
    // Vtx_mcode_ = model->MakeField<std::vector<int>>("Vtx_mcode", "Vertex MC code");
    // Vtx_x_ = model->MakeField<std::vector<float>>("Vtx_x", "Vertex x-coordinate");
    // Vtx_y_ = model->MakeField<std::vector<float>>("Vtx_y", "Vertex y-coordinate");
    // Vtx_z_ = model->MakeField<std::vector<float>>("Vtx_z", "Vertex z-coordinate");
    // Vtx_errXX_ = model->MakeField<std::vector<float>>("Vtx_errXX", "Vertex error xx");
    // Vtx_errXY_ = model->MakeField<std::vector<float>>("Vtx_errXY", "Vertex error xy");
    // Vtx_errYY_ = model->MakeField<std::vector<float>>("Vtx_errYY", "Vertex error yy");
    // Vtx_errXY_ = model->MakeField<std::vector<float>>("Vtx_errXY", "Vertex error xy");
    // Vtx_errXZ_ = model->MakeField<std::vector<float>>("Vtx_errXZ", "Vertex error xz");
    // Vtx_errYZ_ = model->MakeField<std::vector<float>>("Vtx_errYZ", "Vertex error yz");
    // Vtx_errZZ_ = model->MakeField<std::vector<float>>("Vtx_errZZ", "Vertex error zz");
    // Vtx_errorFlag_ = model->MakeField<std::vector<int>>("Vtx_errorFlag", "Vertex error flag");
    // Vtx_status_ = model->MakeField<std::vector<int>>("Vtx_status", "Vertex status");

    // if (mc_)
    // {
    //     nSimPart_ = model->MakeField<int>("nSimPart", "Number of simulated particles");
    //     SimPart_px_ = model->MakeField<std::vector<float>>("SimPart_px", "Simulated particle x-momentum");
    //     SimPart_py_ = model->MakeField<std::vector<float>>("SimPart_py", "Simulated particle y-momentum");
    //     SimPart_pz_ = model->MakeField<std::vector<float>>("SimPart_pz", "Simulated particle z-momentum");
    //     SimPart_energy_ = model->MakeField<std::vector<float>>("SimPart_energy", "Simulated particle energy");
    //     SimPart_mass_ = model->MakeField<std::vector<float>>("SimPart_mass", "Simulated particle mass");
    //     SimPart_pdgId_ = model->MakeField<std::vector<float>>("SimPart_pdgId", "Simulated particle PDG ID");
    //     SimPart_partIdx_ = model->MakeField<std::vector<int>>("SimPart_partIdx", "Simulated particle index");
    //     SimPart_genIdx_ = model->MakeField<std::vector<int>>("SimPart_genIdx", "Simulated particle generation index");
    //     SimPart_originVtxIdx_ = model->MakeField<std::vector<int>>("SimPart_originVtxIdx", "Simulated particle origin vertex index");
    //     SimPart_decayVtxIdx_ = model->MakeField<std::vector<int>>("SimPart_decayVtxIdx", "Simulated particle decay vertex index");

    //     nGenPart_ = model->MakeField<int>("nGenPart", "Number of generated particles");
    //     GenPart_status_ = model->MakeField<std::vector<int>>("GenPart_status", "Generated particle status");
    //     GenPart_pdgId_ = model->MakeField<std::vector<int>>("GenPart_pdgId", "Generated particle PDG ID");
    //     GenPart_parentIdx_ = model->MakeField<std::vector<int>>("GenPart_parentIdx", "Generated particle parent index");
    //     GenPart_firstChildIdx_ = model->MakeField<std::vector<int>>("GenPart_firstChildIdx", "Generated particle first child index");
    //     GenPart_lastChildIdx_ = model->MakeField<std::vector<int>>("GenPart_lastChildIdx", "Generated particle last child index");
    //     GenPart_px_ = model->MakeField<std::vector<float>>("GenPart_px", "Generated particle x-momentum");
    //     GenPart_py_ = model->MakeField<std::vector<float>>("GenPart_py", "Generated particle y-momentum");
    //     GenPart_pz_ = model->MakeField<std::vector<float>>("GenPart_pz", "Generated particle z-momentum");
    //     GenPart_energy_ = model->MakeField<std::vector<float>>("GenPart_energy", "Generated particle energy");
    //     GenPart_mass_ = model->MakeField<std::vector<float>>("GenPart_mass", "Generated particle mass");
    //     GenPart_vx_ = model->MakeField<std::vector<float>>("GenPart_vx", "Generated particle vertex x");
    //     GenPart_vy_ = model->MakeField<std::vector<float>>("GenPart_vy", "Generated particle vertex y");
    //     GenPart_vz_ = model->MakeField<std::vector<float>>("GenPart_vz", "Generated particle vertex z");
    //     GenPart_vt_ = model->MakeField<std::vector<float>>("GenPart_vt", "Generated particle vertex time");
    //     GenPart_tau_ = model->MakeField<std::vector<float>>("GenPart_tau", "Generated particle lifetime");
    //     GenPart_simIdx_ = model->MakeField<std::vector<float>>("GenPart_simIdx", "Generated particle simulation index");

    //     nSimVtx_ = model->MakeField<int>("nSimVtx", "Number of simulated vertices");
    //     SimVtx_firstOutIdx_ = model->MakeField<std::vector<int>>("SimVtx_firstOutIdx", "Simulated vertex first outgoing particle index");
    //     SimVtx_firstInIdx_ = model->MakeField<std::vector<int>>("SimVtx_firstInIdx", "Simulated vertex first incoming particle index");
    //     SimVtx_nOut_ = model->MakeField<std::vector<int>>("SimVtx_nOut", "Simulated vertex number of outgoing particles");
    //     SimVtx_mcode_ = model->MakeField<std::vector<int>>("SimVtx_mcode", "Simulated vertex MC code");
    //     SimVtx_x_ = model->MakeField<std::vector<float>>("SimVtx_x", "Simulated vertex x-coordinate");
    //     SimVtx_y_ = model->MakeField<std::vector<float>>("SimVtx_y", "Simulated vertex y-coordinate");
    //     SimVtx_z_ = model->MakeField<std::vector<float>>("SimVtx_z", "Simulated vertex z-coordinate");
    //     SimVtx_errorFlag_ = model->MakeField<std::vector<int>>("SimVtx_errorFlag", "Simulated vertex error flag");
    //     SimVtx_status_ = model->MakeField<std::vector<int>>("SimVtx_status", "Simulated vertex status");
    // }

    // writer_ = RNTupleWriter::Recreate(std::move(model), "Events", output_.string());
};

int NanoAODWriter::user01()
{
    return super::user01();
};

void NanoAODWriter::user02()
{

    *Event_nrun_ = phdst::IIIRUN;
    *Event_ievt_ = phdst::IIIEVT;
    *Event_fill_ = phdst::IIFILL;
    *Event_magField_ = sk::BMAG;
    *Event_dstVersion_ = sk::ISVER;
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
    // Part_px_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                              { return sk::VECP(1, i); });
    // Part_py_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                              { return sk::VECP(2, i); });
    // Part_pz_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                              { return sk::VECP(3, i); });
    // Part_energy_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(4, i); });
    // Part_mass_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(5, i); });
    // Part_mass_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(6, i); });
    // Part_charge_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(7, i); });
    // Part_pdgId_ = makeVector<float>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(8, i); });
    // Part_massid_ = makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(9, i); });
    // Part_jetnr_= makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::VECP(10, i); });
    // Part_lock_ = makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                      { return sk::LVLOCK(i); });
    // // if (mc_)
    // {
    //     Part_simIdx_ = makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                                   { return sk::IPAST(i); });
    //     Part_originVtxIdx_ = makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                                   { return sk::IPAPV(1,i); });
    //     Part_decayVtxIdx_ = makeVector<int>(sk::LVPART, sk::NVECP, [](int i)
    //                                   { return sk::IPAPV(2,i); });
    // }
    *nJet_ = sk::NJET;
    // Jet_px_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                             { return sk::VECP(1, i); });
    // Jet_py_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                             { return sk::VECP(2, i); });
    // Jet_pz_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                             { return sk::VECP(3, i); });
    // Jet_energy_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                                 { return sk::VECP(4, i); });
    // Jet_mass_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                               { return sk::VECP(5, i); });
    // Jet_charge_ = makeVector<float>(sk::LVJET, sk::NJET, [](int i)
    //                                { return sk::VECP(6, i); });



    writer_->Fill();
};

void NanoAODWriter::user99()
{
};
