#include "TEF6686.h"
#include "Tuner_Patch_Lithio_V102_p224.h"
#include "Tuner_Patch_Lithio_V205_p512.h"
#include <Wire.h>

const unsigned char tuner_init_tab[] PROGMEM = {
  7, 0x20, 0x0B, 0x01, 0x03, 0x98, 0x00, 0x00,
  5, 0x20, 0x14, 0x01, 0x00, 0x00,
  5, 0x20, 0x16, 0x01, 0x00, 0x00,
  7, 0x20, 0x17, 0x01, 0x00, 0x00, 0x03, 0xE8,
  5, 0x20, 0x20, 0x01, 0x00, 0x01,
  5, 0x20, 0x1F, 0x01, 0x01, 0xF4,
  5, 0x21, 0x27, 0x01, 0xfe, 0xd4,
  9, 0x20, 0x2A, 0x01, 0x00, 0x02, 0x00, 0x96, 0x00, 0xDC,
  9, 0x21, 0x2A, 0x01, 0x00, 0x02, 0x01, 0x18, 0x00, 0xFA,
  7, 0x20, 0x2D, 0x01, 0x00, 0x00, 0x00, 0xC8,
  11, 0x20, 0x32, 0x01, 0x00, 0x3C, 0x00, 0x78, 0x00, 0xC8, 0x00, 0xC8,
  11, 0x20, 0x33, 0x01, 0x00, 0x00, 0x00, 0xFA, 0x00, 0x82, 0x01, 0xF4,
  9, 0x20, 0x36, 0x01, 0x00, 0x00, 0x01, 0x68, 0x01, 0x2C,
  7, 0x20, 0x37, 0x01, 0x00, 0x00, 0x0F, 0xA0,
  7, 0x20, 0x39, 0x01, 0x00, 0x00, 0x00, 0x64,
  7, 0x20, 0x3A, 0x01, 0x00, 0x00, 0x00, 0xAA,
  5, 0x20, 0x3B, 0x01, 0x00, 0x01,
  11, 0x20, 0x3C, 0x01, 0x00, 0x3C, 0x00, 0x78, 0x00, 0x64, 0x00, 0xC8,
  11, 0x20, 0x3D, 0x01, 0x00, 0x00, 0x00, 0xD2, 0x00, 0x5A, 0x01, 0xF4,
  11, 0x20, 0x46, 0x01, 0x01, 0xF4, 0x07, 0xD0, 0x00, 0xC8, 0x00, 0xC8,
  11, 0x20, 0x47, 0x01, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x78, 0x02, 0x9E,
  9, 0x20, 0x48, 0x01, 0x00, 0x00, 0x02, 0x58, 0x00, 0xF0,
  9, 0x20, 0x49, 0x01, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x8C,
  9, 0x20, 0x4A, 0x01, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x8C,
  7, 0x20, 0x4B, 0x01, 0x00, 0x00, 0x0F, 0xA0,
  5, 0x20, 0x56, 0x01, 0x03, 0x84,
  7, 0x30, 0x15, 0x01, 0x00, 0x80, 0x00, 0x01,
  13, 0x30, 0x16, 0x01, 0x00, 0x20, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x12, 0xc0,
  13, 0x30, 0x16, 0x01, 0x00, 0x21, 0x00, 0x02, 0x00, 0x20, 0x01, 0x00, 0x12, 0xC0,
  7, 0x30, 0x0D, 0x01, 0x00, 0x11, 0x00, 0xF0
};

const unsigned char tuner_init_tab4000[] PROGMEM = {
  3, 0x14, 0x00, 0x01,
  2, 0xff, 50,
  9, 0x40, 0x04, 0x01, 0x00, 0x3D, 0x09, 0x00, 0x00, 0x00,
  5, 0x40, 0x05, 0x01, 0x00, 0x01,
  2, 0xff, 100,
};

