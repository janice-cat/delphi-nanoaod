#ifndef NANOAOD_WRITER_HPP
#define NANOAOD_WRITER_HPP

#include <filesystem>
#include <vector>

#include "skelana_analysis.hpp"
#include "skelana.hpp"

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <Math/Vector4D.h>
#include <Math/Vector3D.h>
#include <Math/SMatrixFfwd.h>

using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
using RNTupleModel = ROOT::Experimental::RNTupleModel;

namespace sk = skelana;

typedef ROOT::Math::XYZTVectorF XYZTVectorF;
typedef ROOT::Math::DisplacementVector3D< ROOT::Math::Cartesian3D<float>, ROOT::Math::DefaultCoordinateSystemTag > XYZVectorF;
typedef ROOT::Math::PositionVector3D< ROOT::Math::Cartesian3D<float>, ROOT::Math::DefaultCoordinateSystemTag > XYZPointF;


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

    std::shared_ptr<int> Event_runNumber_;
    std::shared_ptr<int> Event_evtNumber_;
    std::shared_ptr<int> Event_fillNumber_;
    std::shared_ptr<int> Event_date_;
    std::shared_ptr<int> Event_time_;
    std::shared_ptr<float> Event_magField_;
    std::shared_ptr<float> Event_cmEnergy_;
    std::shared_ptr<int8_t> Event_shortDstVersion_;
    std::shared_ptr<int8_t> Event_hadronTag_;
    std::shared_ptr<int16_t> Event_nChaMultT4_;
    std::shared_ptr<int16_t> Event_nChaMult_;
    std::shared_ptr<int16_t> Event_nNeuMult_;
    std::shared_ptr<float> Event_totalChaEnergy_;
    std::shared_ptr<float> Event_totalEMEnergy_;
    std::shared_ptr<float> Event_totalHadEnergy_;
    std::shared_ptr<std::string> Event_DSTType_;

    std::shared_ptr<int16_t> nPart_;
    std::shared_ptr<std::vector<XYZTVectorF>> Part_vector_;
    std::shared_ptr<std::vector<int16_t>> Part_charge_;
    std::shared_ptr<std::vector<int16_t>> Part_pdgId_;
    std::shared_ptr<std::vector<int>> Part_massid_;
    std::shared_ptr<std::vector<int16_t>> Part_jetnr_;
    std::shared_ptr<std::vector<int>> Part_lock_;
    std::shared_ptr<std::vector<int16_t>> Part_simIdx_;
    std::shared_ptr<std::vector<int16_t>> Part_originVtxIdx_;
    std::shared_ptr<std::vector<int16_t>> Part_decayVtxIdx_;

    std::shared_ptr<int16_t> nJet_;
    std::shared_ptr<std::vector<XYZTVectorF>> Jet_vector_;
    std::shared_ptr<std::vector<int16_t>> Jet_charge_;

    std::shared_ptr<std::vector<XYZVectorF>> Thrust_vector_;

    std::shared_ptr<std::vector<XYZVectorF>> Sphericity_vector_;

    std::shared_ptr<int16_t> nSimPart_;
    std::shared_ptr<std::vector<XYZTVectorF>> SimPart_vector_;
    std::shared_ptr<std::vector<int16_t>> SimPart_charge_;
    std::shared_ptr<std::vector<int16_t>> SimPart_pdgId_;
    std::shared_ptr<std::vector<int16_t>> SimPart_partIdx_;
    std::shared_ptr<std::vector<int16_t>> SimPart_genIdx_;
    std::shared_ptr<std::vector<int16_t>> SimPart_originVtxIdx_;
    std::shared_ptr<std::vector<int16_t>> SimPart_decayVtxIdx_;

    std::shared_ptr<int16_t> nGenPart_;
    std ::shared_ptr<std::vector<int16_t>> GenPart_status_;
    std::shared_ptr<std::vector<int16_t>> GenPart_pdgId_;
    std::shared_ptr<std::vector<int16_t>> GenPart_parentIdx_;
    std::shared_ptr<std::vector<int16_t>> GenPart_firstChildIdx_;
    std::shared_ptr<std::vector<int16_t>> GenPart_lastChildIdx_;
    std::shared_ptr<std::vector<XYZTVectorF>> GenPart_vector_;
    std::shared_ptr<std::vector<XYZTVectorF>> GenPart_vertex_;
    std::shared_ptr<std::vector<float>> GenPart_tau_;
    std::shared_ptr<std::vector<int16_t>> GenPart_simIdx_;

    std::shared_ptr<int16_t> nVtx_;
    std::shared_ptr<std::vector<int16_t>> Vtx_firstOutIdx_;
    std::shared_ptr<std::vector<int16_t>> Vtx_firstInIdx_;
    std::shared_ptr<std::vector<int16_t>> Vtx_nOut_;
    std::shared_ptr<std::vector<int16_t>> Vtx_ndf_;
    std::shared_ptr<std::vector<int16_t>> Vtx_mcode_;
    std::shared_ptr<std::vector<XYZPointF>> Vtx_position_;
    std::shared_ptr<std::vector<float>> Vtx_chi2_;
    std::shared_ptr<std::vector<ROOT::Math::SMatrixSym3F>> Vtx_errorMatrix_;
    std::shared_ptr<std::vector<int16_t>> Vtx_errorFlag_;
    std::shared_ptr<std::vector<int16_t>> Vtx_status_;

    std::shared_ptr<int16_t> nSimVtx_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_firstOutIdx_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_firstInIdx_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_nOut_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_mcode_;
    std::shared_ptr<std::vector<XYZPointF>> SimVtx_vertex_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_errorFlag_;
    std::shared_ptr<std::vector<int16_t>> SimVtx_status_;

    // std::shared_ptr<int16_t> Part_tracIdx_;
    // std::shared_ptr<int16_t> Trac_originVtxIdx_;
    // std::shared_ptr<int16_t> Trac_decayVtxIdx_;
    // std::shared_ptr<float> Trac_impactParRPhi_;
    // std::shared_ptr<float> Trac_impactParZ_;
    // std::shared_ptr<float> Trac_thetaPerigee_;
    // std::shared_ptr<float> Trac_phiPerigee_;
    // std::shared_ptr<float> Trac_curvaturePerigee_;
    // std::shared_ptr<ROOT::Math::SMatrixSym5F> Trac_weightMatrix_;
    // std::shared_ptr<int16_t> Trac_detectors_;
    // std::shared_ptr<float> Trac_rFirstPoint_;
    // std::shared_ptr<float> Trac_zFirstPoint_;
    // std::shared_ptr<float> Trac_chi2NoVD_;
    // std::shared_ptr<float> Trac_chi2VD_;
    // std::shared_ptr<int16_t> Trac_ndfNoVD_;
    // std::shared_ptr<int16_t> Trac_ndfVD_;
    // std::shared_ptr<int16_t> Trac_nHitVDRPhi_;
    // std::shared_ptr<int16_t> Trac_nHitVDZ_;
    // std::shared_ptr<float> Trac_resRPhiFirstPoint_;
    // std::shared_ptr<float> Trac_errorResRPhiFirstPoint_;
    // std::shared_ptr<float> Trac_resZFirstPoint_;
    // std::shared_ptr<float> Trac_errorResZFirstPoint_;
    // std::shared_ptr<float> Trac_impactParameterVertexGeomSign_;
    // std::shared_ptr<float> Trac_impactParameterZGeomSign_;
    // std::shared_ptr<float> Trac_impactParameterBeamSpotGeomSign_;
    // std::shared_ptr<float> Trac_energyError_;
    // std::shared_ptr<float> Trac_chi2VD_;

    // std::shared_ptr<int16_t> Part_muidIdx_;
    // std::shared_ptr<int> Muid_tag_;
    // std::shared_ptr<float> Muid_looseChi2_;
    // std::shared_ptr<int16_t> Muid_hitPattern_;

    // std::shared_ptr<int16_t> Part_elidIdx_;
    // std::shared_ptr<int16_t> Elid_gammaConversion_;
    // std::shared_ptr<int16_t> Elid_px_;
    // std::shared_ptr<int16_t> Elid_py_;
    // std::shared_ptr<int16_t> Elid_pz_;

    // std::shared_ptr<int16_t> Part_haidIdx_;
    // std::shared_ptr<int16_t> Haid_pionTag_;
    // std::shared_ptr<int16_t> Haid_kaonTag_;
    // std::shared_ptr<int16_t> Haid_protonTag_;
    // std::shared_ptr<int16_t> Haid_heavyTag_;
    // std::shared_ptr<int16_t> Haid_pionSelection_;
    // std::shared_ptr<int16_t> Haid_kaonSelection_;
    // std::shared_ptr<int16_t> Haid_protonSelection_;
    // std::shared_ptr<int16_t> Haid_heavySelection_;


};

#endif // NANOAOD_WRITER_HPP