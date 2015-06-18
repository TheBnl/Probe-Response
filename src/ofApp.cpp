#include "ofApp.h"
#include "compairFirst.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;


void ofApp::setup()
{
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    http.setup("http://127.0.0.1:5000");
    probeRequestIn.setup( 11999 );
    hostNameIn.setup( 22999 );
    
    speak.start();
    speak.voice = "Bruce";
    
    string knownHostNames = http.GET("/api/victimswithhostnames");
    hostNames.setup(knownHostNames);
    
    // SET INITIAL VALUES
    float initialHue = 150.0;
    lastHue = initialHue;
    backgroundHue.reset( initialHue );
    backgroundHue.setRepeatType( PLAY_ONCE );
    backgroundHue.setCurve( EASE_IN_EASE_OUT );
    backgroundColor.setHsb( initialHue, 255, 0 );
    lastMacAdress = "ff:ff:ff:ff:ff:ff";
    lastSSID = "";
    
    //blacklist.push_back("18:0c:ac:12:36:ed"); // Blijft maar om BJNSETUP roepen
    
    // SET UP THE GUI
    hideGUI = false;
    setupGUI();
    gui->loadSettings("settings.xml");
}


void ofApp::update()
{
    probeRequestIn.update();
    hostNameIn.update();
    
    string hostMacAddress = hostNameIn.getMacAddress();
    string hostName = hostNameIn.getHostName();
    if (lastMacAdress != hostMacAddress) { // TODO: if (newData) ...
        string host = hostNames.resolveName(hostMacAddress);
        lastMacAdress = hostMacAddress;
        if (host == "Someone" && !hostMacAddress.empty() && !hostName.empty()) {
            ofLogNotice("ofApp") << "host is not in list add him!";
            ofLogNotice("ofApp") << "Adding: " << hostMacAddress << " " << hostName;
 
            hostNames.saveHostName(hostMacAddress, hostName);
            string saveData = "macAddress="+ hostMacAddress +"&hostName="+ hostName;
            http.POST("/api/victims", saveData);
        }
    }
    
    string SSID = probeRequestIn.getSSID();
    string BSSID = probeRequestIn.getBSSID();
    string DA = probeRequestIn.getDA();
    string macAddress = probeRequestIn.getMacAddress();
    string timestamp = probeRequestIn.getTimestamp();
    string signal = probeRequestIn.getSignal();
    string host = hostNames.resolveName(macAddress);
    string ssid = (SSID.empty() || ofToString(SSID[0]) == ")") ? "something" : SSID;
    float hue = hueFromSignalStrength(ofToFloat( signal ));
    int volume = volumeFromSignalStrength(ofToFloat( signal ));
    
    if (lastTimestamp != timestamp && isNotInBlacklist(macAddress)) {
        vector< pair <string, prVictim> >::iterator victimMacAddressPair = std::find_if( victims.begin(), victims.end(), VictimExists(macAddress) );
        if (victimMacAddressPair != victims.end() && useProbeRequestVictimList) {
            victimMacAddressPair->second.newProbeRequest(ssid, ofToFloat( signal ));
        } else if (useProbeRequestVictimList) {
            prVictim victim;
            victim.setup(host, ofToFloat( signal ));
            victim.newProbeRequest(ssid, ofToFloat( signal ));
            std::pair<string, prVictim> victimMacAddressPair;
            victimMacAddressPair = std::make_pair(macAddress, victim);
            victims.push_back( victimMacAddressPair );
        }
        
        if (useSingleProbeRequest) {
            prSingleProbeRequest singleProbeRequest;
            singleProbeRequest.setup(ssid, ofToFloat( signal ), minSig, maxSig, minHue, maxHue);
            singleProbeRequests.push_back(singleProbeRequest);
        }
        
        if (useProbeRequestPing) {
            prProbeRequestPing ping;
            ping.setup(ofToFloat( signal ), minSig, maxSig, minHue, maxHue);
            pings.push_back(ping);
        }
        
        http.POST("/emit/color", "color="+ofToString(hue) );
        backgroundHue.animateTo(hue);
        
        speak.volume = mute ? 0 : volume;
        speak.line = host +" searched for, "+ ssid + "?";
        speak.timestamp = timestamp;
        
        lastTimestamp = timestamp;
    }
    
    if ( lastSSID != SSID && isNotInBlacklist(macAddress) ) { // TODO: if (newData) ... && Is not in blacklist
        ofLogNotice("ofApp") << SSID +
        ", with mac: "+ BSSID +
        "/"+ DA +
        " probed for by: "+ macAddress +
        ", on: "+ timestamp +
        ", signal: "+ signal;
        
        string saveData = makeSendableData(macAddress, SSID, BSSID, DA, signal, ofToString(volume), ofToString(hue),  timestamp);
        ofLogNotice("ofApp") << "SaveData: " << saveData;
        http.POST("/api/victims", saveData);
        
        lastSSID = SSID;
    }
    
    for (int i=0; i < victims.size(); i++) {
        if (victims.at(i).second.finished) {
            victims.erase(victims.begin() + i);
        } else {
            victims.at(i).second.update(minSig, maxSig, minHue, maxHue);
        }
    }
    
    for (int i=0; i < singleProbeRequests.size(); i++) {
        if (singleProbeRequests.at(i).finished) {
            singleProbeRequests.erase(singleProbeRequests.begin() + i);
        } else {
            singleProbeRequests.at(i).update(minSig, maxSig, minHue, maxHue);
        }
    }
    
    for (int i=0; i < pings.size(); i++) {
        if (pings.at(i).finished) {
            pings.erase(pings.begin() + i);
        } else {
            pings.at(i).update();
        }
    }
    
    backgroundHue.update( 5.0f/60.0f );
    backgroundColor.setHue( backgroundHue.val() );
    ofBackground(backgroundColor);
}


