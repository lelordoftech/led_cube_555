/*
 * effect.c
 *
 * Created: 20-Jan-2018
 * Author: Vuong Le
 */

#include "effect.h"

// PRIVATE FUNCTION
void quet74HC595(uint8_t input)
{
    uint8_t i = 0;

    for (i=0; i<8; i++)
    {
        // check tung bit tu cao xuong thap (7 -> 0)
        // Chan DATA - PA3
        if ((input & 0x80) == 0x80) //0b10000000 so sanh bit 7 voi 1
        {
            PORTA = PINA | DATA_PIN; // DATA_PIN ON
        }
        else
        {
            PORTA = PINA & ~DATA_PIN; // DATA_PIN OFF
        }

        input = input << 1; //dich bit lay bit trong so thap hon

        // Chan CLOCK - PA1
        PORTA = PINA & ~CLOCK_PIN; // CLOCK_PIN OFF
        _delay_us(10);
        PORTA = PINA | CLOCK_PIN; // CLOCK_PIN ON
    }
}

void quetLayer(uint8_t layer, uint8_t x1, uint8_t ic1, uint8_t ic2, uint8_t ic3)
{
    // 0x01: LAYER 5
    // 0x02: LAYER 4
    // 0x04: LAYER 3
    // 0x08: LAYER 2
    // 0x10: LAYER 1
    PORTB = 0x10 >> (layer-1);

    quet74HC595(ic3);
    quet74HC595(ic2);
    quet74HC595(ic1);

    // Chan LATCH - PA2
    PORTA = PINA & ~LATCH_LED_PIN; // LATCH_LED_PIN OFF
    _delay_us(10);

    if (x1 == 1)
    {
        PORTA = PINA | LATCH_LED_PIN; // LATCH_LED_PIN ON
    }
    else
    {
        PORTA = PINA | LATCH_PIN; // LATCH_PIN ON
    }
}

void quetPoint(uint8_t x, uint8_t y, uint8_t z)
{
    uint8_t x1  = 0x00;
    uint8_t ic1 = 0x00;
    uint8_t ic2 = 0x00;
    uint8_t ic3 = 0x00;

    if (x >= 0 && x <=4 &&
        y >= 0 && y <=4 &&
        z >= 0 && z <=4)
    {
        uint8_t pos = 5*y+x;

        if (pos == 0)
        {
            x1 = 0x01;
        }
        else if (pos >= 1 && pos <= 8)
        {
            ic1 = 0x01;
            ic1 = ic1 << (pos-1);
        }
        else if (pos >= 9 && pos <= 16)
        {
            ic2 = 0x01;
            ic2 = ic2 << (pos-9);
        }
        else if (pos >= 17 && pos <= 24)
        {
            ic3 = 0x01;
            ic3 = ic3 << (pos-17);
        }
        else
        {
            // Do nothing
        }
    }
    else
    {
        // Do nothing
    }

    quetLayer(z+1, x1, ic1, ic2, ic3);
}

void lanToaLayer(uint8_t z)
{
   uint8_t r = 0, x = 0, y = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=0; r<4; r++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) >= (r*r-1) &&
                     ((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= (r*r+1))
               {
                  quetPoint(x, y, z);
               }
            }
         }
      }
   }
}

void thuNhoLayer(uint8_t z)
{
   uint8_t r = 0, x = 0, y = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=4; r>0;)
   {
      r--;
      for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) >= (r*r-1) &&
                     ((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= (r*r+1))
               {
                  quetPoint(x, y, z);
               }
            }
         }
      }
   }
}

void lanToaPhuDay(uint8_t z)
{
   uint8_t r = 0, x = 0, y = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=0; r<4; r++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER_FULL; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= r*r)
               {
                  quetPoint(x, y, z);
               }
            }
         }
      }
   }
}

