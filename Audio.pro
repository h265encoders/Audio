QT += core network testlib  websockets
QT -= gui

TARGET = Audio
CONFIG += console c++14
CONFIG -= app_bundle

TEMPLATE = app

chip=HI3531D
include(../../LinkLib/Link.pri)

SOURCES += main.cpp \
    Audio.cpp \
    jcon/json_rpc_client.cpp \
    jcon/json_rpc_debug_logger.cpp \
    jcon/json_rpc_endpoint.cpp \
    jcon/json_rpc_error.cpp \
    jcon/json_rpc_file_logger.cpp \
    jcon/json_rpc_logger.cpp \
    jcon/json_rpc_request.cpp \
    jcon/json_rpc_server.cpp \
    jcon/json_rpc_success.cpp \
    jcon/json_rpc_tcp_client.cpp \
    jcon/json_rpc_tcp_server.cpp \
    jcon/json_rpc_tcp_socket.cpp \
    jcon/json_rpc_websocket_client.cpp \
    jcon/json_rpc_websocket_server.cpp \
    jcon/json_rpc_websocket.cpp \
    jcon/string_util.cpp

HEADERS += \
    Audio.h \
    jcon/jcon_assert.h \
    jcon/jcon.h \
    jcon/json_rpc_client.h \
    jcon/json_rpc_debug_logger.h \
    jcon/json_rpc_endpoint.h \
    jcon/json_rpc_error.h \
    jcon/json_rpc_file_logger.h \
    jcon/json_rpc_logger.h \
    jcon/json_rpc_request.h \
    jcon/json_rpc_result.h \
    jcon/json_rpc_server.h \
    jcon/json_rpc_socket.h \
    jcon/json_rpc_success.h \
    jcon/json_rpc_tcp_client.h \
    jcon/json_rpc_tcp_server.h \
    jcon/json_rpc_tcp_socket.h \
    jcon/json_rpc_websocket_client.h \
    jcon/json_rpc_websocket_server.h \
    jcon/json_rpc_websocket.h \
    jcon/string_util.h

