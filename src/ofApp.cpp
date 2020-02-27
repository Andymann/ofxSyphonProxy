#include "ofApp.h"

const string TITLE = "ofxSyphonProxy v0.5";
const int FRAMERATE= 30;
int iWarperIndex = 0;
const int MAXWARPER = 4;
//const string FILENAME = "ofxSyphonProxySettings_" + ofToString(iWarperIndex) +".xml";
bool bInfo = true;
bool bFullScreen =false;
int iCacheHeight;
int iCacheWidth;

std::vector<ofxGLWarper> vecWarp;
std::vector<ofxSyphonClient> vecClient;
//--------------------------------------------------------------
void ofApp::setup(){
        
    ofSetWindowTitle( TITLE );
    ofSetEscapeQuitsApp(false);
    ofSetWindowShape(512, 384);
    iCacheWidth=512;
    iCacheHeight=384;
    ofSetFrameRate(FRAMERATE);
    ofBackground(0,0,0);
    mainOutputSyphonServer.setName(TITLE);
    
    //setup our directory
    dir.setup();
    
    //setup our client
    //client.setup();

    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);

    dirIdx = -1;
    
    ofBackground(255, 0, 0);
    
    //----Den ersten immer
    ofApp::addWarper();
    
    for(int i=1; i<MAXWARPER; i++){
        if(ofFile().doesFileExist("ofxSyphonProxySettings_" + ofToString(i) +".xml")){
            ofApp::addWarper();
        }
    }
    
    if(sender_.setup("ofxNDISender example")) {
        video_.setup(sender_);
        video_.setAsync(true);
    }
    
    
    
    //publisher.publish("_tl_tcpvt.", "TCPSyphonXY", 59852);
    //browser.setup();
    //browser.setFoundNotificationReceiver(&receiver);
    //browser.startBrowse("_ofxbonjour_example._tcp");
       
    //publisher.setup();
    //publisher.publish("_ofxbonjour_example._tcp", "test_pub", 12345);
    /*
    publisher.setup();
    if( publisher.publish("_tl_tcpvt._tcp", "TCPSyphonUKU", iTCPSyphonPort) ){
        ofLogNotice("Bonjour OK");
    }
    */

    
}



//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
    
    if(dir.size()>=2){
        nextSyphon();
    }
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

void ofApp::removeWarper(){
    if(vecWarp.size()>1){
        ofFile().removeFile("ofxSyphonProxySettings_" + ofToString(vecWarp.size()-1) +".xml");
        vecWarp.erase(vecWarp.end()-1);
        vecClient.erase(vecClient.end()-1);
    }
}

void ofApp::addWarper(){
    if(vecWarp.size()<MAXWARPER){
        for(int i=0; i<vecWarp.size(); i++){
            vecWarp.at(i).deactivate();
        }
        
        vecWarp.push_back(ofxGLWarper());
        vecWarp.back().setup(10, 10, ofGetWidth()-20, ofGetHeight()-20);
        vecWarp.back().activate();
        vecWarp.back().drawSettings.bDrawRectangle = true;
        vecWarp.back().load( "ofxSyphonProxySettings_" + ofToString(vecWarp.size()-1) +".xml" );
        
        vecClient.push_back( ofxSyphonClient() );
        vecClient.back().setup();
        
    }
}

void ofApp::nextWarper(){
    bool bDone = false;
    for(int i=0; i<vecWarp.size()-1; i++){
        if(vecWarp.at(i).isActive()){
            vecWarp.at(i).deactivate();
            vecWarp.at(i+1).activate();
            bDone = true;
            break;
        }
        
    }
    if(!bDone){
        //----Der letzte ist aktiv; der erste wird aktiviert
        if(vecWarp.at(vecWarp.size()-1).isActive()){
            vecWarp.at(vecWarp.size()-1).deactivate();
        }
        vecWarp.front().activate();
    }
}

