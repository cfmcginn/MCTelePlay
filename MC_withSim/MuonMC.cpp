// MuonMC.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include "eventGenerator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//const int events = std::atoi(argv[1]);
	//const int signalOnly = std::atoi(argv[2]);

	//signalOnly forces events to decay to an electron (although an extremely small amount of events might still miss the main signal detectors I think)
	const int events = 10;
	const int signalOnly = 1;

	float time = 0;
	//Rate of muons coming out of the source
	//0.007 is the muon flux/s/str times 2pi time area times 
	//the numeric factor 0.25 comes from evaluating the integral
	//gathering power is the fraction accepted times the total gathering power of the top panel
    //0.253165 comes from the integral(cos(theta)^2.95,d(cos(theta)),0,1), which is the top panel's gathering panel for an incoming intensity
    //that is proportional to cos(theta)^1.95
	float muonRate = muonFlux*2*pi*xT*yT*0.253164556962;

	for(int i = 0; i<events; i++)
	{
		//wait for an amount of time distributed as a exponential described by decayrate muonRate
		float waitTime = 1/muonRate*log(1.0/getRandom());
		time = time+waitTime;
		
		//generate muon properties
		Muon muon = getEvent(time, signalOnly);

		//generate electron properties
		Electron e;
		if(muon.decay)  e = getElectron(muon.xDecay, muon.yDecay, muon.zDecay);

		//output to TTree here 
		
		//output for debugging, comment out when making large samples!
		
		std::cout << "Trajectory: " << muon.phi<< " " <<muon.theta << std::endl;
		std::cout << std::endl;
		std::cout << "Trigger: "<< muon.xTPlane<< " " <<muon.yTPlane << std::endl;
		std::cout << "S1T    : " <<muon.xS1PlaneT<< " " <<muon.yS1PlaneT << std::endl;
		std::cout << "S1B    : " <<muon.xS1PlaneB<< " " <<muon.yS1PlaneB << std::endl;
		std::cout << "S2T    : " <<muon.xS2PlaneT<< " " <<muon.yS2PlaneT << std::endl;
		std::cout << "S2B    : " <<muon.xS2PlaneB<< " " <<muon.yS2PlaneB << std::endl;
		std::cout << "A      : " <<muon.xAPlane<< " " <<muon.yAPlane << std::endl;
		std::cout << std::endl;
		std::cout << "Trigger: "<<muon.T<< " " <<muon.S1<<" " << muon.Foam<< " " <<muon.S2<<" " << muon.A<<" " <<muon.decay << std::endl;
		std::cout << "Decay Coordinates: "<<muon.xDecay<<" " <<muon.yDecay<<" " <<muon.zDecay << std::endl;
		std::cout << "Exit Coordinates: "<<muon.xExit<<" " << muon.yExit<<" " << muon.zExit<<" " << muon.distance << std::endl;
		std::cout << std::endl;
		std::cout << "e Trajectory: " << e.phi << " " <<e.theta << std::endl;
		std::cout << "e Origin: " << e.xOrigin << " " << e.yOrigin << " " << e.zOrigin << std::endl;
		std::cout << "e Exit: " << e.xExit << " " << e.yExit << " " << e.zExit << std::endl;
		std::cout << "e Decay Spot: " << e.xFinal << " " << e.yFinal << " " << e.zFinal << std::endl;
		
		std::cout << "Energies: " << e.energy << " " << e.energyLoss << " " << e.fractionalEnergyLoss << " " << e.energyLossDetected << " " << e.fractionalEnergyLossDetected << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}

	return 0;
}

