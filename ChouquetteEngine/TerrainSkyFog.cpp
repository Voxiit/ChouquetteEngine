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

#include "SdkSample.h"
#include "OgrePageManager.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPaging.h"

//New
#define TERRAIN_FILE_PREFIX String("testTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

#define SHADOWS_IN_LOW_LOD_MATERIAL false

//#define PAGING
#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

using namespace Ogre;
using namespace OgreBites;

class ChouquetteEngine : public ApplicationContext, public InputListener
{
public:
    //Values
    Vector3 mTerrainPos = Vector3(1000, 0, 5000);
    bool mTerrainsImported = false;

    //Functions
    ChouquetteEngine();
    virtual ~ChouquetteEngine();

    void setup();
    bool keyPressed(const KeyboardEvent& evt);

    //Terrain functions
    void configureTerrainDefaults(Light* l);
    void defineTerrain(long x, long y, bool flat = false);
    void initBlendMaps(Terrain* terrain);
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

    //Camera
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(mTerrainPos + Vector3(1683, 50, 2116));
    camNode->lookAt(Vector3(1963, 50, 1660), Node::TS_PARENT);
    Camera* cam = scnMgr->createCamera("MainCamera");
    cam->setNearClipDistance(40);
    cam->setFarClipDistance(0);
    camNode->attachObject(cam);


    //Viewport
    Viewport* vp = getRenderWindow()->addViewport(cam);
    vp->setBackgroundColour(ColourValue(0, 0, 0));

    //Set camera ratio
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));


    //Create directionl light
    Ogre::Light* l = scnMgr->createLight();
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDiffuseColour(ColourValue::White);
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
    Ogre::SceneNode* ln = scnMgr->getRootSceneNode()->createChildSceneNode();
    ln->setDirection(Vector3(0.55, -0.3, 0.75).normalisedCopy());
    ln->attachObject(l);

    //Create terrain global option
    Ogre::TerrainGlobalOptions* mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    //Create terrain group object
    Ogre::TerrainGroup* mTerrainGroup = new Ogre::TerrainGroup(scnMgr, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    mTerrainGroup->setOrigin(mTerrainPos);

    //call our terrain configuration method
    configureTerrainDefaults(l);

    //Define our terrain, and ask the terrain group to load them all
    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y);
    //sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    //initialize the blend maps for our terrain
    if (mTerrainsImported)
    {
        for (const auto& ti : mTerrainGroup->getTerrainSlots())
        {
            initBlendMaps(ti.second->instance);
        }
    }
    mTerrainGroup->freeTemporaryResources();

    //we configure the level of detail (LOD)
    mTerrainGlobals->setMaxPixelError(8); // largest allowed error for geometry
    mTerrainGlobals->setCompositeMapDistance(3000); // determines the distance at which Ogre will reduce the texture resolution
    // Important to set these so that the terrain knows what to use for baked (non-realtime) data
    mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(scnMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());

    // get a reference to the import settings of our TerrainGroup and set some basic values
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    // adding the textures our terrain will us
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 200;
    defaultimp.layerList[0].textureNames.push_back("Ground37_diffspec.dds");
    defaultimp.layerList[0].textureNames.push_back("Ground37_normheight.dds");
    defaultimp.layerList[1].worldSize = 200;
    defaultimp.layerList[1].textureNames.push_back("Ground23_diffspec"); // loaded from memory
    defaultimp.layerList[1].textureNames.push_back("Ground23_normheight.dds");
    defaultimp.layerList[2].worldSize = 400;
    defaultimp.layerList[2].textureNames.push_back("Rock20_diffspec.dds");
    defaultimp.layerList[2].textureNames.push_back("Rock20_normheight.dds");

    // pre-merge your textures accordingly e.g. using ImageMagick
    Image combined;
    combined.loadTwoImagesAsRGBA("Ground23_col.jpg", "Ground23_spec.png", "General");
    TextureManager::getSingleton().loadImage("Ground23_diffspec", "General", combined);

}

void ChouquetteEngine::configureTerrainDefaults(Light* l)
{

}

void ChouquetteEngine::defineTerrain(long x, long y, bool flat)
{

}

void ChouquetteEngine::initBlendMaps(Terrain* terrain)
{

}
*/