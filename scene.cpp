#define GLEW_STATIC
#include <..\GL\glew.h>
#include "scene.h"
#include <iostream>

using namespace std;
using namespace glm;

LineVertex axisVertices[] =
{
	LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
	LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
	LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
	LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
	LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
	LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
};
addRemovLinks::addRemovLinks(clock_t currTime, bool addOrRemove) {
	time = currTime;
	add = addOrRemove;
}
bullet::bullet(clock_t currTime) {
	time = currTime;
	shot = new Shape(0, 2, "./res/textures/plane.png", shotObj);
	shot->makeKDTree(shot->mesh->model);
}
bullet::~bullet(void) {
	delete shot;
}
//Vertex axisVertices[] = 
//	{
//		Vertex(glm::vec3(1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
//		Vertex(glm::vec3(-1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
//		Vertex(glm::vec3(0,1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
//		Vertex(glm::vec3(0,-1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
//		Vertex(glm::vec3(0,0,1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
//		Vertex(glm::vec3(0,0,-1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
//	};

unsigned int axisIndices[] =
{
	0,1,
	2,3,
	4,5
};

Scene::Scene()
{
	glLineWidth(3);

	cameras.push_back(new Camera(vec3(0, 0, -20.0f), 60.0f, 1.0f, 0.1f, 100.0f));
	pickedShape = -1;

}

Scene::Scene(vec3 position, float angle, float hwRelation, float near, float far)
{
	glLineWidth(3);
	cameras.push_back(new Camera(position, angle, hwRelation, near, far));
	cameras.push_back(new Camera(glm::vec3(0,-2,-1.01), angle, hwRelation, near, far));
	cameras.at(1)->Pitch(-100.0f);
	//	axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
	pickedShape = -1;
}

void Scene::addShape(int Cylparts, int linkPosition, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(Cylparts, linkPosition));
}
void Scene::addShape(int CylParts, int linkPosition, const std::string& textureFileName, int parent, int kind)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(CylParts, linkPosition, textureFileName,kind));
}
void Scene::addShape(int type, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(*shapes[type]));
}

void Scene::addShape(const std::string& fileName, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(fileName));
}

void Scene::addShape(const std::string& fileName, const std::string& textureFileName, int parent, int kind)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(fileName, textureFileName,kind));
}



void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(vertices, numVertices, indices, numIndices));
}

void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent, int kind)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(vertices, numVertices, indices, numIndices, textureFlieName,kind));
}



void Scene::addShader(const std::string& fileName)
{
	shaders.push_back(new Shader(fileName));
	if (!axisMesh)
	{
		axisMesh = new Shape(axisVertices, sizeof(axisVertices) / sizeof(axisVertices[0]), axisIndices, sizeof(axisIndices) / sizeof(axisIndices[0]));
		axisMesh->myScale(vec3(2.0*scaleFactor, 2.0*scaleFactor, 2.0*scaleFactor));
		axisMesh->myTranslate2(vec3(0, 0, 0.5), 1);
	}
}

mat4 Scene::GetViewProjection(int indx) const
{
	return cameras[indx]->GetViewProjection();
}

mat4 Scene::GetShapeTransformation() const
{
	return shapes[pickedShape]->makeTrans();
}

