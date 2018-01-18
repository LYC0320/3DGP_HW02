//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"

using namespace Ogre;
BasicTutorial_00::BasicTutorial_00(void) {}

void BasicTutorial_00::createScene(void) 
{
	mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) ); //bright
	mEntity = mSceneMgr->createEntity( "Robot", "robot.mesh" ); 
	SceneNode *sNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", Vector3( 50, 0, 0 ) ); 
	sNode->attachObject( mEntity ); 
	mSceneNode = sNode;
	mObjScale = 0.2;
	mSceneNode->scale(mObjScale, mObjScale, mObjScale);

	
	mAnimationState = mEntity->getAnimationState("Walk");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
}


bool BasicTutorial_00::frameStarted(const FrameEvent &evt)
{
	Real motion_speed = 0.5;
	Real speedFactor = 20;
	Vector3 velocity(1, 0, 0);
	mAnimationState->addTime(
		mObjScale*motion_speed*evt.timeSinceLastFrame); // time step 
	mSceneNode->translate(
					  mObjScale*motion_speed*speedFactor*velocity
					  *evt.timeSinceLastFrame
					  );


/*


	Real speedFactor = 3*mObjScale*10;
	speedFactor = mObjScale;
	Vector3 velocity(20, 0, 0);
	mAnimationState->addTime(speedFactor*evt.timeSinceLastFrame);
	//mSceneNode->translate(speedFactor*velocity*evt.timeSinceLastFrame);
	mSceneNode->translate(4*speedFactor*velocity*evt.timeSinceLastFrame);
	
	*/
	
	mKeyboard->capture( );
	
	
	if (mKeyboard->isKeyDown(OIS::KC_S)) {
		mAnimationState = mEntity->getAnimationState("Walk");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
	}

	return true;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
