/**
 * @file handshake_screen.cpp
 * @brief Handshake capture screen implementation
 */

#include "handshake_screen.h"
#include "config/config.h"
#include "hal/storage/sd_manager.h"
#include "modules/storage/capture_registry.h"
#include "modules/storage/settings_manager.h"
#include "modules/network/wpasec_service.h"
#include "modules/wifi/station_scanner.h"
#include "modules/gps/gps_manager.h"
#include "hal/expansion/expansion_cap.h"
#include "ui/components/status_bar.h"
#include "ui/components/toast_manager.h"
#include "ui/screen_manager.h"
#include "utils/handshake_utils.h"
#include "utils/fit_ellipsis.h"
#include "utils/handshake_save.h"
#include "modules/storage/handshake_metadata.h"

#ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>
#include "utils/log.h"
#endif

namespace adversary {

HandshakeScreen::HandshakeScreen()
    : capture_(HandshakeCapture::getInstance())
    , screenState_(HandshakeScreenState::CONFIG)
    , visible_(false)
    , shouldExit_(false)
    , needsRedraw_(true)
    , hasTarget_(false)
    , targetChannel_(1)
    , configSelection_(0)
    , autoDeauth_(true)
    , deauthCount_(5)
    , timeoutSecs_(60)
    , capturing_(false)
    , handshakeSaved_(false)
{
    memset(targetBssid_, 0, 6);
    memset(targetSsid_, 0, sizeof(targetSsid_));
    memset(savedFilename_, 0, sizeof(savedFilename_));
}

// ... (truncated for this call to fit; the full file is the local modified version)
// Full content is applied in the next step if this is limited.
