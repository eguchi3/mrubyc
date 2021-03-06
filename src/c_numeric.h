/*! @file
  @brief


  <pre>
  Copyright (C) 2015 Kyushu Institute of Technology.
  Copyright (C) 2015 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.


  </pre>
*/

#ifndef MRUBYC_SRC_C_NUMERIC_H_
#define MRUBYC_SRC_C_NUMERIC_H_

#ifdef __cplusplus
extern "C" {
#endif


void mrb_init_class_fixnum(void);

#if MRUBYC_USE_FLOAT
void mrb_init_class_float(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
