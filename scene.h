#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>

class addRemovLinks {
public:
	bool add;
	clock_t time;
	addRemovLinks(clock_t time, bool add);
	//void setAddLink(clock_t currTime, bool addOrRemove);
	//virtual ~addRemovLinks(void);
};
class bullet {
public:
	enum objKind { snake, fruit, obj, shotingObj, killObj, shotObj };
	Shape *shot;
	clock_t time;
	bullet(clock_t time);
	virtual ~bullet(void);
	//void setAddLink(clock_t currTime, bool addOrRemove);
	//virtual ~addRemovLinks(void);
};
class Scene : public MovableGLM
{


protected:
	void checkIftimeToAddRemove();
	//bool checkIftimeToAddRemove();
	
	int pickedShape;
	static const int scaleFactor = 3;

	Shape *axisMesh;
	int Links;
	int linksNum = 4;
	int counter = 0;


	float speed = 0.007;
	bool stopMove = false;


	 //light will have the properties of camera
	std::vector<glm::vec3> rotPositions;
	std::vector<clock_t> TimeOfRotiains;
	std::vector<Shader*> shaders;
	
	std::vector<Shape*> shapes;
	std::vector<addRemovLinks> addRemoveLinksVec;
	std::vector<int>tranlateBack;
	std::vector<int> chainParents;
	std::vector<bullet*> snakeShots;
	std::vector<bullet*> objectsShots;
	void moveSnakeShot();
	bool toopView = true;;
	int freeShots = 2;
	void updateShapesNormal(glm::mat4 MVP);
	bool straighSnake = false;;
	clock_t straight_time ;

public:
	
	std::vector<Camera*> cameras;
	inline void incresFreeShots() { freeShots++; }
	inline void DecreseFreeShots() { freeShots--; }
	inline int GetFreeShots() { return freeShots; }
	inline void incresFreeShots(int shots) { freeShots += shots; }

	bool gameOver = false;
	inline bool getCameraMode() { return toopView;}
	inline void setCameraMode() { 
		toopView = !toopView; }
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float FAR = 100.0f;
	const float NEAR = 1.0f;
	const float CAM_ANGLE = 0.0f;
	float relation = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;
	enum objKind { snake, fruit, obj, shotingObj, killObj, shotObj };
	void shot(clock_t this_time);
	void checekr();
	void addRemoveLinks(clock_t curr_time, bool add) ;
	inline void increseSpeed() { speed = speed + 0.0002; };
	inline void decreseSpeed() { speed = speed - 0.0002; };
	inline void setSpeed() { speed = 0.007; }
	inline void setFreeShots(int shots) { this->freeShots = 2; }
	inline void addRotTime(clock_t curr_time) { TimeOfRotiains.push_back(curr_time); }
	inline void addRotPositions(glm::vec3 rotPosition) { rotPositions.push_back(rotPosition); }
	inline int GetLinkNum() { return linksNum; }
	void move();
	void inline setPicked(int pickID) { pickedShape = pickID; }
	void addVectorToShapes(glm::vec2 addVector);
	bool checkIftimeToMove(int shapeIdx);
	std::vector<glm::mat4> shapesNormal;
	enum axis { xAxis, yAxis, zAxis };
	enum transformations {
		xLocalTranslate, yLocalTranslate, zLocalTranslate, xGlobalTranslate, yGlobalTranslate, zGlobalTranslate,
		xLocalRotate, yLocalRotate, zLocalRotate, xGlobalRotate, yGlobalRotate, zGlobalRotate, xScale, yScale, zScale, xCameraTranslate, yCameraTranslate, zCameraTranslate, zLocalRotate2, xLocalRotate2, yLocalRotate2
	};
	
	Scene();
	void rotateCamera(glm::vec2);
	Scene(glm::vec3 position, float angle, float hwRelation, float near, float far);
	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName, const std::string& textureFileName, int parent,int tind);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName, int parent,int kind);
	void addShape(int Cyparts, int linkPosition, int parent);
	void addShape(int CylParts, int linkPosition, const std::string& textureFileName, int parent,int kind);
	void addTerrain(const std::string & fileName, const std::string & textureFileName);

	void addShader(const std::string& fileName);
	void moveCamera();
	void addLink();
	void removeLink();
	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx, int cameraIndx, bool drawAxis,int camerType);
	void shapeTransformation(int type, float amt);
	void shapeRotation(glm::vec3 v, float ang, int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(double x, double y);
	void resize(int width, int hight, int near, int far);
	//void updateTipPosition(int indx);
	glm::vec3 getTipPosition(int indx);
	glm::vec3 getDestination(int indx);
	glm::vec3 getAxisDirection(int indx, int axis);
	inline void setParent(int indx, int newValue) { chainParents[indx] = newValue; }
	virtual ~Scene(void);

	int inline getPickedShape() { return pickedShape; }
	int inline getShapesSize() { return shapes.size(); }
	Camera * getCamera() { return cameras.at(getCameraMode()); }

	bool checkCollision(int shape1, int shape2);
	void createKDTreesForShapes();
	bool paused = false;
	inline void Pause() { paused = !paused; }
	float xMovement = 0;
	float yMovement = 0;
	float zMovement = 0;

	bool hardDif = false;
};



