
class Muon
{
	public:
		Muon();

		float time;
		//x,y at the top of the Plane of T (also the plane the particle is generated at)
		//all z values are at the top of the plane of the object the muon is interacting with
		float xTPlane;
		float yTPlane;
		float theta;
		float phi;

		//top and bottom panels of the siganl counters 1 and 2
		float xS1PlaneT;
		float yS1PlaneT;
		float xS1PlaneB;
		float yS1PlaneB;

		float xS2PlaneT;
		float yS2PlaneT;
		float xS2PlaneB;
		float yS2PlaneB;

		//anticoincidence
		float xAPlane;
		float yAPlane;

		//interactions with layers
		bool T;
		bool S1;
		bool Foam;
		bool S2;
		bool A;

		//location of decay
		bool decay;
		float xDecay;
		float yDecay;
		float zDecay;

		//exit location
		float xExit;
		float yExit;
		float zExit;

		//distance traveled through scintillator only from entering S1+S2 to exit of system assuming no decay
		float distanceSignal;
		float distance;
};

Muon::Muon()
{
	time = -999;
	xTPlane = -999;
	yTPlane = -999;
	theta = -999;
	phi = -999;
	xS1PlaneT = -999;
	yS1PlaneT = -999;
	xS1PlaneB = -999;
	yS1PlaneB = -999;

	xS2PlaneT = -999;
	yS2PlaneT = -999;
	xS2PlaneB = -999;
	yS2PlaneB = -999;

	xAPlane = -999;
	yAPlane = -999;
	T = 0;
	S1 = 0;
	Foam = 0;
	S2 = 0;
	A = 0;
	decay = 0;
	xDecay = -999;
	yDecay = -999;
	zDecay = -999;
	xExit = -999;
	yExit = -999;
	zExit = -999;
	distanceSignal = -999;
	distance = -999;
}

class Electron
{
	public:
		Electron();

		//initial Energy in MeV/c
		float energy;
		float energyLoss;
		float fractionalEnergyLoss;
		//this is the energy lost in the signal counters and not the foam
		float energyLossDetected;
		float fractionalEnergyLossDetected;

		//origin position and trajectory
		float xOrigin;
		float yOrigin;
		float zOrigin;

		//here phi is from 0-2pi, theta is 0 to pi
		float phi;
		float theta;

		//Exit position (assuming it makes it all the way out of the signal region)
		float xExit;
		float yExit;
		float zExit;

		//final point of the electron (equal to x,y,zExit if it doesn't totally decay in detector)
		//otherwise it is approximately where the electron loses all of its initial energy
		float xFinal;
		float yFinal;
		float zFinal;
};

Electron::Electron()
{
	energy = -999;
	energyLoss = -999;
	fractionalEnergyLoss = -999;
	energyLossDetected = -999;
	fractionalEnergyLossDetected = -999;

	xOrigin = -999;
	yOrigin = -999;
	zOrigin = -999;

	phi = -999;
	theta = -999;

	xExit = -999;
	yExit = -999;
	zExit = -999;

	xFinal = -999;
	yFinal = -999;
	zFinal = -999;
}