#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 128000		/* テーブルの大きさ */
#define NOT_FOUND (-1)		/* データが見つからなかった時に返す数 */

#define LINE_LENGTH 300		/* 住所の文字数の最大値 */

typedef struct data_t {
	int	key;		/* キー（郵便番号） */
	char	*otherinfo;	/* その他の情報（住所） */
} data_type;

data_type	table[SIZE];	/* 大きさ SIZE の配列を宣言 */
int		n,k;		/* table に格納されているデータの数 */

void insert_end (int x, char *other) {	/* テーブルの最後にデータを追加する */
	/* 挿入するデータは、既にあるデータのどれよりも大きくなくては
	   ならない。そうでないと大小関係が崩れ、２分探索はできなくなる */
	if (n < SIZE) {				/* テーブルに空きがあるなら */
		table[n].key = x;		/* n 番目に入れて */
		table[n].otherinfo = other;
		n++;				/* n を増やす */
	} else {				/* 一杯なら */
		printf ("テーブルが一杯です。\n"); /* メッセージを出して */
		exit (1);			/* 終了する */
	}
}

void read_file (char *filename) { /* ファイルから郵便番号データを読み込む */
	FILE *file;
	int code;
	char *address;
	int r;

	file = fopen (filename, "r");	/* ファイルを読み込み専用で開く */
	if (file == NULL) {		/* ファイルが見つからなかったら */
	  printf ("ファイル %s が見つかりません\n", filename);
	  exit (1);			/* エラーを出して終了 */
	}
	while (1) {			/* 以下をデータがなくなるまで行う */
	  address = (char *) malloc (LINE_LENGTH);
	  /* 住所を格納するメモリを確保 */
	  r = fscanf (file, "%d,%s\n", &code, address);
	  /* １行、郵便番号データを読み込む */
	  if (r == EOF) {	/* ファイルの終わりなら */
	    fclose (file);	/* ファイルを閉じて */
	    return;		/* 終了 */
	  } else {		/* データがあったなら */
	    insert_end (code, address); /* それを追加して、読み込みを続ける */
	  }
	}
}

int search (int target) {	/* table の中で target が何番目にあるか */
  int lo, hi, mid,k=0;	/* ２分探索をして返す */

	lo = 0;
	hi = n - 1;
	while (lo <= hi) {		/* lo <= hi の間、以下を繰り返す */
		mid = (lo + hi) / 2;	/* mid を lo と hi の真ん中にする */
		if (target <= table[mid].key) { /* 目的のキーが真ん中よりも左なら */
			hi = mid - 1;		/* hi を真ん中にする */
		}
		if (target >= table[mid].key) { /* 目的のキーが真ん中よりも右なら */
			lo = mid + 1;		/* lo を真ん中にする */
		}
		k++;
	}
	if (lo == hi + 2) {		/* lo = hi + 2 なら見つかったので */
		return (lo - 1);	/* lo - 1 を返す */
	} else {			/* そうでなければ */
		return (NOT_FOUND);	/* NOT_FOUND を返す */
	}
}

void insert (int x, char *other, int pos) {
	/* テーブルの pos 番目にデータを追加する */
	int i;

	if (n < SIZE) {				/* テーブルに空きがあるなら */
		for (i = n - 1; i >= pos; i--) { /* pos 番目以上をずらし */
			table[i + 1].key = table[i].key;
			table[i + 1].otherinfo = table[i].otherinfo;
		}
		table[pos].key = x;		/* pos 番目に入れて */
		table[pos].otherinfo = other;
		n++;				/* n を増やす */
	} else {				/* 一杯なら */
		printf ("テーブルが一杯です。\n"); /* メッセージを出して */
		exit (1);			/* 終了する */
	}
}

void delete (int pos) {
	/* テーブルの pos 番目のデータを削除する */
	int i;

	for (i = pos; i < n; i++) {		/* pos 番目以上をずらし */
		table[i].key = table[i + 1].key;
		table[i].otherinfo = table[i + 1].otherinfo;
	}
	n--;					/* n を減らす */
}

main () {
	int i, pos;

	n = 0;				/* データ数を 0 に初期化し */
	read_file ("postal-sorted.txt");	/* 郵便番号データを読み込む */

	while (1) {			/* 以下を繰り返す */
		printf ("郵便番号：");
		scanf ("%d", &i);	/* 入力した文字を */
		pos = search (i);	/* table の中から探す */
		if (pos == NOT_FOUND) {
			printf ("見つかりませんでした。\n");
		} else {
			printf ("%s\n", table[pos].otherinfo);
			printf("実行回数：%d回\n",k);
		}
	}
}

/* コンパイル：		gcc binarysearch.c -o binarysearch
   実行：		./binarysearch

終了するには ctrl-C。*/