void line(int8_t k, int8_t b, uint8_t z)
{
   // y = kx + b;
   uint8_t x = 0, y = 0;

   for (x=0; x<5; x++)
   {
      for (y=0; y<5; y++)
      {
         if (k*x+b == y)
         {
            quetPoint(x, y, z);
         }
      }
   }
}
/*
void bungNo(uint8_t xCen, uint8_t yCen, uint8_t zCen)
{
   uint8_t r = 0, x = 0, y = 0, z = 0, brightness = 0;

   for (r=0; r<5; r++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               for (z=0; z<5; z++)
               {
                  if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen) + (z-zCen)*(z-zCen)) <= r*r)
                  {
                     quetPoint(x, y, z);
                  }
               }
            }
         }
      }
   }
}
*/
void bungNo(void)
{
    uint8_t i, j;
    uint8_t  ma[]={   0,    0,    0,    0,    1,    1,    1,    1,    1,    0};//ma led
    uint8_t ma1[]={0x00, 0x40, 0xE2, 0xF7, 0xFF, 0xFF, 0xBF, 0x1D, 0x08, 0x00};//ma led
    uint8_t ma2[]={0x08, 0x1C, 0xBE, 0xFF, 0xFF, 0xF7, 0xE3, 0x41, 0x00, 0x00};//ma led
    uint8_t ma3[]={0x00, 0x01, 0x23, 0x77, 0xFF, 0xFF, 0xFE, 0xDC, 0x88, 0x00};//ma led

    //No tung
    for(i=0;i<10;i++)
    {
        for(j=1; j<30; j++) //Delay
        {
            quetLayer(3, ma[i], ma1[i], ma2[i], ma3[i]);
            _delay_us(1000);
            
            if(i >= 1)
            {
                quetLayer(2, ma[i-1], ma1[i-1], ma2[i-1], ma3[i-1]);
                _delay_us(1000);
                quetLayer(4, ma[i-1], ma1[i-1], ma2[i-1], ma3[i-1]);
                _delay_us(1000);
            }
            
            if(i >= 2)
            {
                quetLayer(1, ma[i-2], ma1[i-2], ma2[i-2], ma3[i-2]);
                _delay_us(1000);
                quetLayer(5, ma[i-2], ma1[i-2], ma2[i-2], ma3[i-2]);
                _delay_us(1000);
            }
        }
    }
}

