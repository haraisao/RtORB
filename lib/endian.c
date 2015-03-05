/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB:
 *    Simple GIOP Implementation for x86 (Little Endian)
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: endian.c,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *  
 */
/*!
 * @file endian.c
 * @if jp
 * @brief ByteOrder(Endian)の入れ替え処理を行う。
 * @else
 * @brief ByteOrder(Endian) action.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

/******* ByteOrder ************/
void swapByteOrder(unsigned char *dist, unsigned char *source, int size){
  unsigned char *in = source;
  unsigned char *out = dist + size - 1;
  while(size > 0){ *out-- = *in++; size--;}
  return;
}

void swapByteOrderInOctet(char *buf, int size){
  char *out = (char *)RtORB_alloc(size, "swapByteOrderInOctet");
  swapByteOrder((unsigned char *)out, (unsigned char *)buf, size);
  memcpy(buf, out, size);
  RtORB_free(out, "swapByteOrderInOctet");
  return;
}

short reverseOrderShort(short v){
  short r;
  unsigned char in[DataLen_short], out[DataLen_short];

  memcpy(in, &v, DataLen_short);
  swapByteOrder(out, in, DataLen_short);
  memcpy(&r, out, DataLen_short);
  return r;
}

int32_t reverseOrderLong(int32_t v){
  int32_t r;
  unsigned char in[DataLen_long], out[DataLen_long];

  memcpy(in, &v, DataLen_long);
  swapByteOrder(out, in, DataLen_long);
  memcpy(&r, out, DataLen_long);
  return r;
}

int64_t reverseOrderLongLong(int64_t v){
  int64_t r;
  unsigned char in[DataLen_longlong], out[DataLen_longlong];

  memcpy(in, &v, DataLen_longlong);
  swapByteOrder(out, in, DataLen_longlong);
  memcpy(&r, out, DataLen_longlong);
  return r;
}

float reverseOrderFloat(float v){
  float r;
  unsigned char in[DataLen_float], out[DataLen_float];

  memcpy(in, &v, DataLen_float);
  swapByteOrder(out, in, DataLen_float);
  memcpy(&r, out, DataLen_float);
  return r;
}

double reverseOrderDouble(double v){
  double r;
  unsigned char in[DataLen_double], out[DataLen_double];

  memcpy(in, &v, DataLen_double);
  swapByteOrder(out, in, DataLen_double);
  memcpy(&r, out, DataLen_double);
  return r;
}

long double reverseOrderLongDouble(long double v){
  long double r;
  unsigned char in[DataLen_longdouble], out[DataLen_longdouble];

  memcpy(in, &v, DataLen_longdouble);
  swapByteOrder(out, in, DataLen_longdouble);
  memcpy(&r, out, DataLen_longdouble);

  return r;
}


