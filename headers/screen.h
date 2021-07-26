#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
//#include "canvas.h"
#include "led-matrix.h"
#include "graphics.h"

// class Canvas {
// public:
//   virtual ~Canvas() {}
//   virtual int width() const = 0; 
//   virtual int height() const = 0;
//   virtual void SetPixel(int x, int y,uint8_t red, uint8_t green, uint8_t blue) = 0;
//   virtual void Clear() = 0;
//   virtual void Fill(uint8_t red, uint8_t green, uint8_t blue) = 0;
// };

class Screen : public rgb_matrix::Canvas {

public:
    // virtual int width() const = 0;
    // virtual int height() const = 0;
    // virtual void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) = 0;
    // virtual void clear() = 0;
    // virtual void fill(uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual bool setTextFont(char *path) = 0;
    virtual int text(int x, int y, const rgb_matrix::Color &color, const rgb_matrix::Color *background_color, const char *utf8_text, int extra_spacing) = 0;
    virtual void draw() = 0;
    // virtual bool startRecording();
    // virtual bool stopRecording();
};

class MatrixSimulator : public Screen {

private:
    const int _width;
    const int _height;
    rgb_matrix::Font font = rgb_matrix::Font();

public:
    MatrixSimulator(int width, int height);
    ~MatrixSimulator();
    virtual int width() const;
    virtual int height() const;
    virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void Clear();
    virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);
    virtual bool setTextFont(char *path);
    virtual int text(int x, int y, const rgb_matrix::Color &color, const rgb_matrix::Color *background_color, const char *utf8_text, int extra_spacing);
    virtual void draw();
    // virtual bool startRecording();
    // virtual bool stopRecording();

};



// class LedMatrix : public Screen, public rgb_matrix::Canvas {

// private:
//     rgb_matrix::RGBMatrix *m;

// public:
//     LedMatrix();
//     ~LedMatrix();
//     virtual int width() const;
//     virtual int height() const;
//     virtual void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
//     virtual void clear();
//     virtual void fill(uint8_t red, uint8_t green, uint8_t blue);
//     virtual void swapOnVSync();
//     // virtual bool startRecording();
//     // virtual bool stopRecording();

// };

#endif