void Scene::moveSnakeShot() {
	if (snakeShots.size() <= 0) {
		return;
	}
	clock_t last_time = snakeShots[0]->time;
	clock_t this_time = clock();
	double time_counter = (double)(this_time - last_time);
	if (time_counter > (double)(10 * CLOCKS_PER_SEC)) {
		snakeShots.erase(snakeShots.begin());
	}
	for (int i = 0; i < snakeShots.size(); i++) {
		snakeShots[i]->shot->myTranslate(vec3(0, 0, -0.2), 1);
	}
	//clock_t last_time = TimeOfRotiains[timeToCheck];
}
void Scene::shot(clock_t this_time) {
	//Shape *tempShape = 
	//tempShape->setTraslateMat(shapes[0]->getTraslateMat());
	
	bullet *bull= new bullet(this_time);
	bull->shot->setTraslateMat(shapes[0]->getTraslateMat());
	snakeShots.push_back(new bullet(this_time));
	snakeShots[snakeShots.size()-1]->shot->setTraslateMat(shapes[0]->getTraslateMat());;
}
void Scene::checekr() {
	chainParents.push_back(-1);
	std::vector<Shape*>::iterator it;
	it = shapes.begin();
	shapesNormal.push_back(glm::mat4(1));

	pickedShape = linksNum;
	Shape *tempShape = new Shape(0, 3, "./res/textures/plane.png",snake);
	tempShape->setRotVectors(shapes[linksNum - 1]->getRotVectors());
	shapes.insert(it + linksNum, tempShape);
	shapes.at(pickedShape)->linkNumber = pickedShape;
	shapeTransformation(zScale, scaleFactor);
	shapeTransformation(zGlobalTranslate, 1.0);
	//shapeTransformation(zGlobalTranslate, 1.0);
	
		Shape *tempShape2 = new Shape(1, 1, "./res/textures/plane.png", snake);
		tempShape2->setRotVectors(shapes[linksNum - 2]->getRotVectors());
		tempShape2->setTraslateMat(shapes[linksNum - 1]->getTraslateMat());
		tempShape2->myScale(vec3(1, 1, scaleFactor));
		shapes[linksNum - 1] = tempShape2;
		
	
	linksNum++;
	setParent(linksNum - 1, linksNum - 2);
}
void Scene::addLink() {


	chainParents.push_back(-1);
	std::vector<Shape*>::iterator it;
	it = shapes.begin();
	shapesNormal.push_back(glm::mat4(1));
	pickedShape = linksNum;
	Shape *tempShape = new Shape(0, 3, "./res/textures/plane.png",snake);
	tempShape->makeKDTree(tempShape->mesh->model);
	tempShape->isSnake = true;
	tempShape->linkNumber = linksNum ;
	tempShape->setRotVectors(linksNum==1 ? shapes[linksNum - 1]->getRotVectors(): shapes[linksNum - 2]->getRotVectors());
	shapes.insert(it + linksNum, tempShape);
	shapes.at(pickedShape)->linkNumber = pickedShape;
	shapeTransformation(zScale, scaleFactor);
	shapeTransformation(zGlobalTranslate, 1.0);
	//shapeTransformation(zGlobalTranslate, 1.0);
	if (linksNum > 1) {
		Shape *tempShape2 = new Shape(1, 1, "./res/textures/plane.png", snake);
		tempShape2->setRotVectors(shapes[linksNum - 2]->getRotVectors());
		tempShape2->setTraslateMat(shapes[linksNum - 1]->getTraslateMat());
		tempShape2->myScale(vec3(1, 1, scaleFactor));
		tempShape2->makeKDTree(tempShape2->mesh->model);
		tempShape2->isSnake = true;
		tempShape2->linkNumber = linksNum - 1;
		shapes.at(linksNum - 1) = tempShape2;
		
	}
	linksNum++;
	setParent(linksNum - 1, linksNum - 2);
}
bool Scene::checkIftimeToMove(int shapeIdx) {
	int rotCurrShapesize = shapes[shapeIdx]->GerRotVecSize();
	int rotShapesSize = TimeOfRotiains.size();
	int timeToCheck = rotShapesSize - rotCurrShapesize;
	
	clock_t last_time = TimeOfRotiains[timeToCheck];
	clock_t this_time = clock();

	double time_counter = (double)(this_time - last_time);
	if (time_counter > (double)(shapeIdx * (1+speed) * CLOCKS_PER_SEC))
	{
		
		if (shapeIdx == linksNum - 1)
		{
			TimeOfRotiains.erase(TimeOfRotiains.begin());
		
			straight_time = clock();
		}
		return true;
	}
	return false;

}

