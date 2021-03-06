/*
 * Dibbler - a portable DHCPv6
 *
 * authors: Tomasz Mrugalski <thomson@klub.com.pl>
 *          Marek Senderski <msend@o2.pl>
 * changes: Michal Kowalczuk <michal@kowalczuk.eu>
 *
 * released under GNU GPL v2 only licence
 */

class TMsg;
#ifndef MSG_H
#define MSG_H

#include <iostream>
#include <string>
#include "SmartPtr.h"
#include "Container.h"
#include "DHCPConst.h"
#include "IPv6Addr.h"
#include "Opt.h"
#include "KeyList.h"

// Hey! It's grampa of all messages
class TMsg
{
  public:
    // Used to create TMsg object (normal way)
    TMsg(int iface, SmartPtr<TIPv6Addr> addr, int msgType);
    TMsg(int iface, SmartPtr<TIPv6Addr> addr, int msgType, long transID);

    // used to create TMsg object based on received char[] data
    TMsg(int iface, SmartPtr<TIPv6Addr> addr, char* &buf, int &bufSize);
    
    virtual int getSize();
    
    // transmit (or retransmit)

    virtual unsigned long getTimeout();

    virtual int storeSelf(char * buffer);

    virtual string getName() = 0;

    // returns requested option (or NULL, there is no such option)
    SmartPtr<TOpt> getOption(int type);
    void firstOption();
    int countOption();

    virtual SmartPtr<TOpt> getOption();
    
    long getType();
    long getTransID();
    TContainer< SmartPtr<TOpt> > getOptLst();
    SmartPtr<TIPv6Addr> getAddr();
    int getIface();
    virtual ~TMsg();
    bool isDone();

    // auth stuff below
    void setAuthInfoPtr(char* ptr);
    int setAuthInfoKey();
    void setAuthInfoKey(char *ptr);
    char * getAuthInfoKey();
    bool validateAuthInfo(char *buf, int bufSize, List(DigestTypes) authLst);
    bool validateAuthInfo(char *buf, int bufSize);
    uint32_t getAAASPI();
    void setAAASPI(uint32_t val);
    uint32_t getSPI();
    void setSPI(uint32_t val);
    uint64_t getReplayDetection();
    void setReplayDetection(uint64_t val);
    void setKeyGenNonce(char *value, unsigned len);
    char* getKeyGenNonce();
    unsigned getKeyGenNonceLen();
    enum DigestTypes DigestType;
    SmartPtr<KeyList> AuthKeys;

  protected:
    int MsgType;

    long TransID;

    List(TOpt) Options;
    
    void setAttribs(int iface, SmartPtr<TIPv6Addr> addr, 
		    int msgType, long transID);
    virtual bool check(bool clntIDmandatory, bool srvIDmandatory);
    
    bool IsDone; // Is this transaction done?
    char * pkt;  // buffer where this packet will be build
    int Iface;   // interface from/to which message was received/should be sent
    SmartPtr<TIPv6Addr> PeerAddr; // server/client address from/to which message was received/should be sent

    // auth stuff below
    char * AuthInfoPtr; // pointer to Authentication Information field of OPTION AUTH and OPTION AAAAUTH
    char * AuthInfoKey; // pointer to key used do calculate Authentication information
    uint32_t SPI; // SPI sent by server in OPTION_KEYGEN
    uint64_t ReplayDetection;
    uint32_t AAASPI; // AAA-SPI sent by client in OPTION_AAAAUTH
    char *KeyGenNonce;
    unsigned KeyGenNonceLen;
};

#endif
