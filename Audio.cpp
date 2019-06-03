#include "Audio.h"
#include "Json.h"
Audio::Audio(QObject *parent) : QObject(parent)
{

}

void Audio::init()
{
    QVariantMap data;
    main=Link::create("InputAi");
    data["interface"]="main";
    main->start(data);

    omni=Link::create("InputAi");
    data["interface"]="omni";
    omni->start(data);

    output=Link::create("OutputAo");
    data["interface"]="output";
    output->start(data);



//    monitor=Link::create("OutputAo");
//    data["interface"]="monitor";
//    monitor->start(data);

    mix=Link::create("MixA");
    data.clear();
    data["main"]=main->name();
    mix->start(data);
    mixOut=Link::create("MixA");
    data.clear();
    data["main"]=main->name();
    mixOut->start(data);

    volMain=Link::create("Volume");
    volMain->start();
    volOmni=Link::create("Volume");
    volOmni->start();
    volMix=Link::create("Volume");
    volMix->start();
    volRemote=Link::create("Volume");
    volRemote->start();
    volOut=Link::create("Volume");
    volOut->start();

    vad=Link::create("Vad");
    data.clear();
    data["volume"]=volMain->name();
    vad->start(data);

    main->linkA(mix);
    main->linkA(mixOut);
    omni->linkA(vad)->linkA(mix);


    main->linkA(volMain);
    vad->linkA(volOmni);
    mix->linkA(volMix);


    net=Link::create("InputNet");
    QVariantMap dataNet;
    dataNet["path"]="rtsp://192.168.1.43/stream0";
    net->start(dataNet);

    decA=Link::create("DecodeA");
    decA->start();

    gain=Link::create("Gain");
    gain->start();

    res=Link::create("Resample");
    res->start();

    net->linkA(decA)->linkA(gain)->linkA(res)->linkA(output);
    res->linkA(volRemote);
    res->linkA(mixOut);
    res->linkA(mix);
    mixOut->linkA(volOut);


    rpcServer=new jcon::JsonRpcWebSocketServer();
    QObjectList list;
    list<<this;
    rpcServer->registerServices(list);
    rpcServer->listen(6002);

    config=Json::loadFile("/link/config/audio/audio.json").toMap();
    main->invoke("setDrvData",config);
}

void Audio::startStream()
{



    LinkObject *vi=Link::create("InputVi");
    QVariantMap dataVi;
    dataVi["interface"]="HDMI-A";
    vi->start(dataVi);

    LinkObject *encV=Link::create("EncodeV");
    encV->start();
    LinkObject *encA1=Link::create("EncodeA");
    encA1->start();

    LinkObject *encA2=Link::create("EncodeA");
    encA2->start();

    LinkObject *rtsp=Link::create("Rtsp");
    rtsp->start();

    LinkObject *mux1=Link::create("Mux");
    QVariantMap dataMux;
    dataMux["path"]="mem://test1";
    dataMux["format"]="rtsp";
    mux1->start(dataMux);
    LinkObject *mux2=Link::create("Mux");
    dataMux["path"]="mem://test2";
    mux2->start(dataMux);

    LinkObject *ai1=mix;
    LinkObject *ai2=omni;


    vi->linkV(encV)->linkV(mux1)->linkV(rtsp);
    encV->linkV(mux2)->linkV(rtsp);

    ai1->linkA(encA1)->linkA(mux1)->linkA(rtsp);
    ai2->linkA(encA2)->linkA(mux2)->linkA(rtsp);
}

QVariantMap Audio::getVolume()
{
    QVariantMap data;
    data["Main"]=volMain->invoke("getVolume").toMap()["max"].toInt();
    data["Omni"]=volOmni->invoke("getVolume").toMap()["max"].toInt();
    data["Output"]=volOut->invoke("getVolume").toMap()["max"].toInt();
    data["remote"]=volRemote->invoke("getVolume").toMap()["max"].toInt();
//    qDebug()<<vad->getData()["active"].toBool();
    if(vad->getData()["active"].toBool() || !vad->getData()["enable"].toBool())
        data["active"]="main";
    else
        data["active"]="omni";

    return data;
}

QVariantMap Audio::getAudio()
{
    return config;
}

bool Audio::setAudio(QVariantMap cfg)
{
    config=cfg;
    Json::saveFile(config,"/link/config/audio/audio.json");
    main->invoke("setDrvData",config);

    QVariantMap dataOut=cfg["output"].toMap();
    {
        QVariantMap vol;
        vol["mute"]=dataOut["mute0"].toBool();
        vol["gain"]=dataOut["gain0"].toInt();
        volOut->setData(vol);
    }

    QVariantMap dataMain=cfg["main"].toMap();
    {
        QVariantMap dataVad;
        dataVad["level"]=4*dataMain["vad"].toInt();
        dataVad["th"]=dataMain["vadth"].toInt();
        if(dataMain["vad"].toInt()==0)
            dataVad["enable"]=false;
        else
            dataVad["enable"]=true;

        vad->setData(dataVad);
    }

    QVariantMap rem=cfg["remote"].toMap();
    {
        QVariantMap dataGain;
        dataGain["mute"]=rem["mute"].toBool();
        dataGain["gain"]=rem["gain"].toInt();
        gain->setData(dataGain);
    }

    return true;
}

