#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_real_distribution.hpp"
#include "particles.h"
#include <cmath>
#include <fstream>

//rng seed
boost::mt19937 gen(time(0));
const double pi = 3.141592653;

//sets up geometry and constants used for getting events
//x coord is the length, y width, z height of rectangular panels, T, S1, S2, and A
//dimensions organized roughly by detector geometry from top to bottom
//assume all detectors centered around x,y = (0,0)

//trigger idealized to be flat
const float xT = 21;
const float yT = 21;

const float L1 = 10;

//signal counter 1
const float xS1 = 121.9;
const float yS1 = 61;
const float zS1 =  15.24;

//assumed to have roughly the same dimensions as above;
//LFoam is thickness of Foam
const float L_Foam = 5;

//signal counter 2 assumed to have same dimensions as S1
//L_2 is distance from bottom of S2 to A
const float L2 = 7.5;

//anticoincidence idealized to be flat
const float xA = 143;
const float yA = 66;

//other parameters to specify
//probability a muon decays
const float decayRate = 0.01;

//units of muons/cm^2/s/str, taken from paper by Dektasoglu and Arslan:  Indian Academy of Sciences Vol 80 No 5 May 2013
const float muonFlux = 0.007;
const float muonAngularPower = 1.95;  //if you change this, you should probably change in integral in the main function too!

//maximum electron energy allowed (in real life its half the mass of a muon ~52.85)
const float electronMaxEnergy = 105.65837/2.0;

//dR step size used for calculating the electron energy loss (smaller dR leads to better calculation within reason, but also makes calculation longer) in cm
const float dRstep = 0.01;

//electron dE/dx values; S is in scintillator; F is in the foam, Ein MeV, x in cm
//values taken from a the NIST's ESTAR database
//call getdEdx to fill the arrays
const float scintillatorDensity = 1.032;
const float foamDensity = 0.04;

float dEdxEnergies[81];
float dEdxS[81];
float dEdxF[81];

//some useful functions
//uses Boost library; mersenne twister
float getRandom()
{
	boost::random::uniform_real_distribution<> dist;
	return (float)dist(gen);
}

//uniform dist from 0 to 2pi
float getPhi()
{
	float rand = getRandom();
	return rand*2*pi;
}

//returns theta for trajectories that are uniform on a sphere (not uniform in theta itself!), 0 to pi
float getThetaUniform()
{
	float rand = getRandom();
	return acos(2*rand-1);
}

//returns theta for a muon trajectory, which include a factor of cos()^1.95 due to atmospheric effects
//restricted to between 0 and pi/2 (only going "down")
float getThetaMuon()
{
	float rand = getRandom();
	return acos(pow((double)rand, (double)1.0/(2.0+muonAngularPower)));
}

//getting the dEdx data out of files
void getdEdx()
{
	std::fstream myfile("Polystyrene_Stopping_Power_electrons.txt", std::ios_base::in);
	float temp1, temp2;
	int i = 0;
	while(myfile >> temp1 >> temp2)
	{
		dEdxEnergies[i] = temp1;
		dEdxF[i] = temp2;
		i++;
	}
	myfile.close();

	std::fstream myfile2("Scintillator_Stopping_Power_electrons.txt", std::ios_base::in);
	i = 0;
	while(myfile2 >> temp1 >> temp2)
	{
		dEdxS[i] = temp2;
		i++;
	}
	myfile2.close();
}

//returns the energy bin index that is 1 greater than where the energy of the particle is
int getdEdxEnergyBin(float E)
{
	int i = 0;
	while(E > dEdxEnergies[i])
	{
		i++;
	}
	return i;
}