void ofApp::draw()
{
    if (!hideGUI) {
        ofSetColor(ofColor::white);
        string SSID = probeRequestIn.getSSID();
        string ssid = (SSID.empty() || ofToString(SSID[0]) == ")") ? "something" : SSID;
        ofDrawBitmapString("Got probe request for; SSID: " + ssid +" mac: "+ probeRequestIn.getBSSID() +"/"+ probeRequestIn.getDA(), 20, 30);
        ofDrawBitmapString("From: " + probeRequestIn.getMacAddress(), 20, 50);
        ofDrawBitmapString("On: " + probeRequestIn.getTimestamp(), 20, 70);
        ofDrawBitmapString("Signal: " + probeRequestIn.getSignal(), 20, 90);
        
        string speaking = speak.speaking ? "true" : "false";
        ofDrawBitmapString("Speaking: "+ speaking, 20, 130);
        
        string guiHidden = hideGUI ? "true" : "false";
        ofDrawBitmapString("Gui is hidden: "+ guiHidden, 20, 150);
        
        ofDrawBitmapString("hue value: "+ ofToString(backgroundHue.val()) , 20, 170);
    }
    
    for (int i=0; i < victims.size(); i++) {
        victims.at(i).second.draw();
    }
    
    for (int i=0; i < singleProbeRequests.size(); i++) {
        singleProbeRequests.at(i).draw();
    }
    
    for (int i=0; i < pings.size(); i++) {
        pings.at(i).draw();
    }
}


/**
 * Checks if the macadress is not in the blacklist
 */
bool ofApp::isNotInBlacklist(string macAddress)
{
    return find(blacklist.begin(), blacklist.end(), macAddress) == blacklist.end();
}


/**
 *
 */
string ofApp::makeSendableData(string macAddress, string SSID, string BSSID, string DA, string signal, string volume, string hue, string timeStamp)
{
    // Construct the data to be saved in the database
    string saveData;
    // Check if mac address and append
    if (!macAddress.empty()) saveData.append("macAddress="+ macAddress +"&");
    // Check if SSID and append
    if (!SSID.empty()) saveData.append("ssidName="+ SSID +"&");
    // Check if Device destination and log TODO: make smarter to detect if DSSID or DA has the mac addr and append
    if ( (!BSSID.empty() && BSSID != "FF:FF:FF:FF:FF:FF") || (!DA.empty() && DA != "FF:FF:FF:FF:FF:FF") ) {
        if (!BSSID.empty()) {
            saveData.append("ssidMacAddress="+ BSSID +"&");
        }
        else if (!DA.empty()) {
            saveData.append("ssidMacAddress="+ DA +"&");
        }
    }
    // Check signal and append
    if (!signal.empty()) saveData.append("ssidSignal="+ signal +"&");
    // Check volume and append
    if (!volume.empty()) saveData.append("ssidVolume="+ volume +"&");
    // Check hue and append
    if (!hue.empty()) saveData.append("ssidHue="+ hue +"&");
    // Check timeStamp and append
    if (!timeStamp.empty()) saveData.append("timeStamp="+ timeStamp +"&");
    // check if last char is & and remove
    if (saveData.at(saveData.length()-1) == '&') saveData.erase((saveData.length() - 1), saveData.length());

    return saveData;
}


