// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT
/*
#include <iostream>
#include <exception>

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class ChouquetteEngine : public ApplicationContext, public InputListener
{
public:
    ChouquetteEngine();
    virtual ~ChouquetteEngine();

    void setup();
    bool keyPressed(const KeyboardEvent& evt);
};

//constructor
ChouquetteEngine::ChouquetteEngine() : ApplicationContext("Chouquette Engine")
{
}

//destructor
ChouquetteEngine::~ChouquetteEngine() {}

//key_handler
bool ChouquetteEngine::keyPressed(const KeyboardEvent& evt)
{
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}

//setup
void ChouquetteEngine::setup(void)
{
    // do not forget to call the base first
    ApplicationContext::setup();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Root* root = getRoot();
    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen
    //turnlights
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    //newlight
    
    //Light* light = scnMgr->createLight("MainLight");
    //SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    //lightNode->setPosition(20, 80, 50);
    //lightNode->attachObject(light);
    

    //Camera
SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
camNode->setPosition(200, 300, 400);
camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
Camera* cam = scnMgr->createCamera("MainCamera");
//cam->setAutoAspectRatio(true);
cam->setNearClipDistance(5);
camNode->attachObject(cam);

//Viewport
Viewport* vp = getRenderWindow()->addViewport(cam);
vp->setBackgroundColour(ColourValue(0, 0, 0));

//Set camera ratio
cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

/*
//Entity 1
Entity* ogreEntity1 = scnMgr->createEntity("ogrehead.mesh");
SceneNode* ogreNode1 = scnMgr->getRootSceneNode()->createChildSceneNode();
ogreNode1->attachObject(ogreEntity1);

//Entity 2
Entity* ogreEntity2 = scnMgr->createEntity("ogrehead.mesh");
SceneNode* ogreNode2 = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(84, 84, 0));
ogreNode2->attachObject(ogreEntity2);

//Entity 3
Entity* ogreEntity3 = scnMgr->createEntity("ogrehead.mesh");
SceneNode* ogreNode3 = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 104, 0));
ogreNode3->setScale(2, 1.2, 1);
ogreNode3->attachObject(ogreEntity3);

//Entity 4
Entity* ogreEntity4 = scnMgr->createEntity("ogrehead.mesh");
SceneNode* ogreNode4 = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(-84, 48, 0));
ogreNode4->roll(Degree(-90));
ogreNode4->attachObject(ogreEntity4);



//Entitiy
Entity* ninjaEntity = scnMgr->createEntity("ninja.mesh");
ninjaEntity->setCastShadows(true);
scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

//Plane for the ninja to stand on
Plane plane(Vector3::UNIT_Y, 0);
//Create plane's mesh
MeshManager::getSingleton().createPlane(
    "ground", RGN_DEFAULT,
    plane,
    1500, 1500, 20, 20,
    true,
    1, 5, 5,
    Vector3::UNIT_Z);
//Create plane entity
Entity* groundEntity = scnMgr->createEntity("ground");
scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
groundEntity->setCastShadows(false);
groundEntity->setMaterialName("Examples/Rockwall");

//Set Ambient lights and shadow techniques
scnMgr->setAmbientLight(ColourValue(0, 0, 0));
scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

//Create a spot light
Light* spotLight = scnMgr->createLight("SpotLight");
spotLight->setDiffuseColour(0, 0, 1);
spotLight->setSpecularColour(0, 0, 1);
spotLight->setType(Light::LT_SPOTLIGHT);
spotLight->setSpotlightRange(Degree(35), Degree(50));
//Setting spot light transform
SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
spotLightNode->attachObject(spotLight);
spotLightNode->setDirection(-1, -1, 0);
spotLightNode->setPosition(200, 200, 0);

//Create directional Light
Light* directionalLight = scnMgr->createLight("DirectionalLight");
directionalLight->setType(Light::LT_DIRECTIONAL);
directionalLight->setDiffuseColour(0.4, 0, 0);
directionalLight->setSpecularColour(0.4, 0, 0);
//Setting directional light direction
SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
directionalLightNode->attachObject(directionalLight);
directionalLightNode->setDirection(Vector3(0, -1, -1));

//Create point light
Light* pointLight = scnMgr->createLight("PointLight");
pointLight->setType(Light::LT_POINT);
pointLight->setDiffuseColour(0.3, 0.3, 0.3);
pointLight->setSpecularColour(0.3, 0.3, 0.3);
//Setting point light transform
SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
pointLightNode->attachObject(pointLight);
pointLightNode->setPosition(Vector3(0, 150, 250));
}

int main(int argc, char* argv[])
{
    ChouquetteEngine app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}
*/