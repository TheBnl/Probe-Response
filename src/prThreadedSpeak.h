/**
 * prThreadedSpeak.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by  on 01-05-15.
 */

#ifndef _PR_THREADED_SPEAK
#define _PR_THREADED_SPEAK

#pragma once
#include "ofMain.h"
#include "ofThread.h"

class prThreadedSpeak: public ofThread {
public:
    string line;
    string voice;
    int volume;
    bool speaking;
    
    /**
     * Set up threaded Speak
     * @author Bram de Leeuw
     */
    prThreadedSpeak() {}
    
    
    /**
     * Start the thread
     * @author Bram de Leeuw
     */
    void start()
    {
        ofThread::startThread();
    }

    
    /**
     * Stop the thread
     * @author Bram de Leeuw
     */
    void stop()
    {
        ofThread::stopThread();
    }
    
    
    /**
     * Things to do while thread is running
     * @author Bram de Leeuw
     */
    void threadedFunction()
    {
        while(ofThread::isThreadRunning()) {
            if(ofThread::lock()) {
                
                if (lastLine != line) {
                    speaking = true;
                    ofSystem("osascript -e 'set volume "+ ofToString(volume) +"'");
                    voice = voice.empty() ? "Bruce" : voice;
                    ofSystem("say -v "+ voice +" "+line);
                    speaking = false;
                    lastLine = line;
                }
                
                ofThread::unlock();
                sleep(200);
                
            } else {
                ofLogWarning("prThreadedApeak") << "Unable to lock mutex";
            }
            
        }
    }
    
protected:
    string lastLine;
    
};

#endif