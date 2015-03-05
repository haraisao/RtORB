/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file any_stream.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */
#ifndef __ANY_STREAM_HH__
#define __ANY_STREAM_HH__

namespace CORBA {

  const int32_t CORBA_ANY_STREAM_MAXBUF= MaxMessageSize;  /*!< TODO */
  static char AnyStructStream_buf[1024];              /*!< TODO */

/*!
 * @class AnyStructStream
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/  
  template <class T>
  class AnyStructStream
  {
  public:
    /*!
     * @brief Abstract (TODO)
     * @param any (TODO)
     * @param tc (TODO)
     * @param from_ (TODO)
     * @return void
     */
    static void from(CORBA::Any &any, CORBA_TypeCode tc, const T &from_)
    {
      int pos = 0;
      T *val = const_cast<T*>(&from_);
      marshal_by_typecode((octet*)AnyStructStream_buf, (void*)val, tc, &pos);
      any.setData(tc, AnyStructStream_buf, pos);
    }

    /*!
     * @brief Abstract (TODO)
     * @param any (TODO)
     * @param tc (TODO)
     * @param to_ (TODO)
     * @return (TODO)
     */
    static CORBA::Boolean to(const CORBA::Any &any, CORBA_TypeCode tc, T * &to_)
    {
      if (tc->kind != tk_struct) { return FALSE; }
      if (!any.hasData(tc)) { return FALSE; }

      int pos = 0;

      T * buf = (T*) RtORB_alloc(sizeof(T), "AnyStructStream::to");
      uint32_t ret = demarshal_by_typecode( (void**)buf, tc, (octet*) any._val->val_encoded.data, &pos, RTORB_BYTE_ORDER);
      if (ret > 0) {
	to_ = buf;
      } else {
	RtORB_free(buf, "AnyStructStream::to");
      }
      return ret > 0? TRUE: FALSE;
    }
  };

/*!
 * @class AnyObjectStream
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/  
  class AnyObjectStream
  {
  public:
    /*!
     * @brief Abstract (TODO)
     * @param any (TODO)
     * @param tc (TODO)
     * @param val); (TODO)
     * @return (TODO)
     */
    static void from(CORBA::Any &any, CORBA_TypeCode tc, CORBA_Object val);
  };
}

#endif /* __ANY_STREAM_HH__ */

