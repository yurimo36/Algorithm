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
int		n;		/* table に格納されているデータの数 */
long int        k;     	       	/* データは table[1]〜table[n] に入る */
				/*（table[0] は、番兵として使うため）*/

void insert_end (int x, char *other) {	/* テーブルの最後にデータを追加する */
	if (n < SIZE - 1) {			/* テーブルに空きがあるなら */
		table[n + 1].key = x;		/* n + 1 番目に入れて */
		table[n + 1].otherinfo = other;
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

void write_file (char *filename) { /* ファイルに郵便番号データを書き込む */
	FILE *file;
	int i;

	file = fopen (filename, "w");	/* ファイルを書き込み専用で開く */
	for (i = 1; i <= n; i++) {	/* データを書き出す */
	  fprintf (file, "%07d,%s\n", table[i].key, table[i].otherinfo);
	}
}

void insertionsort () {		/* 配列 table を挿入法で整列する */
	int i, j;
	data_type w;

	for (i = 2; i <= n; i++) {
		w.key = table[i].key;
		w.otherinfo = table[i].otherinfo;
		table[0].key = w.key;	/* 番兵 */
		j = i - 1;
		while (/* j < 0 && */ w.key > table[j].key) {
			table[j+1].key = table[j].key;
			table[j+1].otherinfo = table[j].otherinfo;
			j--;
			k++;
		}
		table[j+1].key = w.key;
		table[j+1].otherinfo = w.otherinfo;
	}
}

main () {
	n = 0;				/* データ数を 0 に初期化し */
	read_file ("jigyosho-insertion.txt");	/* 郵便番号データを読み込む */
	insertionsort ();		/* データを整列し */
	write_file ("jigyosho-insertion.txt"); /* 結果をファイルに書き込む */
	printf("関数insertinsort内のj--の実行回数：%ld回\n",k);
}

/* コンパイル：		gcc insertionsort.c -o insertionsort
   実行：		time ./insertionsort
*/
