
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <cv.h>
#include <highgui.h>
#include <ctime>

using namespace std;
using namespace cv;

const float REEL_SIZE = 15.0f;                   //The height of the Reel
const float REEL_SPACING = REEL_SIZE / 2.6;

float _angle[3] = {0.0f,0.0f,0.0f};            //The rotation of the box
bool _spin_done[3] = {true,true,true};
float FRAME_DELAY = 15;
GLuint _textureId;                             //The OpenGL id of the texture

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Mat image) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D,
      0,
      GL_RGB,
      image.cols, image.rows,
      0,
      GL_BGR,
      GL_UNSIGNED_BYTE,
      image.data);
  return textureId;
}

void initRendering() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  Mat image = imread("Reels.png");
  Mat flipped;
  flip( image, flipped, 0 );  // because lower left is 0,0 in opengl
  _textureId = loadTexture(flipped);
}

void handleResize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
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

  //	glRotatef(-_angle, 1.0f, 1.0f, 0.0f);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glColor3f(1.0f, 1.0f, 1.0f);

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

  glutSwapBuffers();
}

//Called every 25 milliseconds
void reel1(int value) {
  _angle[0] += 0.0125f;
  if (_angle[0] >= 1.0) {
    _angle[0] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, reel1, value-1);
  else
    _spin_done[0] = true;    // let others know the spin is complete
}

//Called every 25 milliseconds
void reel2(int value) {
  _angle[1] += 0.0125f;
  if (_angle[1] >= 1.0) {
    _angle[1] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, reel2, value-1);
  else
    _spin_done[1] = true;    // let others know the spin is complete
}

//Called every 25 milliseconds
void reel3(int value) {
  _angle[2] += 0.0125f; 
  if (_angle[2] >= 1.0) {
    _angle[2] -= 1.0;
  }
  glutPostRedisplay();
  if ( value >= 1 )
    glutTimerFunc(FRAME_DELAY, reel3, value-1);
  else
    _spin_done[2] = true;    // let others know the spin is complete
}

void handleKeypress(unsigned char key, int x, int y) {
  switch (key) {
    case 27: //Escape key
      exit(0);
      break;
    case 'q':
      if (_spin_done[0] && _spin_done[1] && _spin_done[2])
      {
        int rand_num = -1;

        rand_num += 4*((rand() % 20) + 10);
        glutTimerFunc(FRAME_DELAY, reel1, rand_num);
        rand_num += 4*((rand() % 20)+1);
        glutTimerFunc(FRAME_DELAY, reel2, rand_num);
        rand_num += 4*((rand() % 20)+1);
        glutTimerFunc(FRAME_DELAY, reel3, rand_num);
        
        _spin_done[0] = false;
        _spin_done[1] = false;
        _spin_done[2] = false;
      }
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

  srand( time(0) );

  // random starting location for each
  _angle[0] = (rand() % 20) * 0.05;
  _angle[1] = (rand() % 20) * 0.05;
  _angle[2] = (rand() % 20) * 0.05;

  glutMainLoop();
  return 0;
}









