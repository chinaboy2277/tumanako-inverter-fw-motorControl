/*
 * This file is part of the tumanako_vc project.
 *
 * Copyright (C) 2010 Johannes Huebner <contact@johanneshuebner.com>
 * Copyright (C) 2010 Edward Cheeseman <cheesemanedward@gmail.com>
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "digio.h"

#define DIG_IO_OFF 0
#define DIG_IO_ON  1

struct DigIo
{
   u32 port;
   u16 pin;
   DIG_IO_MODE mode;
};

#define DIG_IO_ENTRY(name, port, pin, mode) { port, pin, mode },
static const struct DigIo ios[] =
{
   DIG_IO_LIST
   { 0, 0, MODE_LAST }
};

void digio_init(void)
{
   const struct DigIo *pCur;

   for (pCur = ios; pCur->mode != MODE_LAST; pCur++)
   {
      u8 mode = GPIO_MODE_INPUT;
      u8 cnf = GPIO_CNF_INPUT_PULL_UPDOWN;
      u16 val = DIG_IO_OFF;

      switch (pCur->mode)
      {
         default:
         case MODE_INPUT_PD:
            /* use defaults */
            break;
         case MODE_INPUT_PU:
            val = DIG_IO_ON;
            break;
         case MODE_INPUT_FLT:
            cnf = GPIO_CNF_INPUT_FLOAT;
            break;
         case MODE_INPUT_AIN:
            cnf = GPIO_CNF_INPUT_ANALOG;
            break;
         case MODE_OUTPUT:
            mode = GPIO_MODE_OUTPUT_50_MHZ;
            cnf = GPIO_CNF_OUTPUT_PUSHPULL;
            break;
      }

      gpio_set_mode(pCur->port, mode, cnf, pCur->pin);
      if (DIG_IO_ON == val)
      {
         gpio_set(pCur->port, pCur->pin);
      }
   }
}

u16 digio_get(enum DigIos io)
{
   const struct DigIo *pIo = ios + io;
   return (gpio_get(pIo->port, pIo->pin) & pIo->pin) > 0;
}

void digio_set(enum DigIos io)
{
   const struct DigIo *pIo = ios + io;
   return gpio_set(pIo->port, pIo->pin);
}

void digio_clear(enum DigIos io)
{
   const struct DigIo *pIo = ios + io;
   return gpio_clear(pIo->port, pIo->pin);
}

void digio_toggle(enum DigIos io)
{
   const struct DigIo *pIo = ios + io;
   return gpio_toggle(pIo->port, pIo->pin);
}