void Scene::move() {
	
	
	checkIftimeToAddRemove();
	shapes[0]->myTranslate(vec3(0, 0, -speed), 1);
	
	
	if (shapes[0]->GerRotVecSize() > 0) {
		pickedShape = 0;
		glm::vec2 tempRot = shapes[0]->getRotVector();
		shapeTransformation((int)tempRot[0], tempRot[1]);
		rotateCamera(tempRot);
		if (linksNum > 1) {
			pickedShape = 1;
			shapeTransformation((int)tempRot[0], -tempRot[1]);
		}
		straighSnake = true;
	}
	
	for (int i = 1; i < linksNum; i++) {
		if (shapes[i]->GerRotVecSize() > 0) {
			if (checkIftimeToMove(i)) {
				pickedShape = i;
				glm::vec2 tempRot = shapes[i]->getRotVector();
				shapeTransformation((int)tempRot[0], tempRot[1]);
				if (i < linksNum - 1) {
					pickedShape = i + 1;
					shapeTransformation((int)tempRot[0], -tempRot[1]);
				}


			}
		}
	}
	moveSnakeShot();
}
void Scene::moveCamera() {
	pickedShape = -1;
	cameras[0]->MoveUp(speed - 0.0015);
	//cameras[1]->Pitch(50);
	//cameras[1]->MoveUp(speed);
	//cameras[1]->MoveUp(0.0055);
}
void Scene::rotateCamera(glm::vec2 rotateVec) {
	switch ((int)rotateVec[0])
	{
	case xLocalRotate: {
		cameras[1]->Pitch(-rotateVec[1]);
		break;
	}
	case yLocalRotate: {
		cameras[1]->RotateZ(-rotateVec[1]);
		break;
	}
	default:
		break;
	}
}
void Scene::addRemoveLinks(clock_t curr_time, bool add) {
	addRemovLinks adder(curr_time, add);
	addRemoveLinksVec.push_back(adder);
}
void Scene::checkIftimeToAddRemove() {
	if (addRemoveLinksVec.size() <= 0)
		return;
	clock_t last_time = addRemoveLinksVec[0].time;
	clock_t this_time = clock();
	double time_counter = (double)(this_time - last_time);
	int temp = linksNum - 1;
	if (time_counter > (double)((1.4014 - speed)*temp) * CLOCKS_PER_SEC) {
		if (addRemoveLinksVec[0].add)
			addLink();
		
		addRemoveLinksVec.erase(addRemoveLinksVec.begin());
	}
	if (!addRemoveLinksVec[0].add) {
		removeLink();
		addRemoveLinksVec.erase(addRemoveLinksVec.begin());
	}
}

void Scene::removeLink() {
	std::vector<Shape*>::iterator it;
	std::vector<glm::mat4>::iterator it2;
	it2 = shapesNormal.begin();
	it = shapes.begin();
	//it2 = shapesNormal.begin();
	if (linksNum == 1) {
		//Pause();
		return;
	}
	if (linksNum == 2) {
		shapes.erase(it + linksNum - 1);
		chainParents.erase(chainParents.end() - 2);
		shapesNormal.erase(it2 + linksNum - 1);
		linksNum--;
		return;
	}
	chainParents.erase(chainParents.end()-2);
	
	
	shapesNormal.erase(it2 + linksNum - 1);
	Shape *tempShape = new Shape(0, 3, "./res/textures/plane.png",snake);
	tempShape->setRotVectors(shapes[linksNum - 2]->getRotVectors());
	tempShape->setTraslateMat(shapes[linksNum - 2]->getTraslateMat());
	tempShape->myScale(vec3(1, 1, scaleFactor));
	tempShape->makeKDTree(tempShape->mesh->model);
	tempShape->isSnake = true;
	tempShape->linkNumber = linksNum - 2;
	
	shapes.erase(it + linksNum - 1);
	shapes.erase(it + linksNum - 2);
	shapes.push_back(tempShape);
	linksNum--;
	
	//setParent(linksNum - 1, linksNum - 2);
	
}







