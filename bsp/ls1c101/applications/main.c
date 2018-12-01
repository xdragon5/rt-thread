/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2008 - 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-10     zhuangwei    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "ls1c101_gpio.h"

int main(int argc, char** argv)
{   
	while(1)
	{
		rt_pin_write(GPIO_PIN_34, GPIO_HIGH);
		rt_thread_delay(RT_TICK_PER_SECOND);
		rt_pin_write(GPIO_PIN_34, GPIO_LOW);
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
    return 0;
}
