#ifndef ASSIGNMENT2_H
#define ASSIGNMENT2_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class Assignment2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_QUAD1,
		GEO_CUBE,
		GEO_CUBE_RED,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_SPHERE1,
		GEO_HEMISPHERE,
		GEO_HEMISPHERE_BLACK,
		GEO_CYLINDER,
		GEO_CYLINDER_BLACK,
		GEO_CYLINDER_RED,
		GEO_CONE,
		GEO_CONE1,
		GEO_HALFCONE,
		GEO_HALFCONEBLACK,
		GEO_CONE_TREE,
		GEO_TREE_LEAVES,
		GEO_LIGHTBALL,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_TOTAL,
	};
public:
	Assignment2();
	~Assignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	MS modelStack, viewStack, projectionStack;
private:
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngleArm=225;
	float rotateAngleLeg1 = 150;
	float rotateAngleLeg2 = 215;
	float BodyTranslate = 0;
	bool RotateArm=true;
	bool RotateLeg=true;
	bool RotateLeg2 = true;
	Light light[1];
	void RenderMesh(Mesh *mesh, bool enableLight);



	Camera2 camera;
};

#endif