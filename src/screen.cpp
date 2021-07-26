#include "screen.h"

#include<GL/glut.h>


MatrixSimulator::MatrixSimulator(int width, int height) : _width(width), _height(height) {


	if (!font.LoadFont("./matrix/fonts/helvR12.bdf")) {
		fprintf(stderr, "Couldn't load font '%s'\n", "./matrix/fonts/helvR12.bdf");
	}

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
void MatrixSimulator::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue){
	    
    int xtrans = 25 + x*15;
    int ytrans = 25 + y*15;

    glColor3ub(red, green, blue);
    glVertex2i(xtrans,ytrans); 

}
void MatrixSimulator::Clear(){
 
    Fill(0,0,0);

}
void MatrixSimulator::Fill(uint8_t red, uint8_t green, uint8_t blue){

    for(int i = 0; i < height(); i++){
        for(int j = 0; j < width(); j++){
            SetPixel(j, i, red, green, blue);
        }
    }

}

bool MatrixSimulator::setTextFont(char *path){
    return font.LoadFont(path);
}

int MatrixSimulator::text(int x, int y, const rgb_matrix::Color &color, const rgb_matrix::Color *background_color, const char *utf8_text, int extra_spacing){
    return rgb_matrix::DrawText((rgb_matrix::Canvas*)this, font, x, y, color, background_color, utf8_text, extra_spacing);
}
void MatrixSimulator::draw(){

    glEnd();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}




// LedMatrix::LedMatrix(){

// 	//Create RGBMatrix
// 	rgb_matrix::RGBMatrix::Options matrix_options;
// 	rgb_matrix::RuntimeOptions runtime_opt;

// 	runtime_opt.drop_privileges = 0;
// 	matrix_options.hardware_mapping = "adafruit-hat-pwm";  // or e.g. "adafruit-hat"
// 	matrix_options.rows = 32;
// 	matrix_options.chain_length = 4;
// 	matrix_options.parallel = 1;
// 	matrix_options.pixel_mapper_config = "U-mapper;Rotate:180";


// 	m = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_opt);


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