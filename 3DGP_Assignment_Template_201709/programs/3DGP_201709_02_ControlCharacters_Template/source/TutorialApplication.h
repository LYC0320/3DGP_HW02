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
// Select and move characters
#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_
 
#include "BaseApplication.h"
#include "selection_rectangle.h"
#include "sound.h"

using namespace Ogre;
#define MOTION_TYPE_NONE 0
#define MOTION_TYPE_IDLE 1
#define MOTION_TYPE_WALK 2

class BasicTutorial_00 : public BaseApplication
{
private:
	SceneNode *mCurrentObject;
	SelectionRectangle *mSelectionRect;
	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	SceneNode *mSceneNodeArr[128];
	Plane floorG;


		void initParticleSystemForExplosion();
void setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos); 

SOUND *mSound;
SceneNode *mParticleNode;
virtual void createCamera();
virtual void createViewports();
virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
virtual bool frameStarted(const FrameEvent &evt);
virtual bool frameRenderingQueued(const FrameEvent &evt);
public:
	BasicTutorial_00(void);
	virtual void createScene(void);

protected:
	void createPlane(void);
	void createBall(void);
	void createLight(void);
	void createRobot(void);
	void initVar(void);
	void collision(void);
	//void initParticleSystem(void);

protected:
	Ogre::SceneNode *robotSnode[40];
	Entity *robotEntity[40];

	AnimationState *mAnimationState[40];

	bool flgShow[40];
	std::vector<int> chosenKids;

	Vector3 direction[40];
	float distance[40];

	Vector3 robotPosition[40];

	Entity *bigBall;



};
 
#endif // #ifndef __BasicTutorial_00_h_