void quetMangSauTruoc(uint8_t* ic3, uint8_t delay)
{
    uint8_t delay_x, z_count, delay_layer, y_count, bit_count;
    uint8_t  x1[] = {   0,    0,    0,    0,    0};
    uint8_t ic1[] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t ic2[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    for (y_count=1; y_count<=5; y_count++) //Dich 5 MAT tu sau ra truoc
    {
        // Display
        for (delay_layer=1; delay_layer<delay; delay_layer++) //Delay MAT
        {
            for(z_count=1; z_count<=5; z_count++) //Dich 5 HANG tu duoi len
            {
                for(delay_x=1; delay_x<4; delay_x++) //Delay HANG
                {
                    quetLayer(z_count, *(x1+z_count-1), *(ic1+z_count-1), *(ic2+z_count-1), *(ic3+z_count-1));
                    quetLayer(z_count, 0, 0, 0, 0);
                }
            }
        }

        // Update data before moving
        for (z_count=0; z_count<5; z_count++) // Quet 5 phan tu mang ic3[] ung voi 5 tang
        {
            // DINH BIT
            for (bit_count=1; bit_count<=5; bit_count++) // Dich 5 bit
            {
                //x1 = ic1(0)
                *(x1+z_count) = *(ic1+z_count) & 0x01;
                //ic1 >>
                *(ic1+z_count) >>= 1;
                //ic1(7) = ic2(0)
                *(ic1+z_count) |= (*(ic2+z_count) & 0x01)<<7;
                //ic2 >>
                *(ic2+z_count) >>= 1;
                //ic2(7) = ic3(0)
                *(ic2+z_count) |= (*(ic3+z_count) & 0x01)<<7;
                //ic3 >>
                *(ic3+z_count) >>= 1;
            }
        }
    }
}
/*
// DO NOT USE AT THIS TIME
void quetMangTruocSau(uint8_t* x1, uint8_t* ic1, uint8_t delay)
{
   uint8_t delay_x, z_count, delay_layer, y_count, bit_count;
   uint8_t ic2[] = {0x00, 0x00, 0x00, 0x00, 0x00};
   uint8_t ic3[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    for (y_count=1; y_count<=5; y_count++) //Dich 5 MAT tu sau ra truoc
    {
        // Display
        for (delay_layer=1; delay_layer<delay; delay_layer++) //Delay MAT
        {
            for(z_count=1; z_count<=5; z_count++) //Dich 5 HANG tu duoi len
            {
                for(delay_x=1; delay_x<4; delay_x++) //Delay HANG
                {
                    quetLayer(z_count, *(x1+z_count-1), *(ic1+z_count-1), *(ic2+z_count-1), *(ic3+z_count-1));
                    quetLayer(z_count, 0, 0, 0, 0);
                }
            }
        }

        // Update data before moving
        for (z_count=0; z_count<5; z_count++) // Quet 5 phan tu mang ic1[] ung voi 5 tang
        {
            // DINH BIT
            for (bit_count=1; bit_count<=5; bit_count++) // Dich 5 bit
            {
                //ic3 <<
                *(ic3+z_count) <<= 1;
                //ic3(0) = ic2(7)
                *(ic3+z_count) |= (*(ic2+z_count) & 0x80)>>7;
                //ic2 <<
                *(ic2+z_count) <<= 1;
                //ic2(0) = ic1(7)
                *(ic2+z_count) |= (*(ic1+z_count) & 0x80)>>7;
                //ic1 <<
                *(ic1+z_count) <<= 1;
                //ic1(0) = x1
                *(ic1+z_count) |= (*(x1+z_count) & 0x01);
                //x1 = 0
                *(x1+z_count) = 0;
            }
        }
   }
}
*/

void xuatKituSauTruoc(char character)
{
    uint8_t buffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    GetBufferFromCharacter(character, buffer);
    quetMangSauTruoc(buffer, 20);
}

void quetMangQuay(uint8_t* ic2, uint8_t delay)
{
    uint8_t delay_x, z_count, delay_layer, rotate_count;
    uint8_t  x1[] = {   0,    0,    0,    0,    0};
    uint8_t ic1[] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t ic3[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    for (rotate_count=1; rotate_count<5; rotate_count++) //4 lan xoay -> Quay
    {
        // Display
        for (delay_layer=1; delay_layer<delay; delay_layer++) //Delay MAT
        {
            for(z_count=1; z_count<=5; z_count++) //Dich 5 HANG tu duoi len
            {
                for(delay_x=1; delay_x<=4; delay_x++) //Delay HANG
                {
                    quetLayer(z_count, *(x1+z_count-1), *(ic1+z_count-1), *(ic2+z_count-1), *(ic3+z_count-1));
                    quetLayer(z_count, 0, 0, 0, 0);
                }
            }
        }

        // Update data before moving
        switch(rotate_count)
        {
            case 1:
            {
                for (z_count=0; z_count<5; z_count++) // Quet 5 phan tu mang ic2[] ung voi 5 tang
                {
                    // CHUYEN BIT
                    // ic2(1) -> x1
                    *(x1+z_count) = (*(ic2+z_count) & 0x02)>>1;
                    *(ic2+z_count) &= ~0x02;
                   // ic2[2] -> ic1(5)
                    *(ic1+z_count) &= ~0x20;
                    *(ic1+z_count) |= (*(ic2+z_count) & 0x04)<<3;
                    *(ic2+z_count) &= ~0x04;
                    // ic2(4) -> ic3(1)
                    *(ic3+z_count) &= ~0x02;
                    *(ic3+z_count) |= (*(ic2+z_count) & 0x10)>>3;
                    *(ic2+z_count) &= ~0x10;
                    // ic2(5) -> ic3(7)
                    *(ic3+z_count) &= ~0x80;
                    *(ic3+z_count) |= (*(ic2+z_count) & 0x20)<<2;
                    *(ic2+z_count) &= ~0x20;
                }
                break;
            }
            case 2:
            {
                for (z_count=0; z_count<5; z_count++)
                {
                    // CHUYEN BIT
                    // ic1(5) -> ic1(6)
                    *(ic1+z_count) |= (*(ic1+z_count) & 0x20)<<1;
                    *(ic1+z_count) &= ~0x20;
                    // x1 -> ic1(1)
                    *(ic1+z_count) |= (*(x1+z_count) & 0x01)<<1;
                    *(x1+z_count) = 0;
                    // ic3(1) -> ic3(0)
                    *(ic3+z_count) |= (*(ic3+z_count) & 0x02)>>1;
                    *(ic3+z_count) &= ~0x02;
                    // ic3(7) -> ic3(5)
                    *(ic3+z_count) |= (*(ic3+z_count) & 0x80)>>2;
                    *(ic3+z_count) &= ~0x80;
                }
                break;
            }
            case 3:
            {
                for (z_count=0; z_count<5; z_count++)
                {
                    // CHUYEN BIT
                    // ic1(6) -> ic1(7)
                    *(ic1+z_count) |= (*(ic1+z_count) & 0x40)<<1;
                    *(ic1+z_count) &= ~0x40;
                    // ic1[1] -> ic1[3]
                    *(ic1+z_count) |= (*(ic1+z_count) & 0x02)<<2;
                    *(ic1+z_count) &= ~0x02;
                    // ic3(0) -> ic2(7)
                    *(ic2+z_count) |= (*(ic3+z_count) & 0x01)<<7;
                    *(ic3+z_count) &= ~0x01;
                    // ic3(5) -> ic3(3)
                    *(ic3+z_count) |= (*(ic3+z_count) & 0x20)>>2;
                    *(ic3+z_count) &= ~0x20;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void xuatKituQuay(char character)
{
    uint8_t buffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    GetBufferFromCharacterSpin(character, buffer);
    quetMangQuay(buffer, 20);
}


// GLOBAL FUNCTION
void LineIncrease(enum LINE line, enum DIRECTION direction)
{
   uint8_t xDir = 0, yDir = 0, zDir = 0, brightness = 0;

   for (xDir = 0; xDir < 5; xDir++)
   {
      for (yDir = 0; yDir < 5; yDir++)
      {
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            for (zDir = 0; zDir < 5; zDir++)
            {
               if (line == X_LINE && direction == Y_DIR)
               {
                  quetPoint(zDir, yDir, xDir);
               }
               else if (line == X_LINE && direction == Z_DIR)
               {
                  quetPoint(zDir, xDir, yDir);
               }
               else if (line == Y_LINE && direction == X_DIR)
               {
                  quetPoint(yDir, zDir, xDir);
               }
               else if (line == Y_LINE && direction == Z_DIR)
               {
                  quetPoint(xDir, zDir, yDir);
               }
               else if (line == Z_LINE && direction == X_DIR)
               {
                  quetPoint(yDir, xDir, zDir);
               }
               else if (line == Z_LINE && direction == Y_DIR)
               {
                  quetPoint(xDir, yDir, zDir);
               }
               else
               {
                  // Do nothing
               }
            }
         }
      }
   }
}

void LineDecrease(enum LINE line, enum DIRECTION direction)
{
   uint8_t xDir = 0, yDir = 0, zDir = 0, brightness = 0;

   for (xDir = 5; xDir > 0;)
   {
      xDir--;
      for (yDir = 5; yDir > 0;)
      {
         yDir--;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            for (zDir = 0; zDir < 5; zDir++)
            {
               if (line == X_LINE && direction == Y_DIR)
               {
                  quetPoint(zDir, yDir, xDir);
               }
               else if (line == X_LINE && direction == Z_DIR)
               {
                  quetPoint(zDir, xDir, yDir);
               }
               else if (line == Y_LINE && direction == X_DIR)
               {
                  quetPoint(yDir, zDir, xDir);
               }
               else if (line == Y_LINE && direction == Z_DIR)
               {
                  quetPoint(xDir, zDir, yDir);
               }
               else if (line == Z_LINE && direction == X_DIR)
               {
                  quetPoint(yDir, xDir, zDir);
               }
               else if (line == Z_LINE && direction == Y_DIR)
               {
                  quetPoint(xDir, yDir, zDir);
               }
               else
               {
                  // Do nothing
               }
            }
         }
      }
   }
}

void LayerIncrease(enum DIRECTION direction)
{
   uint8_t xDir = 0, yDir = 0, zDir = 0, brightness = 0;

   for (zDir = 0; zDir <= 4; zDir++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (xDir = 0; xDir <= 4; xDir++)
         {
            for (yDir = 0; yDir <= 4; yDir++)
            {
               if (direction == X_DIR)
               {
                  quetPoint(zDir, xDir, yDir);
               }
               else if (direction == Y_DIR)
               {
                  quetPoint(yDir, zDir, xDir);
               }
               else if (direction == Z_DIR)
               {
                  quetPoint(xDir, yDir, zDir);
               }
               else
               {
                  // Do nothing
               }
            }
         }
      }
   }
}

void LayerDecrease(enum DIRECTION direction)
{
   uint8_t xDir = 0, yDir = 0, zDir = 0, brightness = 0;

   for (zDir = 5; zDir > 0;)
   {
      zDir--;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (xDir = 0; xDir <= 4; xDir++)
         {
            for (yDir = 0; yDir <= 4; yDir++)
            {
               if (direction == X_DIR)
               {
                  quetPoint(zDir, xDir, yDir);
               }
               else if (direction == Y_DIR)
               {
                  quetPoint(yDir, zDir, xDir);
               }
               else if (direction == Z_DIR)
               {
                  quetPoint(xDir, yDir, zDir);
               }
               else
               {
                  // Do nothing
               }
            }
         }
      }
   }
}

void BeginToEndPoint(void)
{
   uint8_t x = 0, y = 0, z= 0;
   for (z = 0; z <= 4; z++)
   {
      for (y = 0; y <= 4; y++)
      {
         for (x = 0; x <= 4; x++)
         {
            quetPoint(x, y, z);
            _delay_ms(50);
         }
      }
   }
}

void EndToBeginPoint(void)
{
   uint8_t x = 5, y = 5, z= 5;
   for (z = 5; z > 0;)
   {
      z--;
      for (y = 5; y > 0;)
      {
         y--;
         for (x = 5; x > 0;)
         {
            x--;
            quetPoint(x, y, z);
            _delay_ms(50);
         }
      }
   }
}

void BlindAllLed(void)
{
   uint8_t i = 0, j = 1;
   for (i=0; i<BRIGHTNESS_ALL; i++)
   {
      for (j=1; j<=5; j++)
      {
         quetLayer(j, 1, 0xff, 0xff, 0xff);
      }
   }

   for (i=0; i<BRIGHTNESS_ALL; i++)
   {
      for (j=1; j<=5; j++)
      {
         quetLayer(j, 0, 0x00, 0x00, 0x00);
      }
   }
}

void LanToaDuoiLen(void)
{
   uint8_t z = 0;
   for(z=0; z<5; z++)
   {
      lanToaLayer(z);
   }
}

void LanToaTrenXuong(void)
{
   uint8_t z = 4;
   for(z=5; z>0;)
   {
      z--;
      lanToaLayer(z);
   }
}

void LanToaHet(void)
{
   uint8_t r = 0, x = 0, y = 0, z = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=0; r<4; r++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) >= (r*r-1) &&
                     ((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= (r*r+1))
               {
                  for (z=0; z<5; z++)
                  {
                     quetPoint(x, y, z);
                  }
               }
            }
         }
      }
   }
}