void ofApp::nextSyphon(){
    int iActiveIndex = -1;
    for(int i=0; i<vecWarp.size();i++){
        if(vecWarp.at(i).isActive()){
            iActiveIndex=i;
            break;
        }
    }
    
    if(iActiveIndex!=-1){
        if (dir.size() > 1) /*Because we are already 1*/{
            dirIdx++;
            bool bShowingSelf=true;
            
            while (bShowingSelf){
                if(dirIdx > dir.size() - 1)
                dirIdx = 0;
                
                //client.set(dir.getDescription(dirIdx));
                //string serverName = client.getServerName();
                //string appName = client.getApplicationName();
                vecClient.at(iActiveIndex).set(dir.getDescription(dirIdx));
                string serverName = vecClient.at(iActiveIndex).getServerName();
                string appName = vecClient.at(iActiveIndex).getApplicationName();
                
                if(serverName != TITLE ){
                    bShowingSelf=false;
                    
                    if(serverName == ""){
                        serverName = "null";
                    }
                    if(appName == ""){
                        appName = "null";
                    }
                    
                    //ofSetWindowTitle(serverName + ":" + appName);
                }else{
                    //We do not want to feed our own Syphon stream back to ourselves.
                    //ofLogNotice("Syphon Feedback. Selecting next");
                    dirIdx++;
                }
            }
            
        }else{
            //ofSetWindowTitle("No Server");
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();
    
    for(int i=0; i<vecWarp.size();i++){
        vecWarp.at(i).begin();
        if(dir.isValidIndex(dirIdx)){
            //client.draw(0, 0, ofGetWidth(), ofGetHeight() );
            vecClient.at(i).draw(0, 0, ofGetWidth(), ofGetHeight() );
        }
        vecWarp.at(i).end();
    }
    
    mainOutputSyphonServer.publishScreen();
    
    
    if(bInfo){
        ofRectangle rect;
        rect.x = 6;
        rect.y = 10;
        rect.width = ofGetWidth()-12;
        rect.height = 250;
        
        ofSetColor(66,66,66);
        ofDrawRectangle(rect);
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("Press SPACE key to cycle through all Syphon servers.", ofPoint(20, 30));
        ofDrawBitmapString("Press 1 to set resolution to 1024 * 768.", ofPoint(20, 45));
        ofDrawBitmapString("Press 2 to set resolution to 512 * 384.", ofPoint(20, 60));
        ofDrawBitmapString("Press 'f' to toggle fullscreen.", ofPoint(20, 75));
        
        ofDrawBitmapString("Press 'w' to toggle warping.", ofPoint(20, 90));
        ofDrawBitmapString("Press 'W' to deactive warping.", ofPoint(20, 105));
        ofDrawBitmapString("Press 's' to save warping.", ofPoint(20, 120));
        ofDrawBitmapString("Press 'R' to reset warping.", ofPoint(20, 135));
        
        ofDrawBitmapString("Press '+' to add, '-' to remove a Warper.", ofPoint(20, 150));
        ofDrawBitmapString("Press 'i' to toggle this info.", ofPoint(20, 165));
        
        ofDrawBitmapString("Output resolution is " + ofToString(ofGetWidth()) + "*" + ofToString(ofGetHeight()) + ". Framerate is " + ofToString(FRAMERATE) + "fps.", ofPoint(20, 195));
    }
    
    /*
     
     //----NDI-Stuff
    ofPixels pixels;
    ofFbo fbo;
    
    //ofGetGLRenderer()->saveFullViewport(pixels); //That'some expensive MoFo
    //ofGetGLRenderer()->saveScreen(10, 10, 100, 100, pixels);
    //fbo.readToPixels(pixels,OF_PIXELS_BGRA);
    //ofLogNotice("PXL Format:" + ofToString( pixels.getPixelFormat() ) );
    //video_.send(pixels);
    
    //----AVAHI
    //----ServiceInfo info = ServiceInfo.create("_tl_tcpvt.", "TCPSyphonXY", 59852, 0, 0, "");
    */
    
   
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key==' '){
        nextSyphon();
    }else if(key=='1'){
        ofSetWindowShape(1024, 768);
    }else if(key=='2'){
        ofSetWindowShape(512, 384);
    }else if(key=='3'){
        ofSetWindowShape(256, 192);
    }else if(key=='w'){
        //if(vecWarp.at(0).isActive()){
        //    vecWarp.at(0).deactivate();
        //}else{
        //    vecWarp.at(0).activate();
        //}
        ofApp::nextWarper();
    }else if(key=='W'){
        for(int i=0; i<vecWarp.size();i++){
            vecWarp.at(i).deactivate();
        }
    }else if(key=='i'){
        if(bInfo){
            bInfo = false;
        }else{
            bInfo=true;
        }
    }else if(key=='s'){
        //warper.save( FILENAME );
        //vecWarp.at(0).save( FILENAME );
        for(int i=0; i<vecWarp.size();i++){
            vecWarp.at(i).save( "ofxSyphonProxySettings_" + ofToString(i) +".xml" );
        }
    }else if(key=='f'){
        
        
        bool bActive = vecWarp.at(0).isActive();
        float fx_tl = vecWarp.at(0).getCorner(ofxGLWarper::TOP_LEFT).x/ofGetWidth();
        float fy_tl = vecWarp.at(0).getCorner(ofxGLWarper::TOP_LEFT).y/ofGetHeight();
        float fx_tr = vecWarp.at(0).getCorner(ofxGLWarper::TOP_RIGHT).x/ofGetWidth();
        float fy_tr = vecWarp.at(0).getCorner(ofxGLWarper::TOP_RIGHT).y/ofGetHeight();
        float fx_br = vecWarp.at(0).getCorner(ofxGLWarper::BOTTOM_RIGHT).x/ofGetWidth();
        float fy_br = vecWarp.at(0).getCorner(ofxGLWarper::BOTTOM_RIGHT).y/ofGetHeight();
        float fx_bl = vecWarp.at(0).getCorner(ofxGLWarper::BOTTOM_LEFT).x/ofGetWidth();
        float fy_bl = vecWarp.at(0).getCorner(ofxGLWarper::BOTTOM_LEFT).y/ofGetHeight();
        
        bFullScreen = !bFullScreen;
        ofSetFullscreen(bFullScreen);
        
        //warper.setup(10, 10, ofGetWidth()-20, ofGetHeight()-20);
        vecWarp.at(0).setup(10, 10, ofGetWidth()-20, ofGetHeight()-20);
        if(bActive){
            vecWarp.at(0).activate();
        }else{
            vecWarp.at(0).deactivate();
        }
       
        vecWarp.at(0).setCorner(ofxGLWarper::TOP_LEFT, glm::vec2(fx_tl*ofGetWidth(), fy_tl*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::TOP_RIGHT, glm::vec2(fx_tr*ofGetWidth(), fy_tr*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::BOTTOM_RIGHT, glm::vec2(fx_br*ofGetWidth(), fy_br*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::BOTTOM_LEFT, glm::vec2(fx_bl*ofGetWidth(), fy_bl*ofGetHeight()));
        
    }else if(key=='R'){
        vecWarp.at(0).setCorner(ofxGLWarper::BOTTOM_LEFT, glm::vec2(.25*ofGetWidth(), .75*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::BOTTOM_RIGHT, glm::vec2(.75*ofGetWidth(), .75*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::TOP_RIGHT, glm::vec2(.75*ofGetWidth(), .25*ofGetHeight()));
        vecWarp.at(0).setCorner(ofxGLWarper::TOP_LEFT, glm::vec2(.25*ofGetWidth(), .25*ofGetHeight()));
        vecWarp.at(0).activate();
    }else if(key=='+'){
        ofApp::addWarper();
    }else if(key=='-'){
        ofApp::removeWarper();
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
