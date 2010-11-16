
#include "loadpng.h"
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>

using namespace std;

const float REEL_SIZE = 15.0f;                //The height of the reels
const float REEL_SPACING = REEL_SIZE / 2.6;   //How far apart are the reels

// speed at which the reel "winds up", should be a multiple of 0.0025
const float REEL_START = 0.0125f;

const float PAYLINE_THICKNESS = 0.09f;

const float FRAME_DELAY = 20;

float _angle[3] = {0.0f,0.0f,0.0f};           //The rotation of each reel
bool _spin_done[3] = {true,true,true};        //Flags to prevent double spinning
float _spin_moment[3] = {0.0f, 0.0f, 0.0f};
float _rot[2] = {0.0f, 0.0f};                 // rotation around x and y axis
float _scale = 1.0f;                          //scaling of image
GLuint _textureId;                            //The OpenGL id of the texture

void initRendering() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.7f, 0.9f, 1.0f, 1.0f);

  _textureId = loadImage("BigFruitNice.png");
  glBindTexture(GL_TEXTURE_2D, _textureId);
}

void handleResize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, -20.0f);

  GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

  GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat lightPos[] = {-2 * REEL_SIZE, REEL_SIZE, 4 * REEL_SIZE, 1.0f};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  // Set up texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glColor3f(1.0f, 1.0f, 1.0f);
  glRotatef(_rot[0], 1.0f, 0.0f, 0.0f);
  glRotatef(_rot[1], 0.0f, 1.0f, 0.0f);
  glScalef(_scale,_scale,_scale);

  glBegin(GL_QUADS);

  float offset;
  for ( int i = 0; i < 3; i++ )
  {
    offset = (i-1.0f) * REEL_SPACING;

    // Draw Reels
    glNormal3f(0.0, 0.0f, 1.0f);
    glTexCoord2f(0.2f*i, 0.0f+_angle[i]);
    glVertex3f(offset+-REEL_SIZE / 6, -REEL_SIZE / 2, -5.0);
    glTexCoord2f(0.2f*(i+1), 0.0f+_angle[i]);
    glVertex3f(offset+REEL_SIZE / 6, -REEL_SIZE / 2, -5.0);
    glTexCoord2f(0.2f*(i+1), 0.15f+_angle[i]);
    glVertex3f(offset+REEL_SIZE / 6, REEL_SIZE / 2, -5.0);
    glTexCoord2f(0.2f*i, 0.15f+_angle[i]);
    glVertex3f(offset+-REEL_SIZE / 6, REEL_SIZE / 2, -5.0);
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);

  // Draw payline
  glBegin(GL_QUADS);
  glColor3f(0.8f,0.0f,0.0f);
  glVertex3f(-REEL_SPACING-REEL_SIZE / 6.0f, -PAYLINE_THICKNESS/2.0f, -4.9f);
  glVertex3f(REEL_SPACING+REEL_SIZE / 6.0f, -PAYLINE_THICKNESS/2.0f, -4.9f);
  glVertex3f(REEL_SPACING+REEL_SIZE / 6.0f, PAYLINE_THICKNESS/2.0f, -4.9f);
  glVertex3f(-REEL_SPACING-REEL_SIZE / 6.0f, PAYLINE_THICKNESS/2.0f, -4.9f);
  glEnd();

  glutSwapBuffers();
}

// value is the number of frames to spin for, 4 frames is 1 symbol
void spin_reel1(int value) {
  _angle[0] += 0.0125f;
  if (_angle[0] >= 1.0) {
    _angle[0] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, spin_reel1, value-1);
  else
    _spin_done[0] = true;    // let others know the spin is complete
}

void spin_reel2(int value) {
  _angle[1] += 0.0125f;
  if (_angle[1] >= 1.0) {
    _angle[1] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, spin_reel2, value-1);
  else if (_spin_done[0])
    _spin_done[1] = true;    // let others know the spin is complete
  else
  {
    value += 80;
    glutTimerFunc(FRAME_DELAY, spin_reel2, value-1);
  }
}

void spin_reel3(int value) {
  _angle[2] += 0.0125f; 
  if (_angle[2] >= 1.0) {
    _angle[2] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, spin_reel3, value-1);
  else if (_spin_done[1])
    _spin_done[2] = true;    // let others know the spin is complete
  else
  {
    value += 80;
    glutTimerFunc(FRAME_DELAY, spin_reel3, value-1);
  }
}

