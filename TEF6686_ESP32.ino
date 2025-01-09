#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <EEPROM.h>
#include <Wire.h>
#include <math.h>
#include <TimeLib.h>
#include <TFT_eSPI.h>               // https://github.com/ohmytime/TFT_eSPI_DynamicSpeed/archive/refs/heads/master.zip (please then edit the User_Setup.h as described in the Wiki)
#include <Hash.h>                   // https://github.com/bbx10/Hash_tng/archive/refs/heads/master.zip
#include <WebServer.h>
#include <SPIFFS.h>
#include "src/WiFiConnect.h"
#include "src/WiFiConnectParam.h"
#include "src/FONT16.h"
#include "src/FONT16_CHS.h"
#include "src/FONT28.h"
#include "src/FONT28_CHS.h"
#include "src/FONT48DEC.h"
#include "src/FREQFONT.h"
#include "src/TEF6686.h"
#include "src/constants.h"
#include "src/config.h"
#include "src/language.h"
#include "src/gui.h"
#include "src/comms.h"
#include "src/rds.h"
#include "src/touch.h"

#define ROTARY_PIN_A    34
#define ROTARY_PIN_B    36
#define ROTARY_BUTTON   39
#define PIN_POT         35
#define BATTERY_PIN     13
#define BANDBUTTON       4
#define BWBUTTON        25
#define MODEBUTTON      26
#define CONTRASTPIN     2
#define STANDBYLED      19
#define SMETERPIN       27
#define TOUCHIRQ        33
#define EXT_IRQ         14

#define DYNAMIC_SPI_SPEED   // uncomment to enable dynamic SPI Speed https://github.com/ohmytime/TFT_eSPI_DynamicSpeed

#ifdef ARS
TFT_eSPI tft = TFT_eSPI(320, 240);
#else
TFT_eSPI tft = TFT_eSPI(240, 320);
#endif

#ifdef DYNAMIC_SPI_SPEED
bool dynamicspi = true;
#else
bool dynamicspi = false;
#endif

bool advancedRDS;
bool afmethodBold;
bool afpage;
bool afscreen;
bool aftest;
bool artheadold;
bool autosquelch = true;
bool batterydetect = true;
bool beepresetstart;
bool beepresetstop;
bool BWreset;
bool bwtouchtune;
bool BWtune;
bool change;
bool compressedold;
bool direction;
bool dropout;
bool dynamicPTYold;
bool edgebeep;
bool externaltune;
bool findMemoryAF;
bool firstTouchHandled = false;
bool flashing;
bool fmsi;
bool fullsearchrds;
bool hasafold;
bool hasCTold;
bool haseonold;
bool hasrtplusold;
bool hastmcold;
bool initdxscan;
bool invertdisplay;
bool leave;
bool LowLevelInit;
bool memorystore;
bool memreset;
bool memtune;
bool menu;
bool menuopen;
bool mwstepsize;
#ifdef HAS_AIR_BAND
bool airstepsize;
#endif
bool nobattery;
bool optenc;
bool rdsflagreset;
bool rdsreset;
bool RDSSPYTCP;
bool RDSSPYUSB;
bool RDSstatus;
bool RDSstatusold;
bool rdsstereoold;
bool rotaryaccelerate = true;
bool rtcset;
bool scandxmode;
bool scanholdflag;
bool scanholdonsignal;
bool scanmem;
bool scanmute;
bool screenmute;
bool screensavertriggered = false;
bool seek;
bool seekinit;
bool setupmode;
bool showclock;
bool showlongps;
bool usesquelch;
bool softmuteam;
bool softmutefm;
bool SQ;
bool Stereostatusold;
bool StereoToggle;
bool store;
bool TAold;
bool TPold;
bool touchrepeat = false;
bool touch_detect;
bool tuned;
bool USBmode;
bool XDRGTKdata;
bool XDRGTKMuteScreen;
bool XDRGTKTCP;
bool XDRGTKUSB;
bool XDRMute;
bool XDRScan;
bool wifi;
bool wificonnected;
byte af_counterold;
byte aid_counterold;
byte af;
byte afpagenr;
byte amagc;
byte amnb;
byte amscansens;
byte audiomode;
byte band;
byte bandAM;
byte bandFM;
byte bandforbidden;
byte battery;
byte batteryold;
byte batteryoptions;
byte BWset;
byte BWsettemp;
byte BWsetAM;
byte BWsetFM;
byte BWsetRecall;
byte BWtemp;
byte charwidth = 8;
byte hardwaremodel;
byte ContrastSet;
byte CurrentSkin;
byte CurrentTheme;
byte displayflip;
byte ECCold;
byte eonptyold[20];
byte EQset;
byte fmagc;
byte fmscansens;
byte fmdefaultstepsize;
byte fmnb;
byte fmdeemphasis;
byte freqfont;
byte amcodect;
byte amcodectcount;
byte amgain;
byte freqoldcount;
byte HighCutLevel;
byte HighCutOffset;
byte items[10] = {10, static_cast<byte>(dynamicspi ? 10 : 9), 7, 10, 10, 10, 9, 6, 10, 9};
byte iMSEQ;
byte iMSset;
byte language;
byte licold;
byte longbandpress;
byte memdoublepi;
byte memorypos;
byte memoryposold;
byte memoryposstatus;
byte mempionly;
byte memstartpos;
byte memstoppos;
byte menuitem;
byte menupage;
byte MSold;
byte poweroptions;
byte rdsblockold;
byte rdsqualityold;
byte region;
byte rotarymode;
byte touchrotating;
byte scancancel;
byte scanstart;
byte scanstop;
byte scanhold;
byte scanmodeold;
byte screensaverOptions[5] = {0, 3, 10, 30, 60};
byte screensaverset;
byte showmodulation;
byte showrdserrors;
byte showSWMIBand;
byte submenu;
byte stationlistid;
byte nowToggleSWMIBand = 1;
byte stepsize;
byte StereoLevel;
byte subnetclient;
byte TEF;
byte tot;
byte tunemode;
byte unit;
byte spispeed;
char buff[16];
char eonpicodeold[20][6];
char programTypePrevious[18];
char rabbitearstime[100][21];
const uint8_t* currentFont = nullptr;
float vPerold;
int ActiveColor;
int ActiveColorSmooth;
int AGC;
int AMLevelOffset;
int BackgroundColor;
int BackgroundColor1;
int BackgroundColor2;
int BackgroundColor3;
int BackgroundColor4;
int BackgroundColor5;
int BarSignificantColor;
int BarInsignificantColor;
int BatteryValueColor;
int BatteryValueColorSmooth;
int batupdatetimer;
int BWAutoColor;
int BWAutoColorSmooth;
int BWOld;
int bwupdatetimer;
int DeEmphasis;
int DisplayedSegments;
int ForceMono;
int FrameColor;
int FreqColor;
int FreqColorSmooth;
int freq_in = 0;
int freqold;
int GreyoutColor;
int InsignificantColor;
int InsignificantColorSmooth;
int menuoption = ITEM1;
int ModBarInsignificantColor;
int ModBarSignificantColor;
int MStatusold;
int offsetupdatetimer;
int OStatusold;
int peakholdold;
int peakholdtimer;
int PrimaryColor;
int PrimaryColorSmooth;
int RDSColor;
int RDSColorSmooth;
int RDSDropoutColor;
int RDSDropoutColorSmooth;
int SignificantColor;
int SignificantColorSmooth;
int StereoColor;
int StereoColorSmooth;
int SquelchShow;
int rotary;
int rotarycounter;
int rotarycounteraccelerator;
int rssi;
int rssiold = 200;
int scanner_filter;
int SecondaryColor;
int SecondaryColorSmooth;
int SNRupdatetimer;
int Sqstatusold;
int Squelch;
int Squelchold;
int SStatusold;
int Stereostatus;
int volume;
int XDRBWset;
int XDRBWsetold;
int xPos;
int xPos2;
int xPos3;
int xPos4;
int xPos5;
int16_t OStatus;
int16_t SAvg;
int16_t SAvg2;
int16_t SAvg3;
int16_t SStatus;
int8_t LevelOffset;
int8_t LowLevelSet;
int8_t CN;
int8_t CNold;
int8_t VolSet;
float batteryVold;
IPAddress remoteip;
String AIDString;
String cryptedpassword;
String ECColdString;
String ECCString;
String eonpsold[20];
String LIColdString;
String LICString;
String pinstringold;
String PIold;
String PSold;
String ptynold = " ";
String PTYold;
String RabbitearsPassword;
String RabbitearsUser;
String rds_clock;
String rds_clockold;
String RDSSPYRDS;
String RDSSPYRDSold;
String RTold;
String salt;
String saltkey = "                ";
String stationIDold;
String stationStateold;
String StereoStatusCommand;
String StereoStatusCommandold;
String SWMIBandstring = String();
String SWMIBandstringold = String();
String XDRGTK_key;
String XDRGTKRDS;
String XDRGTKRDSold;
uint16_t BW;
uint16_t MStatus;
uint16_t rabbitearspi[100]; // first is for 88.1, 2nd 88.3, etc. to 107.9 MHz
uint16_t SWMIBandPos;
uint16_t SWMIBandPosold;
uint16_t TouchCalData[5];
uint16_t USN;
uint16_t WAM;
uint8_t buff_pos;
unsigned int ConverterSet;
unsigned int freq_scan;
unsigned int frequency;
unsigned int frequency_OIRT;
unsigned int frequency_AM;
unsigned int frequency_LW;
unsigned int frequency_MIBand_11M;
unsigned int frequency_MIBand_120M;
unsigned int frequency_MIBand_13M;
unsigned int frequency_MIBand_15M;
unsigned int frequency_MIBand_160M;
unsigned int frequency_MIBand_16M;
unsigned int frequency_MIBand_19M;
unsigned int frequency_MIBand_22M;
unsigned int frequency_MIBand_25M;
unsigned int frequency_MIBand_31M;
unsigned int frequency_MIBand_41M;
unsigned int frequency_MIBand_49M;
unsigned int frequency_MIBand_60M;
unsigned int frequency_MIBand_75M;
unsigned int frequency_MIBand_90M;
unsigned int frequency_MW;
unsigned int frequency_SW;
#ifdef HAS_AIR_BAND
unsigned int frequency_AIR;
unsigned int AIRHighEdgeSet;
unsigned int AIRLowEdgeSet;
#endif
unsigned int frequencyold;
unsigned int HighEdgeOIRTSet;
unsigned int HighEdgeSet;
unsigned int LowEdgeOIRTSet;
unsigned int LowEdgeSet;
unsigned int LWHighEdgeSet;
unsigned int LWLowEdgeSet;
unsigned int mappedfreqold[20];
unsigned int mappedfreqold2[20];
unsigned int mappedfreqold3[20];
unsigned int memstartfreq;
unsigned int memstopfreq;
unsigned int MWHighEdgeSet;
unsigned int MWLowEdgeSet;
unsigned int scanner_end;
unsigned int scanner_start;
unsigned int scanner_step;
unsigned int SWHighEdgeSet;
unsigned int SWLowEdgeSet;
unsigned long afticker;
unsigned long aftickerhold;
unsigned long aftimer;
unsigned long autosquelchtimer;
unsigned long eonticker;
unsigned long eontickerhold;
unsigned long flashingtimer;
unsigned long keypadtimer;
unsigned long lastTouchTime = 0;
unsigned long lowsignaltimer;
unsigned long ModulationpreviousMillis;
unsigned long ModulationpeakPreviousMillis;
unsigned long peakholdmillis;
unsigned long pslongticker;
unsigned long pslongtickerhold;
unsigned long rtplusticker;
unsigned long rtplustickerhold;
unsigned long rtticker;
unsigned long rttickerhold;
unsigned long rotarytimer;
unsigned long scantimer;
unsigned long signalstatustimer;
unsigned long tottimer;
unsigned long tuningtimer;
unsigned long udptimer;

mem presets[EE_PRESETS_CNT];
TEF6686 radio;
ESP32Time rtc(0);

TFT_eSprite FrequencySprite = TFT_eSprite(&tft);
TFT_eSprite RDSSprite = TFT_eSprite(&tft);
TFT_eSprite SquelchSprite = TFT_eSprite(&tft);
TFT_eSprite FullLineSprite = TFT_eSprite(&tft);
TFT_eSprite OneBigLineSprite = TFT_eSprite(&tft);
TFT_eSprite SignalSprite = TFT_eSprite(&tft);
TFT_eSprite PSSprite = TFT_eSprite(&tft);

WiFiConnect wc;
WiFiServer Server(7373);
WiFiClient RemoteClient;
WiFiUDP Udp;
WebServer webserver(80);

hw_timer_t *timScreensaver = NULL;
byte screensaver_IRQ = OFF;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  gpio_set_drive_capability((gpio_num_t) 5, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 16, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 17, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 18, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 19, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 21, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 22, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 23, GPIO_DRIVE_CAP_0);

  setupmode = true;
  EEPROM.begin(EE_TOTAL_CNT);
  if (EEPROM.readByte(EE_BYTE_CHECKBYTE) != EE_CHECKBYTE_VALUE) DefaultSettings();

  frequency = EEPROM.readUInt(EE_UINT16_FREQUENCY_FM);
  frequency_OIRT = EEPROM.readUInt(EE_UINT16_FREQUENCY_OIRT);
  VolSet = EEPROM.readByte(EE_BYTE_VOLSET);
  ConverterSet = EEPROM.readUInt(EE_UINT16_CONVERTERSET);
  LowEdgeSet = EEPROM.readUInt(EE_UINT16_FMLOWEDGESET);
  HighEdgeSet = EEPROM.readUInt(EE_UINT16_FMHIGHEDGESET);
  ContrastSet = EEPROM.readByte(EE_BYTE_CONTRASTSET);
  StereoLevel = EEPROM.readByte(EE_BYTE_STEREOLEVEL);
  bandFM = EEPROM.readByte(EE_BYTE_BANDFM);
  bandAM = EEPROM.readByte(EE_BYTE_BANDAM);
  HighCutLevel = EEPROM.readByte(EE_BYTE_HIGHCUTLEVEL);
  HighCutOffset = EEPROM.readByte(EE_BYTE_HIGHCUTOFFSET);
  LevelOffset = EEPROM.readByte(EE_BYTE_LEVELOFFSET);
  radio.rds.rtbuffer = EEPROM.readByte(EE_BYTE_RTBUFFER);
  edgebeep = EEPROM.readByte(EE_BYTE_EDGEBEEP);
  softmuteam = EEPROM.readByte(EE_BYTE_SOFTMUTEAM);
  softmutefm = EEPROM.readByte(EE_BYTE_SOFTMUTEFM);
  frequency_AM = EEPROM.readUInt(EE_UINT16_FREQUENCY_AM);
  language = EEPROM.readByte(EE_BYTE_LANGUAGE);
  showrdserrors = EEPROM.readByte(EE_BYTE_SHOWRDSERRORS);
  TEF = EEPROM.readByte(EE_BYTE_TEF);
  displayflip = EEPROM.readByte(EE_BYTE_DISPLAYFLIP);
  rotarymode = EEPROM.readByte(EE_BYTE_ROTARYMODE);
  tunemode = EEPROM.readByte(EE_BYTE_TUNEMODE);
  if (tunemode == TUNE_MAN) stepsize = EEPROM.readByte(EE_BYTE_STEPSIZE); else stepsize = 0;
  optenc = EEPROM.readByte(EE_BYTE_OPTENC);
  iMSset = EEPROM.readByte(EE_BYTE_IMSSET);
  EQset = EEPROM.readByte(EE_BYTE_EQSET);
  band = EEPROM.readByte(EE_BYTE_BAND);
  LowLevelSet = EEPROM.readByte(EE_BYTE_LOWLEVELSET);
  memorypos = EEPROM.readByte(EE_BYTE_MEMORYPOS);
  region = EEPROM.readByte(EE_BYTE_REGION);
  radio.underscore = EEPROM.readByte(EE_BYTE_RDS_UNDERSCORE);
  USBmode = EEPROM.readByte(EE_BYTE_USBMODE);
  wifi = EEPROM.readByte(EE_BYTE_WIFI);
  subnetclient = EEPROM.readByte(EE_BYTE_SUBNETCLIENT);
  showSWMIBand = EEPROM.readByte(EE_BYTE_SHOWSWMIBAND);
  radio.rds.filter = EEPROM.readByte(EE_BYTE_RDS_FILTER);
  radio.rds.pierrors = EEPROM.readByte(EE_BYTE_RDS_PIERRORS);
  frequency_LW = EEPROM.readUInt(EE_UINT16_FREQUENCY_LW);
  frequency_MW = EEPROM.readUInt(EE_UINT16_FREQUENCY_MW);
  frequency_SW = EEPROM.readUInt(EE_UINT16_FREQUENCY_SW);
#ifdef HAS_AIR_BAND
  frequency_AIR = EEPROM.readUInt(EE_UINT16_FREQUENCY_AIR);
#endif
  XDRGTK_key = EEPROM.readString(EE_STRING_XDRGTK_KEY);
  RabbitearsUser = EEPROM.readString(EE_STRING_RABBITEARSUSER);
  RabbitearsPassword = EEPROM.readString(EE_STRING_RABBITEARSPASSWORD);
  usesquelch = EEPROM.readByte(EE_BYTE_USESQUELCH);
  showmodulation = EEPROM.readByte(EE_BYTE_SHOWMODULATION);
  amnb = EEPROM.readByte(EE_BYTE_AM_NB);
  fmnb = EEPROM.readByte(EE_BYTE_FM_NB);
  audiomode = EEPROM.readByte(EE_BYTE_AUDIOMODE);
  touchrotating = EEPROM.readByte(EE_BYTE_TOUCH_ROTATING);
  hardwaremodel = EEPROM.readByte(EE_BYTE_HARDWARE_MODEL);
  poweroptions = EEPROM.readByte(EE_BYTE_POWEROPTIONS);
  CurrentTheme = EEPROM.readByte(EE_BYTE_CURRENTTHEME);
  fmdefaultstepsize = EEPROM.readByte(EE_BYTE_FMDEFAULTSTEPSIZE);
  screensaverset = EEPROM.readByte(EE_BYTE_SCREENSAVERSET);
  AMLevelOffset = EEPROM.readInt(EE_INT16_AMLEVELOFFSET);
  unit = EEPROM.readByte(EE_BYTE_UNIT);
  af = EEPROM.readByte(EE_BYTE_AF);
  if (af == 2) radio.rds.afreg = true; else radio.rds.afreg = false;
  StereoToggle = EEPROM.readByte(EE_BYTE_STEREO);
  batteryoptions = EEPROM.readByte(EE_BYTE_BATTERY_OPTIONS);
  amcodect = EEPROM.readByte(EE_BYTE_AM_CO_DECT);
  amcodectcount = EEPROM.readByte(EE_BYTE_AM_CO_DECT_COUNT);
  amgain = EEPROM.readByte(EE_BYTE_AM_RF_GAIN);
  radio.rds.sortaf = EEPROM.readByte(EE_BYTE_SORTAF);
  stationlistid = EEPROM.readByte(EE_BYTE_STATIONLISTID);
  fmdeemphasis = EEPROM.readByte(EE_BYTE_FM_DEEMPHASIS);
  BWsetFM = EEPROM.readByte(EE_BYTE_BWSET_FM);
  BWsetAM = EEPROM.readByte(EE_BYTE_BWSET_AM);
  nowToggleSWMIBand = EEPROM.readByte(EE_BYTE_BANDAUTOSW);
  radio.rds.fastps = EEPROM.readByte(EE_BYTE_FASTPS);
  tot = EEPROM.readByte(EE_BYTE_TOT);
  mwstepsize = EEPROM.readByte(EE_BYTE_MWREGION);
#ifdef HAS_AIR_BAND
  airstepsize = EEPROM.readByte(EE_BYTE_AIRSTEPSIZE);
