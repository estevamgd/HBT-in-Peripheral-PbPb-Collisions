#include <iostream>
#include "Math/SVector.h"
#include "Math/SMatrix.h"
#include "TLorentzVector.h"
#include "Math/Vector4D.h"



void test_signal() {
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
    
    std::cout << "antes: " << AllTrackFourVector.size() << std::endl;

    int totalElements = 0;

    std::cout << "0" << std::endl;
    for (Long64_t i = 0; i < testAllTrackFourVector.size(); i++) {

        std::vector<TLorentzVector> testTrackFourVector;
        
        for (int j = 0; j < testAllTrackFourVector[i].size(); j++) {
            TLorentzVector vec;
            
            vec.SetPtEtaPhiM(values[j], values[j], values[j], values[j]);
            testTrackFourVector.push_back(vec);
        }

        if (testTrackFourVector.size() > 1) {
            int theseElements = testTrackFourVector.size()*(testTrackFourVector.size()-1); 
            totalElements += theseElements;

            AllTrackFourVector.push_back(testTrackFourVector);
        }
        
    }
    std::cout << "depois: " << AllTrackFourVector.size() << std::endl;

    std::cout << std::endl;
    std::cout << "1" << std::endl;

    for (int i = 0; i < AllTrackFourVector.size(); i++) {
        for (size_t p1 = 0; p1 < AllTrackFourVector[i].size(); p1++) {
            for (size_t p2 = p1 + 1; p2 < AllTrackFourVector[i].size(); p2++) {
                std::cout << "i: " << i << " p1: " << p1 << " p2: " << p2 << std::endl;
                //AllTrackFourVector[i][p1].Print();
                //AllTrackFourVector[i][p2].Print();
            }
        }
    }

    std::cout << std::endl;
    std::cout << "2" << std::endl;
    
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
    std::cout << std::endl;
    std::cout << "3" << std::endl;
    std::cout << "total elements: " << totalElements << std::endl;
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
}
