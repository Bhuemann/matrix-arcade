#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>



class Screen {

public:
    virtual int width() const;
    virtual int height() const;
    virtual void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void clear();
    virtual void fill(uint8_t red, uint8_t green, uint8_t blue);
    virtual void swapOnVSync();
    virtual bool startRecording();
    virtual bool stopRecording();
};

class MatrixSimulator : public Screen {

private:
    const int _width;
    const int _height;

public:
    MatrixSimulator(int width, int height);
    ~MatrixSimulator();
    virtual int width() const;
    virtual int height() const;
    virtual void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void clear();
    virtual void fill(uint8_t red, uint8_t green, uint8_t blue);
    virtual void swapOnVSync();
    // virtual bool startRecording();
    // virtual bool stopRecording();

};

class LedMatrix : public Screen {

public:
    LedMatrix();
    ~LedMatrix();
    virtual int width() const;
    virtual int height() const;
    virtual void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void clear();
    virtual void fill(uint8_t red, uint8_t green, uint8_t blue);
    virtual void swapOnVSync();
    // virtual bool startRecording();
    // virtual bool stopRecording();

};

#endif