//actual generator starts here
//returns muon information
Muon getEvent(float time, bool SignalOnly)
{
	Muon muon;
	muon.time = time;

	//initial trajectory seed
	muon.xTPlane = (getRandom()-1/2.0)*xT;
	muon.yTPlane = (getRandom()-1/2.0)*yT;
	muon.theta = getThetaMuon();
	muon.phi = getPhi();
	
	//tracking through detector
	muon.xS1PlaneT = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*L1;
	muon.yS1PlaneT = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*L1;
	muon.xS1PlaneB = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*(L1+zS1);
	muon.yS1PlaneB = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*(L1+zS1);
	muon.xS2PlaneT = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*(L1+zS1+L_Foam);
	muon.yS2PlaneT = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*(L1+zS1+L_Foam);
	muon.xS2PlaneB = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*(L1+2*zS1+L_Foam);
	muon.yS2PlaneB = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*(L1+2*zS1+L_Foam);
	muon.xAPlane = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*(L1+2*zS1+L_Foam+L2);
	muon.yAPlane = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*(L1+2*zS1+L_Foam+L2);

	//model for if muon decays
	//assume decay likelyhood is proportional to length of time (pathlength) in the signal region of the detector
	
	//calculating pathlength while avoiding divide by zero errors
	if(muon.theta>0.01)
	{
		if(fabs(cos(muon.phi))>0.999)
		{
			float zXExit1 = (xS1/2.0-muon.xS1PlaneT)/cos(muon.phi)/tan(muon.theta);
			float zXExit2 = (-xS1/2.0-muon.xS1PlaneT)/cos(muon.phi)/tan(muon.theta);

			if(zXExit1>0 && zXExit1<2*zS1+L_Foam) muon.zExit = zXExit1+L1;
			else if(zXExit2>0 && zXExit2<2*zS1+L_Foam) muon.zExit = zXExit2+L1;
			else muon.zExit = L1+2*zS1+L_Foam;
		}
		else if(fabs(cos(muon.phi))<0.001)
		{
			float zYExit1 = (yS1/2.0-muon.yS1PlaneT)/sin(muon.phi)/tan(muon.theta);
			float zYExit2 = (-yS1/2.0-muon.yS1PlaneT)/sin(muon.phi)/tan(muon.theta);

			if(zYExit1>0 && zYExit1<2*zS1+L_Foam) muon.zExit = zYExit1+L1;
			else if(zYExit2>0 && zYExit2<2*zS1+L_Foam) muon.zExit = zYExit2+L1;
			else muon.zExit = L1+2*zS1+L_Foam;
		}
		else
		{
			float zXExit1 = (xS1/2.0-muon.xS1PlaneT)/cos(muon.phi)/tan(muon.theta);
			float zXExit2 = (-xS1/2.0-muon.xS1PlaneT)/cos(muon.phi)/tan(muon.theta);
			float zYExit1 = (yS1/2.0-muon.yS1PlaneT)/sin(muon.phi)/tan(muon.theta);
			float zYExit2 = (-yS1/2.0-muon.yS1PlaneT)/sin(muon.phi)/tan(muon.theta);

			if(zXExit1>0 && zXExit1<2*zS1+L_Foam) muon.zExit = zXExit1;
			else if(zXExit2>0 && zXExit2<2*zS1+L_Foam) muon.zExit = zXExit2;
			else muon.zExit = 2*zS1+L_Foam;
			if(zYExit1>0 && zYExit1<muon.zExit) muon.zExit = zYExit1;
			if(zYExit2>0 && zYExit2<muon.zExit) muon.zExit = zYExit2;

			muon.zExit = muon.zExit+L1;
		}
	}
	else
	{
		muon.zExit = L1+2*zS1+L_Foam;
	}
	muon.xExit = muon.xTPlane+cos(muon.phi)*tan(muon.theta)*muon.zExit;
	muon.yExit = muon.yTPlane+sin(muon.phi)*tan(muon.theta)*muon.zExit;
	muon.distance = pow((double)pow(muon.zExit-L1,2)+pow(muon.xExit-muon.xS1PlaneT,2)+pow(muon.yExit-muon.yS1PlaneT,2),0.5);

	//calculating distance that is spent in the scintillator region only
	if(muon.zExit < L1+zS1 && muon.zExit > L1) muon.distanceSignal = pow((double)pow(muon.zExit-L1,2)+pow(muon.xExit-muon.xS1PlaneT,2)+pow(muon.yExit-muon.yS1PlaneT,2),0.5);
	else if(muon.zExit > L1+zS1 && muon.zExit < L1+zS1+L_Foam && muon.zExit > L1) muon.distanceSignal = pow((double)pow(zS1,2)+pow(muon.xS1PlaneB-muon.xS1PlaneT,2)+pow(muon.yS1PlaneB-muon.yS1PlaneT,2),0.5);
	else if(muon.zExit > L1) muon.distanceSignal = pow((double)pow(muon.zExit-L1,2)+pow(muon.xExit-muon.xS1PlaneT,2)+pow(muon.yExit-muon.yS1PlaneT,2),0.5)-pow((double)pow(L_Foam,2)+pow(muon.xS2PlaneT-muon.xS1PlaneB,2)+pow(muon.yS2PlaneT-muon.yS1PlaneB,2),0.5);

	//see if it decays, assuming probability is weighted by rate*distance/(characteristic thickness of detector (3 middle slabs))
	//if it does decay, roll a dice to see how far along it's path it goes
    // can override this by specifying SignalOnly

	if((muon.distanceSignal*decayRate/(2.0*zS1) > getRandom()) || SignalOnly)
	{
		while(muon.decay == false)
		{
			muon.decay = true;
			//relative distance it gets before the track decays
			float r = getRandom();
			muon.xDecay = muon.xS1PlaneT+cos(muon.phi)*tan(muon.theta)*r*(muon.zExit-L1);
			muon.yDecay = muon.yS1PlaneT+sin(muon.phi)*tan(muon.theta)*r*(muon.zExit-L1);
			muon.zDecay = L1+r*(muon.zExit-L1);

			if(muon.zDecay > L1+zS1 && muon.zDecay < L1+zS1+L_Foam && foamDensity*(muon.distance-muon.distanceSignal)/(foamDensity*(muon.distance-muon.distanceSignal)+scintillatorDensity*muon.distanceSignal) < getRandom()) muon.decay = false;
		}
	}
	else muon.decay = false;

	//trigger bits
	muon.T = true;
	if(fabs(muon.xS1PlaneT)<xS1/2.0 && fabs(muon.yS1PlaneT)<yS1/2.0 && (!muon.decay || muon.zDecay>L1)) muon.S1 = true;
	else muon.S1 = false;
	if(fabs(muon.xS1PlaneB)<xS1/2.0 && fabs(muon.yS1PlaneB)<yS1/2.0 && (!muon.decay || muon.zDecay>L1+zS1)) muon.Foam = true;
	else muon.Foam = false;
	if(fabs(muon.xS2PlaneT)<xS1/2.0 && fabs(muon.yS2PlaneT)<yS1/2.0 && (!muon.decay || muon.zDecay>L1+zS1+L_Foam)) muon.S2 = true;
	else muon.S2 = false;
	if(fabs(muon.xAPlane)<xA/2.0 && fabs(muon.yAPlane)<yA/2.0 && (!muon.decay)) muon.A = true;
	else muon.A = false;

	return muon;
}


