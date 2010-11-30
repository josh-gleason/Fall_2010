
#include "loadpng.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>

#include "randomc.h"
#include "mother.cpp"

using namespace std;


const float REEL_SIZE = 15.0f;                //The height of the reels
const float REEL_SPACING = REEL_SIZE / 2.6;   //How far apart are the reels

// speed at which the reel "winds up", should be a multiple of 0.0025
const float REEL_START = 0.0125f;

const float PAYLINE_THICKNESS = 0.09f;

const float FRAME_DELAY = 8;

float _angle[3] = {0.0f,0.0f,0.0f};           //The rotation of each reel
bool _spin_done[3] = {true,true,true};        //Flags to prevent double spinning
float _spin_moment[3] = {0.0f, 0.0f, 0.0f};
float _rot[2] = {0.0f, 0.0f};                 // rotation around x and y axis
float _scale = 1.0f;                          //scaling of image
GLuint _textureId;                            //The OpenGL id of the texture
int _currentTex = 0; // 0 FruitBig.png, 1 FruitMedium.png, 2 FruitSmall.png
string _textures[] = {"SlotMM.png", "SlotSM.png", "SlotBi.png"};

int total_credits = 100;
int credits_played = 0;
int _credits_won = 0;
int _credits_lost = 0;

int reelvals[3] = {2,2,2};

CRandomMother rndCls(time(0));

// prints the console to the terminal window
void printConsole()
{
  cout << "\rCredits: " << total_credits << "     Bet: " << credits_played
       << "     Current Payback: " << setprecision(4)
       << (_credits_lost > 0?_credits_won * 100.0 / _credits_lost:0.0)
       << "%           " << flush;
}

// This function is called every time the reels finish spinning, 
// if the index values indicate a winning combination then the
// amount won is calculated and added to the total number of credits
// This should ALWAYS zero out the credits_played variable
void calcPayback( int reelIndex1, int reelIndex2, int reelIndex3 )
{
  reelIndex1--;
  reelIndex2--;
  reelIndex3--;

 // Test the combination made from the reelIndex
 // Table for of the images
 
 // note: forgot to say, these count from bottom up, I reversed them -Josh
 //int _reel1[20] = {5,3,6,7,2,5,1,6,5,4,7,3,6,7,4,6,5,4,3,6};
 //int _reel2[20] = {3,7,5,4,6,3,7,5,4,1,5,7,3,2,5,7,4,6,7,4};
 //int _reel3[20] = {4,6,7,4,2,7,6,5,7,4,6,7,5,4,7,5,3,2,5,7};

 // static for slight time improvement
 static const int _reel1[20] = {6,3,4,5,6,4,7,6,3,7,4,5,6,1,5,2,7,6,3,5};
 static const int _reel2[20] = {4,7,6,4,7,5,2,3,7,5,1,4,5,7,3,6,4,5,7,3};
 static const int _reel3[20] = {7,5,2,3,5,7,4,5,7,6,4,7,5,6,7,2,4,7,6,4};
 
 // Where 1 = President; 2 = Rockstar; 3 = Wedding;
 // 4 = Homerun; 5 = Banana; 6 = Pie

 int credits_won = 0;

 // Get the combination
 int _a = _reel1[reelIndex1];
 int _b = _reel2[reelIndex2];
 int _c = _reel3[reelIndex3];
 // Test the combination
 // President = 500 (1,1,1)
 if( _a == 1 && _b == 1 && _c == 1 )
 {
  credits_won = 500;
 }
 // Rockstar = 200 (2,2,2)
 else if( _a == 2 && _b == 2 && _c == 2 )
 {
  credits_won = 200;
 }
 // Wedding = 50 (3,3,3)
 else if( _a == 3 && _b == 3 && _c == 3 )
 {
  credits_won = 50;
 }
 // Homerun = 20 (4,4,4)
 else if( _a == 4 && _b == 4 && _c == 4 )
 {
  credits_won = 20;
 }
 // Banana = 15 (5,5,5)
 else if( _a == 5 && _b == 5 && _c == 5 )
 {
  credits_won = 15;
 }

 /*
 // 3 Pies = 10 (6,6,6)
 // 2 Pies = 5 (6,6,x)
 // 1 Pie = 2 (6,x,x)
 // Testing the Pies
 if( _a == 6 )
 {
 _pie++;
 }
 if( _b == 6 )
 {
 _pie++;
 }
 if( _c == 6 )
 {
 _pie++;
 }
 switch (_pie) // tests how many pies exist
 {
  case 1: credits_won = 2;
   break;
  case 2: credits_won = 5;
   break;
  case 3: credits_won = 10;
   break;
 }
 */

 // Note: I like the way the above code was implemented, and I hate to
 // remove it, but the cherry's need be left-aligned in order for the
 // odds to work out correctly, per the wizard of odds website :(
 // -Josh
 if ( _a == 6 )
 {
    credits_won = 2;
    if ( _b == 6 )
    {
      credits_won += 3; // makes 5
      if ( _c == 6 )
        credits_won += 5; // makes 10
    }
 }

 // add payback to total number of credits
 total_credits += credits_won;

 _credits_won += credits_won; // running total of credits payed back
 _credits_lost += credits_played; // keep track of total credits lost
 
 // reset the credits played
 // Credits_played will return to 0 in case of loss so when the 
 // function increadseBet is called, credits_played will stay at 0.
 credits_played = 0;
}

