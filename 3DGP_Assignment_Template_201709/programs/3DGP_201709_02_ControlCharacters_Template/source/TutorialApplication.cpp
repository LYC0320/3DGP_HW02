//
// Student Name:
// Student ID:
// Student Email Address:
//
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include <math.h>
#include <string>

int iRobotNum = 20;
int oRobotNum = 20;
float radius = 200;
const float PI = 3.141592654;
bool buttonPressed = false;
bool buttonMoved = false;

Vector3 destination = Vector3(0, 0, 0);

Vector2 mouseStart;
Vector2 mouseStop;

using namespace Ogre;
using namespace std;
BasicTutorial_00::BasicTutorial_00(void) {
mSound = new SOUND;
	mSound->init();
}

void BasicTutorial_00::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCamera");

	mCamera->setPosition(Ogre::Vector3(0,700,800));
	mCamera->lookAt(Ogre::Vector3(0,0,0));

	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createViewports(void)
{
	Ogre::Viewport* vp;
	// Create one viewport, entire window
	vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createScene(void) 
{
	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 

	createPlane();
	createBall();
	createRobot();
	initVar();

	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSelectionRect = new SelectionRectangle("SelectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);

	
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	createLight();

	initParticleSystemForExplosion();
	setOffParticleSystem(mParticleNode, "explosion", Vector3::ZERO);

}

bool BasicTutorial_00::frameStarted(const FrameEvent &evt)
{
	for (int i = 0; i < iRobotNum+oRobotNum; i++) 
	{
		mAnimationState[i]->addTime(evt.timeSinceLastFrame * 1.5);
		//robotSnode[i]->setPosition(robotPosition[i]);
	}
    return BaseApplication::frameStarted(evt);
}

void BasicTutorial_00::initParticleSystemForExplosion() {

	mParticleNode = static_cast<SceneNode*>(robotSnode[20]->createChild());

	ParticleSystem *p = mSceneMgr->createParticleSystem("explosion", "Examples/GreenyNimbus");

	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(false);
	mParticleNode->attachObject(p);
	mParticleNode->setPosition(Vector3::ZERO);
	mParticleNode->setScale(1.5, 1.5, 1.5);
	mParticleNode->setVisible(false);
}

void BasicTutorial_00::setOffParticleSystem(Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos) 
{
	fNode->setPosition(pos);
	fNode->setVisible(true);
	MovableObject *s = fNode->getAttachedObject(pname);
	ParticleSystem* p =static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);
	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(true);
}

void BasicTutorial_00::createPlane(void)
{
	Plane floor(Vector3::UNIT_Y, 0);
	MeshPtr mFloor = MeshManager::getSingleton().createPlane(
		"floor1",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		floor,
		1500, 1500,
		20, 20,
		true,
		1,
		5, 5,
		Vector3::UNIT_Z
	);

	Entity *floorEnt = mSceneMgr->createEntity("floor1");

	SceneNode *fnode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	fnode->attachObject(floorEnt);

	floorEnt->setMaterialName("Examples/Rockwall");

	floorG = floor;
}

void BasicTutorial_00::createBall(void)
{
	Entity *ball = mSceneMgr->createEntity("centerBall", Ogre::SceneManager::PT_SPHERE);
	SceneNode *ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ballNode->attachObject(ball);
	ballNode->setScale(2, 2, 2);
	ball->setMaterialName("Examples/black");

	bigBall = ball;
}

void BasicTutorial_00::createLight(void)
{
	Light *light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setDiffuseColour(1, 1, 1);
	light->setSpecularColour(1, 1, 1);
	light->setPosition(Vector3(150, 250, 100));

}

void BasicTutorial_00::createRobot(void)
{
	float angle = 0;
	String name;

	for(int i = 0; i < iRobotNum ; i++ )
	{
		
		genNameUsingIndex("robot", i, name);
		
		Entity *ent = mSceneMgr->createEntity(name, "robot.mesh");
		SceneNode *snode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		snode->attachObject(ent);
		snode->setPosition(radius*cos(angle), 0, radius*sin(angle));
		snode->lookAt(Vector3(0,0,0),Node::TS_WORLD,Vector3::UNIT_X);

		angle = angle + ((2 * PI) / 360) * (360 / iRobotNum);

		robotSnode[i] = snode;
		robotEntity[i] = ent;
		robotPosition[i] = snode->getPosition();


		mAnimationState[i] = ent->getAnimationState("Idle");
		mAnimationState[i]->setLoop(true);
		mAnimationState[i]->setEnabled(true);

		
	}



	for (int i = iRobotNum; i < iRobotNum + oRobotNum; i++)
	{

		genNameUsingIndex("robot", i, name);

		Entity *ent = mSceneMgr->createEntity(name, "robot.mesh");
		SceneNode *snode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

		if (i == iRobotNum)
			snode->setScale(1.7, 1.7, 1.7);

		snode->attachObject(ent);
		snode->setPosition((radius+90)*cos(angle), 0, (radius+90)*sin(angle));
		snode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD, Vector3::UNIT_X);

		angle = angle + ((2 * PI) / 360) * (360 / oRobotNum);

		robotSnode[i] = snode;
		robotEntity[i] = ent;
		robotPosition[i] = snode->getPosition();

		mAnimationState[i] = ent->getAnimationState("Idle");
		mAnimationState[i]->setLoop(true);
		mAnimationState[i]->setEnabled(true);
	}


}

