#!/usr/bin/env python3

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Simple plots')
    parser.add_argument('--input', help='Input file', required=True)
    parser.add_argument('--output', help='Output file')
    args = parser.parse_args()        
        
    reader = ROOT.Experimental.RNTupleReader.Open("Events",args.input)
    nrEntries = reader.GetNEntries()
    Event_runNumber = reader.GetView[int]("Event_runNumber")
    Event_evtNumber = reader.GetView[int]("Event_evtNumber")
    Event_fillNumber = reader.GetView[int]("Event_fillNumber")
    Event_time = reader.GetView[int]("Event_time")
    Event_date = reader.GetView[int]("Event_date")
    Event_magField = reader.GetView[float]("Event_magField")
    Event_shortDstVersion = reader.GetView[int]("Event_shortDstVersion")                                         
    Event_hadronTag  = reader.GetView[int]("Event_hadronTag")                                              
    Event_nChaMultT4 = reader.GetView[int]("Event_nChaMultT4")
    Event_nChaMult = reader.GetView[int]("Event_nChaMult")
    Event_nNeuMult = reader.GetView[int]("Event_nNeuMult")
    Event_cmEnergy = reader.GetView[float]("Event_cmEnergy")
    Event_totalChaEnergy = reader.GetView[float]("Event_totalChaEnergy")
    Event_totalEMEnergy = reader.GetView[float]("Event_totalEMEnergy")
    Event_totalHadEnergy = reader.GetView[float]("Event_totalHadEnergy")
    Event_DSTType = reader.GetView[str]("Event_DSTType")                         
                    
    nPart = reader.GetView[int]("nPart")
    Part_charge = reader.GetView["std::vector<int16_t>"]("Part_charge")
    Part_decayVtxIdx = reader.GetView["std::vector<int16_t>"]("Part_decayVtxIdx")
    Part_jetnr = reader.GetView["std::vector<int16_t>"]("Part_jetnr")
    Part_lock = reader.GetView["std::vector<int32_t>"]("Part_lock")
    Part_massid = reader.GetView["std::vector<int32_t>"]("Part_massid")
    Part_originVtxIdx = reader.GetView["std::vector<int16_t>"]("Part_originVtxIdx")
    Part_pdgId = reader.GetView["std::vector<int16_t>"]("Part_pdgId")
    Part_simIdx = reader.GetView["std::vector<int16_t>"]("Part_simIdx")
    # Part_vector = reader.GetView["std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>"]("Part_vector")                    
                             
    for i in range(nrEntries):
        print(f"Event {i}")
        print( f"   Event_runNumber: {Event_runNumber(i)}")
        print( f"   Event_evtNumber: {Event_evtNumber(i)}")
        print( f"   Event_fillNumber: {Event_fillNumber(i)}")
        print( f"   Event_time: {Event_time(i)}")
        print( f"   Event_date: {Event_date(i)}")
        print( f"   Event_magField: {Event_magField(i)}")
        print( f"   Event_shortDstVersion: {Event_shortDstVersion(i)}")
        print( f"   Event_hadronTag: {Event_hadronTag(i)}")
        print( f"   Event_nChaMultT4: {Event_nChaMultT4(i)}")
        print( f"   Event_nChaMult: {Event_nChaMult(i)}")
        print( f"   Event_nNeuMult: {Event_nNeuMult(i)}")
        print( f"   Event_cmEnergy: {Event_cmEnergy(i)}")
        print( f"   Event_totalChaEnergy: {Event_totalChaEnergy(i)}")
        print( f"   Event_totalEMEnergy: {Event_totalEMEnergy(i)}")
        print( f"   Event_totalHadEnergy: {Event_totalHadEnergy(i)}")
        print( f"   Event_DSTType: {Event_DSTType(i)}")
        print()
        print( f"   nPart: {nPart(i)}")
        print(  f"   Part_charge: {Part_charge(i)}")
        for i in range(nPart(i)):
            # print( f"      Part_vector: {Part_vector(i)[i]}")
            print( f"      Part_charge: {Part_charge(i)[i]}")
            # print( f"      Part_decayVtxIdx: {Part_decayVtxIdx(i)[i]}")
            # print( f"      Part_jetnr: {Part_jetnr(i)[i]}")
            # print( f"      Part_lock: {Part_lock(i)[i]}")
            # print( f"      Part_massid: {Part_massid(i)[i]}")
            # print( f"      Part_originVtxIdx: {Part_originVtxIdx(i)[i]}")
            # print( f"      Part_pdgId: {Part_pdgId(i)[i]}")
            # print( f"      Part_simIdx: {Part_simIdx(i)[i]}")

            print()

    # else it crashes with a segfault
    del Event_date
    del Event_time
    del Event_fillNumber
    del Event_evtNumber
    del Event_runNumber
    del Event_magField
    del Event_shortDstVersion
    del Event_hadronTag
    del Event_nChaMultT4
    del Event_nChaMult
    del Event_nNeuMult
    del Event_cmEnergy
    del Event_totalChaEnergy
    del Event_totalEMEnergy
    del Event_totalHadEnergy
    del Event_DSTType
    del nPart
    del Part_charge
    del Part_decayVtxIdx
    del Part_jetnr
    del Part_lock
    del Part_massid
    del Part_originVtxIdx
    del Part_pdgId
    del Part_simIdx
    # del Part_vector
                
if __name__ == '__main__':
    main()