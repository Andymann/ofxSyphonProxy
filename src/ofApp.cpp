#include "ofApp.h"

const string TITLE = "ofxSyphonProxy";
const int FRAMERATE= 30;
const string FILENAME = "ofxSyphonProxySettings.xml";
bool bInfo = true;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( TITLE );
    ofSetEscapeQuitsApp(false);
    ofSetWindowShape(512, 384);
    ofSetFrameRate(FRAMERATE);
    ofBackground(0,0,0);
    mainOutputSyphonServer.setName("ofxSyphonProxy");

    //setup our directory
    dir.setup();
    //setup our client
    client.setup();

    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);

    dirIdx = -1;
    
    warper.setup(10, 10, ofGetWidth()-20, ofGetHeight()-20);
    warper.activate();
    
    ofBackground(255, 0, 0);
    warper.drawSettings.bDrawRectangle = true;
    warper.load( FILENAME );
    
}


//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();

    warper.begin();
    if(dir.isValidIndex(dirIdx))
        client.draw(0, 0, ofGetWidth(), ofGetHeight() );
    warper.end();
    
    mainOutputSyphonServer.publishScreen();
    
    
    if(bInfo){
        ofRectangle rect;
        rect.x = 6;
        rect.y = 10;
        rect.width = ofGetWidth()-12;
        rect.height = 180;
        
        ofSetColor(66,66,66);
        ofDrawRectangle(rect);
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("Press SPACE key to cycle through all Syphon servers.", ofPoint(20, 30));
        ofDrawBitmapString("Press 1 to set resolution to 1024 * 768.", ofPoint(20, 45));
        ofDrawBitmapString("Press 2 to set resolution to 512 * 384.", ofPoint(20, 60));
        
        ofDrawBitmapString("Press 'w' to toggle warping.", ofPoint(20, 90));
        ofDrawBitmapString("Press 's' to save warping.", ofPoint(20, 105));
        
        ofDrawBitmapString("Press 'i' to toggle this info.", ofPoint(20, 135));
        
        ofDrawBitmapString("Output resolution is " + ofToString(ofGetWidth()) + "*" + ofToString(ofGetHeight()) + ". Framerate is " + ofToString(FRAMERATE) + "fps.", ofPoint(20, 165));
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key==' '){
        if (dir.size() > 1) //Because we are already 1
        {
            dirIdx++;
            bool bShowingSelf=true;
            
            while (bShowingSelf){
                if(dirIdx > dir.size() - 1)
                dirIdx = 0;
                
                client.set(dir.getDescription(dirIdx));
                string serverName = client.getServerName();
                string appName = client.getApplicationName();
                
                
                if(serverName != TITLE ){
                    bShowingSelf=false;
                    
                    if(serverName == ""){
                        serverName = "null";
                    }
                    if(appName == ""){
                        appName = "null";
                    }
                    
                    ofSetWindowTitle(serverName + ":" + appName);
                }else{
                    //We do not want to feed our own Syphon stream back to ourselves.
                    //ofLogNotice("Syphon Feedback. Selecting next");
                    dirIdx++;
                }
            }
            
        }
        else
        {
            ofSetWindowTitle("No Server");
        }
    }else if(key=='1'){
        ofSetWindowShape(1024, 768);
    }else if(key=='2'){
        ofSetWindowShape(512, 384);
    }else if(key=='3'){
        ofSetWindowShape(256, 192);
    }else if(key=='w'){
        if(warper.isActive()){
            warper.deactivate();
        }else{
            warper.activate();
        }
    }else if(key=='i'){
        if(bInfo){
            bInfo = false;
        }else{
            bInfo=true;
        }
    }else if(key=='s'){
        warper.save( FILENAME );
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
