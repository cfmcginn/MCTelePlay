#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_real_distribution.hpp"
#include "particles.h"
#include <cmath>

//rng seed
boost::mt19937 gen(time(0));
const double pi = 3.141592653;

//sets up geometry and constants used for getting events
//x coord is the length, y width, z height of rectangular panels, T, S1, S2, and A
//dimensions organized roughly by detector geometry from top to bottom
//assume all detectors centered around x,y = (0,0)

//trigger idealized to be flat
const float xT = 20;
const float yT = 20;

const float L1 = 3;

//signal counter 1
const float xS1 = 122.4;
const float yS1 = 61;
const float zS1 =  15.8;

//assumed to have roughly the same dimensions as above;
//LFoam is thickness of Foam
const float L_Foam = 4.85;

//signal counter 2 assumed to have same dimensions as S1
//L_2 is distance from bottom of S2 to A
const float L2 = 5;

//anticoincidence idealized to be flat
const float xA = 146;
const float yA = 64;

//other parameters to specify
//probability a muon decays
const float decayRate = 0.01;

//units of muons/cm^2/s/str, taken from paper by Dektasoglu and Arslan:  Indian Academy of Sciences Vol 80 No 5 May 2013
const float muonFlux = 0.007;
const float muonAngularPower = 1.95;  //if you change this, you should probably change in integral in the main function too!

//maximum electron energy allowed (in real life its half the mass of a muon ~52.85)
const float electronMaxEnergy = 55.0;

//dR step size used for calculating the electron energy loss (smaller dR leads to better calculation within reason, but also makes calculation longer) in cm
const float dRstep = 0.01;

//electron dE/dx values; S is in scintillator; F is in the foam, Ein MeV, x in cm
const float dEdxS = 2.5;
const float dEdxF = 2.5;

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

	//see if it decays, assuming probability is weighted by rate*distance/(characteristic thickness of detector (3 middle slabs))
	//if it does decay, roll a dice to see how far along it's path it goes
    // can override this by specifying SignalOnly
	if((muon.distance*decayRate/(2*zS1+L_Foam) > getRandom()) || SignalOnly)
	{
		muon.decay = true;
		//relative distance it gets before the track decays
		float r = getRandom();
		muon.xDecay = muon.xS1PlaneT+cos(muon.phi)*tan(muon.theta)*r*(muon.zExit-L1);
		muon.yDecay = muon.yS1PlaneT+sin(muon.phi)*tan(muon.theta)*r*(muon.zExit-L1);
		muon.zDecay = L1+r*(muon.zExit-L1);
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


Electron getElectron(float xDecay, float yDecay, float zDecay)
{
	Electron e;

	e.theta = getThetaUniform();
	e.phi   = getPhi();

	e.xOrigin = xDecay;
	e.yOrigin = yDecay;
	e.zOrigin = zDecay;

	//energy is uniformly distributed for now (change this distribution to simulate real life if wanted)
	e.energy = getRandom()*electronMaxEnergy;

	//calculation of electron trajectory and energy deposit; x,y,z,E are working variables for stepping through the trajectory
	float x = e.xOrigin;
	float y = e.yOrigin;
	float z = e.zOrigin;
	float E = e.energy;
	float energyDetected = 0;

	bool isInSignalRegion = true;
	bool hasDecayed = false;
	while(isInSignalRegion)
	{
		//updating energy of electron after moving through a distance dRstep in signal region
		if(z>L1+zS1 && z<L1+zS1+L_Foam)  E = E-dRstep*dEdxF;
		else
		{
			E = E-dRstep*dEdxS;
			if(hasDecayed == false && E>0)  energyDetected = energyDetected+dRstep*dEdxS;
		}

		//upating position
		x = x + dRstep*sin(e.theta)*cos(e.phi);
		y = y + dRstep*sin(e.theta)*sin(e.phi);
		z = z + dRstep*cos(e.theta);

		//if the electron as decayed, in the signal region note the position at which is ended
		if(hasDecayed == false && E<0)
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
