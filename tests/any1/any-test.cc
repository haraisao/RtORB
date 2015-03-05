/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//#include <RtORB/corba.hh>
#include <stdio.h>
#include "AnyTest.h"

static void char_test()
{
	printf("char_test\n");
	CORBA::Any any;
	any <<= CORBA::Any::from_char('A');

	CORBA::Char c;
	if (any >>= CORBA::Any::to_char(c) ) {
		printf("c = %c\n", c);
	} else {
		printf("failed\n");
		return;
	}
}

static void octet_test()
{
	printf("octet_test\n");
		       
	CORBA::Any any;
	any <<= CORBA::Any::from_octet(1);

	CORBA::Octet c = 0;
	if (any >>= CORBA::Any::to_octet(c)) {
		printf("c = %d\n", c);
	} else {
		printf("failed\n");
		return;
	}
}

static void boolean_test()
{
	printf("boolean_test\n");
	CORBA::Any any;

	any <<= CORBA::Any::from_boolean(true);

	CORBA::Boolean b = false;
	if (any >>= b) {
		printf("c = %d\n", b);
	} else {
		printf("failed\n");
		return;
	}
}

static void string_test()
{
	printf("string_test\n");
	CORBA::Any any;
	char *msg = "Hello World";
	any <<= CORBA::Any::from_string(msg);

	{
		char *buf;
		if (any >>= buf) {
			printf("c = %s\n", buf);
		} else {
			printf("failed\n");
			return;
		}
	}
	{
		CORBA::Any o;
		o = any;
		char *buf;
		if (o >>= buf) {
			printf("c = %s\n", buf);
		} else {
			printf("copy failed\n");
			return;
		}
		o = any;
	}
}

//void operator<<=(CORBA::Any &any, const AnyTest::StructTest &);
//CORBA::Boolean operator>>=(const CORBA::Any &any, AnyTest::StructTest *);

static void struct_test()
{
  printf("struct_test\n");
  
  CORBA::Any any;

  {
    AnyTest::StructTest st;
    st.lnum = 10;
  
    st.lseq.length(2);
    st.lseq[0] = 10;
    st.lseq[1] = 20;

    any <<= st;
  }

  {
    AnyTest::StructTest *st = NULL;
    if (any >>= st) {
      printf("lnum = %ld\n", st->lnum);
      for (int i=0; i<st->lseq.length(); i++) {
	printf("lseq[%d] = %ld\n", i, st->lseq[i]);
      }
	
      printf("OK.\n");
    } else {
      printf("FAILED.\n");
    }
  }
}

int main()
{
	printf("sizeof(CORBA_any) = %d\n", sizeof(CORBA_any));
	char_test();
	octet_test();
	boolean_test();

	string_test();

	struct_test();
	return 0;
}

