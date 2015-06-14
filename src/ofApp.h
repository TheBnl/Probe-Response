#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxAnimatableFloat.h"
#include "ofxUI.h"
#include "prThreadedSpeak.h"
//#include "prThreadedHostNameIn.h"
//#include "prThreadedProbeRequestIn.h"
#include "prProbeRequestIn.h"
#include "prHostNameIn.h"
#include "prHostNames.h"
#include "prVictim.h"
#include "prMan.h"
#include "prLight.h"
#include "prHTTP.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void createVictim(string victimName, string victimMacAddress, float probeSignal);
        void victimExists(string macAddress);
        void setupGUI();
        void guiEvent(ofxUIEventArgs &event);
    
        float hueFromSignalStrength(float signal);
        int volumeFromSignalStrength(float signal);
        bool isNotInBlacklist(string macAddress);
        string makeSendableData(string macAddress, string SSID, string BSSID, string DA, string signal, string volume, string hue, string timeStamp);
    
        ofxUDPManager connection;
        ofxUISuperCanvas *gui;
        ofxUIDropDownList *dropDownList;
        prThreadedSpeak speak;
        prProbeRequestIn probeRequestIn;
        prHostNameIn hostNameIn;
        prMan man;
        prLight *light;
        prHTTP http;
        prHostNames hostNames;
    
        string lastSSID;
        string lastMacAdress;
        string lastTimestamp;
        float lastHue;
        ofColor backgroundColor;
        ofxAnimatableFloat backgroundHue;
    
        vector<string> blacklist;
        vector< pair <string, prVictim> > victims;
        bool hideGUI;
        float maxVol;
        float minVol;
        bool mute;
        bool useSecondVoice;
        float maxHue;
        float minHue;
        float maxSig;
        float minSig;
    
};
