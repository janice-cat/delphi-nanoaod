#ifndef NANOAOD_WRITER_HPP
#define NANOAOD_WRITER_HPP

#include <filesystem>
#include <vector>

#include "skelana_analysis.hpp"
#include "skelana.hpp"

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriter.hxx>

using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
using RNTupleModel = ROOT::Experimental::RNTupleModel;

namespace sk = skelana;

class NanoAODWriter : public sk::Analysis
{
private:
    typedef sk::Analysis super;

public:
    NanoAODWriter(const NanoAODWriter &) = delete;
    NanoAODWriter &operator=(const NanoAODWriter &) = delete;
    virtual ~NanoAODWriter();
    static NanoAODWriter *getInstance();
    void setOutput(const std::filesystem::path &output);
    void setMC();

protected:
    NanoAODWriter();
    virtual void user00();
    virtual int user01();
    virtual void user02();
    virtual void user99();

private:
    std::filesystem::path output_;
    std::unique_ptr<RNTupleWriter> writer_;
    bool mc_;

    std::shared_ptr<int> Event_nrun_;
    std::shared_ptr<int> Event_ievt_;
    std::shared_ptr<int> Event_fill_;
    std::shared_ptr<float> Event_magField_;
    std::shared_ptr<float> Event_cmEnergy_;
    std::shared_ptr<int> Event_dstVersion_;
    std::shared_ptr<int> Event_hadronTag_;
    std::shared_ptr<int> Event_nChaMultT4_;
    std::shared_ptr<int> Event_nChaMult_;
    std::shared_ptr<int> Event_nNeuMult_;
    std::shared_ptr<float> Event_totalChaEnergy_;
    std::shared_ptr<float> Event_totalEMEnergy_;
    std::shared_ptr<float> Event_totalHadEnergy_;
    std::shared_ptr<std::string> Event_DSTType_;

    std::shared_ptr<int> nPart_;
    std::shared_ptr<std::vector<float>> Part_px_;
    std::shared_ptr<std::vector<float>> Part_py_;
    std::shared_ptr<std::vector<float>> Part_pz_;
    std::shared_ptr<std::vector<float>> Part_energy_;
    std::shared_ptr<std::vector<float>> Part_mass_;
    std::shared_ptr<std::vector<float>> Part_p_;
    std::shared_ptr<std::vector<float>> Part_charge_;
    std::shared_ptr<std::vector<float>> Part_pdgId_;
    std::shared_ptr<std::vector<int>> Part_massid_;
    std::shared_ptr<std::vector<int>> Part_jetnr_;
    std::shared_ptr<std::vector<int>> Part_lock_;
    std::shared_ptr<std::vector<int>> Part_simIdx_;
    std::shared_ptr<std::vector<int>> Part_originVtxIdx_;
    std::shared_ptr<std::vector<int>> Part_decayVtxIdx_;

    std::shared_ptr<int> nJet_;
    std::shared_ptr<std::vector<float>> Jet_px_;
    std::shared_ptr<std::vector<float>> Jet_py_;
    std::shared_ptr<std::vector<float>> Jet_pz_;
    std::shared_ptr<std::vector<float>> Jet_energy_;
    std::shared_ptr<std::vector<float>> Jet_mass_;
    std::shared_ptr<std::vector<float>> Jet_p_;
    std::shared_ptr<std::vector<float>> Jet_charge_;

    std::shared_ptr<std::vector<float>> Thrust_x_;
    std::shared_ptr<std::vector<float>> Thrust_y_;
    std::shared_ptr<std::vector<float>> Thrust_z_;
    std::shared_ptr<std::vector<float>> Thrust_norm_;

    std::shared_ptr<std::vector<float>> Sphericity_x_;
    std::shared_ptr<std::vector<float>> Sphericity_y_;
    std::shared_ptr<std::vector<float>> Sphericity_z_;
    std::shared_ptr<std::vector<float>> Sphericity_norm_;

