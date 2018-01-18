//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"
using namespace Ogre;
using namespace std;
BasicTutorial_00::BasicTutorial_00(void) {}

void BasicTutorial_00::createScene(void) 
{
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);

	//
	Light *light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(0, 150, 250)); 
	light->setDiffuseColour(0.0, 1.0, 0.5);		//red
	light->setSpecularColour(0.0, 1.0, 0.5);	//red


	int i;

	for (int j = 0; j < 5; ++j) 
	{
		for (int k = 0; k < 5; ++k) 
		{

			i = j*5+k;
			String name;
			genNameUsingIndex("r", i, name);

			mEntityArr[i] 
			= mSceneMgr
				->createEntity( name, "robot.mesh" ); 

			mSceneNodeArr[i]  
			= mSceneMgr
				->getRootSceneNode()
				->createChildSceneNode(); 

			Vector3 p;
			p.x = k*100 - 100*5/2;
			p.y = 0.0;
			p.z = j*100 - 100*5/2;
			mSceneNodeArr[i]->setPosition(p);
			mSceneNodeArr[i]->attachObject( mEntityArr[i] );
		}
	}
	//
	mCurrentObject = mSceneNodeArr[0];

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

}
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;
	top = scn.y;
	mSelectionRect->setCorners(left, top, right, bottom);
	mSelectionRect->setVisible(true);

	return BaseApplication::mouseMoved( arg);

}

bool BasicTutorial_00::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
/*
	if (left==right && top==bottom) {
		return BaseApplication::mouseReleased( arg, id );
		
	}
	*/
	/* CEGUI
	Ray topLeft = mCamera->getCameraToViewportRay(left, top);
	Ray topRight = mCamera->getCameraToViewportRay(right, top);
	Ray bottomLeft = mCamera->getCameraToViewportRay(left, bottom);
	Ray bottomRight = mCamera->getCameraToViewportRay(right, bottom);
	*/

	//USING mTrayMgr=============
	Real nleft = left;
	Real nright = right;
	Real ntop = 1+top;
	Real nbottom = 1+bottom;
	//Left = Xmin
	//Right= Xmax
	//Bottom=Ymin
	//Top = Ymax
	cout << "nleft:" << nleft << endl;
	cout << "nright:" << nright << endl;
	cout << "nbottom:" << nbottom << endl;
	cout << "ntop:" << ntop << endl;
	Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));
	Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
	Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));
	//End Using mTrayMgr=============

	// The plane faces the counter clockwise position.
	PlaneBoundedVolume vol;
	int np = 100;
	vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         // top plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), 	topLeft.getPoint(np)));       // left plane
	vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), 	bottomLeft.getPoint(np)));   // bottom plane
	vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), 	bottomRight.getPoint(np)));     // right plane 

	PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	mVolQuery->setVolumes(volList);

	SceneQueryResult result = mVolQuery->execute();

	SceneQueryResultMovableList::iterator itr = result.movables.begin();


	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.
	for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
	{
		if (*itr)
		{
			mCurrentObject = (*itr)->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			mCurrentObject->showBoundingBox(!flgShow);
		}
	}


	return BaseApplication::mouseReleased( arg, id );

}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;  // (left, top) = (scn.x, scn.y)
	top = scn.y;

	right = scn.x;
	bottom = scn.y;

	mSelectionRect->setCorners(left, top, right, bottom);

	return BaseApplication::mousePressed( arg, id );
}

bool BasicTutorial_00::frameStarted(const FrameEvent &evt)
{
	mSelectionRect->updateDT(evt.timeSinceLastFrame);
	mSelectionRect->updateMesh();
	for (int i= 0; i < 25; ++i)
	{
		if (mSceneNodeArr[i]->getShowBoundingBox()==false) continue;
		AnimationState *animateState = mEntityArr[i]->getAnimationState("Walk");
		animateState->setLoop(true);
		animateState->setEnabled(true);
		animateState->addTime(evt.timeSinceLastFrame);
	}
	return BaseApplication::frameStarted(evt);
}
int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