#endif
  spispeed = EEPROM.readByte(EE_BYTE_SPISPEED);
  amscansens = EEPROM.readByte(EE_BYTE_AMSCANSENS);
  fmscansens = EEPROM.readByte(EE_BYTE_FMSCANSENS);
  freqfont = EEPROM.readByte(EE_BYTE_FREQFONT);
  CurrentSkin = EEPROM.readByte(EE_BYTE_SKIN);
  XDRGTKMuteScreen = EEPROM.readByte(EE_BYTE_XDRGTKMUTE);
  fmagc = EEPROM.readByte(EE_BYTE_FMAGC);
  amagc = EEPROM.readByte(EE_BYTE_AMAGC);
  fmsi = EEPROM.readByte(EE_BYTE_FMSI);
  scanstart = EEPROM.readByte(EE_BYTE_SCANSTART);
  scanstop = EEPROM.readByte(EE_BYTE_SCANSTOP);
  scanhold = EEPROM.readByte(EE_BYTE_SCANHOLD);
  scanmem = EEPROM.readByte(EE_BYTE_SCANMEM);
  scancancel = EEPROM.readByte(EE_BYTE_SCANCANCEL);
  scanmute = EEPROM.readByte(EE_BYTE_SCANMUTE);
  autosquelch = EEPROM.readByte(EE_BYTE_AUTOSQUELCH);
  longbandpress = EEPROM.readByte(EE_BYTE_LONGBANDPRESS);
  showclock = EEPROM.readByte(EE_BYTE_SHOWCLOCK);
  showlongps = EEPROM.readByte(EE_BYTE_SHOWLONGPS);
  memstartfreq = EEPROM.readUInt(EE_UINT16_MEMSTARTFREQ);
  memstopfreq = EEPROM.readUInt(EE_UINT16_MEMSTOPFREQ);
  memstartpos = EEPROM.readByte(EE_BYTE_MEMSTARTPOS);
  memstoppos = EEPROM.readByte(EE_BYTE_MEMSTOPPOS);
  mempionly = EEPROM.readByte(EE_BYTE_MEMPIONLY);
  memdoublepi = EEPROM.readByte(EE_BYTE_MEMDOUBLEPI);
  scanholdonsignal = EEPROM.readByte(EE_BYTE_WAITONLYONSIGNAL);
  TouchCalData[0] = EEPROM.readUInt(EE_UINT16_CALTOUCH1);
  TouchCalData[1] = EEPROM.readUInt(EE_UINT16_CALTOUCH2);
  TouchCalData[2] = EEPROM.readUInt(EE_UINT16_CALTOUCH3);
  TouchCalData[3] = EEPROM.readUInt(EE_UINT16_CALTOUCH4);
  TouchCalData[4] = EEPROM.readUInt(EE_UINT16_CALTOUCH5);
  invertdisplay = EEPROM.readByte(EE_BYTE_INVERTDISPLAY);

  if (spispeed == SPI_SPEED_DEFAULT) {
    tft.setSPISpeed(SPI_FREQUENCY / 1000000);
  } else if (spispeed == 7) {
    setAutoSpeedSPI();
  } else {
    tft.setSPISpeed(spispeed * 10);
  }

  LWLowEdgeSet = FREQ_LW_LOW_EDGE_MIN;
  LWHighEdgeSet = FREQ_LW_HIGH_EDGE_MAX;
  MWLowEdgeSet = mwstepsize == false ? FREQ_MW_LOW_EDGE_MIN_9K : FREQ_MW_LOW_EDGE_MIN_10K;
  MWHighEdgeSet = mwstepsize == false ? FREQ_MW_HIGH_EDGE_MAX_9K : FREQ_MW_HIGH_EDGE_MAX_10K;
  SWLowEdgeSet = FREQ_SW_LOW_EDGE_MIN;
  SWHighEdgeSet = FREQ_SW_HIGH_EDGE_MAX;
  LowEdgeOIRTSet = FREQ_FM_OIRT_START;
  HighEdgeOIRTSet = FREQ_FM_OIRT_END;

  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].band = EEPROM.readByte(i + EE_PRESETS_BAND_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].frequency = EEPROM.readUInt((i * 4) + EE_PRESETS_FREQUENCY_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].bw = EEPROM.readByte(i + EE_PRESET_BW_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].ms = EEPROM.readByte(i + EE_PRESET_MS_START);

  for (int i = 0; i < EE_PRESETS_CNT; i++) {
    for (int y = 0; y < 9; y++) {
      presets[i].RDSPS[y] = EEPROM.readByte((i * 9) + y + EE_PRESETS_RDSPS_START);
    }
    for (int y = 0; y < 5; y++) {
      presets[i].RDSPI[y] = EEPROM.readByte((i * 5) + y + EE_PRESETS_RDSPI_START);
    }
  }

  if (USBmode) Serial.begin(19200); else Serial.begin(115200);

  if (iMSset && EQset) iMSEQ = 2;
  if (!iMSset && EQset) iMSEQ = 3;
  if (iMSset && !EQset) iMSEQ = 4;
  if (!iMSset && !EQset) iMSEQ = 1;

  switch (band) {
    case BAND_LW:
      frequency_LW = frequency_AM;
      if (stepsize > 3) stepsize = 3;
      break;
    case BAND_MW:
      frequency_MW = frequency_AM;
      if (stepsize > 3) stepsize = 3;
      break;
    case BAND_SW: frequency_SW = frequency_AM; break;
    case BAND_FM:
      if (frequency % 10 != 0) {
        if (fmdefaultstepsize == 1) Round100K(frequency); else Round50K(frequency);
      }
      break;
    case BAND_OIRT:
      if (frequency % FREQ_OIRT_STEP_30K != 2) {
        Round30K(frequency_OIRT);
      }
      break;
  }

  tft.init();
  tft.initDMA();

  webserver.on("/", handleRoot);
  webserver.on("/downloadCSV", HTTP_GET, handleDownloadCSV);
  webserver.on("/logo.png", handleLogo);

  doTheme();

  if (displayflip == 0) {
#ifdef ARS
    tft.setRotation(0);
#else
    tft.setRotation(3);
#endif
  } else {
#ifdef ARS
    tft.setRotation(2);
#else
    tft.setRotation(1);
#endif
  }

  tft.invertDisplay(!invertdisplay);

  pinMode(BANDBUTTON, INPUT);
  pinMode(MODEBUTTON, INPUT);
  pinMode(BWBUTTON, INPUT);
  pinMode(ROTARY_BUTTON, INPUT);
  pinMode(ROTARY_PIN_A, INPUT);
  pinMode(ROTARY_PIN_B, INPUT);
  pinMode (STANDBYLED, OUTPUT);
  pinMode(TOUCHIRQ, INPUT);
  pinMode(EXT_IRQ, INPUT_PULLUP);
  digitalWrite(STANDBYLED, HIGH);

  attachInterrupt(digitalPinToInterrupt(TOUCHIRQ), Touch_IRQ_Handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), read_encoder, CHANGE);

  tft.setSwapBytes(true);
  tft.fillScreen(BackgroundColor);

  SPIFFS.begin();
  if (!SPIFFS.exists("/logbook.csv")) handleCreateNewLogbook();

  FrequencySprite.createSprite(200, 50);
  FrequencySprite.setTextDatum(TR_DATUM);
  FrequencySprite.setSwapBytes(true);

  RDSSprite.createSprite(165, 19);
  RDSSprite.setTextDatum(TL_DATUM);

  PSSprite.createSprite(150, 32);
  PSSprite.setTextDatum(TL_DATUM);
  PSSprite.setSwapBytes(true);

  SquelchSprite.createSprite(47, 19);
  SquelchSprite.setTextDatum(TL_DATUM);
  SquelchSprite.setSwapBytes(true);

  FullLineSprite.createSprite(308, 19);
  FullLineSprite.setSwapBytes(true);

  OneBigLineSprite.createSprite(270, 30);
  OneBigLineSprite.setSwapBytes(true);

  SignalSprite.createSprite(80, 48);
  SignalSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
  SignalSprite.setTextDatum(TR_DATUM);
  SignalSprite.setSwapBytes(true);

  UpdateFonts(0);

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    if (rotarymode == 0) rotarymode = 1; else rotarymode = 0;
    EEPROM.writeByte(EE_BYTE_ROTARYMODE, rotarymode);
    EEPROM.commit();
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    tftPrint(0, myLanguage[language][1], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][2], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(BWBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == LOW && digitalRead(BANDBUTTON) == HIGH) {
    if (displayflip == 0) {
      displayflip = 1;
#ifdef ARS
      tft.setRotation(2);
#else
      tft.setRotation(1);
#endif
    } else {
      displayflip = 0;
#ifdef ARS
      tft.setRotation(0);
#else
      tft.setRotation(3);
#endif
    }
    EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, displayflip);
    EEPROM.commit();
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    tftPrint(0, myLanguage[language][3], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][2], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(MODEBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(SMETERPIN, 511);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    tftPrint(0, myLanguage[language][4], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][5], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(BANDBUTTON) == LOW) delay(50);
    analogWrite(SMETERPIN, 0);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == LOW && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    if (optenc == 0) {
      optenc = 1;
      tftPrint(0, myLanguage[language][6], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    } else {
      optenc = 0;
      tftPrint(0, myLanguage[language][7], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    }
    EEPROM.writeByte(EE_BYTE_OPTENC, optenc);
    EEPROM.commit();
    tftPrint(0, myLanguage[language][2], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == LOW && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    DefaultSettings();
    tftPrint(0, myLanguage[language][66], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][2], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(ROTARY_BUTTON) == LOW && digitalRead(BWBUTTON) == LOW) delay(50);
    ESP.restart();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == LOW && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    tftPrint(0, myLanguage[language][282], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][283], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    tft.calibrateTouch(TouchCalData, PrimaryColor, BackgroundColor, 30);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH1, TouchCalData[0]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH2, TouchCalData[1]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH3, TouchCalData[2]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH4, TouchCalData[3]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH5, TouchCalData[4]);
    EEPROM.commit();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    tftPrint(0, myLanguage[language][284], 155, 70, ActiveColor, ActiveColorSmooth, 28);
    tftPrint(0, myLanguage[language][2], 155, 130, ActiveColor, ActiveColorSmooth, 28);
    invertdisplay = !invertdisplay;
    tft.invertDisplay(!invertdisplay);
    while (digitalRead(BWBUTTON) == LOW && digitalRead(BANDBUTTON) == LOW) delay(50);
    EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, invertdisplay);
    EEPROM.commit();
  }

  tft.setTouch(TouchCalData);

  tft.fillScreen(BackgroundColor);
  tftPrint(0, myLanguage[language][8], 160, 3, PrimaryColor, PrimaryColorSmooth, 28);
  tftPrint(0, "Software " + String(VERSION), 160, 152, PrimaryColor, PrimaryColorSmooth, 16);

  tft.fillRect(120, 230, 16, 6, GreyoutColor);
  tft.fillRect(152, 230, 16, 6, GreyoutColor);
  tft.fillRect(184, 230, 16, 6, GreyoutColor);

  tft.pushImage (78, 34, 163, 84, openradiologo);
  tft.drawBitmap(130, 124, TEFLogo, 59, 23, ActiveColor);

  for (int x = 0; x <= ContrastSet; x++) {
    analogWrite(CONTRASTPIN, map(x, 0, 100, 15, 255));
    delay(30);
  }

  tft.fillRect(120, 230, 16, 6, PrimaryColor);

  TEF = EEPROM.readByte(EE_BYTE_TEF);

  if (TEF != 102 && TEF != 205) SetTunerPatch();

  radio.init(TEF);
  uint16_t device;
  uint16_t hw;
  uint16_t sw;

  radio.getIdentification(device, hw, sw);
  if (TEF != (highByte(hw) * 100 + highByte(sw))) SetTunerPatch();

  if (lowByte(device) == 14) {
    fullsearchrds = false;
    fmsi = false;
    tft.fillRect(152, 230, 16, 6, PrimaryColor);
    tftPrint(0, "TEF6686 Lithio", 160, 172, ActiveColor, ActiveColorSmooth, 28);
  } else if (lowByte(device) == 1) {
    fullsearchrds = true;
    tft.fillRect(152, 230, 16, 6, PrimaryColor);
    tftPrint(0, "TEF6687 Lithio FMSI", 160, 172, ActiveColor, ActiveColorSmooth, 28);
  } else if (lowByte(device) == 9) {
    fullsearchrds = false;
    fmsi = false;
    tft.fillRect(152, 230, 16, 6, PrimaryColor);
    tftPrint(0, "TEF6688 Lithio DR", 160, 172, ActiveColor, ActiveColorSmooth, 28);
  } else if (lowByte(device) == 3) {
    fullsearchrds = true;
    tft.fillRect(152, 230, 16, 6, PrimaryColor);
    tftPrint(0, "TEF6689 Lithio FMSI DR", 160, 172, ActiveColor, ActiveColorSmooth, 28);
  } else {
    tftPrint(0, myLanguage[language][9], 160, 172, SignificantColor, SignificantColorSmooth, 28);
    tft.fillRect(152, 230, 16, 6, SignificantColor);
    while (true);
    for (;;);
  }
  tftPrint(0, "Patch: v" + String(TEF), 160, 202, ActiveColor, ActiveColorSmooth, 28);

  Wire.beginTransmission(0x20);
  Wire.write(0x06);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.endTransmission();

  if (analogRead(BATTERY_PIN) < 200) batterydetect = false;

  if (wifi) {
    tryWiFi();
    tft.fillRect(184, 230, 16, 6, PrimaryColor);
    delay(2000);
  } else {
    Server.end();
    Udp.stop();
    tft.fillRect(184, 230, 16, 6, SignificantColor);
  }
  delay(1500);

  radio.setVolume(VolSet);
  radio.setOffset(LevelOffset);
  radio.setAMOffset(AMLevelOffset);
  if (band > BAND_GAP && band != BAND_AIR) {
    radio.setAMCoChannel(amcodect, amcodectcount);
    radio.setAMAttenuation(amgain);
  }
  radio.setStereoLevel(StereoLevel);
  radio.setHighCutLevel(HighCutLevel);
  radio.setHighCutOffset(HighCutOffset);
  radio.clearRDS(fullsearchrds);
  radio.setMute();
  if (!StereoToggle) radio.setMono(true);
  radio.setSoftmuteFM(softmutefm);
  radio.setSoftmuteAM(softmuteam);
  radio.setAMNoiseBlanker(amnb);
  radio.setFMNoiseBlanker(fmnb);
  radio.setAudio(audiomode);
  radio.setDeemphasis(fmdeemphasis);
  radio.rds.region = region;
  radio.setAGC(fmagc);
  radio.setAMAGC(amagc);
  if (fmsi) radio.setFMSI(2); else radio.setFMSI(1);
  LowLevelInit = true;

  if (ConverterSet >= 200) {
    Wire.beginTransmission(0x12);
    Wire.write(ConverterSet >> 8);
    Wire.write(ConverterSet & (0xFF));
    Wire.endTransmission();
  }

  BuildDisplay();
  SelectBand();
  if (tunemode == TUNE_MEM) DoMemoryPosTune();

  setupmode = false;

  if (edgebeep) radio.tone(50, -5, 2000);
  if (!usesquelch) radio.setUnMute();

  if (screensaverset) {
    ScreensaverTimerInit();
    ScreensaverTimerSet(screensaverOptions[screensaverset]);
  }

  tottimer = millis();
}

