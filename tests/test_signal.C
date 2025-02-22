#include <iostream>
#include "Math/SVector.h"
#include "Math/SMatrix.h"
#include "TLorentzVector.h"
#include "Math/Vector4D.h"
#include "TBenchmark.h"
#include "TStopwatch.h"
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiM4D.h"



void test_signal() {
    Float_t pionMass;
    TLorentzVector TrackFourVectorA0;
    TLorentzVector TrackFourVectorA1;
    TLorentzVector TrackFourVectorB0;
    TLorentzVector TrackFourVectorB1;
    TLorentzVector TrackFourVectorB2;
    TLorentzVector TrackFourVectorB3;
    TLorentzVector TrackFourVectorC0;
    TLorentzVector TrackFourVectorC1;
    TLorentzVector TrackFourVectorC2;
    TLorentzVector TrackFourVectorD0;
    TLorentzVector TrackFourVectorD1;
    TLorentzVector TrackFourVectorD2;
    TLorentzVector TrackFourVectorD3;

    std::vector<TLorentzVector> TrackFourVectorA;
    std::vector<TLorentzVector> TrackFourVectorB;
    std::vector<TLorentzVector> TrackFourVectorC;
    std::vector<TLorentzVector> TrackFourVectorD;
    
    std::vector<std::vector<TLorentzVector>> AllTrackFourVector;
    std::vector<std::vector<TLorentzVector>> testAllTrackFourVector;

    double values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    TrackFourVectorA0.SetPtEtaPhiM(values[0], values[0], values[0], values[0]);
    TrackFourVectorA1.SetPtEtaPhiM(values[1], values[1], values[1], values[1]);

    TrackFourVectorB0.SetPtEtaPhiM(values[2], values[2], values[2], values[2]);
    TrackFourVectorB1.SetPtEtaPhiM(values[3], values[3], values[3], values[3]);
    TrackFourVectorB2.SetPtEtaPhiM(values[4], values[4], values[4], values[4]);
    TrackFourVectorB3.SetPtEtaPhiM(values[5], values[5], values[5], values[5]);
    
    TrackFourVectorC0.SetPtEtaPhiM(values[6], values[6], values[6], values[6]);
    TrackFourVectorC1.SetPtEtaPhiM(values[7], values[7], values[7], values[7]);
    TrackFourVectorC2.SetPtEtaPhiM(values[8], values[8], values[8], values[8]);
    
    TrackFourVectorD0.SetPtEtaPhiM(values[9], values[9], values[9], values[9]);
    TrackFourVectorD1.SetPtEtaPhiM(values[10], values[10], values[10], values[10]);
    TrackFourVectorD2.SetPtEtaPhiM(values[11], values[11], values[11], values[11]);
    TrackFourVectorD3.SetPtEtaPhiM(values[12], values[12], values[12], values[12]);

    TrackFourVectorA.push_back(TrackFourVectorA0);
    TrackFourVectorA.push_back(TrackFourVectorA1);
    TrackFourVectorB.push_back(TrackFourVectorB0);
    TrackFourVectorB.push_back(TrackFourVectorB1);
    TrackFourVectorB.push_back(TrackFourVectorB2);
    TrackFourVectorB.push_back(TrackFourVectorB3);
    TrackFourVectorC.push_back(TrackFourVectorC0);
    TrackFourVectorC.push_back(TrackFourVectorC1);
    TrackFourVectorC.push_back(TrackFourVectorC2);
    TrackFourVectorD.push_back(TrackFourVectorD0);
    TrackFourVectorD.push_back(TrackFourVectorD1);
    TrackFourVectorD.push_back(TrackFourVectorD2);
    TrackFourVectorD.push_back(TrackFourVectorD3);

    testAllTrackFourVector.push_back(TrackFourVectorA);
    testAllTrackFourVector.push_back(TrackFourVectorB);
    testAllTrackFourVector.push_back(TrackFourVectorC);
    testAllTrackFourVector.push_back(TrackFourVectorD);
    
    TStopwatch stopwatch, stopwatch1, stopwatch2;
    int poolSize = 3;
    
    std::cout << "antes: " << AllTrackFourVector.size() << std::endl;

    int totalElements = 0;

    std::cout << "0" << std::endl;

    std::vector<std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>>>> eventTracks;

    TBenchmark benchmark;

    //benchmark.Start("4-Vector");
    stopwatch.Start(kFALSE);
    for (Long64_t i = 0; i < testAllTrackFourVector.size(); i++) {

        std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>>> TrackFourVector;
        for (int j = 0; j < testAllTrackFourVector[i].size(); j++) {
            ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>> vec(values[j], values[j], values[j], pionMass);
            TrackFourVector.push_back(vec);
            // --- MIXING ---
            // Double-loop method
            // começar se o tamanho do vetor de eventos é igual ao poolSize -1
            if (eventTracks.size() == poolSize - 1) {
                // loop entre os eventos dentro do vetor de eventos
                for (int k = 0; k < eventTracks.size(); k++) {
                    // loop entre as tracks do evento
                    for (int z = 0; z < eventTracks[k].size(); z++) {
                    // mixar com a track que acamos de montar (vec)
                        std::cout << "i: " << i << " j: " << j << " || k: " << k << " z: " << z << std::endl; 
                    }
                }
            }

            // Single-loop method
            if (eventTracks.size() == poolSize - 1) {
                
            }
        }

        eventTracks.push_back(TrackFourVector);
        // depois de mixar resetamos o vetor de eventos para proxima poool
        if (eventTracks.size() == poolSize) eventTracks.clear();

        // --- SIGNAL ---
        if (TrackFourVector.size() > 1) {
            // Double-loop method
            stopwatch1.Start(kFALSE);
            for (size_t p1 = 0; p1 < TrackFourVector.size(); p1++) {
                for (size_t p2 = p1 + 1; p2 < TrackFourVector.size(); p2++) {
                    std::cout << "i: " << i << " p1: " << p1 << " p2: " << p2 << std::endl;
                }
            }
            stopwatch1.Stop();

            // Single-loop method
            stopwatch2.Start(kFALSE);
            for (int k = 0; k < TrackFourVector.size()*(TrackFourVector.size() - 1); k++) {
                int p1 = k / (TrackFourVector.size() - 1);
                int p2 = (k % (TrackFourVector.size() - 1)) + 1;

                if (p1 > p2) continue;
                std::cout << "i: " << i << " p1: " << p1 << " p2: " << p2 << std::endl;

            }
            stopwatch2.Stop();
        }
        
    }
    stopwatch.Stop();

    std::cout << "Benchmark Results:" << std::endl;
    stopwatch.Print();
    stopwatch1.Print();
    stopwatch2.Print();
    /*benchmark.Stop("4-Vector");

    std::cout << "depois: " << AllTrackFourVector.size() << std::endl;

    std::cout << std::endl;
    std::cout << "1" << std::endl;
    benchmark.Start("TripleLoop");
    for (int i = 0; i < AllTrackFourVector.size(); i++) {
        for (size_t p1 = 0; p1 < AllTrackFourVector[i].size(); p1++) {
            for (size_t p2 = p1 + 1; p2 < AllTrackFourVector[i].size(); p2++) {
                //AllTrackFourVector[i][p1].Print();
                //AllTrackFourVector[i][p2].Print();
            }
        }
    }
    benchmark.Stop("TripleLoop");

    std::cout << std::endl;
    std::cout << "2" << std::endl;
    benchmark.Start("DoubleLoop");
    
    for (int i = 0; i < AllTrackFourVector.size(); i++) {
        
        for (int k = 0; k < AllTrackFourVector[i].size()*(AllTrackFourVector[i].size() - 1); k++) {
            int p1 = k / (AllTrackFourVector[i].size() - 1);
            int p2 = (k % (AllTrackFourVector[i].size() - 1)) + 1;

            if (p2 > p1) {
                std::cout << "i: " << i << " p1: " << p1 << " p2: " << p2 << std::endl;
                //AllTrackFourVector[i][p1].Print();
                //AllTrackFourVector[i][p2].Print();
            }
        }
    }
    benchmark.Stop("DoubleLoop");

    std::cout << std::endl;
    std::cout << "3" << std::endl;
    std::cout << "total elements: " << totalElements << std::endl;
    benchmark.Start("SingleLoop");

    for (int j = 0, a = 0, b = 0, i = 0; j < totalElements; j++) {
        int trigger = AllTrackFourVector[i].size() * (AllTrackFourVector[i].size() - 1);

        int k = j - a;
        
        int p1 = k / (AllTrackFourVector[i].size() - 1);
        int p2 = (k % (AllTrackFourVector[i].size() - 1)) + 1;
        
        if (p2 > p1) {
            std::cout << "i: " << i << " p1: " << p1 << " p2: " << p2 << std::endl;
            //AllTrackFourVector[i][p1].Print();
            //AllTrackFourVector[i][p2].Print();
        }

        b++;
        if (b == trigger) {
            a = j + 1;
            b = 0;
            i++;
        }
    }
    benchmark.Stop("SingleLoop");

     std::cout << "Benchmark Results:" << std::endl;
    benchmark.Show("4-Vector");
    benchmark.Show("TripleLoop");
    benchmark.Show("DoubleLoop");
    benchmark.Show("SingleLoop"); */
}


/*
if (eventTracks.size() >= poolSize) { 
            for (int i = 0; i < eventTracks.size() - 1; i++) {
                for (int j = 0; j < eventTracks[i].size(); j++) {
                    for (int k = i+1; k < eventTracks.size(); k++) {
                        for (int z = 0; z < eventTracks[k].size(); z++) {
                            std::cout << "i: " << i << " j: " << j << " || k: " << k << " z: " << z << std::endl; 
                        }
                    }
                }
            }
            eventTracks.clear();
        }
*/