    std::shared_ptr<int> nSimPart_;
    std::shared_ptr<std::vector<float>> SimPart_px_;
    std::shared_ptr<std::vector<float>> SimPart_py_;
    std::shared_ptr<std::vector<float>> SimPart_pz_;
    std::shared_ptr<std::vector<float>> SimPart_energy_;
    std::shared_ptr<std::vector<float>> SimPart_mass_;
    std::shared_ptr<std::vector<float>> SimPart_p_;
    std::shared_ptr<std::vector<float>> SimPart_charge_;
    std::shared_ptr<std::vector<float>> SimPart_pdgId_;
    std::shared_ptr<std::vector<int>> SimPart_partIdx_;
    std::shared_ptr<std::vector<int>> SimPart_genIdx_;
    std::shared_ptr<std::vector<int>> SimPart_originVtxIdx_;
    std::shared_ptr<std::vector<int>> SimPart_decayVtxIdx_;

    std::shared_ptr<int> nGenPart_;
    std ::shared_ptr<std::vector<int>> GenPart_status_;
    std::shared_ptr<std::vector<int>> GenPart_pdgId_;
    std::shared_ptr<std::vector<int>> GenPart_parentIdx_;
    std::shared_ptr<std::vector<int>> GenPart_firstChildIdx_;
    std::shared_ptr<std::vector<int>> GenPart_lastChildIdx_;
    std::shared_ptr<std::vector<float>> GenPart_px_;
    std::shared_ptr<std::vector<float>> GenPart_py_;
    std::shared_ptr<std::vector<float>> GenPart_pz_;
    std::shared_ptr<std::vector<float>> GenPart_energy_;
    std::shared_ptr<std::vector<float>> GenPart_mass_;
    std::shared_ptr<std::vector<float>> GenPart_vx_;
    std::shared_ptr<std::vector<float>> GenPart_vy_;
    std::shared_ptr<std::vector<float>> GenPart_vz_;
    std::shared_ptr<std::vector<float>> GenPart_vt_;
    std::shared_ptr<std::vector<float>> GenPart_tau_;
    std::shared_ptr<std::vector<float>> GenPart_simIdx_;

    std::shared_ptr<int> nVtx_;
    std::shared_ptr<std::vector<int>> Vtx_firstOutIdx_;
    std::shared_ptr<std::vector<int>> Vtx_firstInIdx_;
    std::shared_ptr<std::vector<int>> Vtx_nOut_;
    std::shared_ptr<std::vector<int>> Vtx_ndf_;
    std::shared_ptr<std::vector<int>> Vtx_mcode_;
    std::shared_ptr<std::vector<float>> Vtx_x_;
    std::shared_ptr<std::vector<float>> Vtx_y_;
    std::shared_ptr<std::vector<float>> Vtx_z_;
    std::shared_ptr<std::vector<float>> Vtx_errXX_;
    std::shared_ptr<std::vector<float>> Vtx_errXY_;
    std::shared_ptr<std::vector<float>> Vtx_errYY_;
    std::shared_ptr<std::vector<float>> Vtx_errXZ_;
    std::shared_ptr<std::vector<float>> Vtx_errYZ_;
    std::shared_ptr<std::vector<float>> Vtx_errZZ_;
    std::shared_ptr<std::vector<int>> Vtx_errorFlag_;
    std::shared_ptr<std::vector<int>> Vtx_status_;

    std::shared_ptr<int> nSimVtx_;
    std::shared_ptr<std::vector<int>> SimVtx_firstOutIdx_;
    std::shared_ptr<std::vector<int>> SimVtx_firstInIdx_;
    std::shared_ptr<std::vector<int>> SimVtx_nOut_;
    std::shared_ptr<std::vector<int>> SimVtx_mcode_;
    std::shared_ptr<std::vector<float>> SimVtx_x_;
    std::shared_ptr<std::vector<float>> SimVtx_y_;
    std::shared_ptr<std::vector<float>> SimVtx_z_;
    std::shared_ptr<std::vector<int>> SimVtx_errorFlag_;
    std::shared_ptr<std::vector<int>> SimVtx_status_;
};

#endif // NANOAOD_WRITER_HPP