void loop() {
  if (wifi) webserver.handleClient();

  if (hardwaremodel == PORTABLE_TOUCH_ILI9341 && touch_detect) {
    if (tft.getTouchRawZ() > 100) {  // Check if the touch is active
      uint16_t x, y;
      tft.getTouch(&x, &y);
      if (x > 0 || y > 0) {
        if (!firstTouchHandled) {
          // Handle the initial touch event immediately
          doTouchEvent(x, y);
          firstTouchHandled = true;  // Mark the first touch as handled
          lastTouchTime = millis(); // Start tracking the time for the delay
        } else if (touchrepeat) {
          // Check if the initial 0.5-second delay has passed
          if (millis() - lastTouchTime >= 500) {
            // Repeat the touch action continuously without delay
            doTouchEvent(x, y);
          }
        }
      }
    } else {
      // Touch has been released
      firstTouchHandled = false;  // Reset the first touch flag
      touch_detect = false;       // Reset the touch detection flag
    }
  }

  Communication();

  if (tot != 0) {
    unsigned long totprobe = tot * 60000;
    if (millis() >= tottimer + totprobe) deepSleep();
  }

  if (freq_in != 0 && millis() >= keypadtimer + 2000) {
    freq_in = 0;
    ShowFreq(0);
  }

  if (scandxmode) {
    unsigned long waitTime = (scanhold == 0) ? 500 : (scanhold * 1000);
    if (!scanholdflag) scanholdflag = (USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80) && (OStatus > -80);
    bool bypassMillisCheck = scanholdonsignal && !scanholdflag;
    bool shouldScan = bypassMillisCheck || (!bypassMillisCheck && (millis() >= scantimer + waitTime));

    if (shouldScan) {
      if (scanmute && scanholdonsignal) {
        radio.setMute();
        if (!screenmute) tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
        SQ = true;
      }
      scanholdflag = false;
      if (scanmem) {
        memorypos++;
        if (memorypos > scanstop) memorypos = scanstart;
        while (IsStationEmpty() || (presets[memorypos].band != BAND_FM && presets[memorypos].band != BAND_OIRT)) {
          memorypos++;
          if (memorypos > scanstop) {
            memorypos = scanstart;
            break;
          }
        }
        DoMemoryPosTune();
        ShowMemoryPos();
      } else {
        TuneUp();
        ShowFreq(0);
        if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
      }
      scantimer = millis();
      initdxscan = false;
    } else {
      if (scanmute && scanholdonsignal) {
        radio.setUnMute();
        SQ = false;
        if (!screenmute) tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
      }
    }

    if (millis() >= flashingtimer + 500) {
      flashing = !flashing;
      if (flashing) {
        tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
        tftPrint(0, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
      } else {
        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
      }
      flashingtimer = millis();
    }

    if (!scanholdflag) delay(100);
    radio.getStatus(SStatus, USN, WAM, OStatus, BW, MStatus, CN);

    if (RabbitearsUser.length() && RabbitearsPassword.length() && region == REGION_US && radio.rds.correctPI != 0 && frequency >= 8810 && frequency <= 10790 && !(frequency % 10) && ((frequency / 10) % 2)) {
      byte i = (frequency / 10 - 881) / 2;
      if (!rabbitearspi[i]) {
        rabbitearspi[i] = radio.rds.correctPI;

        const time_t epoch = rtc.getEpoch() + rtc.offset;
        strftime(rabbitearstime[i], 21, "%FT%TZ", localtime(&epoch));
      }
    }

    if (!initdxscan) {
      switch (scancancel) {
        case CORRECTPI:
          if (RDSstatus && radio.rds.correctPI != 0) cancelDXScan();
          break;
        case SIGNAL:
          if (scanhold) cancelDXScan();
          break;
      }
    }
  }

  if (millis() >= tuningtimer + 200) rdsflagreset = false;

  if (millis() >= tuningtimer + 2000) {
    if (store) {
      StoreFrequency();
      store = false;
    }
  }

  if (!BWtune && !menu && !afscreen && !scandxmode) {
    if (af != 0 && dropout && millis() >= aftimer + 1000) {
      aftimer = millis();
      if (radio.af_counter == 0) {
        if (findMemoryAF && radio.rds.correctPI != 0 && tunemode == TUNE_MEM && (USN > 250 || WAM > 250)) {
          radio.setMute();
          tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
          SQ = true;
          if (!screenmute) {
            if (advancedRDS) {
              tft.drawRoundRect(10, 30, 300, 170, 2, ActiveColor);
              tft.fillRoundRect(12, 32, 296, 166, 2, BackgroundColor);
              tftPrint(0, myLanguage[language][34], 160, 100, ActiveColor, ActiveColorSmooth, 28);
            } else {
              ShowFreq(1);
            }
          }

          bool foundmemaf = false;
          for (int x = 8750; x <= 10800; x += 10) {
            if (rotary != 0 || digitalRead(BANDBUTTON) == LOW || digitalRead(MODEBUTTON) == LOW || digitalRead(BWBUTTON) == LOW || digitalRead(ROTARY_BUTTON) == LOW) break;
            radio.SetFreq(x);
            unsigned long millisold = millis();
            while (millis() - millisold < 187) {
              if (!screenmute && !advancedRDS) ShowModLevel();
            }
            if (radio.rds.correctPI == radio.getBlockA()) {
              frequency = x;
              foundmemaf = true;
              break;
            }
          }

          if (!foundmemaf) {
            frequency = freqold;
            radio.SetFreq(frequency);
          }

          if (!screenmute) {
            if (advancedRDS) {
              leave = true;
              BuildAdvancedRDS();
              freq_in = 0;
            } else {
              ShowFreq(0);
            }
          }

          radio.setUnMute();
          SQ = false;
          tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
        }
        findMemoryAF = false;
      } else {
        frequency = radio.TestAF();
      }
      if (freqold != frequency) {
        ShowFreq(0);
        dropout = true;
        if (radio.afmethodB) {
          afmethodBold = true;
          radio.clearRDS(fullsearchrds);
        }
      }
      if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
      if (screenmute) {
        freqold = frequency;
        dropout = false;
      }
      store = true;
    }

    if (band == BAND_FM && af != 0 && radio.rds.correctPI != 0) {
      if ((aftest && millis() >= aftimer + 3000) || ((USN > 250 || WAM > 250) && millis() >= aftimer + 1000)) {
        aftimer = millis();
        aftest = false;
        frequency = radio.TestAF();
        if (freqold != frequency) {
          ShowFreq(0);
          dropout = true;
          if (radio.afmethodB) {
            afmethodBold = true;
            radio.clearRDS(fullsearchrds);
          }
          if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
          if (screenmute) {
            freqold = frequency;
            dropout = false;
          }
          store = true;
        }
      }

      if (band == BAND_FM && millis() >= aftimer + 10000) {
        aftimer = millis();
        if (USN > 150 || WAM > 150) {
          frequency = radio.TestAF();
          if (freqold != frequency) {
            ShowFreq(0);
            dropout = true;
            if (radio.afmethodB) {
              afmethodBold = true;
              radio.clearRDS(fullsearchrds);
            }
            if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
            store = true;
            if (screenmute) {
              freqold = frequency;
              dropout = false;
            }
          }
        }
      }
    }
  }

  if (seek) Seek(direction);

  if ((SStatus / 10 > LowLevelSet) && !LowLevelInit && !BWtune && !menu && band < BAND_GAP) {
    if (!screenmute && !advancedRDS && !afscreen) {
      if (showmodulation) {
        tftPrint(-1, "10", 27, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(-1, "30", 57, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(-1, "50", 87, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(-1, "70", 117, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(-1, "100", 164, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(0, "M", 7, 128, ActiveColor, ActiveColorSmooth, 16);
        for (byte segments = 0; segments < 94; segments++) {
          if (segments > 54) {
            if (((segments - 53) % 10) == 0) tft.fillRect(16 + (2 * segments), 141, 2, 2, BarSignificantColor);
          } else {
            if (((segments + 1) % 6) == 0) tft.fillRect(16 + (2 * segments), 141, 2, 2, ModBarInsignificantColor);
          }
        }
      }
      if (region == REGION_EU) tftPrint(-1, "PI:", 212, 193, ActiveColor, ActiveColorSmooth, 16);
      if (region == REGION_US) {
        tftPrint(-1, "PI:", 212, 184, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(-1, "ID:", 212, 201, ActiveColor, ActiveColorSmooth, 16);
      }
      tftPrint(-1, "PS:", 3, 193, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(-1, "RT:", 3, 221, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(-1, "PTY:", 3, 163, ActiveColor, ActiveColorSmooth, 16);
      if (!showmodulation) tft.drawLine(16, 143, 203, 143, GreyoutColor); else tft.drawLine(16, 143, 203, 143, ActiveColor);
    }
    LowLevelInit = true;
  }

  if ((SStatus / 10 <= LowLevelSet) && band < BAND_GAP) {
    if (LowLevelInit && !BWtune && !menu) {
      if (!screenmute && !afscreen && !advancedRDS) {
        for (byte segments = 0; segments < 94; segments++) {
          if (segments > 54) {
            if (((segments - 53) % 10) == 0) tft.fillRect(16 + (2 * segments), 141, 2, 2, GreyoutColor);
          } else {
            if (((segments + 1) % 6) == 0) tft.fillRect(16 + (2 * segments), 141, 2, 2, GreyoutColor);
          }
        }
        if (showmodulation) {
          tftPrint(-1, "10", 27, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(-1, "30", 57, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(-1, "50", 87, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(-1, "70", 117, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(-1, "100", 164, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(0, "M", 7, 128, GreyoutColor, BackgroundColor, 16);
          tft.fillRect(16, 133, 188, 6, GreyoutColor);
        }
        if (region == REGION_EU) tftPrint(-1, "PI:", 212, 193, GreyoutColor, BackgroundColor, 16);
        if (region == REGION_US) {
          tftPrint(-1, "PI:", 212, 184, GreyoutColor, BackgroundColor, 16);
          tftPrint(-1, "ID:", 212, 201, GreyoutColor, BackgroundColor, 16);
        }
        tftPrint(-1, "PS:", 3, 193, GreyoutColor, BackgroundColor, 16);
        tftPrint(-1, "RT:", 3, 221, GreyoutColor, BackgroundColor, 16);
        tftPrint(-1, "PTY:", 3, 163, GreyoutColor, BackgroundColor, 16);
        tft.drawLine(16, 143, 203, 143, GreyoutColor);
        tft.drawBitmap(122, 5, RDSLogo, 35, 22, GreyoutColor);
      }
      LowLevelInit = false;
    }

    if (!BWtune && !menu && (screenmute || radio.rds.correctPI != 0)) readRds();
    if (millis() >= lowsignaltimer + 300) {
      lowsignaltimer = millis();
      if (af || (!screenmute || (screenmute && (XDRGTKTCP || XDRGTKUSB)))) {
        if (band < BAND_GAP) {
          radio.getStatus(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
        } else {
          radio.getStatusAM(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
        }
      }
      if (!BWtune && !menu) {
        doSquelch();
        GetData();
      }
    }

  } else {
    if (af || (!screenmute || (screenmute && (XDRGTKTCP || XDRGTKUSB)))) {
      if (band < BAND_GAP) {
        radio.getStatus(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
      } else {
        radio.getStatusAM(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
      }
    }
    if (!BWtune && !menu) {
      doSquelch();
      if (millis() >= tuningtimer + 200) readRds();
      GetData();
      if (!screenmute && !afscreen && !advancedRDS) ShowModLevel();
    }
  }

  if (rotary == -1) {
    tottimer = millis();
    if (screensavertriggered) {
      if (!touchrotating) {
        rotary = 0;
        WakeToSleep(REVERSE);
      } else {
        if (BWtune) doBWtuneUp(); else KeyUp();
      }
    } else {
      if (BWtune) doBWtuneUp(); else KeyUp();
      if (rotaryaccelerate && rotarycounter > 2 && !BWtune && !menu) {
        for (int i = 0; i < rotarycounteraccelerator; i++) KeyUp();
        rotarycounter = 0;
      }
      if (screensaverset && !BWtune && !menu && !screensavertriggered) ScreensaverTimerRestart();
    }
  }

  if (rotary == 1) {
    tottimer = millis();
    if (screensavertriggered) {
      if (!touchrotating) {
        rotary = 0;
        WakeToSleep(REVERSE);
      } else {
        if (BWtune) doBWtuneDown(); else KeyDown();
      }
    } else {
      if (BWtune) doBWtuneDown(); else KeyDown();
      if (rotaryaccelerate && rotarycounter > 2 && !BWtune && !menu) {
        for (int i = 0; i < rotarycounteraccelerator; i++) KeyDown();
        rotarycounter = 0;
      }
      if (screensaverset && !BWtune && !menu && !screensavertriggered) ScreensaverTimerRestart();
    }
  }

  if (digitalRead(BANDBUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(BANDBUTTON) == LOW);
    } else {
      BANDBUTTONPress();
    }
  }

  if (digitalRead(ROTARY_BUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(ROTARY_BUTTON) == LOW);
    } else {
      if (!afscreen) ButtonPress();
    }
  }

  if (digitalRead(MODEBUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(MODEBUTTON) == LOW);
    } else {
      if (!screenmute) ModeButtonPress();
    }
  }

  if (digitalRead(BWBUTTON) == LOW && !BWtune) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(BWBUTTON) == LOW);
    } else {
      if (!screenmute && !afscreen) BWButtonPress();
    }
  }

  if (digitalRead(EXT_IRQ) == LOW) {
    int num;
    num = GetNum();
    if (num != -1)
    {
      if (!screenmute && !BWtune && !menu && !advancedRDS && !afscreen)
      {
        NumpadProcess(num);
      }
    }
  }

  if (screensaverset) {
    if (screensaver_IRQ)
    {
      screensaver_IRQ = OFF;
      if (!screensavertriggered && !BWtune && !menu) {
        WakeToSleep(true);
      }
    }
  }
}

void GetData() {
  if (!afscreen) {
    showCT();
    ShowSignalLevel();
  }

  if (!BWtune && !menu) showPS();

  if (band < BAND_GAP && !BWtune && !menu) {
    if (advancedRDS && !afscreen && !screenmute) ShowAdvancedRDS();
    if (afscreen && !screenmute) ShowAFEON();
    if (!afscreen) {
      if (!screenmute) ShowErrors();
      showPTY();
      showECC();
      showRadioText();
      if (millis() >= tuningtimer + 200) doAF();
    }
    showPI();
  }

  ShowStereoStatus();

  if (!screenmute) {
    ShowRSSI();
    ShowOffset();
    ShowBW();
    updateCodetect();
    if (millis() >= tuningtimer + 200) ShowBattery();
  }
}

void WakeToSleep(bool yes) {
  if (yes) {
    screensavertriggered = true;
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(1);
        StoreFrequency();
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
        analogWrite(CONTRASTPIN, map(ContrastSet / 100, 0, 100, 15, 255));
        break;
      case LCD_BRIGHTNESS_A_QUARTER:
        analogWrite(CONTRASTPIN, map(ContrastSet / 4, 0, 100, 15, 255));
        break;
      case LCD_BRIGHTNESS_HALF:
        analogWrite(CONTRASTPIN, map(ContrastSet / 2, 0, 100, 15, 255));
        break;
    }
  } else {
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(0);
        screensavertriggered = false;
        screensaver_IRQ = OFF;
        ScreensaverTimerReopen();
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
      case LCD_BRIGHTNESS_A_QUARTER:
      case LCD_BRIGHTNESS_HALF:
        MuteScreen(0);
        screensavertriggered = false;
        screensaver_IRQ = OFF;
        ScreensaverTimerReopen();
        break;
    }
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
  }
}

void ScreensaverTimerInit() {
  timScreensaver = timerBegin(0, 80, true);
  timerAttachInterrupt(timScreensaver, ScreensaverInterrupt, true);
}

void ScreensaverTimerSet(byte value) {
  if (timScreensaver == NULL) {
    ScreensaverTimerInit();
    ScreensaverTimerSet(screensaverOptions[screensaverset]);
  }

  if (value == OFF) {
    if (screensaverset) timerStop(timScreensaver);
  } else {
    timerStop(timScreensaver);
    timerAlarmWrite(timScreensaver, value * TIMER_SCREENSAVER_BASE, true);
    timerStart(timScreensaver);
    timerAlarmEnable(timScreensaver);
  }
}

void ScreensaverTimerRestart() {
  if (timScreensaver == NULL) {
    ScreensaverTimerInit();
    ScreensaverTimerSet(screensaverOptions[screensaverset]);
  }
  timerRestart(timScreensaver);
}

void ScreensaverTimerReopen() {
  ScreensaverTimerSet(OFF);
  ScreensaverTimerSet(screensaverOptions[screensaverset]);
  ScreensaverTimerRestart();
}

void ScreensaverInterrupt()
{
  screensaver_IRQ = ON;
}

void CheckBandForbiddenFM () {
  switch (band) {
    case BAND_FM:
      if (bandFM == FM_BAND_OIRT) bandforbidden = 1; else bandforbidden = 0;
      break;
    case BAND_OIRT:
      if (bandFM == FM_BAND_FM) bandforbidden = 1; else bandforbidden = 0;
      break;
  }
}

void doBandSelectionFM() {
  if (band > BAND_GAP) return;

  switch (bandFM) {
    case FM_BAND_ALL:
      break;
    case FM_BAND_OIRT:
      if (band == BAND_FM) {
        band = BAND_OIRT;
        if (frequency_OIRT > FREQ_FM_OIRT_END) {
          frequency_OIRT = FREQ_FM_OIRT_START;
          if (edgebeep) EdgeBeeper();
        } else if (frequency_OIRT < FREQ_FM_OIRT_START) {
          frequency_OIRT = FREQ_FM_OIRT_END;
          if (edgebeep) EdgeBeeper();
        }
        SelectBand();
      }
      break;
    case FM_BAND_FM:
      if (band == BAND_OIRT) {
        band = BAND_FM;
        if (frequency > HighEdgeSet * 10) {
          frequency = LowEdgeSet * 10;
          if (edgebeep) EdgeBeeper();
        } else if (frequency < LowEdgeSet * 10) {
          frequency = HighEdgeSet * 10;
          if (edgebeep) EdgeBeeper();
        }
        SelectBand();
      }
      break;
    case FM_BAND_NONE:
      ToggleBand(band);
      SelectBand();
      break;
  }
}

void CheckBandForbiddenAM() {
#ifdef HAS_AIR_BAND
  switch (band) {
    case BAND_LW:
      if (bandAM == AM_BAND_MW_SW_AIR || bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW_AIR || bandAM == AM_BAND_SW_AIR || bandAM == AM_BAND_MW || bandAM == AM_BAND_SW || bandAM == AM_BAND_AIR)
        bandforbidden = 1;
      else
        bandforbidden = 0;
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_LW_SW_AIR || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW_AIR || bandAM == AM_BAND_SW_AIR || bandAM == AM_BAND_LW || bandAM == AM_BAND_SW || bandAM == AM_BAND_AIR)
        bandforbidden = 1;
      else
        bandforbidden = 0;
      break;
    case BAND_SW:
      if (bandAM == AM_BAND_LW_MW_AIR || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW_AIR || bandAM == AM_BAND_MW_AIR || bandAM == AM_BAND_LW || bandAM == AM_BAND_MW || bandAM == AM_BAND_AIR)
        bandforbidden = 1;
      else
        bandforbidden = 0;
      break;
    case BAND_AIR:
      if (bandAM == AM_BAND_LW_MW_SW || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_LW || bandAM == AM_BAND_MW || bandAM == AM_BAND_SW)
        bandforbidden = 1;
      else
        bandforbidden = 0;
      break;
  }
#else
  switch (band) {
    case BAND_LW:
      if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW || bandAM == AM_BAND_SW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW || bandAM == AM_BAND_SW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
    case BAND_SW:
      if (bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW || bandAM == AM_BAND_MW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
  }
#endif
}

void doBandSelectionAM() {
  if (band < BAND_GAP) return;

#ifdef HAS_AIR_BAND  // has air band
  switch (bandAM) {
    case AM_BAND_ALL:
      break;
    case AM_BAND_LW_MW_SW:
      if (band == BAND_AIR) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_MW_AIR:
      if (band == BAND_SW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_SW_AIR:
      if (band == BAND_MW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW_SW_AIR:
      if (band == BAND_LW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_MW:
      if (band == BAND_SW || band == BAND_AIR) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_SW:
      if (band == BAND_MW || band == BAND_AIR) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_AIR:
      if (band == BAND_MW || band == BAND_SW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW_SW:
      if (band == BAND_LW || band == BAND_AIR) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_MW_AIR:
      if (band == BAND_LW || band == BAND_SW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_SW_AIR:
      if (band == BAND_LW || band == BAND_MW) {
        band = BAND_SW;
        SelectBand();
      }
      break;
    case AM_BAND_LW:
      if (band != BAND_LW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW:
      if (band != BAND_MW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_SW:
      if (band != BAND_SW) {
        band = BAND_SW;
        SelectBand();
      }
      break;
    case AM_BAND_AIR:
      if (band != BAND_AIR) {
        band = BAND_AIR;
        SelectBand();
      }
      break;
  }
#else  // has no air band
  switch (bandAM) {
    case AM_BAND_ALL:
      break;
    case AM_BAND_LW_MW:
      if (band == BAND_SW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_SW:
      if (band == BAND_MW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW_SW:
      if (band == BAND_LW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_LW:
      if (band != BAND_LW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW:
      if (band != BAND_MW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_SW:
      if (band != BAND_SW) {
        band = BAND_SW;
        SelectBand();
      }
      break;
    case AM_BAND_NONE:
      ToggleBand(band);
      SelectBand();
      break;
  }
#endif
}

void AMjumptoFM() {
  if (bandFM != FM_BAND_NONE) {
    if (bandFM == FM_BAND_FM) band = BAND_FM; else band = BAND_OIRT;
  } else {
    FMjumptoAM();
  }
}

void FMjumptoAM() {
#ifdef HAS_AIR_BAND
  if (bandAM == AM_BAND_ALL || bandAM == AM_BAND_LW_MW_AIR || bandAM == AM_BAND_LW_SW_AIR || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW_AIR || bandAM == AM_BAND_LW) {
    band = BAND_LW;
    if (stepsize > 3) stepsize = 3;
  } else if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW_AIR || bandAM == AM_BAND_MW) {
    band = BAND_MW;
  } else if (bandAM == AM_BAND_SW_AIR || bandAM == AM_BAND_SW) {
    band = BAND_SW;
  } else if (bandAM == AM_BAND_AIR) {
    band = BAND_AIR;
  }
#else
  if (bandAM == AM_BAND_ALL || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW) {
    band = BAND_LW;
    if (stepsize > 3) stepsize = 3;
  } else if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW) {
    band = BAND_MW;
  } else if (bandAM == AM_BAND_SW) {
    band = BAND_SW;
  }
#endif
}

void ToggleBand(byte nowBand) {
  switch (nowBand) {
#ifdef HAS_AIR_BAND
    case BAND_LW:
      if (bandAM == AM_BAND_LW_MW_SW || bandAM == AM_BAND_LW_MW_AIR || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_ALL) {
        band = BAND_MW;
      } else if (bandAM == AM_BAND_LW_SW_AIR || bandAM == AM_BAND_LW_SW) {
        band = BAND_SW;
      } else if (bandAM == AM_BAND_LW_MW_AIR || bandAM == AM_BAND_LW_SW_AIR || bandAM == AM_BAND_LW_AIR) {
        band = BAND_AIR;
      } else if (bandAM == AM_BAND_LW || bandAM == AM_BAND_NONE) {
        AMjumptoFM();
      }
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_LW_MW_SW || bandAM == AM_BAND_MW_SW_AIR || bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_ALL) {
        band = BAND_SW;
      } else if (bandAM == AM_BAND_LW_MW_AIR || bandAM == AM_BAND_MW_AIR) {
        band = BAND_AIR;
      } else if (bandAM == AM_BAND_MW || bandAM == AM_BAND_NONE) {
        AMjumptoFM();
      }
      break;
    case BAND_SW:
      if (bandAM == AM_BAND_LW_SW_AIR || bandAM == AM_BAND_MW_SW_AIR || bandAM == AM_BAND_SW_AIR || bandAM == AM_BAND_ALL) {
        band = BAND_AIR;
      } else if (bandAM == AM_BAND_LW_MW_SW || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_SW || bandAM == AM_BAND_NONE) {
        AMjumptoFM();
      }
      break;
    case BAND_AIR:
      AMjumptoFM();
      break;
#else  // has no air band
    case BAND_LW:
      if (bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_ALL) {
        band = BAND_MW;
      } else if (bandAM == AM_BAND_LW_SW) {
        band = BAND_SW;
      } else if (bandAM == AM_BAND_LW || bandAM == AM_BAND_NONE) {
        AMjumptoFM();
      }
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_ALL) {
        band = BAND_SW;
      } else if (bandAM == AM_BAND_LW_MW) {
        if (bandFM != FM_BAND_NONE) {
          if (bandFM == FM_BAND_FM) band = BAND_FM; else band = BAND_OIRT;
        } else {
          band = BAND_LW;
        }
      } else if (bandAM == AM_BAND_MW || bandAM == AM_BAND_NONE) {
        AMjumptoFM();
      }
      break;
    case BAND_SW:
      if (bandFM != FM_BAND_NONE) {
        if (bandFM == FM_BAND_FM) band = BAND_FM;
        else band = BAND_OIRT;
      } else { // FM be disabled
        if (bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_ALL) {
          band = BAND_LW;
        } else if (bandAM == AM_BAND_MW_SW) {
          band = BAND_MW;
        } else if (bandAM == AM_BAND_SW || bandAM == AM_BAND_NONE) {
          AMjumptoFM();
        }
      }
      break;
#endif
    case BAND_OIRT:
      if (bandFM == FM_BAND_ALL || bandFM == FM_BAND_FM) {
        band = BAND_FM;
      } else if (bandFM == FM_BAND_OIRT) {
        if (bandAM != AM_BAND_NONE) {
          FMjumptoAM();
        } else {
          // do nothing
        }
      }
      break;
    case BAND_FM:
      if (bandAM != AM_BAND_NONE) {
        FMjumptoAM();
      } else {
        if (bandFM == FM_BAND_OIRT || bandFM == FM_BAND_ALL) {
          band = BAND_OIRT;
        } else if (bandFM == FM_BAND_FM) {
          // do nothing
        }
      }
      break;
  }
}

void BANDBUTTONPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (memorystore) {
      EEPROM.writeByte(memorypos + EE_PRESETS_BAND_START, BAND_FM);
      EEPROM.writeUInt((memorypos * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
      EEPROM.commit();
      presets[memorypos].band = BAND_FM;
      presets[memorypos].frequency = EE_PRESETS_FREQUENCY;
      memorystore = false;
      ShowTuneMode();
      if (memoryposstatus == MEM_DARK || memoryposstatus == MEM_EXIST) {
        memoryposstatus = MEM_NORMAL;
        ShowMemoryPos();
      }
    } else {
      if (!usesquelch) radio.setUnMute();
      unsigned long counterold = millis();
      unsigned long counter = millis();
      if (!BWtune && !menu) {
        while (digitalRead(BANDBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold < 1000) {
          if (afscreen) {
            leave = true;
            BuildAdvancedRDS();
            freq_in = 0;
          } else if (advancedRDS) {
            leave = true;
            BuildDisplay();
            freq_in = 0;
            SelectBand();
            ScreensaverTimerReopen();
          } else {
            doBandToggle();
          }
        } else {
          if (band < BAND_GAP) {
            if (advancedRDS && !seek) {
              BuildAFScreen();
              freq_in = 0;
            } else {
              BuildAdvancedRDS();
              freq_in = 0;
            }
          } else {
            WakeToSleep(true);
          }
          while (digitalRead(BANDBUTTON) == LOW && counter - counterold <= 2500) counter = millis();
          if (counter - counterold > 2499) {
            switch (longbandpress) {
              case STANDBY:
                deepSleep();
                break;

              case SCREENOFF:
                screensavertriggered = true;
                MuteScreen(1);
                break;
            }
          }
        }
      }
    }
  }
  while (digitalRead(BANDBUTTON) == LOW) delay(50);
  delay(100);
}

void StoreFrequency() {
  switch (band) {
    case BAND_LW: freqold = frequency_LW; frequency_AM = frequency_LW; break;
    case BAND_MW: freqold = frequency_MW; frequency_AM = frequency_MW; break;
    case BAND_SW: freqold = frequency_SW; frequency_AM = frequency_SW; break;
#ifdef HAS_AIR_BAND
    case BAND_AIR: freqold = frequency_AIR; frequency_AM = frequency_AIR; break;
#endif
  }
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, frequency);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_OIRT, frequency_OIRT);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AM, frequency_AM);
  EEPROM.writeByte(EE_BYTE_BAND, band);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_LW, frequency_LW);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_MW, frequency_MW);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_SW, frequency_SW);
#ifdef HAS_AIR_BAND
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AIR, frequency_AIR);
#endif
  EEPROM.commit();
}

void LimitAMFrequency() {//todo air
  switch (band) {
    case BAND_LW:
      frequency_AM = frequency_LW;
      if (frequency_AM > LWHighEdgeSet || frequency_AM < LWLowEdgeSet) {
        frequency_AM = LWLowEdgeSet;
      }
      break;
    case BAND_MW:
      frequency_AM = frequency_MW;
      if (frequency_AM > MWHighEdgeSet || frequency_AM < MWLowEdgeSet) {
        frequency_AM = MWLowEdgeSet;
      }
      break;
    case BAND_SW:
      frequency_AM = frequency_SW;
      if (frequency_AM > SWHighEdgeSet || frequency_AM < SWLowEdgeSet) {
        frequency_AM = SWLowEdgeSet;
      }
      break;
  }
}

void DivdeSWMIBand() {
  if (frequency_AM >= FREQ_SW_160M_START && frequency_AM <= FREQ_SW_160M_END) {
    SWMIBandPos = SW_MI_BAND_160M;
    frequency_MIBand_160M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_120M_START && frequency_AM <= FREQ_SW_120M_END) {
    SWMIBandPos = SW_MI_BAND_120M;
    frequency_MIBand_120M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_90M_START && frequency_AM <= FREQ_SW_90M_END) {
    SWMIBandPos = SW_MI_BAND_90M;
    frequency_MIBand_90M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_75M_START && frequency_AM <= FREQ_SW_75M_END) {
    SWMIBandPos = SW_MI_BAND_75M;
    frequency_MIBand_75M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_60M_START && frequency_AM <= FREQ_SW_60M_END) {
    SWMIBandPos = SW_MI_BAND_60M;
    frequency_MIBand_60M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_49M_START && frequency_AM <= FREQ_SW_49M_END) {
    SWMIBandPos = SW_MI_BAND_49M;
    frequency_MIBand_49M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_41M_START && frequency_AM <= FREQ_SW_41M_END) {
    SWMIBandPos = SW_MI_BAND_41M;
    frequency_MIBand_41M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_31M_START && frequency_AM <= FREQ_SW_31M_END) {
    SWMIBandPos = SW_MI_BAND_31M;
    frequency_MIBand_31M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_25M_START && frequency_AM <= FREQ_SW_25M_END) {
    SWMIBandPos = SW_MI_BAND_25M;
    frequency_MIBand_25M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_22M_START && frequency_AM <= FREQ_SW_22M_END) {
    SWMIBandPos = SW_MI_BAND_22M;
    frequency_MIBand_22M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_19M_START && frequency_AM <= FREQ_SW_19M_END) {
    SWMIBandPos = SW_MI_BAND_19M;
    frequency_MIBand_19M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_16M_START && frequency_AM <= FREQ_SW_16M_END) {
    SWMIBandPos = SW_MI_BAND_16M;
    frequency_MIBand_16M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_15M_START && frequency_AM <= FREQ_SW_15M_END) {
    SWMIBandPos = SW_MI_BAND_15M;
    frequency_MIBand_15M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_13M_START && frequency_AM <= FREQ_SW_13M_END) {
    SWMIBandPos = SW_MI_BAND_13M;
    frequency_MIBand_13M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_11M_START && frequency_AM <= FREQ_SW_11M_END) {
    SWMIBandPos = SW_MI_BAND_11M;
    frequency_MIBand_11M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else {
    if (SWMIBandPos > SW_MI_BAND_GAP) {
      SWMIBandPosold = SWMIBandPos;
      SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
    }
    SWMIBandPos = SW_MI_BAND_GAP;
  }

}

void ToggleSWMIBand(bool frequencyup) {
  switch (SWMIBandPosold) {
    case SW_MI_BAND_GAP:
      if (frequencyup) {
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        frequency_AM = FREQ_SW_160M_START;
      } else {
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        frequency_AM = FREQ_SW_11M_END;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_11M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_160M == 0 ? FREQ_SW_160M_START : frequency_MIBand_160M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_13M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_13M == 0 ? FREQ_SW_13M_END : frequency_MIBand_13M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_13M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_11M == 0 ? FREQ_SW_11M_START : frequency_MIBand_11M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_15M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_15M == 0 ? FREQ_SW_15M_END : frequency_MIBand_15M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_15M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_13M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_13M == 0 ? FREQ_SW_13M_START : frequency_MIBand_13M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_16M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_16M == 0 ? FREQ_SW_16M_END : frequency_MIBand_16M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_16M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_15M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_15M == 0 ? FREQ_SW_15M_START : frequency_MIBand_15M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_19M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_19M == 0 ? FREQ_SW_19M_END : frequency_MIBand_19M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_19M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_16M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_16M == 0 ? FREQ_SW_16M_START : frequency_MIBand_16M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_22M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_22M == 0 ? FREQ_SW_22M_END : frequency_MIBand_22M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_22M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_19M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_19M == 0 ? FREQ_SW_19M_START : frequency_MIBand_19M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_25M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_25M == 0 ? FREQ_SW_25M_END : frequency_MIBand_25M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_25M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_22M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_22M == 0 ? FREQ_SW_22M_START : frequency_MIBand_22M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_31M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_31M == 0 ? FREQ_SW_31M_END : frequency_MIBand_31M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_31M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_25M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_25M == 0 ? FREQ_SW_25M_START : frequency_MIBand_25M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_41M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_41M == 0 ? FREQ_SW_41M_END : frequency_MIBand_41M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_41M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_31M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_31M == 0 ? FREQ_SW_31M_START : frequency_MIBand_31M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_49M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_49M == 0 ? FREQ_SW_49M_END : frequency_MIBand_49M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_49M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_41M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_41M == 0 ? FREQ_SW_41M_START : frequency_MIBand_41M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_60M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_60M == 0 ? FREQ_SW_60M_END : frequency_MIBand_60M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_60M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_49M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_49M == 0 ? FREQ_SW_49M_START : frequency_MIBand_49M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_75M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_75M == 0 ? FREQ_SW_75M_END : frequency_MIBand_75M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_75M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_60M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_60M == 0 ? FREQ_SW_60M_START : frequency_MIBand_60M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_90M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_90M == 0 ? FREQ_SW_90M_END : frequency_MIBand_90M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_90M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_75M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_75M == 0 ? FREQ_SW_75M_START : frequency_MIBand_75M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_120M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_120M == 0 ? FREQ_SW_120M_END : frequency_MIBand_120M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_120M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_90M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_90M == 0 ? FREQ_SW_90M_START : frequency_MIBand_90M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_160M == 0 ? FREQ_SW_160M_END : frequency_MIBand_160M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_160M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_120M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_120M == 0 ? FREQ_SW_120M_START : frequency_MIBand_120M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_11M == 0 ? FREQ_SW_49M_END : frequency_MIBand_11M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
  }
}

void SelectBand() {
  if (afscreen || advancedRDS) {
    BuildDisplay();
    freq_in = 0;
  }

  if (band > BAND_GAP) {
    if (!screenmute) tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
    if (tunemode == TUNE_MI_BAND && band != BAND_SW) tunemode = TUNE_MAN;
    BWreset = true;
    BWset = BWsetAM;
    switch (band) {
      case BAND_LW: freqold = frequency_LW; frequency_AM = frequency_LW; break;
      case BAND_MW: freqold = frequency_MW; frequency_AM = frequency_MW; break;
      case BAND_SW: freqold = frequency_SW; frequency_AM = frequency_SW; break;
#ifdef HAS_AIR_BAND
      case BAND_AIR: freqold = frequency_AIR; frequency_AM = frequency_AIR; break;
#endif
    }
    LimitAMFrequency();
    if (!externaltune && tunemode != TUNE_MEM) CheckBandForbiddenAM();
#ifdef HAS_AIR_BAND
    if (band == BAND_AIR) {
      radio.SetFreqAIR(10700);
    } else {
      radio.SetFreqAM(frequency_AM);
    }
#else
    radio.SetFreqAM(frequency_AM);
#endif
    radio.setAMAttenuation(amgain);
    radio.setAMCoChannel(amcodect, amcodectcount);
    doBW();
    if (!screenmute) {
      if (region == REGION_EU) tftPrint(-1, "PI:", 212, 193, GreyoutColor, BackgroundColor, 16);
      if (region == REGION_US) {
        tftPrint(-1, "PI:", 212, 184, GreyoutColor, BackgroundColor, 16);
        tftPrint(-1, "ID:", 212, 201, GreyoutColor, BackgroundColor, 16);
      }

      tftPrint(-1, "PS:", 3, 193, GreyoutColor, BackgroundColor, 16);
      tftPrint(-1, "RT:", 3, 221, GreyoutColor, BackgroundColor, 16);
      tftPrint(-1, "PTY:", 3, 163, GreyoutColor, BackgroundColor, 16);
      tft.drawBitmap(122, 5, RDSLogo, 35, 22, GreyoutColor);
      tft.fillRoundRect(249, 57, 30, 18, 2, GreyoutColor);
      tftPrint(0, "iMS", 265, 59, BackgroundColor, GreyoutColor, 16);
      tft.fillRoundRect(287, 57, 30, 18, 2, GreyoutColor);
      tftPrint(0, "EQ", 301, 59, BackgroundColor, GreyoutColor, 16);
      tftReplace(-1, "MHz", "kHz", 258, 76, ActiveColor, ActiveColorSmooth, BackgroundColor, 28);
      // todo
      // if (band == AM_BAND_AIR) tftPrint(-1, "MHz", 258, 76, ActiveColor, ActiveColorSmooth, 28);
      // else tftPrint(-1, "KHz", 258, 76, ActiveColor, ActiveColorSmooth, 28);
    }
  } else {
    if (tunemode == TUNE_MI_BAND) tunemode = TUNE_MAN;

    if (!leave) {
      radio.power(0);
      delay(50);
      if (band == BAND_FM) radio.SetFreq(frequency);
      if (band == BAND_OIRT) radio.SetFreq(frequency_OIRT);
    }

    BWreset = true;
    BWset = BWsetFM;
    freqold = frequency_AM;
    if (!externaltune && tunemode != TUNE_MEM) CheckBandForbiddenFM();
    doBW();
    if (region == REGION_EU) tftPrint(-1, "PI:", 212, 193, ActiveColor, ActiveColorSmooth, 16);
    if (region == REGION_US) {
      tftPrint(-1, "PI:", 212, 184, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(-1, "ID:", 212, 201, ActiveColor, ActiveColorSmooth, 16);
    }
    tftPrint(-1, "PS:", 3, 193, ActiveColor, ActiveColorSmooth, 16);
    tftPrint(-1, "RT:", 3, 221, ActiveColor, ActiveColorSmooth, 16);
    tftPrint(-1, "PTY:", 3, 163, ActiveColor, ActiveColorSmooth, 16);

    tftReplace(-1, "kHz", "MHz", 258, 76, ActiveColor, ActiveColorSmooth, BackgroundColor, 28);
  }

  if (!leave) radio.clearRDS(fullsearchrds);
  ShowFreq(0);

  if (!screenmute) {
    tft.fillRect(113, 38, 124, 4, BackgroundColor);
    ShowErrors();
    showPTY();
    showRadioText();
    showPI();
    updateiMS();
    updateEQ();
    ShowTuneMode();
    ShowStepSize();

    tftPrint(-1, myLanguage[language][102], 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(-1, myLanguage[language][103], 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(-1, myLanguage[language][104], 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(-1, myLanguage[language][105], 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(-1, myLanguage[language][106], 70, 32, BackgroundColor, BackgroundColor, 16);

#ifdef HAS_AIR_BAND
    tftPrint(-1, myLanguage[language][223], 70, 32, BackgroundColor, BackgroundColor, 16);
#endif

    switch (band) {
      case BAND_LW: tftPrint(-1, myLanguage[language][102], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_MW: tftPrint(-1, myLanguage[language][103], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_SW: tftPrint(-1, myLanguage[language][104], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_FM: tftPrint(-1, myLanguage[language][105], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_OIRT: tftPrint(-1, myLanguage[language][106], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;

#ifdef HAS_AIR_BAND
      case BAND_AIR: tftPrint(-1, myLanguage[language][223], 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
#endif
    }
  }
  leave = false;
}

void BWButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (!usesquelch) radio.setUnMute();
    if (!BWtune && !menu) {
      if (!screenmute) tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
      unsigned long counterold = millis();
      unsigned long counter = millis();
      while (digitalRead(BWBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

      if (counter - counterold < 1000) {
        BuildBWSelector();
        freq_in = 0;
        BWtune = true;
        BWtemp = BWset;
      } else {
        if (band == BAND_FM || band == BAND_OIRT) {
          doStereoToggle();
        } else {
          BuildBWSelector();
          freq_in = 0;
          BWtune = true;
        }
      }
    }
  }
  while (digitalRead(BWBUTTON) == LOW) delay(50);
  delay(100);
}

void doStereoToggle() {
  if (StereoToggle) {
    if (!screenmute) {
      tft.drawCircle(66, 15, 10, BackgroundColor);
      tft.drawCircle(66, 15, 9, BackgroundColor);
      tft.drawCircle(76, 15, 10, BackgroundColor);
      tft.drawCircle(76, 15, 9, BackgroundColor);
      tft.drawCircle(71, 15, 10, SecondaryColor);
      tft.drawCircle(71, 15, 9, SecondaryColor);
    }
    radio.setMono(true);
    StereoToggle = false;
  } else {
    if (!screenmute) {
      tft.drawCircle(71, 15, 10, BackgroundColor);
      tft.drawCircle(71, 15, 9, BackgroundColor);
      tft.drawCircle(66, 15, 10, GreyoutColor);
      tft.drawCircle(66, 15, 9, GreyoutColor);
      tft.drawCircle(76, 15, 10, GreyoutColor);
      tft.drawCircle(76, 15, 9, GreyoutColor);
    }
    radio.setMono(false);
    Stereostatusold = false;
    StereoToggle = true;
  }
  EEPROM.writeByte(EE_BYTE_STEREO, StereoToggle);
  EEPROM.commit();
}

void ModeButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (!usesquelch) radio.setUnMute();
    if (advancedRDS) {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
      ScreensaverTimerReopen();
    } else if (afscreen) {
      if (afpagenr == 1) afpagenr = 2; else if (afpagenr == 2 && afpage) afpagenr = 3; else afpagenr = 1;
      BuildAFScreen();
      freq_in = 0;
    } else {
      if (!BWtune && !menu) {
        if (!screenmute) {
          tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
        }
        memorystore = false;
        unsigned long counterold = millis();
        unsigned long counter = millis();
        while (digitalRead(MODEBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold <= 1000) {
          doTuneMode();
        } else {
          if (!BWtune && !menu) {
            menuoption = ITEM1;
            menupage = INDEX;
            menuitem = 0;
            if (spispeed == 7) tft.setSPISpeed(40);
            PSSprite.unloadFont();
            if (language == LANGUAGE_CHS) PSSprite.loadFont(FONT16_CHS); else PSSprite.loadFont(FONT16);
            BuildMenu();
            freq_in = 0;
            menu = true;
            ScreensaverTimerSet(OFF);
          }
        }
      } else {
        if (menuopen && !submenu) {
          ButtonPress();
        } else if (!submenu) {
          OStatusold = 1000;
          Stereostatusold = false;
          SStatusold = 2000;
          BWOld = 0;
          radio.clearRDS(fullsearchrds);
          RDSstatus = false;
          if (frequency % 10 != 0) {
            if (fmdefaultstepsize == 1) Round100K(frequency); else Round50K(frequency);
            EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, frequency);
          }
          CheckBandForbiddenFM();
          CheckBandForbiddenAM();
          if (!usesquelch) radio.setUnMute();
          MWLowEdgeSet = mwstepsize == false ? FREQ_MW_LOW_EDGE_MIN_9K : FREQ_MW_LOW_EDGE_MIN_10K;
          MWHighEdgeSet = mwstepsize == false ? FREQ_MW_HIGH_EDGE_MAX_9K : FREQ_MW_HIGH_EDGE_MAX_10K;
          doBandSelectionFM();
          doBandSelectionAM();
          ScreensaverTimerSet(screensaverOptions[screensaverset]);
          if (screensaverset) ScreensaverTimerRestart();
          endMenu();
        } else {
          if (submenu) {
            if (menuopen) {
              if (menupage == CONNECTIVITY && menuoption == ITEM2 && wifi) {
                tryWiFi();
                delay(2000);
              }
              if (menupage == DISPLAYSETTINGS && menuoption == ITEM5) {
                doTheme();
              }
              menuopen = false;
            }
            submenu = false;
            menuoption = ITEM1;
            menupage = INDEX;
            menuitem = 0;
            PSSprite.unloadFont();
            if (language == LANGUAGE_CHS) PSSprite.loadFont(FONT16_CHS); else PSSprite.loadFont(FONT16);
            BuildMenu();
            freq_in = 0;
          }
        }
      }
    }
  }
  while (digitalRead(MODEBUTTON) == LOW) delay(50);
  delay(100);
}

void ShowStepSize() {
  if (!advancedRDS) {
    tft.fillRect(191, 38, 15, 4, GreyoutColor);
    tft.fillRect(222, 38, 15, 4, GreyoutColor);
    if (band < BAND_GAP) tft.fillRect(113, 38, 15, 4, GreyoutColor); else if (band != BAND_LW && band != BAND_MW) tft.fillRect(129, 38, 15, 4, GreyoutColor);
    if (band < BAND_GAP) tft.fillRect(144, 38, 15, 4, GreyoutColor); else tft.fillRect(159, 38, 15, 4, GreyoutColor);
    if (stepsize == 1) tft.fillRect(222, 38, 15, 4, InsignificantColor);
    if (stepsize == 2) tft.fillRect(191, 38, 15, 4, InsignificantColor);
    if (stepsize == 3) {
      if (band < BAND_GAP) tft.fillRect(144, 38, 15, 4, InsignificantColor); else tft.fillRect(159, 38, 15, 4, InsignificantColor);
    }
    if (stepsize == 4) {
      if (band < BAND_GAP) tft.fillRect(113, 38, 15, 4, InsignificantColor); else tft.fillRect(129, 38, 15, 4, InsignificantColor);
    }
  }
}

void Round30K(unsigned int freq) {
  if (freq % FREQ_OIRT_STEP_30K == 1) {
    frequency_OIRT = (freq + 1);
  } else if (freq % FREQ_OIRT_STEP_30K == 0) {
    frequency_OIRT = (freq - 1);
  }
}

void Round50K(unsigned int freq) {
  if (freq % 10 < 3) {
    frequency = (freq - freq % 10);
  } else if (freq % 10 > 2 && freq % 10 < 8) {
    frequency = (freq - (freq % 10 - 5));
  } else if (freq % 10 > 7) {
    frequency = (freq - (freq % 10) + 10);
  }
}

void Round100K(unsigned int freq) {
  if (freq % 10 < 5) {
    frequency = (freq - freq % 10);
  } else {
    frequency = (freq - (freq % 10) + 10);
  }
}

void Round200K(unsigned int freq) {
  frequency = (freq / 10) * 10;

  if (freq % 10 >= 5) frequency += 10;
  if ((frequency / 100) % 2 == 0) frequency += 10;
  if (frequency > 10800) frequency = 10790;
  if (frequency % 20 == 0 && (frequency / 10) % 2 == 0) frequency -= 10;
}

void Round5K(unsigned int freqAM) {
  if (freqAM % 10 < 3) {
    frequency_AM = (freqAM - freqAM % 10);
  } else if (freqAM % 10 > 2 && freqAM % 10 < 8) {
    frequency_AM = (freqAM - (freqAM % 10 - 5));
  } else if (freqAM % 10 > 7) {
    frequency_AM = (freqAM - (freqAM % 10) + 10);
  }
}

void RoundStep() {//todo air
  if (band == BAND_FM) {
    unsigned int freq = frequency;
    switch (fmdefaultstepsize) {
      case 0: Round50K(freq); break;
      case 1: Round100K(freq); break;
      case 2: Round200K(freq); break;
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    Round30K(frequency_OIRT);
    radio.SetFreq(frequency_OIRT);
  } else {
    if (band == BAND_LW) {
      unsigned int freq = frequency_AM / (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_AM = freq * (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_LW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    } else if (band == BAND_MW) {
      unsigned int freq = frequency_AM / (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_AM = freq * (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_MW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    } else if (band == BAND_SW) {
      Round5K(frequency_AM);
      frequency_SW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    }
  }

  StoreFrequency();

  while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
}

void ButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (!usesquelch) radio.setUnMute();
    if (advancedRDS) {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
    }
    if (!BWtune && !menu) {
      if (tunemode == TUNE_MEM) {
        if (!memorystore) {
          memorystore = true;
          if (!IsStationEmpty()) memoryposstatus = MEM_EXIST;
          else memoryposstatus = MEM_NORMAL;
          ShowMemoryPos();

          ShowTuneMode();
        } else {
          memorystore = false;
          StoreMemoryPos(memorypos);

          ShowTuneMode();
          if (memoryposstatus == MEM_DARK || memoryposstatus == MEM_EXIST) {
            memoryposstatus = MEM_NORMAL;
            ShowMemoryPos();
          }
        }
      } else {
        if (!screenmute) tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
        unsigned long counterold = millis();
        unsigned long counter = millis();
        while (digitalRead(ROTARY_BUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold < 1000) {
          if (tunemode == TUNE_MAN) {
            stepsize++;
            if (band == BAND_SW || band < BAND_GAP) {
              if (stepsize > 4) stepsize = 0;
            } else {
              if (stepsize > 3) stepsize = 0;
            }

            if (!screenmute) ShowStepSize();

            EEPROM.writeByte(EE_BYTE_STEPSIZE, stepsize);
            EEPROM.commit();
            if (stepsize == 0) {
              RoundStep();
              ShowFreq(0);
            }
          }
        } else {
          if (band < BAND_GAP) {
            if (addRowToCSV()) ShowFreq(2); else ShowFreq(3);
            delay(200);
            while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
            ShowFreq(0);
          }
        }
      }
      if (screensaverset) {
        ScreensaverTimerRestart();
      }
    } else {
      if (menu) DoMenu();
      if (BWtune) {
        if (BWsettemp == 18 || BWsettemp == 19) {
          if (BWsettemp == 18) iMSset = !iMSset;
          if (BWsettemp == 19) EQset = !EQset;
          if (!iMSset && !EQset) iMSEQ = 0;
          if (iMSset && EQset) iMSEQ = 2;
          if (!iMSset && EQset) iMSEQ = 3;
          if (iMSset && !EQset) iMSEQ = 4;
          EEPROM.writeByte(EE_BYTE_IMSSET, iMSset);
          EEPROM.writeByte(EE_BYTE_EQSET, EQset);
          EEPROM.commit();
          updateiMS();
          updateEQ();
          if (XDRGTKUSB || XDRGTKTCP) DataPrint("G" + String(!EQset) + String(!iMSset) + "\n");
          showBWSelector();
          if (band < BAND_GAP) {
            drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          } else {
            drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          }
        } else if (BWsettemp == 20) {
          BuildDisplay();
          freq_in = 0;
          SelectBand();
        } else {
          doBW();
          showBWSelector();
          if (band < BAND_GAP) {
            drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          } else {
            drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          }
        }
      }
    }
  }
  while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
}

void KeyUp() {
  rotary = 0;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (!afscreen) {
      if (!BWtune && !menu) {
        switch (tunemode) {
          case TUNE_MAN:
            TuneUp();
            break;

          case TUNE_AUTO:
            direction = true;
            seek = true;
            seekinit = true;
            Seek(direction);
            break;

          case TUNE_MEM:
            memorypos++;
            if (memorypos > EE_PRESETS_CNT - 1) memorypos = 0;
            if (!memorystore) {
              while (IsStationEmpty()) {
                memorypos++;
                if (memorypos > EE_PRESETS_CNT - 1) {
                  memorypos = 0;
                  break;
                }
              }
            }
            if (!memorystore) {
              DoMemoryPosTune();
            } else {
              if (!IsStationEmpty()) memoryposstatus = MEM_EXIST; else memoryposstatus = MEM_DARK;
            }
            ShowMemoryPos();
            EEPROM.writeByte(EE_BYTE_MEMORYPOS, memorypos);
            EEPROM.commit();
            break;

          case TUNE_MI_BAND:
            if (showSWMIBand) {
              if (displayflip) {
                ToggleSWMIBand(true);
              } else {
                ToggleSWMIBand(REVERSE);
              }
            }
            break;
        }
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS(fullsearchrds);
          memtune = false;
          ShowFreq(0);
          store = true;
        }
      } else {
        MenuUp();
      }
    }
  }
}

void KeyDown() {
  rotary = 0;
  if (scandxmode) {
    cancelDXScan();
  } else {
    if (!afscreen) {
      if (!BWtune && !menu) {
        switch (tunemode) {
          case TUNE_MAN:
            TuneDown();
            break;

          case TUNE_AUTO:
            direction = false;
            seek = true;
            seekinit = true;
            Seek(direction);
            break;

          case TUNE_MEM:
            memorypos--;
            if (memorypos > EE_PRESETS_CNT - 1) memorypos = EE_PRESETS_CNT - 1;
            if (!memorystore) {
              while (IsStationEmpty()) {
                memorypos--;
                if (memorypos > EE_PRESETS_CNT - 1) {
                  memorypos = EE_PRESETS_CNT - 1;
                  break;
                }
              }
            }
            if (!memorystore) {
              DoMemoryPosTune();
            } else {
              if (!IsStationEmpty()) memoryposstatus = MEM_EXIST; else memoryposstatus = MEM_DARK;
            }
            ShowMemoryPos();
            EEPROM.writeByte(EE_BYTE_MEMORYPOS, memorypos);
            EEPROM.commit();
            break;
          case TUNE_MI_BAND:
            if (showSWMIBand) {
              if (displayflip) {
                ToggleSWMIBand(REVERSE);
              } else {
                ToggleSWMIBand(true);
              }
            }
            break;
        }
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS(fullsearchrds);
          memtune = false;
          ShowFreq(0);
          store = true;
        }
      } else {
        MenuDown();
      }
    }
  }
}

bool IsStationEmpty() {
  if (presets[memorypos].band == BAND_FM && presets[memorypos].frequency == EE_PRESETS_FREQUENCY) {
    return true;
  }
  return false;
}

bool IsFrequencyUsed(unsigned int freq) {
  bool result = false;
  for (byte x = scanstart; x <= scanstop; x++) {
    if ((presets[x].band == BAND_FM || presets[x].band == BAND_OIRT) && presets[x].frequency == freq) {
      result = true;
      break;
    }
  }
  return result;
}

void ShowMemoryPos() {
  if (tunemode == TUNE_MEM) {
    int memposcolor = 0;
    int memposcolorsmooth = 0;

    if (!memorystore) {
      if (IsStationEmpty()) memoryposstatus = MEM_DARK; else memoryposstatus = MEM_NORMAL;
    }

    switch (memoryposstatus) {
      case MEM_DARK:
        memposcolor = InsignificantColor;
        memposcolorsmooth = InsignificantColorSmooth;
        break;

      case MEM_NORMAL:
        memposcolor = PrimaryColor;
        memposcolorsmooth = PrimaryColorSmooth;
        break;

      case MEM_EXIST:
        memposcolor = SignificantColor;
        memposcolorsmooth = SignificantColorSmooth;
        break;
    }
    if (advancedRDS) {
      tftReplace(-1, String(memoryposold + 1), String(memorypos + 1), 215, 36, SecondaryColor, SecondaryColorSmooth, BackgroundColor, 16);
    } else {
      tftReplace(-1, String(memoryposold + 1), String(memorypos + 1), 50, 32, memposcolor, memposcolorsmooth, BackgroundColor, 16);
    }
    memoryposold = memorypos;
  } else {
    if (advancedRDS) {
      tftPrint(-1, String(memorypos + 1), 215, 36, BackgroundColor, BackgroundColor, 16);
    } else {
      tftPrint(-1, String(memorypos + 1), 50, 32, BackgroundColor, BackgroundColor, 16);
    }
  }
}

void DoMemoryPosTune() {
  if (spispeed == 7) tft.setSPISpeed(50);
  radio.clearRDS(fullsearchrds);

  // Process empty stations
  if (IsStationEmpty()) {
    memoryposstatus = MEM_DARK;
    return;
  } else {
    memoryposstatus = MEM_NORMAL;
  }

  if (band != presets[memorypos].band) {
    band = presets[memorypos].band;
    SelectBand();
  } else {
    band = presets[memorypos].band;
  }

  switch (band) {
    case BAND_FM:
      frequency = presets[memorypos].frequency;
      radio.SetFreq(frequency);
      break;
    case BAND_OIRT:
      frequency_OIRT = presets[memorypos].frequency;
      radio.SetFreq(frequency_OIRT);
      break;
    case BAND_LW:
      frequency_LW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_LW);
      break;
    case BAND_MW:
      frequency_MW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_MW);
      break;
    case BAND_SW:
      frequency_SW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_SW);
      break;
#ifdef HAS_AIR_BAND
    case BAND_AIR:
      frequency_AIR = presets[memorypos].frequency;
      // radio.SetFreq(frequency_AIR); // todo
      radio.SetFreqAM(10700); // todo
      break;
#endif
  }

  if (band == BAND_FM || band == BAND_OIRT) {
    StereoToggle = presets[memorypos].ms;
    if (!StereoToggle) {
      Stereostatusold = false;
      tft.drawCircle(66, 15, 10, BackgroundColor);
      tft.drawCircle(66, 15, 9, BackgroundColor);
      tft.drawCircle(76, 15, 10, BackgroundColor);
      tft.drawCircle(76, 15, 9, BackgroundColor);
      tft.drawCircle(71, 15, 10, SecondaryColor);
      tft.drawCircle(71, 15, 9, SecondaryColor);
    } else {
      Stereostatusold = false;
      tft.drawCircle(71, 15, 10, BackgroundColor);
      tft.drawCircle(71, 15, 9, BackgroundColor);
      tft.drawCircle(66, 15, 10, GreyoutColor);
      tft.drawCircle(66, 15, 9, GreyoutColor);
      tft.drawCircle(76, 15, 10, GreyoutColor);
      tft.drawCircle(76, 15, 9, GreyoutColor);
    }
  }

  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");

  String stationText = "";
  if (presets[memorypos].RDSPS[0] != '\0') {
    for (byte i = 0; i < 9; i++) stationText += presets[memorypos].RDSPS[i];
  }

  if (presets[memorypos].RDSPI[0] != '\0') {
    for (byte i = 0; i < 4; i++) {
      radio.rds.picode[i] = presets[memorypos].RDSPI[i];
    }
    radio.rds.picode[4] = ' ';
    radio.rds.picode[5] = ' ';
    radio.rds.picode[6] = '\0';
    radio.rds.correctPI = (uint16_t)strtol(radio.rds.picode, NULL, 16);
  } else {
    for (byte i = 0; i < 6; i++) {
      radio.rds.picode[i] = '\0';
    }
  }

  radio.rds.stationName = stationText;

  BWset = presets[memorypos].bw;
  doBW();
  memtune = true;
  memreset = true;
  rdsflagreset = false;
  findMemoryAF = true;
  ShowFreq(0);
}

void ShowFreq(int mode) {
  if (!setupmode) {
    if (band != BAND_FM && band != BAND_OIRT) {
      if (freqold < 2000 && frequency_AM >= 2000 && stepsize == 0) if (frequency_AM != 27000 && freqold != 144) radio.SetFreqAM(2000);
      if (freqold >= 2000 && frequency_AM < 2000 && stepsize == 0) if (frequency_AM != 144 && freqold != 27000) radio.SetFreqAM(1998);
    }
  }

  if (band > BAND_GAP) {
    switch (band) {
      case BAND_LW: frequency_AM = frequency_LW; break;
      case BAND_MW: frequency_AM = frequency_MW; break;
      case BAND_SW: frequency_AM = frequency_SW; break;
#ifdef HAS_AIR_BAND
      case BAND_AIR: frequency_AM = frequency_AIR; break; // todo air
#endif
    }

    if (!screenmute) {
      FrequencySprite.fillSprite(BackgroundColor);

      switch (freqfont) {
        case 0: FrequencySprite.loadFont(FREQFONT0); break;
        case 1: FrequencySprite.loadFont(FREQFONT1); break;
        case 2: FrequencySprite.loadFont(FREQFONT2); break;
        case 3: FrequencySprite.loadFont(FREQFONT3); break;
        case 4: FrequencySprite.loadFont(FREQFONT4); break;
        case 5: FrequencySprite.loadFont(FREQFONT5); break;
      }

      FrequencySprite.setTextDatum(TR_DATUM);
      FrequencySprite.setTextColor(FreqColor, FreqColorSmooth, false);
      if (band == BAND_AIR) {
        FrequencySprite.drawString(String(frequency_AM / 1000) + "." + String(frequency_AM % 1000) + " ", 218, -6);
      } else {
        FrequencySprite.drawString(String(frequency_AM) + " ", 218, -6);
      }
      FrequencySprite.unloadFont();
      FrequencySprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
      FrequencySprite.setTextDatum(TL_DATUM);
      FrequencySprite.loadFont(FONT16);

      if (band == BAND_SW && showSWMIBand) {
        DivdeSWMIBand();
        updateSWMIBand();
      }

      FrequencySprite.unloadFont();
      FrequencySprite.pushSprite(46, 46);
    }

    freqold = frequency_AM;
  } else {
    unsigned int freq = 0;
    if (band == BAND_FM) freq = frequency + ConverterSet * 100;
    if (band == BAND_OIRT) freq = frequency_OIRT;

    if (!screenmute) {
      if (advancedRDS) {
        for (int i = 0; i < 33; i++) tft.fillCircle((6 * i) + 10, 133, 2, GreyoutColor);
        tftReplace(1, String(freqold / 100) + "." + (freqold % 100 < 10 ? "0" : "") + String(freqold % 100), String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100), 273, 35, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
        freqold = freq;
      } else if (afscreen) {
        tftReplace(1, String(freqold / 100) + "." + (freqold % 100 < 10 ? "0" : "") + String(freqold % 100) + " MHz", String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100), 290, 201, BWAutoColor, BWAutoColorSmooth, BackgroundColor, 16);
        freqold = freq;
      } else {
        switch (freqfont) {
          case 0: FrequencySprite.loadFont(FREQFONT0); break;
          case 1: FrequencySprite.loadFont(FREQFONT1); break;
          case 2: FrequencySprite.loadFont(FREQFONT2); break;
          case 3: FrequencySprite.loadFont(FREQFONT3); break;
          case 4: FrequencySprite.loadFont(FREQFONT4); break;
          case 5: FrequencySprite.loadFont(FREQFONT5); break;
        }

        FrequencySprite.setTextDatum(TR_DATUM);
        switch (mode) {
          case 0:
            FrequencySprite.fillSprite(BackgroundColor);
            FrequencySprite.setTextColor(FreqColor, FreqColorSmooth, false);
            FrequencySprite.drawString(String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100) + " ", 218, -6);
            FrequencySprite.pushSprite(46, 46);
            freqold = freq;
            break;

          case 1:
            FrequencySprite.fillSprite(BackgroundColor);
            FrequencySprite.pushSprite(46, 46);
            tftPrint(0, myLanguage[language][34], 146, 58, ActiveColor, ActiveColorSmooth, 28);
            break;

          case 2:
            FrequencySprite.fillSprite(BackgroundColor);
            FrequencySprite.pushSprite(46, 46);
            tftPrint(0, myLanguage[language][290], 146, 58, ActiveColor, ActiveColorSmooth, 28);
            break;

          case 3:
            FrequencySprite.fillSprite(BackgroundColor);
            FrequencySprite.pushSprite(46, 46);
            tftPrint(0, myLanguage[language][291], 146, 58, ActiveColor, ActiveColorSmooth, 28);
            break;
        }
        FrequencySprite.unloadFont();
      }
    }
  }

  if (spispeed == 7) setAutoSpeedSPI();
  rdsreset = true;
  licold = 254;
  ECCold = 253;
  afmethodBold = false;
  aid_counterold = 0;
  dropout = false;

  if (wifi) {
    Udp.beginPacket(remoteip, 9030);
    if (band == BAND_FM) {
      Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency) + "0000");
    } else if (band == BAND_OIRT) {
      Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency_OIRT) + "0000");
    } else {
      Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency_AM) + "000");
    }
    Udp.endPacket();
  }
  tuningtimer = millis();

  if (!rdsflagreset && !screenmute && !afscreen) {
    ShowRDSLogo(false);
    if (!advancedRDS) {
      FullLineSprite.fillSprite(BackgroundColor);
      FullLineSprite.drawLine(283, 0, 283, 19, FrameColor);
      FullLineSprite.pushSprite(36, 220);
      tft.fillCircle(314, 223, 2, GreyoutColor);
      tft.fillCircle(314, 234, 2, GreyoutColor);
    } else {
      RDSSprite.fillSprite(BackgroundColor);
      RDSSprite.pushSprite(36, 220);
      tft.fillCircle(203, 223, 2, GreyoutColor);
      tft.fillCircle(203, 234, 2, GreyoutColor);
    }
    rdsflagreset = true;
  }
}

void ShowSignalLevel() {
  if (!screenmute) {
    if (millis() >= SNRupdatetimer + TIMER_SNR_TIMER) {
      SNRupdatetimer = millis();
      if (!advancedRDS) {
        if (CN > (CNold + 1) || CN < (CNold - 1)) {
          if (CNold == 0) tftPrint(1, "--", 295, 163, BackgroundColor, BackgroundColor, 16); else tftPrint(1, String(CNold), 295, 163, BackgroundColor, BackgroundColor, 16);
          if (tuned) {
            if (CN == 0) tftPrint(1, "--", 295, 163, PrimaryColor, PrimaryColorSmooth, 16); else tftPrint(1, String(CN), 295, 163, PrimaryColor, PrimaryColorSmooth, 16);
            CNold = CN;
          } else {
            tftPrint(1, "--", 295, 163, PrimaryColor, PrimaryColorSmooth, 16);
            CNold = 0;
          }
        }
      }
    }
    SAvg = (((SAvg * 9) + 5) / 10) + SStatus;
    SAvg2 = (((SAvg2 * 9) + 5) / 10) + CN;

    float sval = 0;
    int16_t smeter = 0;
    int16_t segments;

    if (SStatus > 0) {
      if (SStatus < 1000) {
        sval = 51 * ((pow(10, (((float)SStatus) / 1000))) - 1);
        smeter = int16_t(sval);
      } else {
        smeter = 511;
      }
    }

    smeter = int16_t(sval);
    SStatus = SAvg / 10;
    CN = SAvg2 / 10;

    if (!menu) analogWrite(SMETERPIN, smeter);

    int SStatusprint = 0;
    if (unit == 0) SStatusprint = SStatus;
    if (unit == 1) SStatusprint = ((SStatus * 100) + 10875) / 100;
    if (unit == 2) SStatusprint = round((float(SStatus) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0);

    if (SStatusprint > (SStatusold + 3) || SStatusprint < (SStatusold - 3)) {
      if (advancedRDS) {
        tftReplace(1, String(SStatusold / 10) + "." + String(abs(SStatusold % 10)), String(SStatusprint / 10) + "." + String(abs(SStatusprint % 10)), 273, 51, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
      } else {
        if (SStatusold / 10 != SStatusprint / 10) tftReplace(1, String(SStatusold / 10), String(SStatusprint / 10), 288, 105, FreqColor, FreqColorSmooth, BackgroundColor, 48);
        tftReplace(1, "." + String(abs(SStatusold % 10)), "." + String(abs(SStatusprint % 10)), 310, 105, FreqColor, FreqColorSmooth, BackgroundColor, 28);

        if (band < BAND_GAP) segments = map(SStatus / 10, 0, 70, 0, 100); else segments = (SStatus + 200) / 10;
        tft.fillRect(16, 105, 2 * constrain(segments, 0, 63), 6, BarInsignificantColor);
        tft.fillRect(16 + 2 * 63, 105, 2 * (constrain(segments, 63, 94) - 63), 6, BarSignificantColor);
        tft.fillRect(16 + 2 * constrain(segments, 0, 94), 105, 2 * (94 - constrain(segments, 0, 94)), 6, GreyoutColor);
      }
      SStatusold = SStatusprint;
    }
  }
  if (wifi && millis() >= udptimer + 2000) {
    udptimer = millis();
    Udp.beginPacket(remoteip, 9030);
    Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";RcvLevel=" + String(SStatus / 10));
    Udp.endPacket();
  }
}

void ShowRDSLogo(bool RDSstatus) {
  if (!screenmute) {
    if (RDSstatus != RDSstatusold) {
      if (RDSstatus) {
        tft.drawBitmap(122, 5, RDSLogo, 35, 22, RDSColor);
      } else {
        tft.drawBitmap(122, 5, RDSLogo, 35, 22, GreyoutColor);
      }
    }
    RDSstatusold = RDSstatus;
  }
}

void ShowStereoStatus() {
  if (StereoToggle) {
    if (band < BAND_GAP) Stereostatus = radio.getStereoStatus(); else Stereostatus = false;
    if (Stereostatus != Stereostatusold) {
      if (Stereostatus && !screenmute) {
        tft.drawCircle(66, 15, 10, StereoColor);
        tft.drawCircle(66, 15, 9, StereoColor);
        tft.drawCircle(76, 15, 10, StereoColor);
        tft.drawCircle(76, 15, 9, StereoColor);
      } else {
        if (!screenmute) {
          tft.drawCircle(66, 15, 10, GreyoutColor);
          tft.drawCircle(66, 15, 9, GreyoutColor);
          tft.drawCircle(76, 15, 10, GreyoutColor);
          tft.drawCircle(76, 15, 9, GreyoutColor);
        }
      }
      Stereostatusold = Stereostatus;
    }
  }
}

void ShowOffset() {
  if (OStatus != OStatusold) {
    if (millis() >= offsetupdatetimer + TIMER_OFFSET_TIMER) {
      offsetupdatetimer = millis();
    } else {
      return;
    }

    if (band < BAND_GAP) {
      if (OStatus < -500) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, SignificantColor);
        tuned = false;
      } else if (OStatus < -250 && OStatus > -500) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, SignificantColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else if (USN < 250 && WAM < 250 && OStatus > -250 && OStatus < 250 && !SQ) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, InsignificantColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = true;
      } else if (OStatus > 250 && OStatus < 500) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, SignificantColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else if (OStatus > 500) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, SignificantColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      }
    } else {
      if (OStatus <= -3) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, SignificantColor);
        tuned = false;
      } else if (OStatus < -2 && OStatus > -3) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, SignificantColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else if (OStatus > -2 && OStatus < 2 && !SQ) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, InsignificantColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = true;
      } else if (OStatus > 2 && OStatus < 3) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, SignificantColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else if (OStatus >= 3) {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, SignificantColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      } else {
        tft.fillTriangle(3, 8, 3, 22, 11, 14, GreyoutColor);
        tft.fillTriangle(13, 8, 13, 22, 21, 14, GreyoutColor);
        tft.fillCircle(26, 15, 3, GreyoutColor);
        tft.fillTriangle(32, 14, 40, 8, 40, 22, GreyoutColor);
        tft.fillTriangle(42, 14, 50, 8, 50, 22, GreyoutColor);
        tuned = false;
      }
    }
    OStatusold = OStatus;
  }
}

void ShowBW() {
  if (!BWtune && millis() >= bwupdatetimer + TIMER_BW_TIMER) {
    bwupdatetimer = millis();
  } else {
    if (!BWtune) return;
  }

  if (BW != BWOld || BWreset) {
    if (BWset == 0) tftReplace(1, String (BWOld, DEC), String (BW, DEC), 201, 4, BWAutoColor, BWAutoColorSmooth, BackgroundColor, 28); else tftReplace(1, String (BWOld, DEC), String (BW, DEC), 201, 4, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 28);
    BWOld = BW;
    BWreset = false;
    if (wifi) {
      Udp.beginPacket(remoteip, 9030);
      Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";bandwidth=" + String(BW * 1000));
      Udp.endPacket();
    }
  }
}

void ShowModLevel() {
  if (showmodulation) {
    int segments;
    if (MStatus > 120) MStatus = 120;

    if (seek || SQ) {
      MStatus = 0;
      MStatusold = 1;
    }

    segments = map(MStatus, 0, 120, 0, 93);

    if (segments < DisplayedSegments && (millis() - ModulationpreviousMillis >= 20)) {
      DisplayedSegments = max(DisplayedSegments - 3, segments);
      ModulationpreviousMillis = millis();
    } else if (segments > DisplayedSegments) {
      DisplayedSegments = segments;
    }

    if (DisplayedSegments > peakholdold) {
      peakholdold = DisplayedSegments;
      peakholdmillis = millis();
    }

    if (millis() - peakholdmillis >= 1000) {
      if (millis() - ModulationpeakPreviousMillis >= 20) {
        peakholdold = max(peakholdold - 3, DisplayedSegments);
        ModulationpeakPreviousMillis = millis();
      }
    }

    tft.fillRect(16, 133, 2 * constrain(DisplayedSegments, 0, 63), 6, ModBarInsignificantColor);

    if (DisplayedSegments > 63) tft.fillRect(16 + 2 * 63, 133, 2 * (DisplayedSegments - 63), 6, ModBarSignificantColor);

    int greyStart = 16 + 2 * DisplayedSegments;
    int greyWidth = 2 * (94 - DisplayedSegments);
    tft.fillRect(greyStart, 133, greyWidth, 6, GreyoutColor);

    int peakHoldPosition = 16 + 2 * constrain(peakholdold, 0, 93);
    tft.fillRect(peakHoldPosition, 133, 2, 6, (MStatus > 80) ? ModBarSignificantColor : PrimaryColor);

    if (millis() - peakholdmillis >= 1000 && peakholdold <= DisplayedSegments) tft.fillRect(peakHoldPosition, 133, 2, 6, GreyoutColor);
  }
}

void showAutoSquelch(bool mode) {
  if (language == LANGUAGE_CHS) SquelchSprite.loadFont(FONT16_CHS); else SquelchSprite.loadFont(FONT16);
  if (mode) {
    SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
  } else {
    SquelchSprite.setTextColor(BackgroundColor, BackgroundColor, false);
  }
  SquelchSprite.fillSprite(BackgroundColor);
  SquelchSprite.drawString(String(myLanguage[language][86]), 0, 0);
  SquelchSprite.pushSprite(235, 145);
}

void doSquelch() {
  if (!XDRGTKUSB && !XDRGTKTCP && usesquelch && !autosquelch) Squelch = map(analogRead(PIN_POT), 0, 4095, -100, 920);
  if (Squelch < - 800) Squelch = -100;
  if (Squelch > 900) Squelch = 920;

  if (unit == 0) SquelchShow = Squelch / 10;
  if (unit == 1) SquelchShow = ((Squelch * 100) + 10875) / 1000;
  if (unit == 2) SquelchShow = round((float(Squelch) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0) / 10;
  if (Squelch > 920) Squelch = 920;

  if (autosquelch) {
    if (band < BAND_GAP) {
      if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 100 && OStatus > -100) && (!scandxmode || (scandxmode && !scanmute))) {
        if (SQ || BWreset) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
          }
          autosquelchtimer = millis();
          SQ = false;
        }
      } else {
        if ((!SQ || BWreset) && (millis() >= autosquelchtimer + 1000)) {
          radio.setMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
          }
          autosquelchtimer = millis();
          SQ = true;
        }
      }
    } else {
      if ((USN < amscansens * 30) && (OStatus < 2 && OStatus > -2) && (!scandxmode || (scandxmode && !scanmute))) {
        if (!seek) radio.setUnMute();
        if (!screenmute && !seek) {
          tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
        }
        SQ = false;
      } else {
        radio.setMute();
        if (!screenmute && !seek) {
          tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
        }
        SQ = true;
      }
    }
  } else {
    if (language == LANGUAGE_CHS) SquelchSprite.loadFont(FONT16_CHS); else SquelchSprite.loadFont(FONT16);

    if (!XDRGTKUSB && !XDRGTKTCP && usesquelch && (!scandxmode || (scandxmode && !scanmute))) {
      if (!screenmute && usesquelch && !advancedRDS && !afscreen) {
        if (!BWtune && !menu && (Squelch > Squelchold + 2 || Squelch < Squelchold - 2)) {
          SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
          SquelchSprite.fillSprite(BackgroundColor);
          if (Squelch == -100) {
            SquelchSprite.drawString(String(myLanguage[language][33]), 0, 0);
          } else if (Squelch == 920) {
            SquelchSprite.drawString("ST", 0, 0);
          } else {
            SquelchSprite.drawString(String(SquelchShow), 0, 0);
          }
          if (Squelch != Squelchold) {
            SquelchSprite.pushSprite(235, 145);
          }
        }
        Squelchold = Squelch;
      }
    }

    if ((XDRGTKUSB || XDRGTKTCP) && (!scandxmode || (scandxmode && !scanmute))) {
      if (!XDRMute) {
        if (Squelch != -1) {
          if (Squelch < SStatus || Squelch == -100 || Squelch == 0) {
            if (!seek) radio.setUnMute();
            if (!screenmute && !seek) {
              tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
            }
            SQ = false;
          } else {
            radio.setMute();
            if (!screenmute && !seek) {
              tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
            }
            SQ = true;
          }
        } else {
          if (Stereostatus) {
            radio.setUnMute();
            if (!screenmute && !seek) {
              tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
            }
            SQ = false;
          } else {
            radio.setMute();
            if (!screenmute && !seek) {
              tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
            }
            SQ = true;
          }
        }
        if (!screenmute && usesquelch && !advancedRDS && !afscreen) {
          if (Squelch != Squelchold) {
            SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
            SquelchSprite.fillSprite(BackgroundColor);

            if (Squelch == -1) {
              SquelchSprite.drawString("ST", 0, 0);
            } else if (Squelch == 0) {
              SquelchSprite.drawString(String(myLanguage[language][33]), 0, 0);
            } else {
              SquelchSprite.drawString(String(SquelchShow), 0, 0);
            }
            if (Squelch != Squelchold) {
              SquelchSprite.pushSprite(235, 145);
            }
            Squelchold = Squelch;
          }
        }
      }
    } else if (usesquelch && (!scandxmode || (scandxmode && !scanmute))) {
      if (Squelch != 920) {
        if (Squelch < SStatus || Squelch == -100 || Squelch == 0) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
          }
          SQ = false;
        } else {
          radio.setMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
          }
          SQ = true;
        }
      } else {
        if (Stereostatus) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
          }
          SQ = false;
        } else {
          radio.setMute();
          if (!screenmute && !seek) {
            tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
          }
          SQ = true;
        }
      }
    }
  }
  SquelchSprite.unloadFont();
}

