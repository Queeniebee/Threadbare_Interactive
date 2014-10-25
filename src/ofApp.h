#pragma once

#define NUM_BYTES 5
#define NUM_CLIPS 12

#include "ofMain.h"
//#include <GLFW/glfw3.h>
//#include "GLFW/glfw3native.h"
#include "ofQTKitPlayer.h"
#include <iostream>
#include "ofxSyphon.h"

using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    ofSerial serial;
    ofSoundPlayer videoSound;
    string paths[NUM_CLIPS];
    
    ofQTKitPlayer *clipsPointer;
    ofQTKitDecodeMode decodeMode;
    ofQTKitPlayer videos[NUM_CLIPS];
    ofFbo fbo, maskFbo;
    ofShader shader;
    
    unsigned char bytesReturned[NUM_BYTES];
    unsigned char message[NUM_BYTES];

    unsigned char *messageBuffer;
    int fromSerialPort[4];

    bool bTimerReached;
    
    float   startTime; // store when we start time timer
    float   endTime; // when do want to stop the timer
//    int firstSensor, secondSensor;
    
    int prevFirst, prevSecond;
    float alphaValue;
    
    int prevSensorReading = 0;

    int selectVideo(int sensorValue);
    void serialEvent();
    int sensors();
    void timerSwitch();
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    
		
};
