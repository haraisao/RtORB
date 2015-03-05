/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */

/*
 *  Sequence:
 *
 *
 */
/*!
 * @file corba-sequence.h
 * @if jp
 * @brief CORBA Sequence関連のデータの扱い用関数群。
 * @else
 * @brief CORBA Sequence data action.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_CORBA_SEQUENCE_H__
#define __FUNC_CORBA_SEQUENCE_H__

/***   CORBA_Sequence ***/
/*!
 * @if jp
 * @brief CORBAタイプコードのデータタイプに応じて、Octetデータサイズの値を返す。
 * @else
 * @brief get Octet data size by data type of CORBA_Type_Code.
 * @endif
 * @param type Type of Type_Code
 * @return octet data size 
 */
uint32_t get_octet_size(uint32_t type);

/*!
 * @if jp
 * @brief CORBAタイプコードのデータタイプに応じて、Sequence型のOctetデータサイズの値を返す。
 * @else
 * @brief get Sequence Octet data size by data type of CORBA_Type_Code.
 * @endif
 * @param seq CORBA_Sequence data
 * @return sequence octet data size 
 */
uint32_t sizeof_CORBA_Sequence(CORBA_Sequence *seq);

/*!
 * @if jp
 * @brief CORBA_Sequence型データ用にメモリ確保し、初期値をセットして新規作成する。
 * @else
 * @brief create CORBA_Sequence data, allocate memory, set default value.
 * @endif
 * @param type Type of Type_Code
 * @param len initial length of Sequence array
 * @return reference of CORBA_Sequence data
 */
CORBA_Sequence *new_CORBA_Sequence(uint32_t type, uint32_t len);

/*!
 * @if jp
 * @brief CORBA_Sequence_Octet型データを複製する。(src --> dist)
 * @else
 * @brief copy CORBA_Sequence_Octet data.
 * @endif
 * @param dist target data for coping
 * @param src reference data for coping
 * @return 1:Copy_OK
 */
int copy_CORBA_Sequence_Octet(CORBA_Sequence_Octet *dist, CORBA_Sequence_Octet *src);

/*!
 * @if jp
 * @brief CORBA_SequenceBase型データ用にメモリ確保し、初期値をセットして新規作成する。
 * @else
 * @brief create CORBA_SequenceBase data, allocate memory, set default value.
 * @endif
 * @param _max max count of sequence array
 * @param _len count of sequence array
 * @return reference of CORBA_SequenceBase data
 */
CORBA_SequenceBase *CORBA_SequenceBase__new(int _max, int _len);

/*!
 * @if jp
 * @brief CORBA_SequenceBase型データの最後尾に新たなデータを追加する。
 * @else
 * @brief add new data to last position of CORBA_SequenceBase data array.
 * @endif
 * @param seq target of CORBA_SequenceBase data
 * @param val data for registration
 * @return void
 */
void CORBA_SequenceBase__add(CORBA_SequenceBase *seq,  void *val);

/*!
 * @if jp
 * @brief CORBA_SequenceBase型データ内に、新たなデータをセットする。既に最大配列長を越えている場合は、メモリの再確保を行い、サイズ変更をする。
 * @else
 * @brief set new data to last position of CORBA_SequenceBase data array. If index of Arrya is bigger than max, re-allocate memory and change size of CORBA_SequenceBase data, after that, set new data.
 * @endif
 * @param seq target of CORBA_SequenceBase data
 * @param idx index of Sequence data
 * @param val data for registration
 * @return void
 */
void CORBA_SequenceBase__set(CORBA_SequenceBase *seq, int idx, void *val);

/*!
 * @if jp
 * @brief CORBA_SequenceBase型データ内の中身のクリア処理を行う。
 * @else
 * @brief clear all contents of CORBA_SequenceBase data.
 * @endif
 * @param seq target of CORBA_SequenceBase data
 * @param _free function pointer for 'free'
 * @return void
 */
void CORBA_SequenceBase__clear(CORBA_SequenceBase *seq, void (*_free)());

/*!
 * @if jp
 * @brief CORBA_SequenceBase型データを削除する。
 * @else
 * @brief delete CORBA_SequenceBase data.
 * @endif
 * @param seq target of CORBA_SequenceBase data
 * @param _free function pointer for 'free'
 * @param flag Flag
 * @return void
 */
void CORBA_SequenceBase__delete(CORBA_SequenceBase *seq, void (*_free)(), int flag);

#endif