void updateBW() {//todo air
  if (BWset == 0) {
    if (!BWtune && !screenmute && !advancedRDS && !afscreen) {
      tft.fillRoundRect(248, 36, 69, 18, 2, SecondaryColor);
      tftPrint(0, "AUTO BW", 282, 38, BackgroundColor, SecondaryColor, 16);
    }
    radio.setFMABandw();
  } else {
    if (!BWtune && !screenmute && !advancedRDS && !afscreen) {
      tft.fillRoundRect(248, 36, 69, 18, 2, GreyoutColor);
      tftPrint(0, "AUTO BW", 282, 38, BackgroundColor, GreyoutColor, 16);
    }
  }
}

void updateiMS() {
  if (band < BAND_GAP) {
    if (iMSset == 0) {
      if (!screenmute && !advancedRDS && !afscreen && !BWtune) {
        tft.fillRoundRect(249, 57, 30, 18, 2, SecondaryColor);
        tftPrint(0, "iMS", 265, 59, BackgroundColor, SecondaryColor, 16);
      }
      radio.setiMS(1);
    } else {
      if (!screenmute && !advancedRDS && !afscreen && !BWtune) {
        tft.fillRoundRect(249, 57, 30, 18, 2, GreyoutColor);
        tftPrint(0, "iMS", 265, 59, BackgroundColor, GreyoutColor, 16);
      }
      radio.setiMS(0);
    }
  }
}

