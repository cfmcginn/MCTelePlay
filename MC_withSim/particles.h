
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

		//exit distance from entering S1+S2 system assuming no decay
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
	distance = -999;
}