void ThuNhoDuoiLen(void)
{
   uint8_t z = 0;
   for(z=0; z<5; z++)
   {
      thuNhoLayer(z);
   }
}

void ThuNhoTrenXuong(void)
{
   uint8_t z = 5;
   for(z=5; z>0;)
   {
      z--;
      thuNhoLayer(z);
   }
}

void ThuNhoHet(void)
{
   uint8_t r = 0, x = 0, y = 0, z = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=4; r>0;)
   {
      r--;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) >= (r*r-1) &&
                     ((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= (r*r+1))
               {
                  for (z=0; z<5; z++)
                  {
                     quetPoint(x, y, z);
                  }
               }
            }
         }
      }
   }
}

void LanToaPhuDayDuoiLen(void)
{
   uint8_t z = 0;
   for(z=0; z<5; z++)
   {
      lanToaPhuDay(z);
   }
}

void LanToaPhuDayTrenXuong(void)
{
   uint8_t z = 5;
   for(z=5; z>0;)
   {
      z--;
      lanToaPhuDay(z);
   }
}
/*
void LanToaPhuDayHet(void)
{
   uint8_t r = 0, x = 0, y = 0, z = 0, xCen = 2, yCen = 2, brightness = 0;

   for (r=0; r<4; r++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER_FULL; brightness++)
      {
         for (x=0; x<5; x++)
         {
            for (y=0; y<5; y++)
            {
               if (((x-xCen)*(x-xCen) + (y-yCen)*(y-yCen)) <= r*r)
               {
                  for (z=0; z<5; z++)
                  {
                     quetPoint(x, y, z);
                  }
               }
            }
         }
      }
   }
}
*/
void LanToaPhuDayHet()
{
    uint8_t i, j, k;
    uint8_t  ma[]={    0,    0,    0,    0,    1,       1,    1,    1,    1,    0};//ma led
    uint8_t ma1[]={0x00, 0x40, 0xE2, 0xF7, 0xFF, 0xFF, 0xBF, 0x1D, 0x08, 0x00};//ma led
    uint8_t ma2[]={0x08, 0x1C, 0xBE, 0xFF, 0xFF, 0xF7, 0xE3, 0x41, 0x00, 0x00};//ma led
    uint8_t ma3[]={0x00, 0x01, 0x23, 0x77, 0xFF, 0xFF, 0xFE, 0xDC, 0x88, 0x00};//ma led

    for(i=0;i<10;i++)
    {
        for(j=1; j<=100; j++)
        {
            for(k=1; k<=5; k++)
            {
                quetLayer(k, ma[i], ma1[i], ma2[i], ma3[i]);
            }
        }
    }
}

