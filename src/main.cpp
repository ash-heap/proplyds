#include <cstdlib>
#include <cstdio>
#include <string>
#include <sciurus/sciurus.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/random.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

//appname
static const char* appname = "Miles Rufat-Latre: Space Simulator 2014 Premium";
//window, infrastructure
static sc::Window* window = NULL;
static const char* wtitle = appname;
static const int scrix = SDL_WINDOWPOS_UNDEFINED; //initial screen x position
static const int scriy = SDL_WINDOWPOS_UNDEFINED; //initial screen y position
static int scrw = 800; //screen width
static int scrh = 600; //screen height
static const uint32_t rflags = SDL_RENDERER_ACCELERATED |
                               SDL_RENDERER_PRESENTVSYNC |
                               SDL_RENDERER_TARGETTEXTURE;
//viewing parameters
static double fov = 70.0; //vertical field of view
static double aspect = (float)scrw / (float)scrh; //aspect ratio
static double cnear = 0.1; //near viewing plane
static double cfar = 3000.0; //far viewing plane
static bool drawnorms = false;
//CUSTOMVALUES: ADJUST THESE VALUES TO CHANGE THE SIMULATION!
static const int NUM_ASTEROIDS = 128; //how many asteroids?
static const float astmin = 1.f; //minimum asteroid radius
static const float astrange = 9.f; //max - min asteroid radius
static const float gconst = 0.4f; //gravitational constant
static const float rotator = 5.f; //initial velocity of objects
static const bool induce = true; //induce circular orbits?
static const float density = 1.f; //density of objects
static const float friction = 0.01f; //friction of objects
static const float bounciness = 0.5f; //between 0 and 1
static const float spawnradius = 500.f; //radius of initial field
static const btVector3 shipinitpos = btVector3(0.f, 0.f, 750.f);
static const btVector3 shipinitvel = btVector3(10.f, 0.f, 0.f);
//what is the position of the ship relative to the camera?
static const glm::vec3 cameraoffset = glm::vec3(0.f, 0.5f, 5.f);
static const float shipthrust = 0.1f; //multiplier for ship thrust
static const float shiptorque = 0.05f; //multiplier for ship torque
static const float xmin = -1500.f; //x coordinate of left wall
static const float xmax = 1500.f; //x coordinate of right wall
static const float ymin = -1500.f; //y coordinate of bottom wall
static const float ymax = 1500.f; //y coordinate of top wall
static const float zmin = -1500.f; //z coordinate of front wall
static const float zmax = 1500.f; //z coordinate of back wall
static const float gridsize = 300.f; //size of marker grid squares
//divide this by the distance to the object for final particle size
//static double jetmul = jetsize * (double)scrh / glm::cos(fov);
//window management, input
static unsigned int ms = 0;
static unsigned int dt = 0;
static bool resume = true;
static const int nkeys = 282; //there are 282 scancodes
static char kbstate[nkeys]; //one keystate for each key
static const char KDOWN = 0x01; //button is currently down
static const char KPRESSED = 0x02; //button was pressed since last clearing
static const char KRELEASED = 0x04; //button was released since last clearing
static const char KPRESSEDNOW = KDOWN | KPRESSED; //do not use for reading
static const char KNOTDOWN = ~KDOWN; //do not use for reading
static inline char presskey(char key){return key | KPRESSEDNOW;}
static inline char releasekey(char key){return (key & KNOTDOWN) | KRELEASED;}
static inline char clearkey(char key){return key & KDOWN;}
static inline void initkb(){for(int i = 0; i < nkeys; i++) kbstate[i] = 0x00;}
static inline void clearkb()
    {for(int i = 0; i < nkeys; i++) kbstate[i] = clearkey(kbstate[i]);}
static inline void keydown(SDL_Scancode s) //key press callback
    {kbstate[s] = presskey(kbstate[s]);}
static inline void keyup(SDL_Scancode s) //key release callback
    {kbstate[s] = releasekey(kbstate[s]);}
static inline char getkey(SDL_Keycode key) //get raw key state
    {return kbstate[SDL_GetScancodeFromKey(key)];}
static inline bool iskeydown(SDL_Keycode key) //get key KDOWN state
    {return getkey(key) & KDOWN;}
static inline bool waskeypressed(SDL_Keycode key) //get key KPRESSED state
    {return getkey(key) & KPRESSED;}