void updateEQ() {
  if (band < BAND_GAP) {
    if (EQset == 0) {
      if (!screenmute && !advancedRDS && !afscreen && !BWtune) {
        tft.fillRoundRect(287, 57, 30, 18, 2, SecondaryColor);
        tftPrint(0, "EQ", 301, 59, BackgroundColor, SecondaryColor, 16);
      }
      radio.setEQ(1);
    } else {
      if (!screenmute && !advancedRDS && !afscreen && !BWtune) {
        tft.fillRoundRect(287, 57, 30, 18, 2, GreyoutColor);
        tftPrint(0, "EQ", 301, 59, BackgroundColor, GreyoutColor, 16);
      }
      radio.setEQ(0);
    }
  }
}

void updateCodetect() {
  if (
    band > BAND_GAP && band != BAND_AIR) {
    if (WAM) {
      tftPrint(-1, "CO", 50, 61, PrimaryColor, PrimaryColorSmooth, 16);
    } else {
      tftPrint(-1, "CO", 50, 61, BackgroundColor, BackgroundColor, 16);
    }
  }
}

void updateSWMIBand() {
  switch (SWMIBandPos) {
    case SW_MI_BAND_11M:
    case SW_MI_BAND_13M:
    case SW_MI_BAND_15M:
    case SW_MI_BAND_16M:
    case SW_MI_BAND_19M:
    case SW_MI_BAND_22M:
    case SW_MI_BAND_25M:
    case SW_MI_BAND_31M:
    case SW_MI_BAND_41M:
    case SW_MI_BAND_49M:
    case SW_MI_BAND_60M:
    case SW_MI_BAND_75M:
    case SW_MI_BAND_90M:
    case SW_MI_BAND_120M:
    case SW_MI_BAND_160M:
      FrequencySprite.drawString(SWMIBandstring + " ", 0, 0);
      beepresetstart = true;
      if (edgebeep && beepresetstop) {
        EdgeBeeper();
        beepresetstop = false;
      }
      break;

    case SW_MI_BAND_GAP:
      beepresetstop = true;
      if (edgebeep && beepresetstart) {
        EdgeBeeper();
        beepresetstart = false;
      }
      break;
  }
}