//}
void Scene::addVectorToShapes(glm::vec2 addVector) {
	for (int i = 0; i < linksNum; i++) {
		shapes[i]->addRotVector(addVector);
	}
}
void Scene::updateShapesNormal(glm::mat4 MVP) {
	for (int i = 0; i < shapes.size(); i++)
	{
		
		glm::mat4 Normal = makeTrans();
		
		mat4 Normal1 = mat4(1);
		for (int j = i; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;//first move
																	 //Normal1 = shapes[chainParents[j]]->makeTrans2() * Normal1;//seconed move
		}
		mat4 MVP1 = MVP * Normal1;
		MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
		if (i<linksNum)
			shapesNormal.at(i) = MVP1;
	}
}
void Scene::draw(int shaderIndx, int cameraIndx, bool drawAxis,int cameraType)
{
	if (gameOver)
		return;
	glm::mat4 Normal = makeTrans();
	//glm::mat4 Normal2 = shapes[0]->makeTrans();
	glm::mat4 MVP; 
	glm::vec3 newPosition = getTipPosition(0);
	cameras[1]->setPosition(glm::vec3(newPosition.x, newPosition.z+1.0, -newPosition.y-1.0));
	MVP = cameras[cameraType]->GetViewProjection()*Normal;
	updateShapesNormal(MVP);
	shaders[shaderIndx]->Bind();
	for (int i = 0; i<shapes.size(); i++)
	{
		//int j = i;
		//			int counter = 0;
		mat4 Normal1 = mat4(1);
		for (int j = i; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;//first move
																	 //Normal1 = shapes[chainParents[j]]->makeTrans2() * Normal1;//seconed move
		}
		mat4 MVP1 = MVP * Normal1;
		Normal1 = Normal * Normal1;
		MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
		Normal1 = Normal1 * shapes[i]->makeTrans();
		//if (i<linksNum)
			//shapesNormal.at(i) = MVP1;
		shaders[shaderIndx]->Update(MVP1, Normal1, i, shapesNormal,linksNum);

		if (shaderIndx == 1)
			shapes[i]->draw(GL_TRIANGLES);
		else
			shapes[i]->draw(GL_TRIANGLES);

	
	
	}
	
	if (shaderIndx == 0)
	{
		shaders[shaderIndx]->Bind();
		shaders[shaderIndx]->Update(cameras[cameraType]->GetViewProjection()*scale(vec3(10, 10, 10)), Normal*scale(vec3(10, 10, 10)), 0, shapesNormal, linksNum);
		axisMesh->draw(GL_LINES);
	} 
	for (int i = 0; i < snakeShots.size(); i++)
	{
		//int j = i;
		//			int counter = 0;
		mat4 Normal1 = mat4(1);

		mat4 MVP1 = MVP * Normal1;
		Normal1 = Normal * Normal1;
		MVP1 = MVP1 * snakeShots[i]->shot->makeTransScale(mat4(1));
		Normal1 = Normal1 * snakeShots[i]->shot->makeTrans();
		//shaders[shaderIndx]->Bind();
		shaders[shaderIndx]->Update(MVP1, Normal1, linksNum+i);
		snakeShots[i]->shot->draw(GL_TRIANGLES);
	}
	for (int i = 0; i < objectsShots.size(); i++)
	{
	
		mat4 Normal1 = mat4(1);

		mat4 MVP1 = MVP * Normal1;
		Normal1 = Normal * Normal1;
		MVP1 = MVP1 * objectsShots[i]->shot->makeTransScale(mat4(1));
		Normal1 = Normal1 * objectsShots[i]->shot->makeTrans();
		shaders[shaderIndx]->Update(MVP1, Normal1, linksNum + i);
		objectsShots[i]->shot->draw(GL_TRIANGLES);
	}
}