static inline bool waskeyreleased(SDL_Keycode key) //get key KRELEASED state
    {return getkey(key) & KRELEASED;}
//builds new normals for mesh, clobbering the old ones if they exist
static inline void mknorms(aiMesh* mesh)
{
    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        unsigned int* indices = mesh->mFaces[f].mIndices;
        aiVector3D a = mesh->mVertices[indices[0]];
        aiVector3D b = mesh->mVertices[indices[1]];
        aiVector3D c = mesh->mVertices[indices[2]];
        aiVector3D u = b - a;
        aiVector3D v = c - a;
        aiVector3D norm;
        norm.x = u[1] * v[2] - u[2] * v[1];
        norm.y = u[2] * v[0] - u[0] * v[2];
        norm.z = u[0] * v[1] - u[1] * v[0];
        norm = norm.Normalize();
        mesh->mNormals[indices[0]] = norm;
        mesh->mNormals[indices[1]] = norm;
        mesh->mNormals[indices[2]] = norm;
    }
}
static inline aiMesh* loadmesh(const char* filename)
{
    Assimp::Importer importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(filename,
          aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_ImproveCacheLocality
        //| aiPocess_GenerateNormals
    );
    aiMesh* dst = new aiMesh();
    aiMesh* src = scene->mMeshes[0];
    dst->mNumVertices = src->mNumVertices;
    dst->mVertices = new aiVector3D[dst->mNumVertices];
    for(unsigned int i = 0; i < dst->mNumVertices; i++)
        dst->mVertices[i] = src->mVertices[i];
    dst->mNormals = new aiVector3D[dst->mNumVertices];
    dst->mNumFaces = src->mNumFaces;
    dst->mFaces = new aiFace[dst->mNumFaces];
    for(unsigned int i = 0; i < dst->mNumFaces; i++)
    {
        aiFace* df = &(dst->mFaces[i]);
        aiFace* sf = &(src->mFaces[i]);
        df->mNumIndices = sf->mNumIndices;
        df->mIndices = new unsigned int[df->mNumIndices];
        for(unsigned int j = 0; j < df->mNumIndices; j++)
            df->mIndices[j] = sf->mIndices[j];
    }
    mknorms(dst);
    return dst;
}
static inline void drawmesh(aiMesh* mesh)
{
    aiVector3D* verts = mesh->mVertices;
    aiVector3D* norms = mesh->mNormals;
    glBegin(GL_TRIANGLES);
    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        unsigned int* indices = mesh->mFaces[f].mIndices;
        glNormal3fv((float*)&norms[indices[0]]);
        glVertex3fv((float*)&verts[indices[0]]);
        glNormal3fv((float*)&norms[indices[1]]);
        glVertex3fv((float*)&verts[indices[1]]);
        glNormal3fv((float*)&norms[indices[2]]);
        glVertex3fv((float*)&verts[indices[2]]);
    }
    glEnd();
    if(drawnorms)
    {
        glDisable(GL_LIGHTING);
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glBegin(GL_LINES);
        for(unsigned int v = 0; v < mesh->mNumVertices; v++)
        {
            aiVector3D a = mesh->mVertices[v];
            aiVector3D n = mesh->mNormals[v];
            aiVector3D b = a + (n * 0.2f);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}
static inline void drawaxes(float dim = 1.f)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(dim, 0.f, 0.f);
        glColor4f(0.f, 1.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(0.f, dim, 0.f);
        glColor4f(0.f, 0.f, 1.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(0.f, 0.f, dim);
    glEnd();
    glEnable(GL_LIGHTING);
}

static inline void drawgrid()
{
    unsigned int nx = (xmax - xmin) / gridsize;
    unsigned int ny = (ymax - ymin) / gridsize;
    unsigned int nz = (zmax - zmin) / gridsize;
    float xpos = xmin;
    float ypos = ymin;
    float zpos = zmin;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    //draw the x-grid
    for(unsigned int y = 0; y <= ny; y++)
    {
        zpos = zmin;
        for(unsigned int z = 0; z <= nz; z++)
        {
            glVertex3f(xmin, ypos, zpos);
            glVertex3f(xmax, ypos, zpos);
            zpos += gridsize;
        }
        ypos += gridsize;
    }
    //draw the y-grid
    xpos = xmin;
    zpos = zmin;
    for(unsigned int x = 0; x <= nx; x++)
    {
        zpos = zmin;
        for(unsigned int z = 0; z <= nz; z++)
        {
            glVertex3f(xpos, ymin, zpos);
            glVertex3f(xpos, ymax, zpos);
            zpos += gridsize;
        }
        xpos += gridsize;
    }
    //draw the z-grid
    xpos = xmin;
    ypos = ymin;
    for(unsigned int x = 0; x <= nx; x++)
    {
        ypos = ymin;
        for(unsigned int y = 0; y <= ny; y++)
        {
            glVertex3f(xpos, ypos, zmin);
            glVertex3f(xpos, ypos, zmax);
            ypos += gridsize;
        }
        xpos += gridsize;
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

//scene information
static string shipfile = "ship-colored.ply";
//static string shipfile = "icosphere.obj";
static string spherefile = "icosphere.obj";
static float totalmass = 0.f;
static float meanmass = 0.f;
static aiMesh* astmeshes[NUM_ASTEROIDS];
static float astmasses[NUM_ASTEROIDS];
static btRigidBody* astbodies[NUM_ASTEROIDS];
static btDiscreteDynamicsWorld* physworld = NULL;
static aiMesh* shipmesh = NULL;
static btRigidBody* shipbody = NULL;
static btDefaultMotionState* shipstate = NULL;
static btRigidBody* boxbody;
static btBroadphaseInterface* broadphase = NULL;
static btDefaultCollisionConfiguration* collisionConfiguration = NULL;
static btCollisionDispatcher* dispatcher = NULL;
static btSequentialImpulseConstraintSolver* solver = NULL;
static GLfloat mat_specular[4] = {1.f, 1.f, 1.f, 1.f};
static GLfloat mat_diffuse[4] = {0.5f, 0.5f, 0.5f, 1.f};
static GLfloat mat_shininess[1] = {50.f};
static GLfloat light_global_ambient[] = {0.05f, 0.05f, 0.05f, 1.f};
static GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

static inline void gouradsphere(aiMesh* sphere, float radius = 1.f)
{
    aiVector3D* verts = sphere->mVertices;
    aiVector3D* norms = sphere->mNormals;
    for(unsigned int i = 0; i < sphere->mNumVertices; i++)
    {
        norms[i] = verts[i].Normalize();
        verts[i] = norms[i] * radius;
    }
}
static inline float getasth(glm::vec3 pos)
{
    return glm::simplex(pos) * 0.3f;
}

static inline void genasteroid(aiMesh* mesh, float radius = 1.f)
{
    aiVector3D* verts = mesh->mVertices;
    glm::vec3 roff = glm::ballRand(10000.f);
    for(unsigned int v = 0; v < mesh->mNumVertices; v++)
    {
        aiVector3D c = verts[v];
        verts[v] =
            ((c + (c * getasth(glm::vec3(c.x, c.y, c.y) + roff))) * radius);
    }
}

static inline void perspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, cnear, cfar);
    glMatrixMode(GL_MODELVIEW);
}
static inline void resize(int w, int h)
{
    scrw = w; scrh = h;
    aspect = (float)scrw / (float)scrh;
    glViewport(0, 0, scrw, scrh);
    perspective();
}
static inline void setfov(double theta){fov = theta; perspective();}

static inline void windowevent(SDL_WindowEvent e)
{
    switch(e.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
            resize(e.data1, e.data2);
            break;
        default:
            break;
    }
}

static inline void handleevents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                resume = false;
                break;
            case SDL_KEYDOWN:
                if(e.key.repeat) break;
                keydown(e.key.keysym.scancode);
                //printf("KEY DN: %d\n", e.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                if(e.key.repeat) break;
                keyup(e.key.keysym.scancode);
                //printf("KEY UP: %d\n", e.key.keysym.scancode);
                break;
            case SDL_WINDOWEVENT:
                windowevent(e.window);
                break;
            default:
                break;
        }
    }
}