void windup1(int value)
{
  _angle[0] += _spin_moment[0];
  _spin_moment[0] += 0.0025f;

  glutPostRedisplay();

  if ( _spin_moment[0] <= REEL_START )
    glutTimerFunc(FRAME_DELAY, windup1, value);
  else
    glutTimerFunc(0, spin_reel1, value);
}

void windup2(int value)
{
  _angle[1] += _spin_moment[1];
  _spin_moment[1] += 0.0025f;

  glutPostRedisplay();

  if ( _spin_moment[1] <= REEL_START )
    glutTimerFunc(FRAME_DELAY, windup2, value);
  else
    glutTimerFunc(0, spin_reel2, value);
}

void windup3(int value)
{
  _angle[2] += _spin_moment[2];
  _spin_moment[2] += 0.0025f;

  glutPostRedisplay();

  if ( _spin_moment[2] <= REEL_START )
    glutTimerFunc(FRAME_DELAY, windup3, value);
  else
    glutTimerFunc(0, spin_reel3, value);
}

void round( float& val )
{
  float intpart;
  if ( modf(val, &intpart) >= 0.5f )
    val = ceil(val);
  else
    val = floor(val);
}

int spinReels()
{
  if (_spin_done[0] && _spin_done[1] && _spin_done[2])
  {
    // remove any truncation errors in _angle
    for ( int i = 0; i < 3; i++ )
    {
      _angle[i] *= 80;
      round(_angle[i]);
      _angle[i] /= 80;
    }

    cout << _angle[0] << endl;
    //if ( modf(_angle[0],1.0f) >= 0.5
    

    _spin_done[0] = false;
    _spin_done[1] = false;
    _spin_done[2] = false;
   
    // will change after new random number generator is implemented

    int vals[3] = 
      { (int)round(_angle[0] / 0.05 + 2) % 20,
        (int)round(_angle[1] / 0.05 + 2) % 20,
        (int)round(_angle[2] / 0.05 + 2) % 20 };

    // wrap to 20 if zero
    vals[0] = (vals[0] == 0 ? 20 : vals[0]);
    vals[1] = (vals[1] == 0 ? 20 : vals[1]);
    vals[2] = (vals[2] == 0 ? 20 : vals[2]);

    // set up the spring action thingy
  	_spin_moment[0] = -REEL_START;
  	_spin_moment[1] = -REEL_START;
  	_spin_moment[2] = -REEL_START;
    
    int delay;

    // These are the new values of the reels, 1-20
    int newvals[3] =
      { rand() % 20 + 1,
        rand() % 20 + 1,
        rand() % 20 + 1 };

    cout << newvals[0] << ' ' << newvals[1] << ' ' << newvals[2] << endl;

    // Spin Reels
    delay = 4 * (20 + newvals[0] - vals[0])-1;
    glutTimerFunc(0, windup1, delay);
    delay = 4 * (20 + newvals[1] - vals[1])-1;
    glutTimerFunc(50, windup2, delay);
    delay = 4 * (20 + newvals[2] - vals[2])-1;
    glutTimerFunc(100, windup3, delay);
  }
  return 0;
}

void handleKeypress(unsigned char key, int x, int y) {
  switch (key) {
    case 27: //Escape key
      exit(0);
      break;
    case 'q':
      spinReels();
      break;
    case 'w': // rotate
      _rot[0] += 5;
      if ( _rot[0] > 360 )
        _rot[0] -= 360;
      glutPostRedisplay();
      break;
    case 'a':
      _rot[1] += 5;
      if ( _rot[1] > 360 )
        _rot[1] -= 360;
      glutPostRedisplay();
      break;
    case 's':
      _rot[0] -= 5;
      if ( _rot[0] < 0 )
        _rot[0] += 360;
      glutPostRedisplay();
      break;
    case 'd':
      _rot[1] -= 5;
      if ( _rot[1] < 0 )
        _rot[1] += 360;
      glutPostRedisplay();
      break;
    case '+': // zoom
      _scale *= 1.1;
      glutPostRedisplay();
      break;
    case '-':
      _scale /= 1.1;
      glutPostRedisplay();
      break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);

  glutCreateWindow("Slot Machine Press 'q'");
  initRendering();

  glutDisplayFunc(drawScene);
  glutKeyboardFunc(handleKeypress);
  glutReshapeFunc(handleResize);

  srand( time(0) );

  // random starting location for each
  //_angle[0] = (rand() % 20) * 0.05;
  //_angle[1] = (rand() % 20) * 0.05;
  //_angle[2] = (rand() % 20) * 0.05;

  glutMainLoop();
  return 0;
}