void Scene::shapeRotation(vec3 v, float ang, int indx)
{
	if (v.x >0.999)
		shapes[indx]->myRotate(ang, v, xAxis1);
	else
		if (v.z >0.999)
			shapes[indx]->myRotate(ang, v, zAxis12);
		else
			shapes[indx]->myRotate(ang, v, -1);
}

void Scene::shapeTransformation(int type, float amt)
{
	vec3 newAxis;
	switch (type)
	{
	case xLocalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(amt, 0, 0), 1);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(amt, 0, 0), 1);
		}
		break;
	case yLocalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, amt, 0), 1);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(0, amt, 0), 1);
		}
		break;
	case zLocalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, 0, amt), 1);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(0, 0, amt), 1);
		}
		break;
	case xGlobalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(amt / 5.0, 0, 0), 0);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(amt, 0, 0), 0);
		}
		break;
	case yGlobalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, amt / 5.0, 0), 0);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(0, amt, 0), 0);
		}
		break;
	case zGlobalTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, 0, amt / 5.0), 0);
		else
		{
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);
			shapes[i]->myTranslate(vec3(0, 0, amt), 0);
		}
		break;
	case xLocalRotate:
		if (pickedShape == -1)
			myRotate(amt, vec3(1, 0, 0), -1);
		else
			shapes[pickedShape]->myRotate(amt, vec3(1, 0, 0), -1);
		break;
	case xLocalRotate2:
		shapes[pickedShape]->myRotate2(amt, vec3(1, 0, 0));
		break;
	case yLocalRotate:
		if (pickedShape == -1)
			myRotate(amt, vec3(0, 1, 0), -1);
		else
			shapes[pickedShape]->myRotate(amt, vec3(0, 1, 0), -1);
		break;
	case yLocalRotate2:
			shapes[pickedShape]->myRotate2(amt, vec3(0, 1, 0));
		break;
	case zLocalRotate:
		if (pickedShape == -1)
			myRotate(amt, vec3(0, 0, 1), zAxis12);
		else
			shapes[pickedShape]->myRotate(amt, vec3(0, 0, 1), zAxis12);
		break;
	case zLocalRotate2:
			shapes[pickedShape]->myRotate2(amt, vec3(0, 0, 1));
		break;
	case xGlobalRotate:
		if (pickedShape == -1)
			globalSystemRot(amt, vec3(1, 0, 0), xAxis1);
		else
			shapes[pickedShape]->globalSystemRot(amt, vec3(1, 0, 0), xAxis1);
		break;
	case yGlobalRotate:
		if (pickedShape == -1)
			globalSystemRot(amt, vec3(0, 1, 0), -1);
		else
			shapes[pickedShape]->globalSystemRot(amt, vec3(0, 1, 0), -1);
		break;
	case zGlobalRotate:
		if (pickedShape == -1)
			globalSystemRot(amt, vec3(0, 0, 1), zAxis12);
		else
			shapes[pickedShape]->globalSystemRot(amt, vec3(0, 0, 1), zAxis12);
		break;
	case xScale:
		if (pickedShape == -1)
			myScale(vec3(amt, 1, 1));
		else
			shapes[pickedShape]->myScale(vec3(amt, 1, 1));
		break;
	case yScale:
		if (pickedShape == -1)
			myScale(vec3(1, amt, 1));
		else
			shapes[pickedShape]->myScale(vec3(1, amt, 1));
		break;
	case zScale:
		if (pickedShape == -1)
			myScale(vec3(1, 1, amt));
		else
			shapes[pickedShape]->myScale(vec3(1, 1, amt));
		break;
	case xCameraTranslate: //camera plane translate
						   //if(pickedShape == -1)
		myTranslate(vec3(amt / 5.0, 0, 0), 0);
		/*else
		{
		//newAxis = findAxis(vec3(1,0,0));
		int i = pickedShape;
		for (; chainParents[i] > -1; i = chainParents[i]);

		shapes[i]->translateInSystem(*this,vec3(amt,0,0),0,false);
		}*/
		break;
	case yCameraTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, amt / 5.0, 0), 0);
		else
		{
			//newAxis = findAxis(vec3(0,1,0));
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);

			shapes[i]->translateInSystem(*this, vec3(0, amt, 0), 0, false);
		}
		break;
	case zCameraTranslate:
		if (pickedShape == -1)
			myTranslate(vec3(0, 0, amt / 5.0), 0);
		else
		{
			//	newAxis = findAxis(vec3(0,0,1));
			int i = pickedShape;
			for (; chainParents[i] > -1; i = chainParents[i]);

			shapes[i]->translateInSystem(*this, vec3(0, 0, amt), 0, false);
		}
		break;
	default:
		break;
	}


}

