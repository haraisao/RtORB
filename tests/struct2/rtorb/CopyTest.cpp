#include <stdio.h>
#include "Struct2Test.h"

static void dump(Struct2Test::Cmpnt_ptr cmpnt)
{
	int n = cmpnt->num;
	printf("num = %ld\n", n);
	char *p = cmpnt->str;
	printf("str = %s\n", p);
	for (int i=0; i<cmpnt->lseq.length(); i++) {
	  printf("seq[%d] = %d\n", i, cmpnt->lseq[i]);
	}
}

static Struct2Test::Cmpnt * create_cmpnt()
{
	Struct2Test::Cmpnt_var cmpnt = Struct2Test_Cmpnt__alloc();
	cmpnt->num = 1;
	cmpnt->str = (char *) "abc";
	Struct2Test::LongSeq_ptr lseq = cmpnt->lseq;
	lseq.length(3);
	lseq[0] = 4;
	lseq[1] = 5;
	lseq[2] = 6;
	return cmpnt._retn();
}

static int test_create()
{
	Struct2Test::Cmpnt_var cmpnt = create_cmpnt();

	dump(cmpnt);
	return 0;
}

static int test_copy()
{
	Struct2Test::Cmpnt to;
	{
		Struct2Test::Cmpnt_var cmpnt = create_cmpnt();
		to = cmpnt;
	}
	dump(to);

}

int main()
{
	test_create();
	test_copy();

	return 0;
}