void QuetXdYdDuoiLen(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (z=0; z<5; z++)
   {
      for (b=0; b<9; b++)
      {
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXdYdTrenXuong(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=0; b<9; b++)
      {
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXdYdZ(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (b=0; b<9; b++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXaYaDuoiLen(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (z=0; z<5; z++)
   {
      for (b=9; b>0;)
      {
         b--;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXaYaTrenXuong(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=9; b>0;)
      {
         b--;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXaYaZ(void)
{
   uint8_t brightness = 0, b = 0, z = 0;

   for (b=9; b>0;)
   {
      b--;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(-1, b, z);
         }
      }
   }
}

void QuetXdYaDuoiLen(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (z=0; z<5; z++)
   {
      for (b=5; b>-4;)
      {
         b--;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(1, b, z);
         }
      }
   }
}

void QuetXdYaTrenXuong(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=5; b>-4;)
      {
         b--;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(1, b, z);
         }
      }
   }
}

void QuetXdYaZ(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (b=5; b>-4;)
   {
      b--;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(1, b, z);
         }
      }
   }
}

void QuetXaYdDuoiLen(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (z=0; z<5; z++)
   {
      for (b=-4; b<5; b++)
      {
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(1, b, z);
         }
      }
   }
}

void QuetXaYdTrenXuong(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=-4; b<5; b++)
      {
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(1, b, z);
         }
      }
   }
}

