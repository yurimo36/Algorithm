#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 150000		/* テーブルの大きさ */
#define NOT_FOUND (-1)		/* データが見つからなかった時に返す数 */

#define LINE_LENGTH 500		/* 住所の文字数の最大値 */

typedef struct data_t {
	int	key;		/* キー（郵便番号） */
	char	*otherinfo;	/* その他の情報（住所） */
} data_type;

data_type	table[SIZE];	/* 大きさ SIZE の配列を宣言 */
int		n;		/* table に格納されているデータの数 */

void insert_end (int x, char *other) {	/* テーブルの最後にデータを追加する */
	table[n].key = x;		/* n 番目に入れて */
	table[n].otherinfo = other;
	n++;				/* n を増やす */
	if(n==SIZE){	                /* n が SIZE より大きかったら*/
	  printf("ERROR\n");           
	  exit(1);                      /* ERROR を表示して終了*/
	}
}

void delete (int pos) {			/* テーブルからデータを削除する */
	table[pos].key = table[n-1].key;		/* n-1 番目のデータを */
	table[pos].otherinfo = table[n-1].otherinfo;	/* pos 番目に移動して */
	n--;						/* n を減らす */
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

int search (int target) {	/* table の中で target がいくつ目にあるか */
	int pos;		/* 線形探索して返す */

	pos = 0;
	while (pos < n				/* 最後まで到達しておらず */
	       &&				/* かつ */
	       target != table[pos].key) {	/* まだ見つからないなら */
		 pos++;				/* pos を次にずらす */	}
	if (pos < n) {		/* pos が n 以下なら見つかったので */
		return (pos);	/* pos を返す */
	} else {		/* そうでなければ */
		return (NOT_FOUND); /* NOT_FOUND を返す */
	}
}

main () {
	int i, pos;

	n = 0;				/* データ数を 0 に初期化し */
	read_file ("postal.txt");	/* 郵便番号データを読み込む */
	read_file ("jigyosho.txt");

	while (1) {			/* 以下を繰り返す */
		printf ("郵便番号：");
		scanf ("%d", &i);	/* 入力した文字を */
		pos = search (i);	/* table の中から探す */
		if (pos == NOT_FOUND) {
			printf ("見つかりませんでした。\n");
		} else {
			printf ("%s\n", table[pos].otherinfo);
			printf("検索回数：%d\n",pos);
		}
	}
}

/* コンパイル：		gcc linearsearch.c -o linearsearch
   実行：		./linearsearch

終了するには ctrl-C。*/
