/*
  cdrMemoryStream 
*/
/*!
 * @file cdrStream.h
 * @if jp
 * @brief cdrMemoryStream (TODO)
 * @else
 * @brief cdrMemoryStream (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CDRSTREAM_H__
#define __CDRSTREAM_H__

#ifdef __cplusplus


#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <RtORB/corba.h>

#define SWAP16(s) ((((s) & 0xff) << 8) | (((s) >> 8 ) & 0xff))
#define SWAP32(l) ((((l) & 0xff000000) >> 24)  | \
                   (((l) & 0x00ff0000) >> 8 )  | \
                   (((l) & 0x0000ff00) << 8 )  | \
                   (((l) & 0x000000ff) << 24 ) ) 

#define SWAP64(l) ((((l) & 0xff00000000000000) >> 56)  | \
                   (((l) & 0x00ff000000000000) >> 40 )  | \
                   (((l) & 0x0000ff0000000000) >> 24 )  | \
                   (((l) & 0x000000ff00000000) >> 8 )  | \
                   (((l) & 0x00000000ff000000) << 8 )  | \
                   (((l) & 0x0000000000ff0000) << 24 )  | \
                   (((l) & 0x000000000000ff00) << 40 )  | \
                   (((l) & 0x00000000000000ff) << 56 ) ) 

#define IS_DIFFERENT_ENDIAN(s) if(s.byte_order != __CDR_ORDER__)

/*!
 * @union f2l
 * @brief f2l (TODO)
 * @param c (TODO)
 * @param l (TODO)
 * @param f (TODO)
 */
typedef union {
 char c[4];
 int32_t l;
 float f;
} f2l;

/*!
 * @union d2l
 * @brief d2l (TODO)
 * @param c (TODO)
 * @param l (TODO)
 * @param d (TODO)
 */
typedef union {
 char c[8];
 int32_t l[2];
 double d;
} d2l;

/*!
 * @class cdrStream
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/
class cdrStream
{
public:
 cdrStream();
 virtual ~cdrStream();

 void *out_buf;             /*!< (TODO) */
 uint32_t out_len;    /*!< (TODO) */
 uint32_t out_c;      /*!< (TODO) */
 uint32_t byte_order;  /*!< (TODO) */

 int32_t _ref;                  /*!< (TODO) */

#define Marshal(s, type, align, a)  do{\
   int32_t len = sizeof(type); 	\
   s.put_octet_array((char *) a, len, align); 	\
  }while(0)  /*!< (TODO) */