bool BasicTutorial_00::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		Ray mRay = mTrayMgr->getCursorRay(mCamera);
		Vector2 temp = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());

		mouseStart = temp;
		mouseStop = temp;

		buttonPressed = true;
	}
	else if (id == OIS::MB_Right)
	{
		Ray mRay = mTrayMgr->getCursorRay(mCamera);

		if (mRay.intersects(floorG).first)
		{
			destination = mRay.getPoint(mRay.intersects(floorG).second);
			//cout << mRay.getPoint(mRay.intersects(floorG).second) << endl;
		}

		for (int i = 0; i < 40; i++) 
		{
			if (robotSnode[i]->getShowBoundingBox())
			{
				direction[i] = destination - robotSnode[i]->getPosition();
				direction[i].y = 0;
				float length = sqrt(direction[i].x * direction[i].x + direction[i].y * direction[i].y + direction[i].z * direction[i].z);
				distance[i] = length;

				//cout << length<< endl;

				//cout << direction << endl;

				direction[i] = direction[i] / length;
			
			}
		
		}
		
	}

	return BaseApplication::mousePressed(arg, id);

}

bool BasicTutorial_00::mouseMoved(const OIS::MouseEvent &arg)
{
	if (buttonPressed)
	{
		buttonMoved = true;

		Ray mRay = mTrayMgr->getCursorRay(mCamera);
		Vector2 temp = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());

		mouseStop = temp;
		
			mSelectionRect->setCorners(mouseStart.x, mouseStart.y, mouseStop.x, mouseStop.y);
			mSelectionRect->setVisible(true);
		
	}
		return BaseApplication::mouseMoved(arg);
	
}

bool BasicTutorial_00::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		Ray mRay = mTrayMgr->getCursorRay(mCamera);
		Vector2 temp = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());

		mouseStop = temp;

		buttonPressed = false;
		mSelectionRect->setVisible(false);

		float left = mouseStart.x;
		float top = mouseStart.y + 1;
		float right = mouseStop.x;
		float bottom = mouseStop.y + 1;

		if (mouseStart.x > mouseStop.x)
		{
			left = mouseStop.x;
			right = mouseStart.x;
		}

		if (mouseStart.y > mouseStop.y)
		{
			top = mouseStop.y + 1;
			bottom = mouseStart.y + 1;
		}

		if (mouseStart.x == mouseStop.x || mouseStart.y == mouseStop.y)
		{
			right = right + 0.00001;
			top = top - 0.00001;
			left = left - 0.00001;
			bottom = bottom + 0.00001;
		}

		cout << "=============" << endl;
		cout << "¥ª¤W(" << left << ", " << top << ")" << endl;
		cout << "¥k¤U(" << right << ", " << bottom << ")" << endl;
		cout << "=============" << endl;


		Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(right, top));
		Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(left, top));
		Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(right, bottom));
		Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(left, bottom));

		PlaneBoundedVolume vol;
		PlaneBoundedVolumeList volList;
		int np = 100;
		vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         // top plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), topLeft.getPoint(np)));       // left plane
		vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), bottomLeft.getPoint(np)));   // bottom plane
		vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), bottomRight.getPoint(np)));     // right plane 

		volList.push_back(vol);
		mVolQuery->setVolumes(volList);
		SceneQueryResult result = mVolQuery->execute();

		SceneQueryResultMovableList::iterator itr;

		for (itr = result.movables.begin(); itr != result.movables.end(); itr++)
		{
			if (*itr && (*itr)->getName().substr(0, 5) == "robot")
			{
				mCurrentObject = (*itr)->getParentSceneNode();

				chosenKids.push_back(stoi((*itr)->getName().substr(5, 2)));

				//cout << stoi((*itr)->getName().substr(5, 2)) << endl;

			}

		}

		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < chosenKids.size(); j++)
			{
				if (i == chosenKids[j])
				{
					robotSnode[i]->showBoundingBox(true);
					flgShow[i] = true;
					//cout << i << ":chosen" << endl;
				}
				else if (i != chosenKids[j] && !flgShow[i])
				{

					robotSnode[i]->showBoundingBox(false);
					flgShow[i] = false;
				}

			}
		}

		for (int i = 0; i < 40; i++)
		{
			if (chosenKids.empty())
			{
				robotSnode[i]->showBoundingBox(false);
			}
		}

		//cout << chosenKids.empty() << endl;

		for (int i = 0; i < 40; i++)
		{
			flgShow[i] = false;
		}

		chosenKids.clear();

	}

	return BaseApplication::mouseReleased(arg, id);
}