/**
 * Returns an hue value based on the given signal strenght.
 * @author Bram de Leeuw
 *
 * @param float signal
 * @return float hue
 */
float ofApp::hueFromSignalStrength(float signal)
{
    return (((maxHue - minHue) / (maxSig - minSig)) * (signal - minSig)) + minHue;
}


/**
 * Returns an volume based on the given signal strenght.
 * @author Bram de Leeuw
 *
 * @param float signal
 * @return int volume
 */
int ofApp::volumeFromSignalStrength(float signal)
{
    return abs(ceil((((maxVol - minVol) / (maxSig - minSig)) * (signal - minSig)) - maxVol));
}


/**
 * Sets up the gui
 * @author Bram de Leeuw
 */
void ofApp::setupGUI()
{
    gui = new ofxUISuperCanvas("AppSettings");
    gui->addSpacer();
    
    gui->addLabel("Press 'h' to Hide,", OFX_UI_FONT_SMALL);
    gui->addLabel("press 'f' to go fullscreen", OFX_UI_FONT_SMALL);
    gui->addSpacer();
    
    gui->addLabel("Hue Controls");
    gui->addSlider("MAXHUE", 0.0, 255.0, maxHue);
    gui->addSlider("MINHUE", 0.0, 255.0, minHue);
    gui->addSpacer();
    
    gui->addLabel("Signal Controls");
    gui->addSlider("MAXSIG", 0.0, 100.0, maxSig);
    gui->addSlider("MINSIG", 0.0, 100.0, minSig);
    gui->addSpacer();
    
    gui->addLabel("Volume Controls");
    gui->addSlider("MAXVOL", 1, 7, maxVol);
    gui->addSlider("MINVOL", 1, 7, minVol);
    gui->addToggle("MUTE", false);
    gui->addSpacer();
    
    gui->addLabel("Voice Controls");
    gui->addToggle("USE SECOND VOICE", false);
    gui->addSpacer();
    
    gui->addLabel("Visualisation type");
    gui->addToggle("USE PROBE REQUEST VICTIM LIST", true);
    gui->addToggle("USE SINGLE PROBE REQUEST", false);
    gui->addToggle("USE PROBE REQUEST PING", false);
    //gui->addSpacer();
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}


/**
 * Listens to gui events
 * @author Bram de Leeuw
 *
 * @return ofxUIEventArgs event
 */
void ofApp::guiEvent(ofxUIEventArgs &event)
{
    string name = event.getName();
    //int kind = event.getKind();
    
    ofLogNotice("ofApp") << "got event from: " << name;
    
    if (name == "MAXHUE") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        maxHue = slider->getScaledValue();
    }
    
    else if (name == "MINHUE") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        minHue = slider->getScaledValue();
    }
    
    else if (name == "MAXSIG") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        maxSig = slider->getScaledValue();
    }
    
    else if (name == "MINSIG") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        minSig = slider->getScaledValue();
    }
    
    else if (name == "MAXVOL") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        maxVol = slider->getScaledValue();
    }
    
    else if (name == "MINVOL") {
        ofxUISlider *slider = (ofxUISlider *) event.getSlider();
        minVol = slider->getScaledValue();
    }
    
    else if (name == "MUTE") {
        ofxUIToggle *toggle = (ofxUIToggle *) event.getToggle();
        mute = toggle->getValue();
    }
    
    else if (name == "USE SECOND VOICE") {
        ofxUIToggle *toggle = (ofxUIToggle *) event.getToggle();
        useSecondVoice = toggle->getValue();
    }
    
    else if (name == "USE SINGLE PROBE REQUEST") {
        ofxUIToggle *toggle = (ofxUIToggle *) event.getToggle();
        useSingleProbeRequest = toggle->getValue();
    }
    
    else if (name == "USE PROBE REQUEST VICTIM LIST") {
        ofxUIToggle *toggle = (ofxUIToggle *) event.getToggle();
        useProbeRequestVictimList = toggle->getValue();
    }
    
    else if (name == "USE PROBE REQUEST PING") {
        ofxUIToggle *toggle = (ofxUIToggle *) event.getToggle();
        useProbeRequestPing = toggle->getValue();
    }
}


/**
 * Handle keyboard presses
 */
void ofApp::keyPressed(int key) {
    switch (key)
    {
        case 'h':
            gui->toggleVisible();
            hideGUI ? hideGUI = false : hideGUI = true;
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;

        default:
            break;
    }
}


/**
 * Save and quit app
 */
void ofApp::exit()
{
    gui->saveSettings("settings.xml");
    delete gui;
    
    speak.stop();
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