// This function is called when the key is pressed that adds a
// credit to play.  So this modifies credits_played and does not
// allow more to played if credits_played == 3, each time this is
// called total_credits should be decreased by 1.  If total_credits
// is zero, don't allow another credit to be played. 
void increaseBet()
{
 // First test if there are credits to be betted on
 if( total_credits > 0 )
 {
  // Three credits will be maxed of the credits played
  if( credits_played < 3 )
  {
   credits_played++;
   total_credits--;
  }
 }
 // otherwise nothing happens to the best
}

// This function is called when the key to insert another coin is
// pressed.  This should probably just add 1 to total_credits
void addCredit()
{
 total_credits++;
}

// This function will be called before the reels are spun, if no 
// credits have been inserted then this should return false, otherwise // return true.
bool allowSpin()
{
 if( credits_played == 0 )
 {
  return false;
 }
 else // if credits_played = 1,2, or 3
 { 
  return true;
 }
}

void initRendering() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.7f, 0.9f, 1.0f, 1.0f);

  _textureId = loadImage(_textures[_currentTex].c_str());
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
  {
    _spin_done[1] = true;    // let others know the spin is complete
  }
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
  {
    _spin_done[2] = true;    // let others know the spin is complete
    // Final reel finished spinning, calculate Payback now
    calcPayback(reelvals[0],reelvals[1],reelvals[2]);
    printConsole();
  }
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

    //cout << _angle[0] << endl;
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
    reelvals[0] = rndCls.IRandom(1,20);
    reelvals[1] = rndCls.IRandom(1,20);
    reelvals[2] = rndCls.IRandom(1,20);

    //cout << reelvals[0] << ' ' << reelvals[1] << ' ' << reelvals[2] << endl;

    // Spin Reels
    delay = 4 * (20 + reelvals[0] - vals[0])-1;
    glutTimerFunc(0, windup1, delay);
    delay = 4 * (20 + reelvals[1] - vals[1])-1;
    glutTimerFunc(50, windup2, delay);
    delay = 4 * (20 + reelvals[2] - vals[2])-1;
    glutTimerFunc(100, windup3, delay);
  }
  return 0;
}

void handleKeypress(unsigned char key, int x, int y) {
  switch (key) {
    case 27: //Escape key
      exit(0);
      break;
    case 'b': // increase bet
      if ( _spin_done[0] && _spin_done[1] && _spin_done[2] )
      {
        increaseBet();
        printConsole();
      }
      break;
    case 'c': // add credit to machine
      if ( _spin_done[0] && _spin_done[1] && _spin_done[2] )
      {
        addCredit();
        printConsole();
      }
      break;
    case 'q': // spin
      if ( allowSpin() )
        spinReels();
      break;
    case 't': // change textures
      _currentTex = (_currentTex + 1) % 3;
      _textureId = loadImage(_textures[_currentTex].c_str());
      glBindTexture(GL_TEXTURE_2D, _textureId);
      glutPostRedisplay();
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
  glutInitWindowSize(400, 400);

  glutCreateWindow("Slot Machine Press 'q'");
  initRendering();

  glutDisplayFunc(drawScene);
  glutKeyboardFunc(handleKeypress);
  glutReshapeFunc(handleResize);

  // random starting location for each
  //_angle[0] = (rand() % 20) * 0.05;
  //_angle[1] = (rand() % 20) * 0.05;
  //_angle[2] = (rand() % 20) * 0.05;

  printConsole();
  glutMainLoop();
  
  cout << endl << endl << "Goodbye!" << endl;

  return 0;
}