void doBW() {
  if (BWtune && !bwtouchtune) BWset = BWsettemp;

  if (band < BAND_GAP) {
    if (BWset > 16) BWset = 0;

    switch (BWset) {
      case 1: radio.setFMBandw(56); break;
      case 2: radio.setFMBandw(64); break;
      case 3: radio.setFMBandw(72); break;
      case 4: radio.setFMBandw(84); break;
      case 5: radio.setFMBandw(97); break;
      case 6: radio.setFMBandw(114); break;
      case 7: radio.setFMBandw(133); break;
      case 8: radio.setFMBandw(151); break;
      case 9: radio.setFMBandw(168); break;
      case 10: radio.setFMBandw(184); break;
      case 11: radio.setFMBandw(200); break;
      case 12: radio.setFMBandw(217); break;
      case 13: radio.setFMBandw(236); break;
      case 14: radio.setFMBandw(254); break;
      case 15: radio.setFMBandw(287); break;
      case 16: radio.setFMBandw(311); break;
    }
    BWsetFM = BWset;
    EEPROM.writeByte(EE_BYTE_BWSET_FM, BWsetFM);

    if ((XDRGTKUSB || XDRGTKTCP) && !XDRScan) {
      switch (BWset) {
        case 0: DataPrint("W0\n"); break;
        case 1: DataPrint("W56000\n"); break;
        case 2: DataPrint("W64000\n"); break;
        case 3: DataPrint("W72000\n"); break;
        case 4: DataPrint("W84000\n"); break;
        case 5: DataPrint("W97000\n"); break;
        case 6: DataPrint("W114000\n"); break;
        case 7: DataPrint("W133000\n"); break;
        case 8: DataPrint("W151000\n"); break;
        case 9: DataPrint("W168000\n"); break;
        case 10: DataPrint("W184000\n"); break;
        case 11: DataPrint("W200000\n"); break;
        case 12: DataPrint("W217000\n"); break;
        case 13: DataPrint("W236000\n"); break;
        case 14: DataPrint("W254000\n"); break;
        case 15: DataPrint("W287000\n"); break;
        case 16: DataPrint("W311000\n"); break;
      }
    }
  } else {
    if (BWset > 4) BWset = 1;

    switch (BWset) {
      case 1: radio.setAMBandw(3); break;
      case 2: radio.setAMBandw(4); break;
      case 3: radio.setAMBandw(6); break;
      case 4: radio.setAMBandw(8); break;
    }
    BWsetAM = BWset;
    EEPROM.writeByte(EE_BYTE_BWSET_AM, BWsetAM);
  }
  updateBW();
  BWreset = true;
  EEPROM.commit();
}

void doBWtuneDown() {
  rotary = 0;
  if (band < BAND_GAP) {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), false);
    BWsettemp--;
    if (BWsettemp > 20 || BWsettemp == 0) BWsettemp = 20;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
  } else {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), false);
    BWsettemp--;
    if (BWsettemp > 4 && BWsettemp < 20) BWsettemp = 4; else if (BWsettemp == 0) BWsettemp = 20;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), true);
  }
}

void doBWtuneUp() {
  rotary = 0;
  if (band < BAND_GAP) {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), false);
    BWsettemp++;
    if (BWsettemp > 20) BWsettemp = 1;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
  } else {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), false);
    BWsettemp++;
    if (BWsettemp > 4 && BWsettemp < 20) BWsettemp = 20; else if (BWsettemp > 20) BWsettemp = 1;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), true);
  }
}

void doTuneMode() {
  switch (tunemode) {
    case TUNE_MAN:
      if (band == BAND_SW) {
        if (showSWMIBand && nowToggleSWMIBand) {
          tunemode = TUNE_MI_BAND;
        } else {
          tunemode = TUNE_AUTO;
        }
      } else {
        tunemode = TUNE_AUTO;
      }
      break;

    case TUNE_MI_BAND:
    case TUNE_AUTO:
      tunemode = TUNE_MEM;
      break;

    case TUNE_MEM:
      if (!bandforbidden) tunemode = TUNE_MAN;
      break;
  }
  ShowTuneMode();
  ShowMemoryPos();
  EEPROM.writeByte(EE_BYTE_TUNEMODE, tunemode);
  EEPROM.commit();
}

void ShowTuneMode() {
  if (!screenmute) {
    switch (tunemode) {
      case TUNE_MAN:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint(0, "AUTO", 22, 60, GreyoutColor, GreyoutColor, 16);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(0, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);
        } else {
          tftPrint(0, "BAND", 22, 60, GreyoutColor, GreyoutColor, 16);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(0, "AUTO", 22, 60, BackgroundColor, GreyoutColor, 16);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, SecondaryColor);
        tftPrint(0, "MAN", 22, 38, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;

      case TUNE_AUTO:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint(0, "AUTO", 22, 60, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;

      case TUNE_MEM:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint(0, "AUTO", 22, 60, GreyoutColor, GreyoutColor, 16);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(0, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);
        } else {
          tftPrint(0, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(0, "AUTO", 22, 60, BackgroundColor, GreyoutColor, 16);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        if (memorystore) {
          tft.fillRoundRect(2, 80, 40, 18, 2, SignificantColor);
          tftPrint(0, "MEM", 22, 82, BackgroundColor, SignificantColor, 16);
        } else {
          tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
          tftPrint(0, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
        }
        break;

      case TUNE_MI_BAND:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint(0, "BAND", 22, 60, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(0, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;
    }
  }
}

void ShowRSSI() {
  if (wifi) rssi = WiFi.RSSI(); else rssi = 0;
  if (rssiold != rssi) {
    rssiold = rssi;
    if (rssi == 0) {
      tft.drawBitmap(250, 4, WiFi4, 25, 25, GreyoutColor);
    } else if (rssi > -50 && rssi < 0) {
      tft.drawBitmap(250, 4, WiFi4, 25, 25, PrimaryColor);
    } else if (rssi > -60) {
      tft.drawBitmap(250, 4, WiFi4, 25, 25, GreyoutColor);
      tft.drawBitmap(250, 4, WiFi3, 25, 25, PrimaryColor);
    } else if (rssi > -70) {
      tft.drawBitmap(250, 4, WiFi4, 25, 25, GreyoutColor);
      tft.drawBitmap(250, 4, WiFi2, 25, 25, PrimaryColor);
    } else if (rssi < -70) {
      tft.drawBitmap(250, 4, WiFi4, 25, 25, GreyoutColor);
      tft.drawBitmap(250, 4, WiFi1, 25, 25, PrimaryColor);
    }
  }
}

void ShowBattery() {
  if (millis() >= batupdatetimer + TIMER_BAT_TIMER) {
    batupdatetimer = millis();
  } else {
    return;
  }

  uint16_t v = 0;
  if (!wifi) v = analogRead(BATTERY_PIN);
  battery = map(constrain(v, BAT_LEVEL_EMPTY, BAT_LEVEL_FULL), BAT_LEVEL_EMPTY, BAT_LEVEL_FULL, 0, BAT_LEVEL_STAGE);
  byte batteryprobe = map(constrain(v, BAT_LEVEL_EMPTY, BAT_LEVEL_FULL), BAT_LEVEL_EMPTY, BAT_LEVEL_FULL, 0, 20);

  if (batteryold != batteryprobe) {
    if (!wifi && batterydetect) {
      if (battery == 0) {
        tft.drawRoundRect(277, 6, 37, 20, 2, BarSignificantColor);
        tft.fillRoundRect(313, 13, 4, 6, 2, BarSignificantColor);
      } else {
        tft.drawRoundRect(277, 6, 37, 20, 2, ActiveColor);
        tft.fillRoundRect(313, 13, 4, 6, 2, ActiveColor);
      }
      if (batteryoptions != BATTERY_VALUE && batteryoptions != BATTERY_PERCENT && battery != 0) {
        tft.fillRoundRect(279, 8, (battery * 8) , 16, 2, BarInsignificantColor);
      } else {
        tft.fillRoundRect(279, 8, 33, 16, 2, BackgroundColor);
      }
    } else {
      tft.drawRoundRect(277, 6, 37, 20, 2, GreyoutColor);
      tft.fillRoundRect(313, 13, 4, 6, 2, GreyoutColor);
      tft.fillRoundRect(279, 8, 33, 16, 2, BackgroundColor);
    }
    batteryold = batteryprobe;
    batteryVold = 0;
    vPerold = 0;


    if (!wifi && batterydetect) {
      float batteryV = constrain((((float)v / 4095.0) * 3.3 * (1100 / 1000.0) * 2.0), 0.0, 5.0);
      float vPer = constrain((batteryV - BATTERY_LOW_VALUE) / (BATTERY_FULL_VALUE - BATTERY_LOW_VALUE), 0.0, 0.99) * 100;

      if (abs(batteryV - batteryVold) > 0.05 && batteryoptions == BATTERY_VALUE) {
        tftPrint(-1, String(batteryV, 2) + "V", 279, 9, BatteryValueColor, BatteryValueColorSmooth, 16);
        batteryVold = batteryV;
      } else if (int(vPer) != int(vPerold) && batteryoptions == BATTERY_PERCENT && abs(vPer - vPerold) > 0.5) {
        tftPrint(-1, String(vPer, 0) + "%", 279, 9, BatteryValueColor, BatteryValueColorSmooth, 16);
        vPerold = vPer;
      }
    }
  }
}

void DataPrint(String string) {
  if (XDRGTKUSB) Serial.print(string);
  if (XDRGTKTCP) RemoteClient.print(string);
}

void TuneUp() {
  aftest = true;
  aftimer = millis();
  unsigned int temp = 0;
  if (stepsize == 0) {
    if (band > BAND_GAP) {
      if (frequency_AM <= MWHighEdgeSet && frequency_AM >= MWLowEdgeSet) {
        if (!mwstepsize) {
          temp = FREQ_MW_STEP_9K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
        } else if (mwstepsize) {
          temp = FREQ_MW_STEP_10K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_10K) * FREQ_MW_STEP_10K;
        }
      } else if (frequency_AM < MWLowEdgeSet) {
        temp = FREQ_MW_STEP_9K;
        frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
      } else if (frequency_AM <= SWHighEdgeSet && frequency_AM >= SWLowEdgeSet) {
        temp = FREQ_SW_STEP_5K;
        frequency_AM = (frequency_AM / FREQ_SW_STEP_5K) * FREQ_SW_STEP_5K;
      }
#ifdef HAS_AIR_BAND
      else if (frequency_AM < AIRHighEdgeSet && frequency_AM > AIRLowEdgeSet) {

        if (airstepsize == 0) temp = FREQ_AIR_STEP_25K;
        else temp = FREQ_AIR_STEP_8K33;
      }
#endif
    } else {
      if (band == BAND_OIRT) {
        temp = FREQ_OIRT_STEP_30K;
      } else {
        switch (fmdefaultstepsize) {
          case 0: temp = FREQ_FM_STEP_50K; break;
          case 1: temp = FREQ_FM_STEP_100K; break;
          case 2: temp = FREQ_FM_STEP_200K; break;
        }
      }
    }
  }
  if (stepsize == 1) temp = 1;
  if (stepsize == 2) temp = 10;
  if (stepsize == 3) temp = 100;
  if (stepsize == 4) temp = 1000;
  if (rotaryaccelerate && rotarycounter > 2) temp *= 2;

  if (band == BAND_FM) {
    frequency += temp;
    if (scandxmode) {
      while (IsFrequencyUsed(frequency) || presets[memorypos].band != BAND_FM) {
        frequency += temp;
      }
    }

    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8795) frequency = 8790;
    if (frequency >= (HighEdgeSet * 10) + 1) {
      if (scandxmode && RabbitearsUser.length() && RabbitearsPassword.length()) {
        byte i = 0;
        bool hasreport = false;
        for (i = 0; i < 100; i++) {
          if (rabbitearspi[i]) {
            hasreport = true;
            break;
          }
        }
        if (hasreport) {
          rabbitearssend();
          for (i = 0; i < 100; i++) {
            rabbitearspi[i] = 0;
          }
        }
      }
      frequency = LowEdgeSet * 10;
      if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8750) frequency = 8775;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    frequency_OIRT += temp;
    if (scandxmode) {
      while (IsFrequencyUsed(frequency_OIRT) || presets[memorypos].band != BAND_OIRT) {
        frequency_OIRT += temp;
      }
    }
    if (frequency_OIRT > HighEdgeOIRTSet) {
      frequency_OIRT = LowEdgeOIRTSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    frequency_AM += temp;
    if (frequency_AM > LWHighEdgeSet) {
      frequency_AM = LWLowEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_LW = frequency_AM;
  } else if (band == BAND_MW) {
    frequency_AM += temp;
    if (frequency_AM > MWHighEdgeSet) {
      frequency_AM = MWLowEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_MW = frequency_AM;
  } else if (band == BAND_SW) {
    if (rotaryaccelerate && rotarycounter > 2) temp *= 2;
    frequency_AM += temp;
    if (frequency_AM > SWHighEdgeSet) {
      frequency_AM = SWLowEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_SW = frequency_AM;
  }
#ifdef HAS_AIR_BAND
  else if (band == BAND_AIR) {
    frequency_AM += temp;
    if (frequency_AIR > AIRHighEdgeSet) {
      frequency_AM = AIRLowEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_AIR = frequency_AM;
  }
#endif
  radio.clearRDS(fullsearchrds);
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void TuneDown() {
  aftest = true;
  aftimer = millis();
  unsigned int temp = 0;
  if (stepsize == 0) {
    if (band > BAND_GAP) {
      if (frequency_AM < MWHighEdgeSet && frequency_AM > MWLowEdgeSet) {
        if (!mwstepsize) {
          temp = FREQ_MW_STEP_9K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
        } else if (mwstepsize) {
          temp = FREQ_MW_STEP_10K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_10K) * FREQ_MW_STEP_10K;
        }
      } else if (frequency_AM < MWLowEdgeSet) {
        temp = FREQ_MW_STEP_9K;
        frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
      } else {
        temp = FREQ_SW_STEP_5K;
        frequency_AM = (frequency_AM / FREQ_SW_STEP_5K) * FREQ_SW_STEP_5K;
      }
    } else {
      if (band == BAND_OIRT) {
        temp = FREQ_OIRT_STEP_30K;
      } else {
        switch (fmdefaultstepsize) {
          case 0: temp = FREQ_FM_STEP_50K; break;
          case 1: temp = FREQ_FM_STEP_100K; break;
          case 2: temp = FREQ_FM_STEP_200K; break;
        }
      }
    }
  }
  if (stepsize == 1) temp = 1;
  if (stepsize == 2) temp = 10;
  if (stepsize == 3) temp = 100;
  if (stepsize == 4) temp = 1000;
  if (rotaryaccelerate && rotarycounter > 2) temp *= 2;

  if (band == BAND_FM) {
    frequency -= temp;
    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8770) frequency = 8775;
    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8755) frequency = 10790;

    if (frequency < LowEdgeSet * 10) {
      frequency = HighEdgeSet * 10;
      if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 10800) frequency = 10790;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    frequency_OIRT -= temp;
    if (frequency_OIRT < LowEdgeOIRTSet) {
      frequency_OIRT = HighEdgeOIRTSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    frequency_AM -= temp;
    if (frequency_AM < LWLowEdgeSet) {
      frequency_AM = LWHighEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_LW = frequency_AM;
  } else if (band == BAND_MW) {
    frequency_AM -= temp;
    if (frequency_AM < MWLowEdgeSet) {
      frequency_AM = MWHighEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_MW = frequency_AM;
  } else if (band == BAND_SW) {
    if (rotaryaccelerate && rotarycounter > 2) temp *= 2;
    frequency_AM -= temp;
    if (frequency_AM < SWLowEdgeSet) {
      frequency_AM = SWHighEdgeSet;
      if (edgebeep) EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_SW = frequency_AM;
  }
  radio.clearRDS(fullsearchrds);
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void EdgeBeeper() {
  radio.tone(50, -5, 2000);
  if (radio.mute) {
    radio.setMute();
    if (!screenmute) {
      tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
    }
  } else {
    radio.setUnMute();
    if (!screenmute) {
      tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
    }
  }
}

void Seek(bool mode) {
  radio.setMute();
  if (seekinit) {
    if (stepsize != 0) {
      stepsize = 0;
      RoundStep();
      ShowStepSize();
    }
    seekinit = false;
  }

  if (!screenmute) {
    tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
  }
  if (!mode) TuneDown(); else TuneUp();
  delay(50);

  ShowFreq(0);
  if (XDRGTKUSB || XDRGTKTCP) {
    if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
  }

  if (band < BAND_GAP) {
    radio.getStatus(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
    if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80 && OStatus > -80) && (Squelch < SStatus || Squelch == 920)) {
      seek = false;
      radio.setUnMute();
      if (!screenmute) {
        tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
      }
      store = true;
    } else {
      seek = true;
      if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
      if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
    }
  } else {
    radio.getStatusAM(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
    if ((USN < amscansens * 30) && (OStatus < 2 && OStatus > -2) && (Squelch < SStatus || Squelch == 920)) {
      seek = false;
      radio.setUnMute();
      if (!screenmute) {
        tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);
      }
      store = true;
    } else {
      seek = true;
    }
  }
}

void SetTunerPatch() {
  if (TEF != 102 && TEF != 205) {
    radio.init(102);
    uint16_t device;
    uint16_t hw;
    uint16_t sw;
    radio.getIdentification(device, hw, sw);
    TEF = highByte(hw) * 100 + highByte(sw);
    tft.fillScreen(BackgroundColor);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));

    if (TEF != 102 && TEF != 205) {
      tftPrint(0, myLanguage[language][35], 150, 78, ActiveColor, ActiveColorSmooth, 28);
      for (;;);
    }
    EEPROM.writeByte(EE_BYTE_TEF, TEF);
    EEPROM.commit();
    Tuner_Reset();
    ESP.restart();
  }
}

void read_encoder() {
  if (!digitalRead(ROTARY_PIN_A) || !digitalRead(ROTARY_PIN_B)) {
    if (millis() - rotarytimer >= 15) {
      rotarycounteraccelerator = 2;  // Steady fast
      rotarycounter = 0;
    }
    if (millis() - rotarytimer >= 30) {
      rotarycounteraccelerator = 4;
      rotarycounter = 0;
    }
    if (millis() - rotarytimer >= 45) {
      rotarycounteraccelerator = 6;  // Quick flicks
      rotarycounter = 0;
    }
  }

  static uint8_t old_AB = 3;
  static int8_t encval = 0;
  static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

  old_AB <<= 2;

  if (digitalRead(ROTARY_PIN_A)) old_AB |= 0x02;
  if (digitalRead(ROTARY_PIN_B)) old_AB |= 0x01;
  encval += enc_states[( old_AB & 0x0f )];
  if (!(255 - old_AB)) encval = 0; // Unstick -2 or 2

  if (optenc) {
    if (encval > 4) {
      if (rotarymode) rotary = -1; else rotary = 1;
      encval = 0;
    } else if (encval < -4) {
      if (rotarymode) rotary = 1; else rotary = -1;
      encval = 0;
    }
  } else {
    if (encval > 3) {
      if (rotarymode) rotary = -1; else rotary = 1;
      rotarycounter++;
      rotarytimer = millis();
      encval = 0;
    } else if (encval < -3) {
      if (rotarymode) rotary = 1; else rotary = -1;
      rotarycounter++;
      rotarytimer = millis();
      encval = 0;
    }
  }
}

void MuteScreen(bool setting) {
  if (!setting && screenmute) {
    screenmute = false;
    setupmode = true;
    leave = true;
    tft.writecommand(0x11);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    if (band < BAND_GAP) {
      if (afscreen) {
        BuildAFScreen();
        freq_in = 0;
      } else if (advancedRDS) {
        BuildAdvancedRDS();
        freq_in = 0;
      } else {
        BuildDisplay();
        freq_in = 0;
        SelectBand();
      }
    } else {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
    }
    setupmode = false;
  } else if (setting && !screenmute) {
    screenmute = true;
    analogWrite(CONTRASTPIN, 0);
    tft.writecommand(0x10);
  }
}

void DefaultSettings() {
  EEPROM.writeByte(EE_BYTE_CHECKBYTE, EE_CHECKBYTE_VALUE);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, 10000);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_OIRT, FREQ_FM_OIRT_START);
  EEPROM.writeByte(EE_BYTE_VOLSET, 0);
  EEPROM.writeUInt(EE_UINT16_CONVERTERSET, 0);
  EEPROM.writeUInt(EE_UINT16_FMLOWEDGESET, 875);
  EEPROM.writeUInt(EE_UINT16_FMHIGHEDGESET, 1080);
  EEPROM.writeByte(EE_BYTE_CONTRASTSET, 50);
  EEPROM.writeByte(EE_BYTE_STEREOLEVEL, 0);
  EEPROM.writeByte(EE_BYTE_BANDFM, FM_BAND_ALL);
  EEPROM.writeByte(EE_BYTE_BANDAM, AM_BAND_ALL);
  EEPROM.writeByte(EE_BYTE_HIGHCUTLEVEL, 70);
  EEPROM.writeByte(EE_BYTE_HIGHCUTOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_LEVELOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_RTBUFFER, 1);
  EEPROM.writeByte(EE_BYTE_EDGEBEEP, 0);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEAM, 0);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEFM, 0);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AM, 828);
  EEPROM.writeByte(EE_BYTE_LANGUAGE, 0);
  EEPROM.writeByte(EE_BYTE_SHOWRDSERRORS, 1);
  EEPROM.writeByte(EE_BYTE_TEF, 0);
  EEPROM.writeByte(EE_BYTE_STEPSIZE, 0);
  EEPROM.writeByte(EE_BYTE_TUNEMODE, 0);
  EEPROM.writeByte(EE_BYTE_OPTENC, 0);
  EEPROM.writeByte(EE_BYTE_IMSSET, 1);
  EEPROM.writeByte(EE_BYTE_EQSET, 1);
  EEPROM.writeByte(EE_BYTE_BAND, BAND_FM);
  EEPROM.writeByte(EE_BYTE_LOWLEVELSET, -15);
  EEPROM.writeByte(EE_BYTE_MEMORYPOS, 0);
  EEPROM.writeByte(EE_BYTE_REGION, 0);
  EEPROM.writeByte(EE_BYTE_RDS_UNDERSCORE, 0);
  EEPROM.writeByte(EE_BYTE_USBMODE, 0);
  EEPROM.writeByte(EE_BYTE_WIFI, 0);
  EEPROM.writeByte(EE_BYTE_SUBNETCLIENT, 1);
  EEPROM.writeByte(EE_BYTE_SHOWSWMIBAND, 1);
  EEPROM.writeByte(EE_BYTE_RDS_FILTER, 1);
  EEPROM.writeByte(EE_BYTE_RDS_PIERRORS, 0);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_LW, 180);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_MW, 540);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_SW, 1800);
#ifdef HAS_AIR_BAND
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AIR, 135350);
#endif
  EEPROM.writeString(EE_STRING_XDRGTK_KEY, "password");
  EEPROM.writeString(EE_STRING_RABBITEARSUSER, "");
  EEPROM.writeString(EE_STRING_RABBITEARSPASSWORD, "");
  EEPROM.writeByte(EE_BYTE_USESQUELCH, 1);
  EEPROM.writeByte(EE_BYTE_SHOWMODULATION, 1);
  EEPROM.writeByte(EE_BYTE_AM_NB, 0);
  EEPROM.writeByte(EE_BYTE_FM_NB, 0);
  EEPROM.writeByte(EE_BYTE_AUDIOMODE, 0);
  EEPROM.writeByte(EE_BYTE_TOUCH_ROTATING, 0);
  EEPROM.writeUInt(EE_UINT16_LOWEDGEOIRTSET, 0);
  EEPROM.writeUInt(EE_UINT16_HIGHEDGEOIRTSET, 0);
  EEPROM.writeByte(EE_BYTE_POWEROPTIONS, 1);
  EEPROM.writeByte(EE_BYTE_CURRENTTHEME, 0);
  EEPROM.writeByte(EE_BYTE_FMDEFAULTSTEPSIZE, 1);
  EEPROM.writeByte(EE_BYTE_SCREENSAVERSET, 0);
  EEPROM.writeInt(EE_INT16_AMLEVELOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_UNIT, 0);
  EEPROM.writeByte(EE_BYTE_AF, 0);
  EEPROM.writeByte(EE_BYTE_STEREO, 1);
  EEPROM.writeByte(EE_BYTE_BATTERY_OPTIONS, BATTERY_VALUE);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT, 100);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT_COUNT, 3);
  EEPROM.writeByte(EE_BYTE_AM_RF_GAIN, 0);
  EEPROM.writeByte(EE_BYTE_SORTAF, 1);
  EEPROM.writeByte(EE_BYTE_STATIONLISTID, 1);
  EEPROM.writeByte(EE_BYTE_FM_DEEMPHASIS, DEEMPHASIS_50);
  EEPROM.writeByte(EE_BYTE_BWSET_FM, 0);
  EEPROM.writeByte(EE_BYTE_BWSET_AM, 2);
  EEPROM.writeByte(EE_BYTE_BANDAUTOSW, 0);
  EEPROM.writeByte(EE_BYTE_FASTPS, 1);
  EEPROM.writeByte(EE_BYTE_TOT, 0);
  EEPROM.writeByte(EE_BYTE_MWREGION, 0);