const unsigned char tuner_init_tab9216[] PROGMEM = {
  3, 0x14, 0x00, 0x01,
  2, 0xff, 50,
  9, 0x40, 0x04, 0x01, 0x00, 0x8C, 0xA0, 0x00, 0x00, 0x00,
  5, 0x40, 0x05, 0x01, 0x00, 0x01,
  2, 0xff, 100,
};

const unsigned char tuner_init_tab12000[] PROGMEM = {
  3, 0x14, 0x00, 0x01,
  2, 0xff, 50,
  9, 0x40, 0x04, 0x01, 0x00, 0xB7, 0x1B, 0x00, 0x00, 0x00,
  5, 0x40, 0x05, 0x01, 0x00, 0x01,
  2, 0xff, 100,
};

const unsigned char tuner_init_tab55000[] PROGMEM = {
  3, 0x14, 0x00, 0x01,
  2, 0xff, 50,
  9, 0x40, 0x04, 0x01, 0x03, 0x4E, 0x5A, 0xAE, 0x00, 0x01,
  5, 0x40, 0x05, 0x01, 0x00, 0x01,
  2, 0xff, 100,
};

bool Tuner_WriteBuffer(unsigned char *buf, uint16_t len) {
  Wire.beginTransmission(0x64);
  for (uint16_t i = 0; i < len; i++) Wire.write(buf[i]);
  uint8_t r = Wire.endTransmission();
  return (r == 0) ? 1 : 0;
}

bool Tuner_ReadBuffer(unsigned char *buf, uint16_t len) {
  Wire.requestFrom(0x64, len);
  if (Wire.available() == len) {
    for (uint16_t i = 0; i < len; i++) buf[i] = Wire.read();
    return 1;
  }
  return 0;
}

static void Tuner_Patch_Load(const unsigned char *pLutBytes, uint16_t size) {
  unsigned char buf[24 + 1];
  uint16_t i, len;
  uint16_t r;
  buf[0] = 0x1b;

  while (size) {
    len = (size > 24) ? 24 : size;
    size -= len;

    for (i = 0; i < len; i++) buf[1 + i] = pgm_read_byte(&pLutBytes[i]);
    pLutBytes += len;

    if (1 != (r = Tuner_WriteBuffer(buf, len + 1))) break;
  }
}

bool Tuner_Table_Write(const unsigned char *tab) {
  if (tab[1] == 0xff) {
    delay(tab[2]);
    return 1;
  } else {
    return Tuner_WriteBuffer((unsigned char *)&tab[1], tab[0]);
  }
}

void Tuner_Reset(void) {
  Wire.beginTransmission(0x64);
  Wire.write(0x1e);
  Wire.write(0x5a);
  Wire.write(0x01);
  Wire.write(0x5a);
  Wire.write(0x5a);
  Wire.endTransmission();
}

void Tuner_Patch(byte TEF) {
  Tuner_Reset();
  delay(100);
  Wire.beginTransmission(0x64);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(0x64);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x74);
  Wire.endTransmission();
  switch (TEF) {
    case 102:
      Tuner_Patch_Load(pPatchBytes102, PatchSize102);
      break;

    case 205:
      Tuner_Patch_Load(pPatchBytes205, PatchSize205);
      break;
  }
  Wire.beginTransmission(0x64);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(0x64);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x75);
  Wire.endTransmission();
  switch (TEF) {
    case 102:
      Tuner_Patch_Load(pLutBytes102, LutSize102);
      break;

    case 205:
      Tuner_Patch_Load(pLutBytes205, LutSize205);
      break;
  }
  Wire.beginTransmission(0x64);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}

void Tuner_I2C_Init() {
  Wire.begin();
  Wire.setClock(400000);
  delay(5);
}

void Tuner_Init(const unsigned char *table) {
  uint16_t r;
  const unsigned char *p = table;

  for (uint16_t i = 0; i < sizeof(tuner_init_tab); i += (pgm_read_byte(p + i) + 1)) {
    if (1 != (r = Tuner_Table_Write(p + i))) break;
  }
}