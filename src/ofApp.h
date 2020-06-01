#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGLWarper.h"
#include "ofxNDISender.h"
#include "ofxNDISendStream.h"
#include "ofxNDIReceiver.h"
#include "ofxNDIRecvStream.h"
#include "ofxBonjour.h"
#include "ofxTCPServer.h"
#include "ofxTCPClient.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
        void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
        void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
        void nextSyphon();
        void addWarper();
        void removeWarper();
        void nextWarper();
    
        ofxSyphonServer mainOutputSyphonServer;
        ofxSyphonServerDirectory dir;
        //ofxSyphonClient client;
        int dirIdx;
    
    ofxNDISender sender_;
    ofxNDISendVideo video_;
    
    //ofxNDIReceiver receiver_;
    //ofxNDIRecvVideoFrameSync video_;
    //ofPixels pixels_;
    
    private:
        //ofxBonjourBrowser browser;
        ofxBonjourPublisher publisher;
        /*
        class NotificationReceiver : public ofxBonjourBrowserFoundNotificationReceiverInterface {
            static const string LogTag;
            
            virtual void foundService(const string &type, const string &name, const string &ip, const string &domain) {
                ofLogVerbose(LogTag) << "Found Device: " << type << ", " << name << "@" << ip << " in " << domain;
            }
        } receiver;
         */
    };