static unsigned int oldms = ms;
static inline void update()
{
    oldms = ms;
    ms = SDL_GetTicks();
    if(oldms > ms) dt = 0;
    else dt = ms - oldms;
    
    handleevents();
    if(waskeypressed(SDLK_ESCAPE)) resume = false;
    if(waskeypressed(SDLK_n)) drawnorms = !drawnorms;
    
    btTransform t;
    shipbody->getMotionState()->getWorldTransform(t);
    t.setOrigin(btVector3(0.f, 0.f, 0.f));
    float dc = iskeydown(SDLK_LSHIFT) ? 3.f * (float)dt : (float)dt;
    if(iskeydown(SDLK_DOWN))
        shipbody->applyTorque(t * (btVector3(shiptorque, 0.f, 0.f) * dt));
    if(iskeydown(SDLK_UP))
        shipbody->applyTorque(t * (btVector3(-shiptorque, 0.f, 0.f) * dt));
    if(iskeydown(SDLK_RIGHT))
        shipbody->applyTorque(t * (btVector3(0.f, 0.f, -shiptorque)
                                                * (float)dt));
    if(iskeydown(SDLK_LEFT))
        shipbody->applyTorque(t * (btVector3(0.f, 0.f, shiptorque)
                                                * (float)dt));
    if(iskeydown(SDLK_q))
        shipbody->applyTorque(t * (btVector3(0.f, shiptorque, 0.f)
                                                * (float)dt));
    if(iskeydown(SDLK_e))
        shipbody->applyTorque(t * (btVector3(0.f, -shiptorque, 0.f)
                                                * (float)dt));
    if(iskeydown(SDLK_a))
        shipbody->applyCentralForce(t * (btVector3(-shipthrust, 0.f, 0.f) 
                                                * (float)dc));
    if(iskeydown(SDLK_d))
        shipbody->applyCentralForce(t * (btVector3(shipthrust, 0.f, 0.f)
                                                * (float)dc));
    if(iskeydown(SDLK_w))
        shipbody->applyCentralForce(t * (btVector3(0.f, 0.f, -shipthrust)
                                                * (float)dc));
    if(iskeydown(SDLK_s))
        shipbody->applyCentralForce(t * (btVector3(0.f, 0.f, shipthrust)
                                                * (float)dc));
    if(iskeydown(SDLK_LCTRL))
        shipbody->applyCentralForce(t * (btVector3(0.f, -shipthrust, 0.f)
                                                * (float)dc));
    if(iskeydown(SDLK_SPACE))
        shipbody->applyCentralForce(t * (btVector3(0.f, shipthrust, 0.f)
                                                * (float)dc));
    
    clearkb();
    
    btVector3 ship = shipbody->getCenterOfMassPosition();
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
    {
        btVector3 ast = astbodies[i]->getCenterOfMassPosition();
        btVector3 diff = ast - ship;
        float len = diff.length();
        btVector3 dir = diff / len;
        btVector3 force = dir * ((gconst * astmasses[i]) / (len * len));
        shipbody->applyCentralForce(force);
    }
    
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
    {
        btVector3 p1 = astbodies[i]->getCenterOfMassPosition();
        for(unsigned int j = i + 1; j < NUM_ASTEROIDS; j++)
        {
            if(j == i) continue;
            btVector3 p2 = astbodies[j]->getCenterOfMassPosition();
            btVector3 diff = p2 - p1;
            float len = diff.length();
            btVector3 dir = diff / len;
            //apply direct force to p1, inverse to p2
            btVector3 force = dir * ((gconst * astmasses[i] * astmasses[j]) /
                                               (len * len));
            astbodies[i]->applyCentralForce(force);
            astbodies[j]->applyCentralForce(-force);
        }
    }
    //step physics
    physworld->stepSimulation(0.001 * (float)dt, 10);
}

