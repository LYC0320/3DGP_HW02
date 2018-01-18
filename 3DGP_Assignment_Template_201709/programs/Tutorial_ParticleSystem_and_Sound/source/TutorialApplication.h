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
	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	Real left, top, right, bottom;
	SelectionRectangle *mSelectionRect;
	SceneNode *mCurrentObject;
	SceneNode *mSceneNodeArr[128];
	SceneNode *mParticleNode;
	Entity *mEntityArr[128];
	bool mFlgTarget;
	Vector3 mTargetPosition;
	bool mFlgSelectNow;
	AnimationState *mAnimationStateArr[128];
	int mMotionStateArr[128];
	int mNumRobots;
	SceneNode *mLightSceneNode;
	void computeTargetPosition();
		void initParticleSystemForExplosion();
void setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos); 

Camera *mCameraMap;
SOUND *mSound;
bool mFlgAllReached;
Real mSphere_Radius;
Vector3 mSphere_Position;
Real mRobot_radius;
SceneNode *mSphere_Node;
virtual void createCamera();
virtual void createViewports();
	int getSceneObjID(const SceneNode *ptr);
void volumeSelect( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
void singleClickSelect ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
virtual bool frameStarted(const FrameEvent &evt);
//
void resolveCollisionRobotPair(int robotA, int robotB, float rA, float rB);

void resolveCollisionForRobots();
public:
	BasicTutorial_00(void);
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_