#include "screen.h"

#include<GL/glut.h>


MatrixSimulator::MatrixSimulator(int width, int height) : _width(width), _height(height) {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );

    // making background color dark gray
    glClearColor(24.0/255.0, 24.0/255.0, 24.0/255.0, 1.0f);
    
    // set initial draw color to black
    glColor3ub(0,0,0);

    // Set draw size
    glPointSize(10.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
     
    // setting window dimension in X and Y direction
    gluOrtho2D(0, 1000, 0, 1000);
    glutCreateWindow( "MatrixSimulator" );

}
MatrixSimulator::~MatrixSimulator(){

}

int MatrixSimulator::height() const {
    return _height;
}
int MatrixSimulator::width() const {
    return _width;
}
void MatrixSimulator::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue){
	    
    int xtrans = 25 + x*15;
    int ytrans = 25 + y*15;

    glColor3ub(red, green, blue);
    glVertex2i(xtrans,ytrans); 

}
void MatrixSimulator::clear(){
 
    fill(0,0,0);

}
void MatrixSimulator::fill(uint8_t red, uint8_t green, uint8_t blue){

    for(int i = 0; i < height(); i++){
        for(int j = 0; j < width(); j++){
            setPixel(j, i, red, green, blue);
        }
    }

}
void MatrixSimulator::swapOnVSync(){

    glEnd();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}



// LedMatrix::LedMatrix(){

// }
// LedMatrix::~LedMatrix(){

// }
// int LedMatrix::height() const {

// }
// int LedMatrix::width() const {

// }
// void LedMatrix::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue){

// }
// void LedMatrix::clear(){

// }
// void LedMatrix::fill(uint8_t red, uint8_t green, uint8_t blue){

// }
// void LedMatrix::swapOnVSync(){

// }
// bool LedMatrix::startRecording(){

// }
// bool LedMatrix::stopRecording(){

// }