#define Unmarshal(s, type, align, a) do{\
   int32_t len = sizeof(type); 	\
   s.get_octet_array((char *) a, len, align); 	\
 }while(0);  /*!< (TODO) */

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_char a, cdrStream& s){
   Marshal(s, char, 1, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_char& a, cdrStream& s){
   Unmarshal(s, char, 1, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_unsigned_char a, cdrStream& s){
   Marshal(s, unsigned char, 1, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_unsigned_char& a, cdrStream& s){
   Unmarshal(s,unsigned char, 1, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_short a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP16(a);
   Marshal(s, int16_t, 2, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_short& a, cdrStream& s){
   Unmarshal(s, int16_t, 2, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP16(a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_unsigned_short a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP16(a);
   Marshal(s, uint16_t, 2, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_unsigned_short& a, cdrStream& s){
   Unmarshal(s, uint16_t, 2, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP16(a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_long a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   Marshal(s, int32_t, 4, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_long& a, cdrStream& s){
   Unmarshal(s, int32_t, 4, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_unsigned_long a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   Marshal(s, uint32_t, 4, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_unsigned_long& a, cdrStream& s){
   Unmarshal(s, uint32_t, 4, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   return;
 }

#if defined(Cygwin) && ( __GNUC__ < 4 )
/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(int a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   Marshal(s, int32_t, 4, &a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(int& a, cdrStream& s){
   Unmarshal(s, int32_t, 4, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(unsigned int a, cdrStream& s){
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   Marshal(s, uint32_t, 4, &a);
   return;
 }
/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(unsigned int& a, cdrStream& s){
   Unmarshal(s, uint32_t, 4, &a);
   IS_DIFFERENT_ENDIAN(s) a=SWAP32(a);
   return;
 }

#endif
/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_float f, cdrStream& s){
   f2l a;
   a.f = f;
   IS_DIFFERENT_ENDIAN(s) a.l=SWAP32(a.l);
   Marshal(s, float, 4, &a.l);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_float& f, cdrStream& s){
   f2l a;
   Unmarshal(s, float, 4, &a.l);
   IS_DIFFERENT_ENDIAN(s) a.l=SWAP32(a.l);
   f=a.f;
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_double d, cdrStream& s){
   d2l a;
   int32_t lup, llow;
   a.d = d;
   IS_DIFFERENT_ENDIAN(s){
      lup=SWAP32(a.l[1]);
      llow=SWAP32(a.l[0]);
      a.l[0]=llow;
      a.l[1]=lup;
   }
   Marshal(s, double, 8, a.l);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_double& d, cdrStream& s){
   d2l a;
   int32_t lup, llow;
   Unmarshal(s, double, 8, a.l);
   IS_DIFFERENT_ENDIAN(s){
      lup=SWAP32(a.l[1]);
      llow=SWAP32(a.l[0]);
      a.l[0]=llow;
      a.l[1]=lup;
   }
   d=a.d;
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_string str, cdrStream& s){
   int32_t len=strlen(str)+1;
   Marshal(s, int32_t, 4, &len);
   s.put_octet_array((char *) str, len, 1);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_string& str, cdrStream& s){
   int32_t len;
   char *buf;
   s.get_octet_array((char *)&len, 4, 4);
   buf = (char *)RtORB_alloc(len, "cdrStrem:string");
   s.get_octet_array(buf, len, 1);
   if(str) RtORB_free(str, "cdrStrem:string");
   str = buf;
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_wstring str, cdrStream& s){ /// for GIOP 1.2 or lator
   int32_t len=wcslen((const wchar_t *)str);
   Marshal(s, int32_t, 4, &len);
   s.put_octet_array((char *) str, len*2, 2);
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator<<=(CORBA_wstring& str, cdrStream& s){
   int32_t len;
   int16_t *buf;
   s.get_octet_array((char *)&len, 4, 4);
   buf = (int16_t *)RtORB_alloc(sizeof(int16_t) * (len+1), "cdrStream:wstring");
   s.get_octet_array((char *)buf, len*2, 2);
   if(str) RtORB_free(str, "cdrStrem:string");
   str = (short *)buf;
   return;
 }

/*!
 * @brief (TODO)
 */
 friend inline void operator>>=(CORBA_any any, cdrStream& s){
   s.marshal_CORBA_any(any);
 }

/*!
 * @brief (TODO)
 */
 friend void operator<<=(CORBA_any& any, cdrStream& s){
   if(any._type) CORBA_any_clear(&any);
   any = s.unmarshal_CORBA_any();
 }

/*!
 * @brief (TODO)
 */
 friend void operator>>=(CORBA_TypeCode typ, cdrStream& s){
   s.marshal_CORBA_TypeCode(typ);
 }

/*!
 * @brief (TODO)
 */
 friend void operator<<=(CORBA_TypeCode& typ, cdrStream& s){
  typ = s.unmarshal_CORBA_TypeCode();
 }

/*!
 * @brief (TODO)
 */
 friend void operator>>=(CORBA_Object obj, cdrStream& s){
   s.marshal_CORBA_Object(obj);
 }

/*!
 * @brief (TODO)
 */
 friend void operator<<=(CORBA_Object& obj, cdrStream& s){
   CORBA_Object_free(obj);
   obj = s.unmarshal_CORBA_Object();
 }

/*!
 * @brief Abstract (TODO)
 * @param octet (TODO)
 * @param size (TODO)
 * @param align default=1 (TODO)
 * @return void
 */
 void get_octet_array(char *octet, int32_t size, int32_t align=1);

/*!
 * @brief Abstract (TODO)
 * @param octet (TODO)
 * @param size (TODO)
 * @param align default=1 (TODO)
 * @return void
 */
 void put_octet_array(char *octet, int32_t size, int32_t align=1);

/*!
 * @brief Abstract (TODO)
 * @param buf (TODO)
 * @param size (TODO)
 * @param tc (TODO)
 * @return void
 */
 void marshal_sequence(void *buf, int32_t size, CORBA_TypeCode tc);

/*!
 * @brief Abstract (TODO)
 * @param size (TODO)
 * @param tc (TODO)
 * @return (TODO)
 */
 void *unmarshal_sequence(int32_t size, CORBA_TypeCode tc);

/*!
 * @brief Abstract (TODO)
 * @return void
 */
 void marshal_CORBA_any(CORBA_any);

/*!
 * @brief Abstract (TODO)
 * @return (TODO)
 */
 CORBA_any unmarshal_CORBA_any();

/*!
 * @brief Abstract (TODO)
 * @return void
 */
 void marshal_CORBA_TypeCode(CORBA_TypeCode);

/*!
 * @brief Abstract (TODO)
 * @return (TODO)
 */
 CORBA_TypeCode unmarshal_CORBA_TypeCode();

/*!
 * @brief Abstract (TODO)
 * @return void
 */
 void marshal_CORBA_Object(CORBA_Object);

/*!
 * @brief Abstract (TODO)
 * @return (TODO)
 */
 CORBA_Object unmarshal_CORBA_Object();

};


/*!
 * @class cdrMemoryStream
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/
class cdrMemoryStream: public cdrStream
{
public:

/*!
 * @brief (TODO)
 */
 cdrMemoryStream(int32_t initBufsize=0, int32_t clsMemory=0);

/*!
 * @brief (TODO)
 */
 cdrMemoryStream(const cdrMemoryStream &cms);

/*
 cdrMemoryStream(const cdrMemoryStream &cms, int32_t read_only=0);
 cdrMemoryStream(void *databuffer);
*/

/*!
 * @brief (TODO)
 */
 cdrMemoryStream(void *databuffer, int32_t maxLen);

/*
 virtual ~cdrMemoryStream();
*/
/*
 void rewindInputPtr();
*/
/*!
 * @brief (TODO)
 */
 void rewindPtrs();

/*!
 * @brief (TODO)
 */
 uint32_t bufSize() const;

/*!
 * @brief (TODO)
 */
 void *bufPtr() const;

/*!
 * @brief (TODO)
 */
 void setByteSwapFlag(int32_t byte_order);

/*!
 * @brief (TODO)
 */
 int32_t readOnly(); /* boolean */

// virtual void* ptrToClass(int32_t *cptr);
// void get_octet_array(const char *octet, int32_t size, int32_t align=1);
// void put_octet_array(char *octet, int32_t size, int32_t align=1);

/*!
 * @brief (TODO)
 */
 void skipInput(uint32_t size);

/*
 int32_t  checkInputOverrun(uint32_t itemSize, uint32_t nItems, int32_t align=1);
*/

/*!
 * @brief (TODO)
 */
 int32_t  checkOutputOverrun(uint32_t itemSize, uint32_t nItems, int32_t align=1);

/*
 void  copy_to(cdrStream &cs, int32_t size, int32_t align=1);
 void  fetchInputData(int32_t align, int32_t size);

 int32_t  reseveOutputSpaceForPrimitiveType(int32_t align, int32_t size);
 int32_t  maybeReseveOutputSpace(int32_t align, int32_t size);

 uint32_t currentInputPtr() const;
*/

/*!
 * @brief (TODO)
 */
 uint32_t currentOutputPtr() const;

/*!
 * @brief (TODO)
 */
 cdrMemoryStream& operator=(const cdrMemoryStream &cms);
};

#endif /* end of __cplusplus */

#endif

