/**
 * prProbeRequestIn.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Voorbeeld data via TCPDump:
 * 05:53:42.576005 2889891532us tsft 1.0 Mb/s 2412 MHz 11g -69dB signal antenna 1 BSSID:Broadcast DA:Broadcast SA:c0:d9:62:8a:b1:05 (oui Unknown) Probe Request (Ziggo395A4) [1.0* 2.0* 5.5* 6.0 9.0 11.0* 12.0 18.0 Mbit][|802.11]
 *
 * Belangrijke data:
 *  1: Timestamp        = 05:53:42.576005 2889891532us
 *  2: Signaal          = -69dB
 *  3: BSSID (router)   = mac addr
 *  4: DA (router)      = mac addr
 *  5: SA (device)      = c0:d9:62:8a:b1:05
 *  6: SSID             = Ziggo395A4
 *
 * Voorbeeld data via Karma:
 * Jan 1 00:12:09 KARMA: Probe Request from cc:29:f5:cb:f8:b8 for SSID 'newlucht-students'
 * Belangrijke data:
 *  1: Timestamp        = Jan 1 00:12:09
 *  5: macAddress       = cc:29:f5:cb:f8:b8
 *  6: SSID             = newlucht-students
 *
 * Created by  on 02-06-15.
 */

#ifndef _PR_PROBE_REQUEST_IN
#define _PR_PROBE_REQUEST_IN

#pragma once
#include "ofMain.h"
#include "ofxNetwork.h"

class prProbeRequestIn {
    
    public:
        void setup(int port);
        void update();
        string getTimestamp();
        string getSignal();
        string getMacAddress();
        string getBSSID();
        string getDA();
        string getSSID();

    protected:
        string message;
        string timestamp;
        string signal;
        string macAddress;
        string BSSID;
        string DA;
        string SSID;
        string regex;
        bool waiting;
        ofxUDPManager connection;

};

#endif