void Scene::resize(int width, int height, int near, int far)
{
	glViewport(0, 0, width, height);
	cameras[0]->setProjection((float)width / (float)height, near, far);
	cameras[1]->setProjection((float)width / (float)height, near, far);

}

float Scene::picking(double x, double y)
{
	float depth;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw(1, 0, false,0);
	GLint viewport[4];
	unsigned char data[4];
	glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	cout << "depth " << depth << endl;
	int pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256 - 1;
	if (data[0] == 0 && data[1] == 0 && data[2] == 0)
	{
		pickedShape = -1;
		//std::cout<<"not picked"<<std::endl;
	}
	else
	{
		pickedShape = pickedID;
		std::cout << "picked " << pickedID << std::endl;
	}
	return depth;
}

vec3 Scene::getTipPosition(int indx)
{
	mat4 Normal1 = mat4(1);
	if (indx>-1)
	{
		for (int j = indx; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
		}
		return shapes[indx]->getPointInSystem(Normal1, vec3(0, 0, 1));
		//return shapes[indx]->getTipPos(Normal1);
	}
	else
	{
		return shapes[0]->getPointInSystem(mat4(1), vec3(0, 0, -1));
		//shapes[0]->getRootPos(mat4(1));
	}
}

vec3 Scene::getDestination(int indx)
{
	mat4 Normal1 = mat4(1);
	if (indx>-1)
	{
		for (int j = indx; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
		}
		return shapes[indx]->getPointInSystem(Normal1, vec3(0, 0, 0));
		//return shapes[indx]->getCenterOfRotation(Normal1);
	}
	else
	{
		return vec3(0, 0, 0);
	}
}

vec3 Scene::getAxisDirection(int indx, int axis)
{
	if (axis == xAxis)
	{
		mat4 Normal1 = mat4(1);
		for (int j = indx; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
		}
		return shapes[indx]->getPointInSystem(Normal1, vec3(1, 0, 0));
		//shapes[indx]->getXdirection(Normal1);
	}
	else
	{
		mat4 Normal1 = mat4(1);
		for (int j = indx; chainParents[j] > -1; j = chainParents[j])
		{
			Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
		}
		return shapes[indx]->getVectorInSystem(Normal1, vec3(0, 0, 1));
		//shapes[indx]->getZdirection(Normal1);
	}

}
Scene::~Scene(void)
{
	for (Shape* shp : shapes)
	{
		delete shp;
	}
	for (Camera* cam : cameras)
	{
		delete cam;
	}
	for (Shader* sdr : shaders)
	{
		delete sdr;
	}
	delete axisMesh;
}

bool Scene::checkCollision(int shape1, int shape2) {
	return shapes.at(shape1)->isColliding(*shapes.at(shape2));
}

void Scene::createKDTreesForShapes()
{
	for (int i = 0; i< shapes.size(); i++)
	{
		shapes.at(i)->makeKDTree(shapes.at(i)->mesh->model);
	}
}
