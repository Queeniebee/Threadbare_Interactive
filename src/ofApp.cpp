#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    bTimerReached = false;
    
    startTime = ofGetElapsedTimeMillis();  // get the start time
    cout<<startTime<<endl;
//    endTime = (int)1500;
    endTime = (int)1800;
    
    paths[0] = "THREADBARE_hair.mov";
    paths[1] = "THREADBARE_shoes4.mov";
    paths[2] = "THREADBARE_v5_2/THREADBARE_v5_2.mov";
    paths[3] = "THREADBARE_shoes3.mov";
    paths[4] = "THREADBARE_v9/THREADBARE_v9.mov";
    paths[5] = "THREADBARE_shoes1.mov";
    paths[6] = "THREADBARE_v6/THREADBARE_v6.mov";
    paths[7] = "THREADBARE_v7/THREADBARE_v7.mov";
    paths[8] = "THREADBARE_v6/THREADBARE_v6.mov";
    paths[9] = "THREADBARE_v4/THREADBARE_v4.mov";
    paths[10] = "THREABARE_v2/THREABARE_v2.mov";
    paths[11] = "THREADBARE_curtain.mov";
    
    
    clipsPointer = &videos[0];
    clipsPointer->setPixelFormat(OF_PIXELS_RGBA);
    
    decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    for(int i = 0; i< sizeof(videos)/sizeof(ofQTKitPlayer); i++){
        videos[i].loadMovie(paths[i], decodeMode);
    }
    
    clipsPointer->setVolume(0.0f);
    clipsPointer->play();
    
    videoSound.loadSound("Threadbare_voiceover.mp3");
    videoSound.setVolume(0.0f);
    videoSound.setLoop(true);
    videoSound.play();
    
    fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    mainOutputSyphonServer.setName("Screen Output");
    
    cout<<endTime<<endl;
    serial.setup(0, 115200);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();

    float timer = ofGetElapsedTimeMillis() - startTime;
    if(timer >= endTime && !bTimerReached) {
        cout<<"endTime: "<<endTime<<endl;

        bTimerReached = true;
        serialEvent();
        
    } else{
        timerSwitch();
    }
//
    //get these values from the fromSerialPort array
    int currentSensorReading = sensors();
    if(currentSensorReading != prevSensorReading){
        //do stuff here
        //else ignore it
        int currentVideoIndex = selectVideo(currentSensorReading);
        clipsPointer->stop();
        clipsPointer = &videos[currentVideoIndex];
        clipsPointer->setVolume(0.0f);
        clipsPointer->play();
        
    }

    prevSensorReading = currentSensorReading;
    clipsPointer->update();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
    ofClear(0, 0, 0, 0);
    clipsPointer->draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    fbo.end();
    
    fbo.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'D'){
        
        ofToggleFullscreen();
    }
}
//--------------------------------------------------------------
void ofApp::timerSwitch(){
    if(bTimerReached){
        //    if((fmod(ofGetElapsedTimef(), 15) >= 0) && fmod(ofGetElapsedTimef(), 15) <= 1.0){
        bTimerReached = false;                     // reset the timer
        startTime = ofGetElapsedTimeMillis();  // get the start time
//        endTime = (int)1500; // in milliseconds
        endTime = (int)1800;

    }
    
}
//--------------------------------------------------------------
int ofApp::selectVideo(int sensorValue){
    int sensorMin = 0;
    int sensorMax = 400;
    int videoIndexMin = 0;
    int videoIndexMax = 11;
//    int currentVideoIndex  = ofMap(sensorValue, sensorMin, sensorMax, videoIndexMin, videoIndexMax,true);
    int currentVideoIndex;
    
    if((sensorValue >= 1)&&(sensorValue < 23)){
        currentVideoIndex = 0;
    }else if ((sensorValue >= 23) && (sensorValue < 56)){
        currentVideoIndex = 1;
    }else if ((sensorValue >= 56) && (sensorValue < 89)){
        currentVideoIndex = 2;
    }else if ((sensorValue >= 89) && (sensorValue < 122)){
        currentVideoIndex = 3;
    }else if ((sensorValue >= 122) && (sensorValue < 155)){
        currentVideoIndex = 4;
    }else if ((sensorValue >= 155) && (sensorValue < 188)){
        currentVideoIndex = 5;
    }else if ((sensorValue >= 188) && (sensorValue < 221)){
        currentVideoIndex = 6;
    }else if ((sensorValue >= 221) && (sensorValue < 281)){
        currentVideoIndex = 7;
    }else if ((sensorValue >= 281) && (sensorValue < 320)){
        currentVideoIndex = 8;
    }else if ((sensorValue >= 320) && (sensorValue < 343)){
        currentVideoIndex = 9;
    }else if ((sensorValue >= 343) && (sensorValue < 383)){
        currentVideoIndex = 10;
    }else {
        currentVideoIndex = 11;
    }
    cout<<" Current Sensor Value "<<sensorValue<<" Current Video Index "<<currentVideoIndex<<endl;
    
    return currentVideoIndex;
}
//--------------------------------------------------------------
int ofApp::sensors(){
    int firstSensor;
    int secondSensor;
    
    int selectedSensor;

/*    if(fromSerialPort[0] == 1){
        firstSensor = fromSerialPort[1];
        prevFirst = firstSensor;
        cout<<firstSensor<<endl;
    }else if (fromSerialPort[0] == 0){
        firstSensor = 390;
        prevFirst = firstSensor;

    }
    if(fromSerialPort[2] == 2){
        secondSensor = fromSerialPort[3];
        prevSecond = secondSensor;
        cout<<secondSensor<<endl;

    } else if(fromSerialPort[2] == 0){
        secondSensor = 400;
        prevSecond = secondSensor;
    } */
    
    if((fromSerialPort[0] == 1) && (fromSerialPort[2] == 2)){
        firstSensor = fromSerialPort[1];
        secondSensor = fromSerialPort[3];

        prevFirst = firstSensor;
        prevSecond = secondSensor;
        selectedSensor = MAX(firstSensor, secondSensor);

    }else if (fromSerialPort[0] == 0){
        selectedSensor = MAX(prevFirst, prevSecond);
    }
        return (selectedSensor);
    cout<<"Selected: "<<selectedSensor<<endl;

}
//--------------------------------------------------------------
void ofApp::serialEvent(){
    //int(&fromSerialPort)[4]
    serial.writeByte('r');

    while(serial.available() > 0){
        
        memset(bytesReturned, 0, NUM_BYTES*sizeof(uint8));
        memset(message, 0, NUM_BYTES*sizeof(uint8));

        serial.readBytes(bytesReturned, NUM_BYTES*sizeof(uint8));
        
        if(bytesReturned[4] == 'B'){
//            cout<<"HELLO I am here "<<endl;
            memcpy(&message, &bytesReturned, NUM_BYTES*sizeof(uint8));
            break;
            
        }
        else{
            if(bytesReturned[4] != 'B'){
                messageBuffer += *bytesReturned;
//                cout<<"I am NOT here"<<endl;
                serial.flush();

            }
        }
    }
    memset(fromSerialPort, 0, NUM_BYTES*sizeof(uint8));
    
    for(int i = 0; i < 4; i++){
        fromSerialPort[i] = message[i];
//        cout<<"Message"<<i<<": "<<fromSerialPort[i]<<endl;
    }

 
}