#ifdef HAS_AIR_BAND
  EEPROM.writeByte(EE_BYTE_AIRSTEPSIZE, 0);
#endif
  EEPROM.writeByte(EE_BYTE_SPISPEED, 0);
  EEPROM.writeByte(EE_BYTE_AMSCANSENS, 4);
  EEPROM.writeByte(EE_BYTE_FMSCANSENS, 4);
  EEPROM.writeByte(EE_BYTE_FREQFONT, 3);
  EEPROM.writeByte(EE_BYTE_SKIN, 0);
  EEPROM.writeByte(EE_BYTE_XDRGTKMUTE, 0);
  EEPROM.writeByte(EE_BYTE_FMAGC, 92);
  EEPROM.writeByte(EE_BYTE_AMAGC, 100);
  EEPROM.writeByte(EE_BYTE_FMSI, 1);
  EEPROM.writeByte(EE_BYTE_SCANSTART, 0);
  EEPROM.writeByte(EE_BYTE_SCANSTOP, 9);
  EEPROM.writeByte(EE_BYTE_SCANHOLD, 5);
  EEPROM.writeByte(EE_BYTE_SCANMEM, 1);
  EEPROM.writeByte(EE_BYTE_SCANCANCEL, 0);
  EEPROM.writeByte(EE_BYTE_SCANMUTE, 0);
  EEPROM.writeByte(EE_BYTE_AUTOSQUELCH, 0);
  EEPROM.writeByte(EE_BYTE_LONGBANDPRESS, 0);
  EEPROM.writeByte(EE_BYTE_SHOWCLOCK, 1);
  EEPROM.writeByte(EE_BYTE_SHOWLONGPS, 1);
  EEPROM.writeUInt(EE_UINT16_MEMSTARTFREQ, 875);
  EEPROM.writeUInt(EE_UINT16_MEMSTOPFREQ, 1080);
  EEPROM.writeByte(EE_BYTE_MEMSTARTPOS, 1);
  EEPROM.writeByte(EE_BYTE_MEMSTOPPOS, 10);
  EEPROM.writeByte(EE_BYTE_MEMPIONLY, 1);
  EEPROM.writeByte(EE_BYTE_MEMDOUBLEPI, 0);
  EEPROM.writeByte(EE_BYTE_WAITONLYONSIGNAL, 1);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH1, 300);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH2, 3450);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH3, 300);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH4, 3450);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH5, 3);

#ifdef DEEPELEC_DP_66X
  EEPROM.writeByte(EE_BYTE_ROTARYMODE, 1);
  EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, 0);
  EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, 1);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, PORTABLE_TOUCH_ILI9341);
#else
  EEPROM.writeByte(EE_BYTE_ROTARYMODE, 0);
  EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, 1);
  EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, 0);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, BASE_ILI9341);
#endif

  for (int i = 0; i < EE_PRESETS_CNT; i++) {
    EEPROM.writeByte(i + EE_PRESETS_BAND_START, BAND_FM);
    EEPROM.writeUInt((i * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
    EEPROM.writeByte(i + EE_PRESET_BW_START, 0);
    EEPROM.writeByte(i + EE_PRESET_MS_START, 1);

    for (int y = 0; y < 9; y++) {
      EEPROM.writeByte((i * 9) + y + EE_PRESETS_RDSPS_START, '\0');
    }

    for (int y = 0; y < 5; y++) {
      EEPROM.writeByte((i * 5) + y + EE_PRESETS_RDSPI_START, '\0');
    }
  }

  EEPROM.commit();
}

void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize) {
  const uint8_t *selectedFont = nullptr;
  if (language == LANGUAGE_CHS) {
    if (fontsize == 16) selectedFont = FONT16_CHS;
    if (fontsize == 28) selectedFont = FONT28_CHS;
  } else {
    if (fontsize == 16) selectedFont = FONT16;
    if (fontsize == 28) selectedFont = FONT28;
  }
  if (fontsize == 48) selectedFont = FONT48;

  tft.loadFont(selectedFont);
  tft.setTextColor(background, background, false);

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  tft.drawString(textold, x, y);
  tft.setTextColor(color, smoothcolor, false);

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.drawString(modifiedText, x, y);
  tft.unloadFont();
}

void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize) {
  const uint8_t *selectedFont = nullptr;
  if (language == LANGUAGE_CHS) {
    if (fontsize == 16) selectedFont = FONT16_CHS;
    if (fontsize == 28) selectedFont = FONT28_CHS;
  } else {
    if (fontsize == 16) selectedFont = FONT16;
    if (fontsize == 28) selectedFont = FONT28;
  }

  if (fontsize == 48) selectedFont = FONT48;

  tft.loadFont(selectedFont);

  tft.setTextColor(color, smoothcolor, (fontsize == 52 ? true : false));

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.drawString(modifiedText, x, y, 1);
  tft.unloadFont();
}

void deepSleep() {
  analogWrite(SMETERPIN, 0);
  pinMode (STANDBYLED, OUTPUT);
  digitalWrite(STANDBYLED, LOW);
  MuteScreen(1);
  StoreFrequency();
  radio.power(1);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, LOW);
  esp_deep_sleep_start();
}

void UpdateFonts(byte mode) {
  switch (mode) {
    case 0:                                 // Use in radio mode
      RDSSprite.unloadFont();
      PSSprite.unloadFont();
      FullLineSprite.unloadFont();
      OneBigLineSprite.unloadFont();

      if (language == LANGUAGE_CHS) {
        RDSSprite.loadFont(FONT16_CHS);
        if (menu) PSSprite.loadFont(FONT16_CHS); else PSSprite.loadFont(FONT28_CHS);
        FullLineSprite.loadFont(FONT16_CHS);
        OneBigLineSprite.loadFont(FONT28_CHS);
      } else {
        RDSSprite.loadFont(FONT16);
        if (menu) PSSprite.loadFont(FONT16); else PSSprite.loadFont(FONT28);
        FullLineSprite.loadFont(FONT16);
        OneBigLineSprite.loadFont(FONT28);
      }
      break;

    case 1:                                 // Unload all
      FullLineSprite.unloadFont();
      OneBigLineSprite.unloadFont();
      RDSSprite.unloadFont();
      PSSprite.unloadFont();
      break;
  }
}

void cancelDXScan() {
  tunemode = scanmodeold;
  scandxmode = false;
  if (scanmute) {
    radio.setUnMute();
    tft.drawBitmap(92, 4, Speaker, 26, 22, GreyoutColor);

    if (!flashing) {
      tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
      tftPrint(0, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
    }

    SQ = false;
    Squelchold = -2;
  }

  ShowTuneMode();
  ShowMemoryPos();
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("J0\n");
}

void endMenu() {
  radio.clearRDS(fullsearchrds);
  menu = false;
  menuopen = false;
  LowLevelInit = true;
  submenu = false;
  menuoption = ITEM1;
  menupage = INDEX;
  menuitem = 0;
  EEPROM.writeByte(EE_BYTE_VOLSET, VolSet);
  EEPROM.writeUInt(EE_UINT16_CONVERTERSET, ConverterSet);
  EEPROM.writeUInt(EE_UINT16_FMLOWEDGESET, LowEdgeSet);
  EEPROM.writeUInt(EE_UINT16_FMHIGHEDGESET, HighEdgeSet);
  EEPROM.writeByte(EE_BYTE_CONTRASTSET, ContrastSet);
  EEPROM.writeByte(EE_BYTE_STEREOLEVEL, StereoLevel);
  EEPROM.writeByte(EE_BYTE_BANDFM, bandFM);
  EEPROM.writeByte(EE_BYTE_BANDAM, bandAM);
  EEPROM.writeByte(EE_BYTE_HIGHCUTLEVEL, HighCutLevel);
  EEPROM.writeByte(EE_BYTE_HIGHCUTOFFSET, HighCutOffset);
  EEPROM.writeByte(EE_BYTE_LEVELOFFSET, LevelOffset);
  EEPROM.writeByte(EE_BYTE_RTBUFFER, radio.rds.rtbuffer);
  EEPROM.writeByte(EE_BYTE_EDGEBEEP, edgebeep);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEAM, softmuteam);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEFM, softmutefm);
  EEPROM.writeByte(EE_BYTE_LANGUAGE, language);
  EEPROM.writeByte(EE_BYTE_SHOWRDSERRORS, showrdserrors);
  EEPROM.writeByte(EE_BYTE_LOWLEVELSET, LowLevelSet);
  EEPROM.writeByte(EE_BYTE_REGION, region);
  EEPROM.writeByte(EE_BYTE_RDS_UNDERSCORE, radio.underscore);
  EEPROM.writeByte(EE_BYTE_USBMODE, USBmode);
  EEPROM.writeByte(EE_BYTE_WIFI, wifi);
  EEPROM.writeByte(EE_BYTE_SUBNETCLIENT, subnetclient);
  EEPROM.writeByte(EE_BYTE_SHOWSWMIBAND, showSWMIBand);
  EEPROM.writeByte(EE_BYTE_RDS_FILTER, radio.rds.filter);
  EEPROM.writeByte(EE_BYTE_RDS_PIERRORS, radio.rds.pierrors);
  EEPROM.writeByte(EE_BYTE_USESQUELCH, usesquelch);
  EEPROM.writeByte(EE_BYTE_SHOWMODULATION, showmodulation);
  EEPROM.writeByte(EE_BYTE_AM_NB, amnb);
  EEPROM.writeByte(EE_BYTE_FM_NB, fmnb);
  EEPROM.writeByte(EE_BYTE_AUDIOMODE, audiomode);
  EEPROM.writeByte(EE_BYTE_TOUCH_ROTATING, touchrotating);
  EEPROM.writeUInt(EE_UINT16_LOWEDGEOIRTSET, LowEdgeOIRTSet);
  EEPROM.writeUInt(EE_UINT16_HIGHEDGEOIRTSET, HighEdgeOIRTSet);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, hardwaremodel);
  EEPROM.writeByte(EE_BYTE_POWEROPTIONS, poweroptions);
  EEPROM.writeByte(EE_BYTE_CURRENTTHEME, CurrentTheme);
  EEPROM.writeByte(EE_BYTE_FMDEFAULTSTEPSIZE, fmdefaultstepsize);
  EEPROM.writeByte(EE_BYTE_SCREENSAVERSET, screensaverset);
  EEPROM.writeInt(EE_INT16_AMLEVELOFFSET, AMLevelOffset);
  EEPROM.writeByte(EE_BYTE_UNIT, unit);
  EEPROM.writeByte(EE_BYTE_AF, af);
  EEPROM.writeByte(EE_BYTE_STEREO, StereoToggle);
  EEPROM.writeByte(EE_BYTE_BATTERY_OPTIONS, batteryoptions);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT, amcodect);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT_COUNT, amcodectcount);
  EEPROM.writeByte(EE_BYTE_AM_RF_GAIN, amgain);
  EEPROM.writeByte(EE_BYTE_SORTAF, radio.rds.sortaf);
  EEPROM.writeByte(EE_BYTE_STATIONLISTID, stationlistid);
  EEPROM.writeByte(EE_BYTE_FM_DEEMPHASIS, fmdeemphasis);
  EEPROM.writeByte(EE_BYTE_FASTPS, radio.rds.fastps);
  EEPROM.writeByte(EE_BYTE_TOT, tot);
  EEPROM.writeByte(EE_BYTE_MWREGION, mwstepsize);
#ifdef HAS_AIR_BAND
  EEPROM.writeByte(EE_BYTE_AIRSTEPSIZE, airstepsize);
#endif
  EEPROM.writeByte(EE_BYTE_SPISPEED, spispeed);
  EEPROM.writeByte(EE_BYTE_AMSCANSENS, amscansens);
  EEPROM.writeByte(EE_BYTE_FMSCANSENS, fmscansens);
  EEPROM.writeByte(EE_BYTE_FREQFONT, freqfont);
  EEPROM.writeByte(EE_BYTE_SKIN, CurrentSkin);
  EEPROM.writeByte(EE_BYTE_XDRGTKMUTE, XDRGTKMuteScreen);
  EEPROM.writeByte(EE_BYTE_FMAGC, fmagc);
  EEPROM.writeByte(EE_BYTE_AMAGC, amagc);
  EEPROM.writeByte(EE_BYTE_FMSI, fmsi);
  EEPROM.writeByte(EE_BYTE_SCANSTART, scanstart);
  EEPROM.writeByte(EE_BYTE_SCANSTOP, scanstop);
  EEPROM.writeByte(EE_BYTE_SCANHOLD, scanhold);
  EEPROM.writeByte(EE_BYTE_SCANMEM, scanmem);
  EEPROM.writeByte(EE_BYTE_SCANCANCEL, scancancel);
  EEPROM.writeByte(EE_BYTE_SCANMUTE, scanmute);
  EEPROM.writeByte(EE_BYTE_AUTOSQUELCH, autosquelch);
  EEPROM.writeByte(EE_BYTE_LONGBANDPRESS, longbandpress);
  EEPROM.writeByte(EE_BYTE_SHOWCLOCK, showclock);
  EEPROM.writeByte(EE_BYTE_SHOWLONGPS, showlongps);
  EEPROM.writeUInt(EE_UINT16_MEMSTARTFREQ, memstartfreq);
  EEPROM.writeUInt(EE_UINT16_MEMSTOPFREQ, memstopfreq);
  EEPROM.writeByte(EE_BYTE_MEMSTARTPOS, memstartpos);
  EEPROM.writeByte(EE_BYTE_MEMSTOPPOS, memstoppos);
  EEPROM.writeByte(EE_BYTE_MEMPIONLY, mempionly);
  EEPROM.writeByte(EE_BYTE_MEMDOUBLEPI, memdoublepi);
  EEPROM.writeByte(EE_BYTE_WAITONLYONSIGNAL, scanholdonsignal);
  EEPROM.commit();
  if (af == 2) radio.rds.afreg = true; else radio.rds.afreg = false;
  Serial.end();
  if (wifi) remoteip = IPAddress (WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], subnetclient);
  if (USBmode) Serial.begin(19200); else Serial.begin(115200);

  leave = true;
  PSSprite.unloadFont();
  if (language == LANGUAGE_CHS) PSSprite.loadFont(FONT28_CHS); else PSSprite.loadFont(FONT28);
  PSSprite.setTextDatum(TL_DATUM);
  BuildDisplay();
  SelectBand();
  ScreensaverTimerRestart();
}

void startFMDXScan() {
  initdxscan = true;
  scanholdflag = false;
  for (byte i = 0; i < 100; i++) {
    rabbitearspi[i] = 0;
    rabbitearstime[i][0] = 0;
  }

  if (menu) endMenu();
  if (afscreen || advancedRDS) {
    BuildDisplay();
    freq_in = 0;
  }

  if (memorypos > scanstop || memorypos < scanstart) memorypos = scanstart;
  scanmodeold = tunemode;

  if (scanmem) {
    tunemode = TUNE_MEM;
    if (band != presets[memorypos].band) {
      band = presets[memorypos].band;
      SelectBand();
    }
    DoMemoryPosTune();
  } else {
    tunemode = TUNE_MAN;
    if (band != presets[memorypos].band) {
      band = presets[memorypos].band;
      SelectBand();
    }
    TuneUp();
    ShowFreq(0);
  }
  if (scanmute) {
    radio.setMute();
    tft.drawBitmap(92, 4, Speaker, 26, 22, PrimaryColor);
    SQ = true;
    Squelchold = -2;
  }
  scantimer = millis();
  scandxmode = true;
  ShowTuneMode();
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("J1\n");
}

void rabbitearssend () {
  if (WiFi.status() != WL_CONNECTED) return;
  String RabbitearsURL = "http://rabbitears.info/tvdx/fm_spot";
  WiFiClient RabbitearsClient;
  HTTPClient http;
  byte i;
  String json = String("{\"tuner_key\":\"");
  json += RabbitearsUser;
  json += String("\",\"password\":\"");
  json += RabbitearsPassword;
  json += String("\",");
  json += String("\"signal\":{");
  for (i = 0; i < 100; i++) {
    if (rabbitearspi[i]) {
      json += String("\"");
      json += String((i * 2 + 881) * 100000);
      json += String("\":{\"time\":\"");
      json += String(rabbitearstime[i]);
      json += String("\",\"pi_code\":");
      json += String(rabbitearspi[i]);
      json += String("},");
    }
  }
  json.remove(json.length() - 1); // remove trailing comma
  json += String("}}");
  http.begin(RabbitearsClient, RabbitearsURL.c_str());
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  http.end();
}

void setAutoSpeedSPI() {
  switch (frequency / 10) {
    case 875 ... 877: tft.setSPISpeed(28); break;
    case 878 ... 881: tft.setSPISpeed(24); break;
    case 882 ... 892: tft.setSPISpeed(42); break;
    case 893 ... 899: tft.setSPISpeed(31); break;
    case 900 ... 904: tft.setSPISpeed(12); break;
    case 905 ... 906: tft.setSPISpeed(16); break;
    case 907 ... 910: tft.setSPISpeed(11); break;
    case 911 ... 916: tft.setSPISpeed(15); break;
    case 917 ... 921: tft.setSPISpeed(24); break;
    case 922 ... 928: tft.setSPISpeed(13); break;
    case 929: tft.setSPISpeed(11); break;
    case 930 ... 932: tft.setSPISpeed(13); break;
    case 933 ... 939: tft.setSPISpeed(18); break;
    case 940 ... 941: tft.setSPISpeed(12); break;
    case 942: tft.setSPISpeed(17); break;
    case 943 ... 949: tft.setSPISpeed(15); break;
    case 950: tft.setSPISpeed(19); break;
    case 951: tft.setSPISpeed(15); break;
    case 952 ... 960: tft.setSPISpeed(22); break;
    case 961 ... 965: tft.setSPISpeed(15); break;
    case 966 ... 973: tft.setSPISpeed(22); break;
    case 974 ... 979: tft.setSPISpeed(17); break;
    case 980 ... 982: tft.setSPISpeed(20); break;
    case 983 ... 987: tft.setSPISpeed(18); break;
    case 988 ... 993: tft.setSPISpeed(11); break;
    case 994 ... 996: tft.setSPISpeed(18); break;
    case 997 ... 1005: tft.setSPISpeed(11); break;
    case 1006: tft.setSPISpeed(13); break;
    case 1007 ... 1011: tft.setSPISpeed(11); break;
    case 1012 ... 1016: tft.setSPISpeed(18); break;
    case 1017 ... 1026: tft.setSPISpeed(13); break;
    case 1027 ... 1035: tft.setSPISpeed(23); break;
    case 1036 ... 1038: tft.setSPISpeed(15); break;
    case 1039 ... 1042: tft.setSPISpeed(12); break;
    case 1043 ... 1047: tft.setSPISpeed(23); break;
    case 1048 ... 1050: tft.setSPISpeed(28); break;
    case 1051 ... 1062: tft.setSPISpeed(15); break;
    case 1063 ... 1068: tft.setSPISpeed(18); break;
    case 1069 ... 1074: tft.setSPISpeed(14); break;
    case 1075: tft.setSPISpeed(17); break;
    case 1076 ... 1080: tft.setSPISpeed(15); break;
    default: tft.setSPISpeed(30); break;
  }
}

uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi) {
  uint8_t error = 0;
  uint8_t counter = 0;
  uint8_t rangestart = startmem;
  uint16_t _current = frequency;
  uint16_t totalIterations = (stopfreq - startfreq) + 1;
  uint16_t currentIteration = 0;
  uint8_t percent = 0;
  uint8_t percentold = 0;
  bool stopScanning = false;
  bool dostore = false;

  radio.setMute();
  radio.power(0);
  band = BAND_FM;
  tunemode = TUNE_MAN;

  tft.drawRect(59, 109, 202, 8, FrameColor);
  tft.fillRect(60, 110, 200, 6, GreyoutColor);
  tftPrint(1, myLanguage[language][272], 120, 155, ActiveColor, ActiveColorSmooth, 16);

  for (frequency = startfreq * 10; frequency <= stopfreq * 10; frequency += 10) {
    if (stopScanning) {
      break;
    }

    currentIteration++;
    percent = (currentIteration * 100) / totalIterations;

    radio.SetFreq(frequency);
    radio.clearRDS(fullsearchrds);
    delay(50);
    radio.getStatus(SStatus, USN, WAM, OStatus, BW, MStatus, CN);
    if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80 && OStatus > -80)) {
      for (byte y = 0; y < 20; y++) {
        delay(50);
        radio.readRDS(showrdserrors);
      }

      dostore = true;
      if (doublepi != 0) {
        for (byte x = (doublepi == 1 ? rangestart : 0); x <= (doublepi == 1 ? stopmem : EE_PRESETS_CNT - 1); x++) {
          if (presets[x].RDSPI[0] != '\0') {
            bool allMatch = true;

            for (byte i = 0; i < 4; i++) {
              if (presets[x].RDSPI[i] != radio.rds.picode[i]) {
                allMatch = false;
                break;
              }
            }

            if (allMatch) {
              dostore = false;
              break;
            }
          }
        }
      }

      if (((rdsonly && radio.rds.hasRDS) || !rdsonly) && dostore) {
        StoreMemoryPos(startmem);
        counter ++;
        startmem++;
        if (startmem > stopmem) {
          error = 1;
          stopScanning = true;
          break;
        }
        dostore = false;
      }
    }

    if (percent != percentold) {
      switch (CurrentTheme) {
        case 7:
          SignalSprite.pushImage(-107, -95, 292, 170, popupbackground_wo);
          SquelchSprite.pushImage(-177, -135, 292, 170, popupbackground_wo);
          break;

        default:
          SignalSprite.pushImage(-107, -95, 292, 170, popupbackground);
          SquelchSprite.pushImage(-177, -135, 292, 170, popupbackground);
          break;
      }

      SignalSprite.setTextColor(SecondaryColor, SecondaryColorSmooth, false);
      SignalSprite.setTextDatum(TC_DATUM);
      SignalSprite.loadFont(FONT28);
      SignalSprite.drawString(String(percent) + "%", 40, 0);
      SignalSprite.unloadFont();
      SignalSprite.pushSprite(120, 125);

      if (language == LANGUAGE_CHS) SquelchSprite.loadFont(FONT16_CHS); else SquelchSprite.loadFont(FONT16);
      SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
      SquelchSprite.drawString(String(counter), 0, 0);
      SquelchSprite.pushSprite(200, 155);

      tft.fillRect(60, 110, 2 * percent, 6, BarInsignificantColor);

      percentold = percent;
    }
    if (digitalRead(MODEBUTTON) == LOW) {
      while (digitalRead(MODEBUTTON)) delay(50);
      error = 2;
      stopScanning = true;
      break;
    }
  }

  frequency = _current;
  radio.SetFreq(frequency);
  radio.clearRDS(fullsearchrds);
  radio.setUnMute();
  SQ = false;

  return error;
}

