#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND NULL		/* データが見つからなかった時に返す数 */

#define LINE_LENGTH 300		/* 住所の文字数の最大値 */

typedef struct tree_t {
	int	key;		/* キー（郵便番号） */
	char	*otherinfo;	/* その他の情報（住所） */
	struct tree_t *left;	/* 左の子へのポインタ */
	struct tree_t *right;	/* 右の子へのポインタ */
} tree_type;

static int count;

tree_type *make_tree (int key, char *otherinfo, /* tree を作る */
		      tree_type *left, tree_type *right) {
	tree_type *tree;

	tree = (tree_type *) malloc (sizeof (tree_type)); /* メモリを確保 */
	tree->key = key;
	tree->otherinfo = otherinfo;
	tree->left = left;
	tree->right = right;
	return (tree);
}

void insert (int key, char *otherinfo, tree_type *tree) {
	/* tree に key を挿入。tree != NULL を仮定している */
	tree_type *new_tree;

	if (key < tree->key) { /* 左側に挿入する */
		if (tree->left == NULL) {
			new_tree = make_tree (key, otherinfo, NULL, NULL);
			tree->left = new_tree;
			return;
		} else {
			insert (key, otherinfo, tree->left);
			return;
		}
	} else if (key > tree->key) { /* 右側に挿入する */
		if (tree->right == NULL) {
			new_tree = make_tree (key, otherinfo, NULL, NULL);
			tree->right = new_tree;
			return;
		} else {
			insert (key, otherinfo, tree->right);
			return;
		}
	} else { /* key が見付かった */
		tree->otherinfo = otherinfo;	/* otherinfo を更新 */
		return;
	}
}

tree_type *read_file (char *filename, tree_type *tree) {
	FILE *file;		/* ファイルから郵便番号データを読み込む */
	int code;
	char *address;
	int r;

	file = fopen (filename, "r");	/* ファイルを読み込み専用で開く */
	if (file == NULL) {		/* ファイルが見つからなかったら */
	  printf ("ファイル %s が見つかりません\n", filename);
	  exit (1);			/* エラーを出して終了 */
	}
	if (tree == NULL) {		/* 最初のデータをひとつ読み込む */
	  address = (char *) malloc (LINE_LENGTH);
	  /* 住所を格納するメモリを確保 */
	  r = fscanf (file, "%d,%s\n", &code, address);
	  /* １行、郵便番号データを読み込む */
	  if (r == EOF) {	/* ファイルの終わりなら */
	    fclose (file);	/* ファイルを閉じて */
	    return (tree);	/* 終了 */
	  } else {		/* データがあったなら */
	    tree = make_tree (code, address, NULL, NULL);
				/* それを追加する */
	  }
	}
	/* 残りのデータを読み込む */
	while (1) {			/* 以下をデータがなくなるまで行う */
	  address = (char *) malloc (LINE_LENGTH);
	  /* 住所を格納するメモリを確保 */
	  r = fscanf (file, "%d,%s\n", &code, address);
	  /* １行、郵便番号データを読み込む */
	  if (r == EOF) {	/* ファイルの終わりなら */
	    fclose (file);	/* ファイルを閉じて */
	    return (tree);	/* 終了 */
	  } else {		/* データがあったなら */
	    insert (code, address, tree);
				/* それを追加して、読み込みを続ける */
	  }
	}
}

char *search (int target, tree_type *tree) {	/* tree の中で target を */
				     /* 探索し、対応する info を返す */
        count++;
	if (tree == NULL) { /* 見付からなかった */
		return (NOT_FOUND);
	} else if (target < tree->key) { /* 左側を探す */
		return (search (target, tree->left));
	} else if (target > tree->key) { /* 右側を探す */
		return (search (target, tree->right));
	} else { /* 見付かった */
		return (tree->otherinfo);
	}
}


int height(tree_type *tree){
  int lh=0, rh=0;
  if(tree == NULL){ return 0; }

  lh=height(tree->left);
  rh=height(tree->right);

  if(lh > rh){ return lh+1; }

  else{ return rh+1; }
}

main () {
  int i,max=0;
	char *str;
	tree_type *tree;

	tree = NULL;				/* 木を空に初期化し */
	tree = read_file ("postal.txt", tree);	/* 郵便番号データを読み込む */

	max=height(tree);
	printf("木の高さ %d\n",max);

	while (1) {			/* 以下を繰り返す */
	        count=0;
		printf ("郵便番号：");
		scanf ("%d", &i);	/* 入力した文字を */ 
		str = search (i, tree);	/* 木の中から探す */
		if (str == NOT_FOUND) {
			printf ("見つかりませんでした。\n");
		} else {
			printf ("%s\n", str);
		}
		printf("関数searchが呼び出された回数 %d回\n",count);
         }

}

/* コンパイル：		gcc treesearch.c -o treesearch
   実行：		./treesearch

終了するには ctrl-C。*/
