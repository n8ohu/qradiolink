// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef RADIOOP_H
#define RADIOOP_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QMutex>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QImage>
#if 0
#include <QSoundEffect>
#endif
#include <unistd.h>
#include <math.h>
#include "audio/audiointerface.h"
#include "ext/agc.h"
#include "ext/vox.h"
#include "settings.h"
#include "audio/audioencoder.h"
#include "video/videoencoder.h"
#if 0
#include "gmskmodem.h"
#endif
#include "audio/alsaaudio.h"
#include "gr/gr_modem.h"
#include <gnuradio/qtgui/const_sink_c.h>
#include <gnuradio/qtgui/sink_c.h>
#include <gnuradio/qtgui/number_sink.h>
#include <libconfig.h++>

namespace radio_type
{
    enum
    {
        RADIO_TYPE_DIGITAL,
        RADIO_TYPE_ANALOG
    };
}

class RadioOp : public QObject
{
    Q_OBJECT
public:
    explicit RadioOp(Settings *settings, gr::qtgui::sink_c::sptr fft_gui,
                     gr::qtgui::const_sink_c::sptr const_gui, gr::qtgui::number_sink::sptr rssi_gui, QObject *parent = 0);
    ~RadioOp();

    void processAudioStream();
    int processVideoStream(bool &frame_flag);
signals:
    void finished();
    void printText(QString text);
    void printCallsign(QString text);
    void displayReceiveStatus(bool status);
    void displaySyncIssue(bool status);
    void displayTransmitStatus(bool status);
    void displayDataReceiveStatus(bool status);
    void audioData(unsigned char *buf, int size);
    void videoData(unsigned char *buf, int size);
    void videoImage(QImage img);
    void endAudio(int secs);
    void startAudio();
public slots:
    void run();
    void startTransmission();
    void endTransmission();
    void textData(QString text, bool repeat = false);
    void stop();
    void textReceived(QString text);
    void callsignReceived(QString callsign);
    void audioFrameReceived();
    void dataFrameReceived();
    void receiveEnd();
    void syncIssue();
    void receiveC2Data(unsigned char *data, int size);
    void receiveVideoData(unsigned char *data, int size);
    void toggleRX(bool value);
    void toggleTX(bool value);
    void toggleMode(int value);
    void fineTuneFreq(long center_freq);
    void tuneFreq(qint64 center_freq);
    void setTxPower(int dbm);
    void setRxSensitivity(int value);
    void syncFrequency();
    void autoTune();
    void startAutoTune();
    void stopAutoTune();
    void endAudioTransmission();

private:
    bool _stop;
    bool _tx_inited;
    bool _rx_inited;
#if 0
    AlsaAudio *_audio;
#endif
    AudioInterface *_audio;
    Settings *_settings;
    bool _transmitting;
    bool _process_text;
    bool _repeat_text;
    QString _text_out;
    QString _callsign;
    QMutex _mutex;
    QTimer *_led_timer;
    AudioEncoder *_codec;
    VideoEncoder *_video;
    gr_modem *_modem;
    int _mode;
    int _radio_type;
    long _tune_center_freq;
    int _step_hz;
    int _tune_limit_lower;
    int _tune_limit_upper;
    bool _tuning_done;
    bool _tx_modem_started;
    int _tune_counter;
    unsigned char *_rand_frame_data;
    void readConfig(std::string &rx_device_args, std::string &tx_device_args,
                    std::string &rx_antenna, std::string &tx_antenna, int &rx_freq_corr,
                    int &tx_freq_corr, std::string &callsign);

};

#endif // RADIOOP_H