Electron getElectron(float xDecay, float yDecay, float zDecay, int eSpectrum)
{
	Electron e;

	e.theta = getThetaUniform();
	e.phi   = getPhi();

	e.xOrigin = xDecay;
	e.yOrigin = yDecay;
	e.zOrigin = zDecay;

	//energy is uniformly distributed for now (change this distribution to simulate real life if wanted)
	if(eSpectrum == 1)
	{
		while(true)
		{
			e.energy = getRandom()*electronMaxEnergy;
			if(getRandom()*8*electronMaxEnergy < (24*pow(e.energy,2)/electronMaxEnergy)*(1-2*e.energy/(3.0*electronMaxEnergy))) break;
		}
	}
	else e.energy = getRandom()*electronMaxEnergy;

	//calculation of electron trajectory and energy deposit; x,y,z,E are working variables for stepping through the trajectory
	float x = e.xOrigin;
	float y = e.yOrigin;
	float z = e.zOrigin;
	float E = e.energy;
	float energyDetected = 0;
	int bin = 0;

	bool isInSignalRegion = true;
	bool hasDecayed = false;
	while(isInSignalRegion)
	{
		//updating energy of electron after moving through a distance dRstep in signal region
		//interpolates between the dEdx data point above and below the energy
		bin = getdEdxEnergyBin(E);
		if(z>L1+zS1 && z<L1+zS1+L_Foam)  E = E-dRstep*foamDensity*(dEdxF[bin-1]+(dEdxF[bin]-dEdxF[bin-1])/(dEdxEnergies[bin]-dEdxEnergies[bin-1])*(E-dEdxEnergies[bin-1]));
		else
		{
			E = E-dRstep*scintillatorDensity*(dEdxS[bin-1]+(dEdxS[bin]-dEdxS[bin-1])/(dEdxEnergies[bin]-dEdxEnergies[bin-1])*(E-dEdxEnergies[bin-1]));
			if(hasDecayed == false && E>0.01)  energyDetected = energyDetected+dRstep*scintillatorDensity*(dEdxS[bin-1]+(dEdxS[bin]-dEdxS[bin-1])/(dEdxEnergies[bin]-dEdxEnergies[bin-1])*(E-dEdxEnergies[bin-1]));
		}

		//upating position
		x = x + dRstep*sin(e.theta)*cos(e.phi);
		y = y + dRstep*sin(e.theta)*sin(e.phi);
		z = z + dRstep*cos(e.theta);

		//if the electron as decayed, in the signal region note the position at which is ended
		if(hasDecayed == false && E<0.01)
		{
			hasDecayed = true;
			e.xFinal = x;
			e.yFinal = y;
			e.zFinal = z;
		}

		//checking if is in signal region
		if(fabs(x)>xS1/2.0 || fabs(y)>yS1/2.0 || z < L1 || z>L1+2*zS1+L_Foam) isInSignalRegion = false;
	}

	//note the position when it exits
	e.xExit = x;
	e.yExit = y;
	e.zExit = z;

	//writing down the energies for the cases of whether or not it decays in or outside the signal region.
	if(hasDecayed == false)
	{
		e.energyLoss = e.energy - E;
		e.fractionalEnergyLoss = e.energyLoss/e.energy;

		e.xFinal = x;
		e.yFinal = y;
		e.zFinal = z;
	}
	else
	{
		e.energyLoss = e.energy;
		e.fractionalEnergyLoss = 1;
	}

	//writing down the detected energies
	
	e.energyLossDetected = energyDetected;
	e.fractionalEnergyLossDetected = energyDetected/e.energy;

	return e;
}

