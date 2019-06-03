#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include "Link.h"
#include <jcon/json_rpc_websocket_server.h>

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject *parent = 0);
    void init();
    void startStream();
    jcon::JsonRpcWebSocketServer *rpcServer;
private:
    LinkObject *main,*omni;
    LinkObject *output/*,*monitor*/;
    LinkObject *volMain,*volOut,*volOmni,*volMix,*volRemote;
    LinkObject *mix,*mixOut,*vad;
    LinkObject *net,*decA,*gain,*res;

    QVariantMap config;
signals:

public slots:
    QVariantMap getVolume();
    QVariantMap getAudio();
    bool setAudio(QVariantMap cfg);
};

#endif // AUDIO_H
