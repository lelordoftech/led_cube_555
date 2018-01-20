/*
 * effect.h
 *
 * Created: 20-Jan-2018
 * Author: Vuong Le
 */

#ifndef EFFECT_H
#define EFFECT_H

#include "define.h"
#include <stdlib.h>
#include "character.h"

#define BRIGHTNESS_LINE         50
#define BRIGHTNESS_LAYER        20
#define BRIGHTNESS_LAYER_FULL   30
#define BRIGHTNESS_ALL          254

enum LINE
{
    UNKNOWN_LINE   = 0x00,
    X_LINE         = 0x01,
    Y_LINE         = 0x02,
    Z_LINE         = 0x03
};

enum DIRECTION
{
    UNKNOWN_DIR = 0x00,
    X_DIR       = 0x01,
    Y_DIR       = 0x02,
    Z_DIR       = 0x03
};

// PRIVATE FUNCTION
void quet74HC595(uint8_t input);
void quetLayer(uint8_t layer, uint8_t x1, uint8_t ic1, uint8_t ic2, uint8_t ic3);
void quetPoint(uint8_t x, uint8_t y, uint8_t z);
void lanToaLayer(uint8_t z);
void thuNhoLayer(uint8_t z);
void lanToaPhuDay(uint8_t z);
void line(int8_t k, int8_t b, uint8_t z);
//void bungNo(uint8_t xCen, uint8_t yCen, uint8_t zCen);
void bungNo(void); // old code
void quetMangSauTruoc(uint8_t* ic3, uint8_t delay);
//void quetMangTruocSau(uint8_t* x1, uint8_t* ic1, uint8_t delay);// DO NOT USE AT THIS TIME
void quetMangQuay(uint8_t* ic2, uint8_t delay);
void xuatKituSauTruoc(char character);
//void xuatKituTruocSau(char character);
void xuatKituQuay(char character);


// GLOBAL FUNCTION
void LineIncrease(enum LINE line, enum DIRECTION direction);
void LineDecrease(enum LINE line, enum DIRECTION direction);
void LayerIncrease(enum DIRECTION direction);
void LayerDecrease(enum DIRECTION direction);
void BeginToEndPoint(void);
void EndToBeginPoint(void);
void BlindAllLed(void);
void LanToaDuoiLen(void);
void LanToaTrenXuong(void);
void LanToaHet(void);
void ThuNhoDuoiLen(void);
void ThuNhoTrenXuong(void);
void ThuNhoHet(void);
void LanToaPhuDayDuoiLen(void);
void LanToaPhuDayTrenXuong(void);
void LanToaPhuDayHet(void); // old code
void QuetXdYdDuoiLen(void);
void QuetXdYdTrenXuong(void);
void QuetXdYdZ(void);
void QuetXaYaDuoiLen(void);
void QuetXaYaTrenXuong(void);
void QuetXaYaZ(void);
void QuetXdYaDuoiLen(void);
void QuetXdYaTrenXuong(void);
void QuetXdYaZ(void);
void QuetXaYdDuoiLen(void);
void QuetXaYdTrenXuong(void);
void QuetXaYdZ(void);
void XoayTronDuoiLen(void);
void XoayTronTrenXuong(void);
void XoayTronHet(void);
void XoayTronNguocDuoiLen(void);
void XoayTronNguocTrenXuong(void);
void XoayTronNguocHet(void);
void MuaRoi(void); // old code
void PhaoHoa(void); // old code
void SongNuoc(void); // old code
void RoiDay(void); // old code
void XoayOc(uint8_t layer);
void XoayOc_full(uint8_t layer); // old code
void XoayOc_fullZ(void); // old code
void XoayOc_blank(uint8_t layer); // old code
void XoayOc_blankZ(void); // old code
void Christmas(void);
void XuatChuSauTruoc(char *word);
void XuatChuQuay(char *word);

#endif