static inline void draw()
{
    //prepare for rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //get ship position
    btTransform t;
    shipbody->getMotionState()->getWorldTransform(t);
    glm::mat4 tf;
    t.getOpenGLMatrix(glm::value_ptr(tf));
    glm::mat4 cameratf = glm::translate(tf, cameraoffset);
    //set up light
    //glm::vec4 lglobpos = glm::vec4(light_position[0], light_position[1],
    //                               light_position[2], light_position[3]);
    float lightparam = (float)ms * 0.0002f;
    glm::vec4 lglobpos = glm::vec4(
                glm::simplex(glm::vec3(lightparam, 0.f, 0.f)),
                glm::simplex(glm::vec3(lightparam + 1032.233f, 0.f, 0.f)),
                glm::simplex(glm::vec3(lightparam + 261.57f, 0.f, 0.f)), 1.f
               ) * spawnradius;
    //glm::vec4 llocpos = glm::inverse(tf) * lglobpos;
    glm::vec4 llocpos = tf * lglobpos;
    llocpos[3] = 1.f;
    lglobpos[3] = 1.f;
    //prepare the basic scene transformations
    glPushMatrix(); //1
    glMultMatrixf(glm::value_ptr(glm::inverse(cameratf)));
    glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lglobpos));
    glPushMatrix(); //2
    glMultMatrixf(glm::value_ptr(tf));
    drawmesh(shipmesh);
    glPopMatrix(); //1
    //draw the light
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
    {
        glShadeModel(GL_SMOOTH);
        astbodies[i]->getMotionState()->getWorldTransform(t);
        glm::mat4 tf;
        t.getOpenGLMatrix(glm::value_ptr(tf));
        
        glPushMatrix(); //2
        glMultMatrixf(glm::value_ptr(tf));
        drawmesh(astmeshes[i]);
        glPopMatrix(); //1
        glShadeModel(GL_FLAT);
    }
    btVector3 com = btVector3(0.f, 0.f, 0.f);
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
        com += astbodies[i]->getCenterOfMassPosition() * astmasses[i];
    com /= (float)NUM_ASTEROIDS * meanmass;
    glPushMatrix(); //2
    //glTranslatef(com.getX(), com.getY(), com.getZ());
    glColor4f(0.5f, 0.5f, 0.5f, 1.f);
    drawgrid();
    drawaxes(1000.f);
    
    glDisable(GL_LIGHTING);
    glPointSize(15.f);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glBegin(GL_POINTS);
    glVertex3f(lglobpos.x, lglobpos.y, lglobpos.z);
    glEnd();
    
    glEnable(GL_LIGHTING);
    //glPopMatrix(); //2
    glPopMatrix(); //1
    glPopMatrix(); //0
    
    window->swapBuffers();
    //SDL_GL_SwapWindow(window);
}

