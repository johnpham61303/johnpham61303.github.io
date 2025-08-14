#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include "AmazonTruck.h"
#include "Liquid.h"

bool AmazonTruck::isFull() {
    if (volume == 0){
        return true;
    }
    else {
        return false;
    }
}

void AmazonTruck::makeEmpty() {
    truck->clear();
}

unsigned short AmazonTruck::totalVolumeSoFar() {
    return volume;
}

void AmazonTruck::fillFromFile(string filename) {
    ifstream infile = ifstream(filename, ios_base::in);

    if (infile.fail() and isFull()) {
        exit(1);
    }

    while(!infile.eof()) {
        string a1;
        string c;
        string st;
        int z;
        short int w;
        short int h;
        short int d;
        string id;
        float weight;
        string type;
        bool f;
        char comma;
        unsigned short packageVol;

        getline(infile, a1, ',');
        getline(infile, c, ',');
        getline(infile, st, ',');
        infile >> z >> comma;
        infile >> w >> comma;
        infile >> h >> comma;
        infile >> d >> comma;
        getline(infile, id, ',');
        infile >> weight >> comma;
        getline(infile, type, ',');
        infile >> f;

        if (w < 0 or h < 0 or d < 0) {
            continue;
        }

        Address currentAddress = Address(a1, c, st, z);
        Dimension currentDimension = Dimension(static_cast<unsigned short int>(w), static_cast<unsigned short int>(h), static_cast<unsigned short int>(d));

        packageVol = w * h * d;

        if (volume - packageVol <= 0) {
            volume = 0;
            break;
        }
        else {
            if (f) {
                Package currentPackage = Package(currentAddress, currentDimension, id, weight);
                truck->push_back(currentPackage);
                volume = volume - packageVol;
            }
            else {
                Liquid currentPackage = Liquid(currentAddress, currentDimension, id, weight, type, f);
                truck->push_back(currentPackage);
                volume = volume - packageVol;
            }
        }
    }
    infile.close();
}

void AmazonTruck::displayLastItemInTruck() {
    cout << truck->back() << endl;
}