void QuetXaYdZ(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t b = 0;

   for (b=-4; b<5; b++)
   {
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(1, b, z);
         }
      }
   }
}

void XoayTronDuoiLen(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (z=0; z<5; z++)
   {
      for (b=10; b>-9;)
      {
         b-=2;
         // y = kx + b
         k = (2-b)/2;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void XoayTronTrenXuong(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=10; b>-9;)
      {
         b-=2;
         // y = kx + b
         k = (2-b)/2;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void XoayTronHet(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (b=10; b>-9;)
   {
      b-=2;
      // y = kx + b
      k = (2-b)/2;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void XoayTronNguocDuoiLen(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (z=0; z<5; z++)
   {
      for (b=-9; b<10;)
      {
         b+=2;
         // y = kx + b
         k = (2-b)/2;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void XoayTronNguocTrenXuong(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (z=5; z>0;)
   {
      z--;
      for (b=-9; b<10;)
      {
         b+=2;
         // y = kx + b
         k = (2-b)/2;
         for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void XoayTronNguocHet(void)
{
   uint8_t brightness = 0, z = 0;
   int8_t k = 0, b = 0;

   for (b=-9; b<10;)
   {
      b+=2;
      // y = kx + b
      k = (2-b)/2;
      for (brightness = 0; brightness <= BRIGHTNESS_LAYER; brightness++)
      {
         for (z=0; z<5; z++)
         {
            line(k, 2-k*2, z); // Re-calculate b
         }
      }
   }
}

void MuaRoi(void)
{
    uint8_t i, j, k;
    uint8_t  ma[]={   0,    0,    0,    0,    1,    0,    1,    0,    1,    0,    1,    0};//ma led
    uint8_t ma1[]={0x00, 0x00, 0x00, 0x10, 0x80, 0x14, 0x64, 0xD9, 0x26, 0x59, 0x02, 0x00};//ma led
    uint8_t ma2[]={0x00, 0x40, 0x02, 0x50, 0x0A, 0xE8, 0x93, 0x64, 0x0B, 0x00, 0x00, 0x00};//ma led
    uint8_t ma3[]={0x08, 0x40, 0x2A, 0x81, 0x7C, 0xA2, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00};//ma led

    for(i=0; i<12; i++)
    {
        for(k=5; k>0; k--)
        {
            for(j=1; j<=100; j++)
            {
                quetLayer(k+4, ma[i], ma1[i], ma2[i], ma3[i]);
                quetLayer(k+2, ma[i], ma1[i], ma2[i], ma3[i]);
                quetLayer(  k, ma[i], ma1[i], ma2[i], ma3[i]);
            }
        }
    }
}
/*
void PhaoHoa(void)
{
   uint8_t i = 0, x = 0, y = 0, z = 0;

   for (i=0; i<5; i++)
   {
      x = rand()%5;
      y = rand()%5;
      z = rand()%5;

      bungNo(x, y, z);
   }
}
*/
void PhaoHoa(void)
{
    uint8_t i = 0;
    // Bay len
    for (i=1; i<5; i++)
    {
        quetLayer(i, 0, 0x00, 0x02, 0);
        _delay_ms(100);
    }
    quetLayer(5, 0, 0x00, 0x04, 0);
    _delay_ms(100);

    //Roi xuong
    for (i=4; i>2; i--)
    {
        quetLayer(i, 0, 0x00, 0x08, 0);
        _delay_ms(100);
    }

    bungNo();
}

void SongNuoc(void)
{
    uint8_t i, j, k;
    uint8_t  ma[5]={   1,    0,    0,    0,    0};//ma led
    uint8_t ma1[5]={0x10, 0x21, 0x42, 0x84, 0x08};//ma led
    uint8_t ma2[5]={0x42, 0x84, 0x08, 0x10, 0x21};//ma led
    uint8_t ma3[5]={0x08, 0x10, 0x21, 0x42, 0x84};//ma led

    uint8_t kh[30]={4,4,3,2,2,
                    3,4,4,3,2,
                    2,3,4,4,3,
                    2,2,3,4,4,
                    3,2,2,3,4,
                    4,3,2,2,3};//ma layer

    for(k=0; k<30; k = k+5 ) // 12 khung hinh chuyen dong
    {
        for(j=1; j<10; j++ ) //Delay
        {
            for (i=0; i<5; i++) // quet trai sang phai 5 dong
            {
                quetLayer(kh[k+i], ma[i], ma1[i], ma2[i], ma3[i]);
                _delay_ms(2);
            }
        }
    }
}

void RoiDay(void)
{
    uint8_t i, k, l;
    uint16_t j;

    // Lap day
    for(k=0; k<5; k++) // So tang giu sang co dinh
    {
        for (i=5; i>k; i--) // Tang 5 4 3 2 1
        {
            for(j=1; j<=BRIGHTNESS_ALL/(k+1); j++) // Delay
            {
                for(l=0; l<=k; l++) // Sang cac tang co dinh
                {
                    quetLayer(l, 1, 0xff, 0xff, 0xff);
                }
                quetLayer(i, 1, 0xff, 0xff, 0xff);
            }
        }
    }

    // Xoa dan
    for(k=4; k>0; k--) // So tang giu sang co dinh
    {
        for (i=(5-k); i>0; i--) // Tang 5 4 3 2 1
        {
            for(j=1; j<BRIGHTNESS_ALL; j++) // Delay
            {
                for(l=5; l>(5-k); l--) // Sang cac tang co dinh
                {
                    quetLayer(l, 1, 0xff, 0xff, 0xff);
                }
                quetLayer(i, 1, 0xff, 0xff, 0xff);
            }
        }
    }
}

void XoayOc(uint8_t layer)
{
    uint8_t i;
    uint8_t x[]={0,1,2,3,4,4,4,4,4,3,2,1,0,0,0,0,1,2,3,3,3,2,1,1,2};
    uint8_t y[]={0,0,0,0,0,1,2,3,4,4,4,4,4,3,2,1,1,1,1,2,3,3,3,2,2};
    for(i=0; i<25; i++)
    {
        quetPoint(x[i], y[i], layer-1);
        _delay_ms(50);
    }
}
/*
void XoayOc_full(uint8_t layer)
{
    uint8_t i, j, brightness;
    uint8_t x[]={0,1,2,3,4,4,4,4,4,3,2,1,0,0,0,0,1,2,3,3,3,2,1,1,2};
    uint8_t y[]={0,0,0,0,0,1,2,3,4,4,4,4,4,3,2,1,1,1,1,2,3,3,3,2,2};
    for(i=0; i<25; i++)
    {
        for (brightness = 0; brightness <= BRIGHTNESS_LINE; brightness++)
        {
            for(j=0; j<=i; j++)
            {
                quetPoint(x[j], y[j], layer-1);
            }
        }
    }
}
*/
void XoayOc_full(uint8_t layer)
{
    uint8_t i;
    uint8_t  ma[]={   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1};//ma led
    uint8_t ma1[]={0x00, 0x01, 0x03, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};//ma led
    uint8_t ma2[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x61, 0x63, 0x63, 0x63, 0x63, 0x63, 0x73, 0x73, 0x73, 0xf3, 0xf7, 0xff};//ma led
    uint8_t ma3[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x84, 0xC4, 0xE4, 0xF4, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xfc, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff};//ma led

    for(i=0; i<25; i++)
    {
        quetLayer(layer, ma[i], ma1[i], ma2[i], ma3[i]);
        _delay_ms(50);
    }
}

void XoayOc_blank(uint8_t layer)
{
    uint8_t i;
    uint8_t  ma[]={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};//ma led
    uint8_t ma1[]={0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//ma led
    uint8_t ma2[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0x9e, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x8c, 0x8c, 0x8c, 0x0c, 0x08, 0x00};//ma led
    uint8_t ma3[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x7b, 0x3b, 0x1b, 0x0b, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00};//ma led

    for(i=0; i<25; i++)
    {
        quetLayer(layer, ma[i], ma1[i], ma2[i], ma3[i]);
        _delay_ms(50);
    }
}

void XoayOc_fullZ(void)
{
    uint8_t i, j, k;
    uint8_t  ma[]={   1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1};//ma led
    uint8_t ma1[]={0x00, 0x01, 0x03, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};//ma led
    uint8_t ma2[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x61, 0x63, 0x63, 0x63, 0x63, 0x63, 0x73, 0x73, 0x73, 0xf3, 0xf7, 0xff};//ma led
    uint8_t ma3[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x84, 0xC4, 0xE4, 0xF4, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xfc, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff};//ma led

    for(i=0; i<25; i++)
    {
        for(j=1; j<=100; j++)
        {
            for(k=1; k<=5; k++)
            {
                quetLayer(k, ma[i], ma1[i], ma2[i], ma3[i]);
            }
        }
    }
}

void XoayOc_blankZ()
{
    uint8_t i, j, k;
    uint8_t  ma[]={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};//ma led
    uint8_t ma1[]={0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//ma led
    uint8_t ma2[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0x9e, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x8c, 0x8c, 0x8c, 0x0c, 0x08, 0x00};//ma led
    uint8_t ma3[]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x7b, 0x3b, 0x1b, 0x0b, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00};//ma led

    for(i=0; i<25; i++)
    {
        for(j=1; j<=100; j++)
        {
            for(k=1; k<=5; k++)
            {
                quetLayer(k, ma[i], ma1[i], ma2[i], ma3[i]);
            }
        }
    }
}

void Christmas(void)
{
    uint8_t i;
    uint8_t k[]={1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,5};//ma led
    uint8_t x[]={0,1,2,3,4,4,4,4,4,3,2,1,0,0,0,0,1,2,3,3,3,2,1,1,2};
    uint8_t y[]={0,0,0,0,0,1,2,3,4,4,4,4,4,3,2,1,1,1,1,2,3,3,3,2,2};
    for(i=0; i<25; i++)
    {
        quetPoint(x[i], y[i], k[i]-1);
        _delay_ms(30);
    }
}

void XuatChuSauTruoc(char *word)
{
    uint8_t i = 0;
    while(*(word+i))
    {
        xuatKituSauTruoc(*(word+i));
        i++;
    }
}

void XuatChuQuay(char *word)
{
    uint8_t i = 0, j = 0;
    while(*(word+i))
    {
        for(j=0; j<2; j++)
        {
            xuatKituQuay(*(word+i));
        }
        i++;
    }
}
