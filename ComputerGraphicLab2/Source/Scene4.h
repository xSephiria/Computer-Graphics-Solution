#ifndef SCENE_4_H
#define SCENE_4_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"

class scene4 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_STAR,
		GEO_MERCURY,
		GEO_VENUS,
		GEO_EARTH,
		GEO_MOON,
		GEO_MARS,
		GEO_JUPITER,
		GEO_SATURN,
		GEO_SATRING,
		GEO_URANUS,
		GEO_NEPTUNE,
		GEO_PLUTO,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
public:
	scene4();
	~scene4();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float planet1RotAngle;
	float planet1RevAngle;
	float StarRot;
	float MercuryRot;
	float VenusRot;
	float EarthRot;
	float MoonRot;
	float MarsRot;
	float JupiterRot;
	float SaturnRot;
	float SatRingRot;
	float UranusRot;
	float NeptuneRot;
	float PlutoRot;
	float moon1RotAngle;

	Camera camera;

	MS modelStack, viewStack, projectStack, projectionStack;
};

#endif