static inline void mainloop()
{
    while(resume)
    {
        update();
        draw();
    }
}

static inline void initGL()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glEnable(GL_LIGHT0);
    perspective();
}

static inline void initscene()
{
    shipmesh = loadmesh(shipfile.c_str());
    
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    physworld = new btDiscreteDynamicsWorld(dispatcher, broadphase, 
                                            solver, collisionConfiguration);
    physworld->setGravity(btVector3(0.f, 0.f, 0.f));
    
    btTriangleMesh* physmesh = new btTriangleMesh();
    aiFace* faces = shipmesh->mFaces;
    aiVector3D* verts = shipmesh->mVertices;
    for(unsigned int i = 0; i < shipmesh->mNumFaces; i++)
    {
        unsigned int* indices = faces[i].mIndices;
        aiVector3D v1 = verts[indices[0]];
        aiVector3D v2 = verts[indices[1]];
        aiVector3D v3 = verts[indices[2]];
        physmesh->addTriangle
        (
            btVector3(v1.x, v1.y, v1.z),
            btVector3(v2.x, v2.y, v2.z),
            btVector3(v3.x, v3.y, v3.z)
        );
    }
    
    btCollisionShape* shipshape = new btBvhTriangleMeshShape(physmesh, false);
    //shipshape = new btSphereShape(1.3f);
    shipstate =
        new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f),
                                             shipinitpos));
    btScalar mass = 1.f;
    btVector3 shipinertia(1.f, 1.f, 1.f);
    btRigidBody::btRigidBodyConstructionInfo info =
        btRigidBody::btRigidBodyConstructionInfo(mass, shipstate,
                                                 shipshape, shipinertia);
    info.m_linearDamping = 0.f;
    info.m_angularDamping = 0.f;
    info.m_friction = 0.1f;
    info.m_rollingFriction = 0.01f;
    info.m_restitution = 0.1f;
    shipbody = new btRigidBody(info);
    shipbody->setActivationState(DISABLE_DEACTIVATION);
    physworld->addRigidBody(shipbody);
    shipbody->setLinearVelocity(shipinitvel);
    //meanmass = 0.f;
    totalmass = 0.f;
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
    {
        astmeshes[i] = loadmesh(spherefile.c_str());
        //float size = glm::simplex(glm::vec3((float)i + 0.223));
        float size = (float)rand() / (float)RAND_MAX;;
        //size = size * size;
        size = astrange * size + astmin;
        gouradsphere(astmeshes[i], size);
        //genasteroid(astmeshes[i], size);
        
        btCollisionShape* shape = new btSphereShape(size);
        glm::vec3 init = glm::ballRand(spawnradius);
        btDefaultMotionState* ms = new btDefaultMotionState(
                        btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f),
                                    btVector3(init.x, init.y, init.z)));
        astmasses[i] = 1.33333333333f * glm::pi<float>() *
                        size * size * size * size * density;
        //meanmass += astmasses[i];
        totalmass += astmasses[i];
        btVector3 astinertia(0.f, 0.f, 0.f);
        shape->calculateLocalInertia(astmasses[i], astinertia);
        btRigidBody::btRigidBodyConstructionInfo info =
            btRigidBody::btRigidBodyConstructionInfo(astmasses[i], ms,
                                                     shape, astinertia);
        info.m_linearDamping = 0.f;
        info.m_angularDamping = 0.f;
        info.m_friction = friction;
        info.m_rollingFriction = 0.1f * friction;
        info.m_restitution = bounciness;
        astbodies[i] = new btRigidBody(info);
        astbodies[i]->setActivationState(DISABLE_DEACTIVATION);
        physworld->addRigidBody(astbodies[i]);
        glm::vec3 vel = glm::ballRand(rotator);
        if(induce)
        {
            glm::vec3 bias = glm::normalize
            (
                glm::cross
                (
                    glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(init.x, init.y, 0.f)
                )
            );
            vel = rotator * bias;
        }
        astbodies[i]->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
    }
    meanmass = totalmass / (float)NUM_ASTEROIDS;
    //meanmass /= (float)NUM_ASTEROIDS;
    btCollisionShape* wall1shape =
        new btStaticPlaneShape(btVector3(0.f, 0.f, 1.f), zmin);
    btCollisionShape* wall2shape =
        new btStaticPlaneShape(btVector3(0.f, 0.f, -1.f), -zmax);
    btCollisionShape* wall3shape =
        new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), ymin);
    btCollisionShape* wall4shape =
        new btStaticPlaneShape(btVector3(0.f, -1.f, 0.f), -ymax);
    btCollisionShape* wall5shape =
        new btStaticPlaneShape(btVector3(1.f, 0.f, 0.f), xmin);
    btCollisionShape* wall6shape =
        new btStaticPlaneShape(btVector3(-1.f, 0.f, 0.f), -xmax);
    btCompoundShape* wallshape = new btCompoundShape();
    wallshape->addChildShape(btTransform::getIdentity(), wall1shape);
    wallshape->addChildShape(btTransform::getIdentity(), wall2shape);
    wallshape->addChildShape(btTransform::getIdentity(), wall3shape);
    wallshape->addChildShape(btTransform::getIdentity(), wall4shape);
    wallshape->addChildShape(btTransform::getIdentity(), wall5shape);
    wallshape->addChildShape(btTransform::getIdentity(), wall6shape);
    btRigidBody::btRigidBodyConstructionInfo info2 =
        btRigidBody::btRigidBodyConstructionInfo
        (
            10000000.f, new btDefaultMotionState(btTransform::getIdentity()),
            wallshape, btVector3(10000000.f, 10000000.f, 10000000.f)
        );
    info2.m_friction = friction;
    info2.m_rollingFriction = 0.1f * friction;
    info2.m_restitution = 0.95f;
    boxbody = new btRigidBody(info2);
    physworld->addRigidBody(boxbody);
}

static inline void cleanscene()
{
    delete shipmesh;
    for(unsigned int i = 0; i < NUM_ASTEROIDS; i++)
    {
        delete astmeshes[i];
        delete astbodies[i]->getMotionState();
        delete astbodies[i];
    }
    
    physworld->removeRigidBody(shipbody);
    delete shipbody->getMotionState();
    delete shipbody;
    
    delete boxbody->getMotionState();
    delete boxbody;
    
    delete physworld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

int main(int argc, char** argv)
{
    sc::init();
    window = new sc::Window();
    if(!window->valid())
    {
        fprintf(stderr, sc::getError().c_str());
        return -1;
    }
    
    initGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window->swapBuffers();
    //SDL_GL_SwapWindow(window);
    //SDL_ShowWindow(window);
    
    clearkb();
    
    initscene();
    mainloop();
    cleanscene();
    
    SDL_Quit();

    return 0;
}

