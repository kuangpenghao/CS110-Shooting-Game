/*!
    \file  systick.c
    \brief the systick configuration file

    \version 2019-6-5, V1.0.0, firmware for GD32VF103
*/

/*
    版权所有 (c) 2019, GigaDevice Semiconductor Inc.

    在满足以下条件的情况下，允许以源代码和二进制形式重新分发和使用，无论是否经过修改：

    1. 源代码的重新分发必须保留上述版权声明、本条件列表以及以下免责声明。
    2. 二进制形式的重新分发必须在随附的文档和/或其他提供的材料中包含上述版权声明、 
       本条件列表以及以下免责声明。
    3. 未经特定事先书面许可，版权持有者的名称或其贡献者的名称不得用于支持或推广从本软件派生的产品。

    本软件由版权持有者和贡献者“按原样”提供，不提供任何明示或暗示的担保，包括但不限于对适销性和适用性
    的暗示担保。在任何情况下，版权持有者或贡献者均不对任何直接、间接、附带、特殊、惩罚性或后果性损害
    （包括但不限于替代商品或服务的采购；使用、数据或利润的丧失；或业务中断）负责，无论是如何引起的，
    无论是基于合同责任、严格责任还是侵权行为（包括疏忽或其他原因），即使已被告知可能发生此类损害。
*/

#include "gd32vf103.h"
#include "systick.h"

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
	uint64_t start_mtime, delta_mtime;

	// Don't start measuruing until we see an mtime tick
	uint64_t tmp = get_timer_value();
	do {
	start_mtime = get_timer_value();
	} while (start_mtime == tmp);

	do {
	delta_mtime = get_timer_value() - start_mtime;
	}while(delta_mtime <(SystemCoreClock/4000.0 *count ));
}