void doBandToggle() {
  if (tunemode != TUNE_MEM) {
    ToggleBand(band);
    radio.clearRDS(fullsearchrds);
    StoreFrequency();
    SelectBand();
    if (XDRGTKUSB || XDRGTKTCP) {
      if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n");
      else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n");
      else DataPrint("M1\nT" + String(frequency_AM) + "\n");
    }
  } else {
    scanmodeold = tunemode;
    startFMDXScan();
    return;
  }
  ScreensaverTimerRestart();
}

void StoreMemoryPos(uint8_t _pos) {
  EEPROM.writeByte(_pos + EE_PRESETS_BAND_START, band);
  EEPROM.writeByte(_pos + EE_PRESET_BW_START, BWset);
  EEPROM.writeByte(_pos + EE_PRESET_MS_START, StereoToggle);
  if (band == BAND_FM) {//todo air
    EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency);
  } else if (band == BAND_OIRT) {
    EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_OIRT);
  } else if (band == BAND_LW) {
    EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_LW);
  } else if (band == BAND_MW) {
    EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_MW);
  } else {
    EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_SW);
  }

  presets[_pos].band = band;
  presets[_pos].bw = BWset;
  presets[_pos].ms = StereoToggle;

  String stationName = radio.rds.stationName;
  char stationNameCharArray[10];
  char picodeArray[7];
  stationName.toCharArray(stationNameCharArray, sizeof(stationNameCharArray));
  memcpy(picodeArray, radio.rds.picode, sizeof(picodeArray));

  for (int y = 0; y < 9; y++) {
    presets[_pos].RDSPS[y] = (y < strlen(stationNameCharArray)) ? stationNameCharArray[y] : '\0';
    EEPROM.writeByte((_pos * 9) + y + EE_PRESETS_RDSPS_START, presets[_pos].RDSPS[y]);
  }

  for (int y = 0; y < 5; y++) {
    presets[_pos].RDSPI[y] = (y < sizeof(picodeArray)) ? picodeArray[y] : '\0';
    EEPROM.writeByte((_pos * 5) + y + EE_PRESETS_RDSPI_START, presets[_pos].RDSPI[y]);
  }

  EEPROM.commit();

  if (band == BAND_FM) {//todo air
    presets[_pos].frequency = frequency;
  } else if (band == BAND_OIRT) {
    presets[_pos].frequency = frequency_OIRT;
  } else if (band == BAND_LW) {
    presets[_pos].frequency = frequency_LW;
  } else if (band == BAND_MW) {
    presets[_pos].frequency = frequency_MW;
  } else {
    presets[_pos].frequency = frequency_SW;
  }
}

void ClearMemoryRange(uint8_t start, uint8_t stop) {
  for (uint8_t pos = start; pos <= stop; pos++) {
    EEPROM.writeByte(pos + EE_PRESETS_BAND_START, BAND_FM);
    EEPROM.writeUInt((pos * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
    EEPROM.writeByte(pos + EE_PRESET_BW_START, 0);
    EEPROM.writeByte(pos + EE_PRESET_MS_START, 1);

    for (int y = 0; y < 9; y++) {
      EEPROM.writeByte((pos * 9) + y + EE_PRESETS_RDSPS_START, '\0');
      presets[pos].RDSPS[y] = '\0';
    }

    for (int y = 0; y < 5; y++) {
      EEPROM.writeByte((pos * 5) + y + EE_PRESETS_RDSPI_START, '\0');
      presets[pos].RDSPI[y] = '\0';
    }

    EEPROM.commit();
    presets[pos].band = BAND_FM;
    presets[pos].frequency = EE_PRESETS_FREQUENCY;
  }
}

void Touch_IRQ_Handler() {
  touch_detect = true;
}

byte numval[16] = {
  2, 3, 127, 5, 6, 0, 9, 13, 8, 7, 4, 1, 0, 0, 0, 0
};

int GetNum(void) {
  int16_t temp;
  int cnt = 0;
  unsigned int num;

  Wire.beginTransmission(0x20);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x20, 2);

  if (Wire.available() == 2) {
    keypadtimer = millis();
    temp = Wire.read() & 0xFF;
    temp |= (Wire.read() & 0xFF) * 256;
    for (int i = 0; i < 16; i++) {
      if ((temp & 0x01) == 0) {
        num = numval[i];
        cnt ++;
      }
      temp >>= 1;
    }
    if (cnt == 1)
      return num;
  }

  return -1;
}

void ShowNum(int val) {
  switch (freqfont) {
    case 0: FrequencySprite.loadFont(FREQFONT0); break;
    case 1: FrequencySprite.loadFont(FREQFONT1); break;
    case 2: FrequencySprite.loadFont(FREQFONT2); break;
    case 3: FrequencySprite.loadFont(FREQFONT3); break;
    case 4: FrequencySprite.loadFont(FREQFONT4); break;
    case 5: FrequencySprite.loadFont(FREQFONT5); break;
  }

  FrequencySprite.setTextDatum(TR_DATUM);

  FrequencySprite.fillSprite(BackgroundColor);
  FrequencySprite.setTextColor(SecondaryColor, SecondaryColorSmooth, false);
  FrequencySprite.drawString(String(val) + " ", 218, -6);
  FrequencySprite.pushSprite(46, 46);

  FrequencySprite.unloadFont();
}

void TuneFreq(int temp) {
  aftest = true;
  aftimer = millis();

  if (band == BAND_FM) {
    while (temp < (LowEdgeSet * 10)) temp = temp * 10;
    if (temp > (HighEdgeSet * 10)) {
      if (edgebeep) EdgeBeeper();
    } else {
      frequency = temp;
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    while (temp < (LowEdgeOIRTSet * 10)) temp = temp * 10;
    if (temp > HighEdgeOIRTSet) {
      if (edgebeep) EdgeBeeper();
    } else {
      frequency_OIRT = temp;
    }
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    while (temp < LWLowEdgeSet) temp = temp * 10;
    if (temp > LWHighEdgeSet) {
      if (edgebeep) EdgeBeeper();
    } else {
      frequency_AM = temp;
    }
    radio.SetFreqAM(frequency_AM);
    frequency_LW = frequency_AM;
  } else if (band == BAND_MW) {
    while (temp < MWLowEdgeSet) temp = temp * 10;
    if (temp > MWHighEdgeSet) {
      if (edgebeep) EdgeBeeper();
    } else {
      frequency_AM = temp;
    }
    radio.SetFreqAM(frequency_AM);
    frequency_MW = frequency_AM;
  } else if (band == BAND_SW) {
    while (temp < SWLowEdgeSet) temp = temp * 10;
    if (temp > SWHighEdgeSet) {
      if (edgebeep) EdgeBeeper();
    } else {
      frequency_AM = temp;
    }
    radio.SetFreqAM(frequency_AM);
    frequency_SW = frequency_AM;
  }

  radio.clearRDS(fullsearchrds);
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void NumpadProcess(int num) {
  if (scandxmode) {
    if (num == 127) cancelDXScan();
  } else {
    if (num == 127) {
      freq_in = 0;
      menuoption = ITEM1;
      menupage = DXMODE;
      menuitem = 0;
      if (spispeed == 7) tft.setSPISpeed(40);
      submenu = true;
      menu = true;
      PSSprite.unloadFont();
      if (language == LANGUAGE_CHS) PSSprite.loadFont(FONT16_CHS); else PSSprite.loadFont(FONT16);
      BuildMenu();
      ScreensaverTimerSet(OFF);
    } else if (num == 13) {
      if (freq_in != 0) {
        TuneFreq(freq_in);
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS(fullsearchrds);
          memtune = false;
          ShowFreq(0);
          store = true;
        }
      } else {
        ShowFreq(0);
      }
      freq_in = 0;
    } else {
      if (freq_in / 10000 == 0) {
        freq_in = freq_in * 10 + num;
      }
      ShowNum(freq_in);
    }
  }
}

void toggleiMSEQ() {
  if (band < BAND_GAP) {
    if (iMSEQ == 0) iMSEQ = 1;

    if (iMSEQ == 4) {
      iMSset = 0;
      EQset = 0;
      updateiMS();
      updateEQ();
      iMSEQ = 0;
    }
    if (iMSEQ == 3) {
      iMSset = 1;
      EQset = 0;
      updateiMS();
      updateEQ();
      iMSEQ = 4;
    }
    if (iMSEQ == 2) {
      iMSset = 0;
      EQset = 1;
      updateiMS();
      updateEQ();
      iMSEQ = 3;
    }
    if (iMSEQ == 1) {
      iMSset = 1;
      EQset = 1;
      updateiMS();
      updateEQ();
      iMSEQ = 2;
    }
    EEPROM.writeByte(EE_BYTE_IMSSET, iMSset);
    EEPROM.writeByte(EE_BYTE_EQSET, EQset);
    EEPROM.commit();
    if (XDRGTKUSB || XDRGTKTCP) DataPrint("G" + String(!EQset) + String(!iMSset) + "\n");
  } else {
    if (band == BAND_SW && tunemode != TUNE_MEM) {
      nowToggleSWMIBand = !nowToggleSWMIBand;
      tunemode = TUNE_MAN;
      EEPROM.writeByte(EE_BYTE_BANDAUTOSW, nowToggleSWMIBand);
      EEPROM.commit();
      if (!screenmute) {
        tftPrint(0, "AUTO", 22, 60, BackgroundColor, BackgroundColor, 16);
        tftPrint(0, "BAND", 22, 60, BackgroundColor, BackgroundColor, 16);
      }
      doTuneMode();
      ShowTuneMode();
    }
  }
}

void handleRoot() {
  // Attempt to open the CSV file stored in SPIFFS (File System)
  fs::File file = SPIFFS.open("/logbook.csv", "r");
  if (!file) {
    // If the file could not be opened, send an error message to the browser
    webserver.send(500, "text/plain", "Failed to open logbook");
    return;  // Exit the function if the file cannot be opened
  }

  // Start building the HTML page to send to the browser
  String html = "<!DOCTYPE html><html lang=\"en\"><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

  // Add CSS styling for a modern, dark-themed look
  html += "<style>";
  html += "body {background-color: #1a1a1a; color: white; font-family: 'Arial', sans-serif; margin: 0; padding: 0;}";
  html += "h1 {text-align: center; color: #ffffff; margin-top: 20px; font-size: 32px;}";
  html += "img {display: block; margin: 0 auto; max-width: 100%; height: auto; padding-top: 20px;}";
  html += "table {width: 90%; margin: 0 auto; border-collapse: collapse; border-radius: 8px; overflow: hidden;}";
  html += "th {background-color: #333; color: white; padding: 12px; text-align: left; font-size: 18px;}";
  html += "td {background-color: #2a2a2a; color: white; padding: 10px; text-align: left; font-size: 16px;}";
  html += "tr:nth-child(even) {background-color: #252525;}";
  html += "tr:hover {background-color: #444; cursor: pointer;}";
  html += "button {background-color: #ffcc00; color: black; border: none; padding: 12px 20px; font-size: 18px; cursor: pointer; border-radius: 5px; display: block; margin: 20px auto;}";
  html += "button:hover {background-color: #ff9900;}";
  html += "@media (max-width: 768px) { table {width: 100%;} th, td {font-size: 14px; padding: 8px;} }";
  html += ".go-to-bottom {position: fixed; bottom: 30px; right: 30px; background-color: #ffcc00; color: black; border: none; padding: 12px 20px; font-size: 18px; cursor: pointer; border-radius: 5px; z-index: 100;}";
  html += "</style>";

  html += "</head><body>";

  // Add the logo image at the top of the page, served from SPIFFS
  html += "<img src=\"/logo.png\" alt=\"FMdx Logo\">";

  // Add a header with a dynamic title from the language array (replace with actual language logic)
  html += "<h1>" + String(myLanguage[language][286]) + "</h1>";

  // Add the "Download CSV" button, which triggers a download action when clicked
  html += "<button onclick=\"window.location.href='/downloadCSV'\">" + String(myLanguage[language][287]) + "</button>";

  // Add "Go to Bottom" button
  html += "<button class=\"go-to-bottom\" onclick=\"scrollToBottom()\">" + String(myLanguage[language][289]) + "</button>";

  // Add JavaScript for scrolling to the bottom
  html += "<script>";
  html += "function scrollToBottom() {";
  html += "  window.scrollTo(0, document.body.scrollHeight);";
  html += "}";
  html += "</script>";

  // Start the HTML table to display CSV data
  html += "<table>";

  // Read and process the first line (header row) from the CSV file
  String header = "";
  if (file.available()) {
    header = file.readStringUntil('\n'); // Read the first line containing the headers
    html += "<tr>"; // Start the header row in the table
    int startIndex = 0;

    // Split the header line by commas and create table headers (<th>) for each column
    while (startIndex < header.length()) {
      int endIndex = header.indexOf(',', startIndex);
      if (endIndex == -1) endIndex = header.length(); // Handle last column (no comma after it)
      String column = header.substring(startIndex, endIndex); // Extract the column name
      html += "<th>" + column + "</th>"; // Add the column as a table header
      startIndex = endIndex + 1; // Move to the next column
    }
    html += "</tr>"; // End the header row
  }

  // Variable to track if there is any data in the CSV file
  bool hasData = false;
  int rowCount = 0; // Counter for rows, used for alternating row colors

  // Process the remaining lines (data rows) in the CSV file
  while (file.available()) {
    String line = file.readStringUntil('\n'); // Read the next line (data row)
    if (line.length() > 0) {
      hasData = true;  // Mark that data rows are present
      rowCount++; // Increment the row count
      html += "<tr>"; // Start a new row in the table
      int startIndex = 0;

      // Split the line by commas and create table data cells (<td>) for each column
      while (startIndex < line.length()) {
        int endIndex = line.indexOf(',', startIndex);
        if (endIndex == -1) endIndex = line.length(); // Handle the last column
        String cell = line.substring(startIndex, endIndex); // Extract the cell data
        html += "<td>" + cell + "</td>"; // Add the cell data to the table
        startIndex = endIndex + 1; // Move to the next column
      }
      html += "</tr>"; // End the data row
    }
  }

  file.close(); // Close the file after reading

  // If no data rows were found, display a "No data available" message in the table
  if (!hasData) {
    html += "<tr><td colspan=\"100%\" style=\"text-align: center; color: red;\">" + String(myLanguage[language][288]) + "</td></tr>";
  }

  // End the HTML table and body
  html += "</table>";
  html += "</body></html>"; // End the HTML page

  // Send the generated HTML content to the browser with a 200 OK response
  webserver.send(200, "text/html", html);
}

void handleDownloadCSV() {
  // Attempt to open the CSV file from SPIFFS in read mode
  fs::File file = SPIFFS.open("/logbook.csv", "r");

  // Check if the file was successfully opened
  if (!file) {
    // If the file could not be opened, send an error response
    webserver.send(500, "text/plain", "Failed to open logbook for download");
    return;  // Exit the function if the file cannot be opened
  }

  // Set the headers to specify that the response will be a CSV file for download
  webserver.sendHeader("Content-Type", "text/csv");  // Set MIME type for CSV files
  webserver.sendHeader("Content-Disposition", "attachment; filename=logbook.csv");  // Suggests the file name for download

  // Stream the CSV file content directly to the browser
  webserver.streamFile(file, "text/csv");

  // Close the file after streaming the content to release resources
  file.close();
}

bool handleCreateNewLogbook() {
  // Check if the file "logbook.csv" already exists
  if (SPIFFS.exists("/logbook.csv")) {
    // If it exists, delete the file
    if (!SPIFFS.remove("/logbook.csv")) {
      // Return false if the file could not be deleted
      return false;
    }
  }

  // Create a new "logbook.csv" file in write mode
  fs::File file = SPIFFS.open("/logbook.csv", "w");

  // Check if the file was successfully created
  if (!file) {
    // Return false if file creation fails
    return false;
  }

  // Write the header to the new CSV file
  String header = "Date,Time,Frequency,PI code,Signal,PS,RadioText\n";
  file.print(header); // Ensure that the header is written properly

  // Make sure the data is written before closing the file
  file.flush(); // Ensure that everything is written to the file
  file.close(); // Close the file after writing

  // Return true if the function runs without problems
  return true;
}

bool addRowToCSV() {
  // Check if there is enough free space in SPIFFS (150 bytes or more)
  if (SPIFFS.totalBytes() - SPIFFS.usedBytes() < 150) {
    return false;  // Return false if there is less than 150 bytes free
  }

  // Open the logbook.csv file in append mode
  fs::File file = SPIFFS.open("/logbook.csv", "a");

  // Check if the file could not be opened
  if (!file) {
    return false;  // Return false if the file can't be opened
  }

  // Get the current date and time from ESP32 (using the built-in time functions)
  String currentDateTime = getCurrentDateTime();  // Get the current date and time

  // If time is not available, replace with "-"
  if (currentDateTime == "") {
    currentDateTime = "-,-";  // Set both date and time to "-"
  }

  // Convert frequency to a string format (XX.XX MHz)
  int freqInt = (int)frequency;  // Assuming frequency is already a float or double, cast it to int

  // Apply the necessary conversion (if any) for frequency
  int convertedFreq = (freqInt + ConverterSet * 100) / 100;
  String frequencyFormatted = String(convertedFreq) + "." + ((freqInt + ConverterSet * 100) % 100 < 10 ? "0" : "") + String((freqInt + ConverterSet * 100) % 100) + " MHz";  // Add " MHz"

  // Format the signal strength (xx.x with the correct unit)
  int SStatusprint = 0;
  if (unit == 0) SStatusprint = SStatus;
  if (unit == 1) SStatusprint = ((SStatus * 100) + 10875) / 100;
  if (unit == 2) SStatusprint = round((float(SStatus) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0);

  // Choose the correct unit suffix for signal based on the `unit` value
  String signal = String(SStatusprint / 10) + "." + String(abs(SStatusprint % 10));
  if (unit == 0) {
    signal += " dBμV";  // Unit for unit == 0
  } else if (unit == 1) {
    signal += " dBf";   // Unit for unit == 1
  } else if (unit == 2) {
    signal += " dBm";   // Unit for unit == 2
  }

  // Format the RadioText with enhanced option if available
  String radioText = String(radio.rds.stationText + " " + radio.rds.stationText32);
  if (radio.rds.hasEnhancedRT) {
    radioText += " eRT: " + String(radio.rds.enhancedRTtext);
  }

  // Replace commas in the station name and radioText just when adding to the row
  String stationName = radio.rds.stationName;
  String radioTextModified = radioText;

  stationName.replace(",", " ");  // Temporarily replace commas in stationName
  radioTextModified.replace(",", " ");  // Temporarily replace commas in radioText

  // Create the row data, replacing stationIDtext with picode
  String row = currentDateTime + "," + frequencyFormatted + "," + radio.rds.picode + "," + signal + "," + stationName + "," + radioTextModified + "\n";

  // Write the row to the CSV file
  if (file.print(row)) {
    // Successfully wrote to the file
    file.close();
    return true;  // Return true when the row is successfully added
  } else {
    // Failed to write to the file
    file.close();
    return false;  // Return false if there was an issue writing
  }
}

String getCurrentDateTime() {
  // Check if time has been set
  if (!rtcset) {
    return "-,-";  // Return placeholder when time is not set
  }

  // Use the ESP32's time functions (assuming time is set correctly via RDS)
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    return "-,-";  // Return placeholder if time is not available
  }

  // Adjust timeInfo using the GMT offset
  time_t currentEpoch = mktime(&timeInfo);  // Convert struct tm to time_t
  currentEpoch += radio.rds.offset;        // Apply the offset (in seconds)
  localtime_r(&currentEpoch, &timeInfo);   // Convert back to struct tm

  // Format date-time based on the region
  char buf[20];  // Buffer size for formatted date string

  if (radio.rds.region == 1) {
    // USA format: MM/DD/YYYY, HH:MM AM/PM
    strftime(buf, sizeof(buf), "%m/%d/%Y", &timeInfo);  // MM/DD/YYYY format

    // Format time in 12-hour format with AM/PM
    int hour = timeInfo.tm_hour;
    String ampm = (hour >= 12) ? "PM" : "AM";
    if (hour == 0) hour = 12;  // Midnight case
    else if (hour > 12) hour -= 12;  // Convert PM to 12-hour format

    String timeWithAMPM = String(hour) + ":" + (timeInfo.tm_min < 10 ? "0" : "") + String(timeInfo.tm_min) + " " + ampm;

    // Return final formatted date-time for USA
    return String(buf) + "," + timeWithAMPM;
  } else {
    // European format: DD/MM/YYYY, HH:MM
    strftime(buf, sizeof(buf), "%d-%m-%Y", &timeInfo);  // DD/MM/YYYY format
    String timeEuropean = String(timeInfo.tm_hour) + ":" + (timeInfo.tm_min < 10 ? "0" : "") + String(timeInfo.tm_min); // Add leading 0 for minutes if necessary
    return String(buf) + "," + timeEuropean;
  }
}

void handleLogo() {
  fs::File file = SPIFFS.open("/logo.png", "r");
  if (!file) {
    webserver.send(404, "text/plain", "Logo not found");
    return;
  }
  webserver.streamFile(file, "image/png");
  file.close();
}

void printLogbookCSV() {
  // Attempt to open the CSV file stored in SPIFFS
  fs::File file = SPIFFS.open("/logbook.csv", "r");

  // Check if the file was successfully opened
  if (!file) {
    Serial.println("Failed to open logbook!");
    return;
  }

  // Print a message indicating the start of the file content
  Serial.println("===== Start of logbook.csv =====");

  // Read and print the contents of the file line by line
  while (file.available()) {
    String line = file.readStringUntil('\n');  // Read one line at a time
    Serial.println(line);  // Print the line to the Serial Monitor
  }

  // Close the file after reading
  file.close();

  // Print a message indicating the end of the file content
  Serial.println("===== End of logbook.csv =====");
}