bool BasicTutorial_00::frameRenderingQueued(const FrameEvent &evt)
{
	for (int i = 0; i < 40; i++)
	{
		
		if (robotSnode[i]->getShowBoundingBox() && distance[i] > 0)
		{
			
			Real speed = 100;
			Real t = evt.timeSinceLastFrame;
			Vector3 walk = direction[i] * speed * t;
			float walkLength = sqrt(walk.x * walk.x + walk.y * walk.y + walk.z * walk.z);
			robotSnode[i]->translate(walk);

			Vector3 src = robotSnode[i]->getOrientation()*Vector3(1, 0, 0);
			Quaternion quat = src.getRotationTo(direction[i]);

			robotSnode[i]->rotate(quat);
			
			distance[i] = distance[i] - walkLength;

			//mAnimationState[i]->addTime(speed*t);

			mAnimationState[i] = robotEntity[i]->getAnimationState("Walk");
			mAnimationState[i]->setLoop(true);
			mAnimationState[i]->setEnabled(true);

			
		}
		else if (robotSnode[i]->getShowBoundingBox() && distance[i] <= 0)
		{
			direction[i] = Vector3(0, 0, 0);
			distance[i] = 0;

			mAnimationState[i] = robotEntity[i]->getAnimationState("Idle");
			mAnimationState[i]->setLoop(true);
			mAnimationState[i]->setEnabled(true);

		}

		if(!robotSnode[i]->getShowBoundingBox())
		{
			direction[i] = Vector3(0, 0, 0);
			distance[i] = 0;

			mAnimationState[i] = robotEntity[i]->getAnimationState("Idle");
			mAnimationState[i]->setLoop(true);
			mAnimationState[i]->setEnabled(true);
		}

	}

	collision();
	
	return BaseApplication::frameRenderingQueued(evt);
}

void BasicTutorial_00::collision(void)
{
	for (int i = 0; i < iRobotNum + oRobotNum; i++)
	{
		for (int j = 0; j < iRobotNum + oRobotNum; j++)
		{
			
			if (robotSnode[i]->getShowBoundingBox() && !robotSnode[j]->getShowBoundingBox())
			{
				Vector3 dv = robotSnode[j]->getPosition() - robotSnode[i]->getPosition();
				float d = sqrt(dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);

				float w = robotEntity[j]->getBoundingRadius() / 3.2 + robotEntity[i]->getBoundingRadius() / 3.2 - d; // getBoundingRadius() is not precise, so it should be divided by a constant
				if (w > 0)
				{
					robotSnode[j]->translate(dv / d);
				}
			}

			else if (robotSnode[i]->getShowBoundingBox() && robotSnode[j]->getShowBoundingBox() && i != j)
			{
				Vector3 dv = robotSnode[j]->getPosition() - robotSnode[i]->getPosition();
				float d = sqrt(dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);

				float w = robotEntity[j]->getBoundingRadius() / 3.2 + robotEntity[i]->getBoundingRadius() / 3.2 - d; // getBoundingRadius() is not precise, so it should be divided by a constant
				if (w > 0)
				{
					

					robotSnode[j]->translate(dv / (2 * d));
					robotSnode[i]->translate(-dv / (2 * d));

					//cout << i << "push" << j << dv / (2 * d) << endl;
					//cout << j << "push" << i << -dv / (2 * d) << endl;
				}
				 
			}
			else if (!robotSnode[i]->getShowBoundingBox() && !robotSnode[j]->getShowBoundingBox() && i != j)
			{
				Vector3 dv = robotSnode[j]->getPosition() - robotSnode[i]->getPosition();
				float d = sqrt(dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);

				float w = robotEntity[j]->getBoundingRadius() / 3.2 + robotEntity[i]->getBoundingRadius() / 3.2 - d; // getBoundingRadius() is not precise, so it should be divided by a constant
				if (w > 0)
				{


					robotSnode[j]->translate(dv / (2 * d));
					robotSnode[i]->translate(-dv / (2 * d));

					//cout << i << "push" << j << dv / (2 * d) << endl;
					//cout << j << "push" << i << -dv / (2 * d) << endl;
				}

			}
		}
	}

	for (int i = 0; i < iRobotNum + oRobotNum; i++)
	{

		Vector3 dv = robotSnode[i]->getPosition() - bigBall->getParentSceneNode()->getPosition();
		float d = sqrt(dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);

		float w = bigBall->getBoundingRadius() + robotEntity[i]->getBoundingRadius() / 3.2 - d; // getBoundingRadius() is not precise, so it should be divided by a constant
		if (w > 0)
		{
			robotSnode[i]->translate(dv / d);
		}
	
	
	}
}

void BasicTutorial_00::initVar(void)
{
	for (int i = 0; i < 40; i++)
	{
		direction[i] = Vector3(0, 0, 0);
		distance[i] = 0;
		flgShow[i] = false;
	}
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
