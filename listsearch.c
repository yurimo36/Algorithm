#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND NULL		/* データが見つからなかった時に返す数 */

#define LINE_LENGTH 300		/* 住所の文字数の最大値 */

typedef struct list_t {
	int	key;		/* キー（郵便番号） */
	char	*otherinfo;	/* その他の情報（住所） */
	struct	list_t *next;	/* 次のノードへのポインタ */
} list_type;

list_type *insert (int key, char *otherinfo, /* list に要素を追加する */
		   list_type *next) {
	list_type *list;

	list = (list_type *) malloc (sizeof (list_type)); /* メモリを確保 */
	list->key = key;		/* データをセットし */
	list->otherinfo = otherinfo;
	list->next = next;
	return (list);			/* 返す */
}

list_type *read_file (char *filename, list_type *list) {
	FILE *file;		/* ファイルから郵便番号データを読み込む */
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
	    return (list);	/* 終了 */
	  } else {		/* データがあったなら */
	    list = insert (code, address, list);
				/* それを追加して、読み込みを続ける */
	  }
	}
}

void delete (int key, list_type *list) { /* list から key を削除する */
	list_type *previous, *l;

	if (key == list->key) { /* key が先頭にあった */
		l = list;
		list = list->next; /* 先頭を削除 */
		free (l); /* メモリを解放 */
		return;
	}
	previous = list;
	for (l = list->next; l != NULL; l = l->next) {
		if (key == l->key) { /* 見付かった */
			previous->next = l->next; /* 削除 */
			free (l); /* メモリを解放 */
			return;
		}
		previous = l;
	}
	return; /* 見付からなかった */
}

char *search (int target, list_type *list) {	/* list の中で target を */
	list_type *l;		     /* 線形探索し、対応する info を返す */

	for (l = list; l != NULL; l = l->next) {
		if (target == l->key) { /* 見付かった */
			return (l->otherinfo);
		}
	}
	return (NOT_FOUND); /* 見付からなかった */
}

main () {
	int i;
	char *str;
	list_type *list;

	list = NULL;				/* リストを空に初期化し */
	list = read_file ("postal.txt", list);	/* 郵便番号データを読み込む */

	while (1) {			/* 以下を繰り返す */
		printf ("郵便番号：");
		scanf ("%d", &i);	/* 入力した文字を */
		str = search (i, list);	/* list の中から探す */
		if (str == NOT_FOUND) {
			printf ("見つかりませんでした。\n");
		} else {
			printf ("%s\n", str);
		}
	}
}

/* コンパイル：		gcc listsearch.c -o listsearch
   実行：		./listsearch

終了するには ctrl-C。*/
