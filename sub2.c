//INT_MAX,INT_MINは入力として使用できない
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define width 3 //挿入されるキーの幅の最大

typedef struct Node {
    int number_keys;//キーの数
    int keys[3]; //キーを入れる
    struct Node *children[4]; //子ノードへのポインタ
    struct Node *parent; //親ノードへのポインタ
} Node;

typedef struct Tree {
    Node *root;
} Tree;
Tree tree; //木を作成
//プロトタイプ宣言
int Actually_delete_leaf(Node **cnode, int i);
int Delete_internal(Node **cnode, int i,int children_index);
int Delete_leaf(Node **cnode, int i,int children_index);
int Actually_delete_internal(Node **cnode,int j,int children_index);
int InsertUP(Node **cnode, int i, Node *left, Node *right);

int Search(Node **cnode, int i) {
    if(*cnode == NULL) {
        return 0; //木が空なら0を返す
    }
    int q;
    //葉ではなかったら葉へ移動
    for(int j = 0; j < (*cnode)->number_keys; j++) {
        if((*cnode)->keys[j] == i) {
            printf("Key %d found!!\n", i);
            return 1; //キーが見つかった場合
        }
    }
    for(int j = 0; j < (*cnode)->number_keys+1; j++) {
        q=Search(&(*cnode)->children[j], i);
        if(q == 1) return 1; //キーが見つかった場合
    }

    //キーが見つからなかった場合
    return 0;
}

int Show(Node **cnode,int space){
    if(tree.root == NULL) {
        printf("Tree is empty.\n");
        return 0;
    }
    for(int i = 0; i < space; i++) {
        printf(" ");
    }
    if((*cnode)->keys[1] == INT_MIN || (*cnode)->keys[1] == INT_MAX){
        
        printf("[%*d",width, (*cnode)->keys[0]);
        for(int i=0;i<width+1;i++){
            printf(" ");
        }
        printf("]\n");
        //printf("] number_keys:%d\n", (*cnode)->number_keys);

    }
    else{
        printf("[%*d,%*d]\n",width, (*cnode)->keys[0],width,(*cnode)->keys[1]);
    }
  
    //子を表示
    for (int i = 0; i < 4; i++) {
        if ((*cnode)->children[i] != NULL) {
            Show(&(*cnode)->children[i],space + 3 + 2*width);
        } else {
            //printf("empty\n");
        }
    }
    return 0;
}


int Insert(Node **cnode, int i, Node *left, Node *right,int children_index) {

    //木が空なら根を作成
    if(*cnode == NULL) {
        Node *new_node = (Node *)malloc(sizeof(Node));
        //メモリ確保ができない場合
        if (new_node == NULL) {
            printf("Memory Allocation ERROR!!!!\n");
            return -1;
        }
        //初期化
        new_node->number_keys = 1;
        new_node->keys[0] = i;
        for (int j = 1; j < 3; j++){
            new_node->keys[j] = INT_MAX;
        }
        for (int j = 0; j < 4; j++){
            new_node->children[j] = NULL;
        }
        new_node->parent = NULL; 
        
        tree.root = new_node; //新ノードを根に
        return 0;
        
    }
    Node *current = *cnode;
    //葉ではなかったら葉へ移動
    if(current->children[0] != NULL) {
        for(int j = 0; j < current->number_keys; j++) {
            if (current->keys[j] > i) {
                Insert(&current->children[j], i, NULL, NULL,j);
                return 0;
            }
        }
        Insert(&current->children[current->number_keys], i, NULL, NULL,current->number_keys);
        return 0;
    }
    //葉ノードへの挿入
    int j = current->number_keys - 1;
    while(j >= 0 && current->keys[j] > i) {
        current->keys[j+1] = current->keys[j];
        
        j--;
    }
    
    current->keys[j+1] = i;
    current->number_keys++;

    //分割処理
    if(current->number_keys == 3) {
        //親ノードに新しいキーを挿入
        if(current->parent!= NULL) {
        }
        InsertUP(&current, current->keys[1], left, right);

    }
    return 0;
}

//子から親に挿入
int InsertUP(Node **cnode, int i, Node *left, Node *right){

    if((*cnode)->parent == NULL){
        Node *left_node = (Node *)malloc(sizeof(Node));
        Node *right_node = (Node *)malloc(sizeof(Node));

        left_node->number_keys = 1;
        right_node->number_keys = 1;
        left_node->keys[0] = (*cnode)->keys[0];
        right_node->keys[0] = (*cnode)->keys[2];
        left_node->keys[1] = left_node->keys[2] = INT_MAX;
        right_node->keys[1] = right_node->keys[2] = INT_MAX;

        for(int j = 0; j < 4; j++) {
            left_node->children[j] = NULL;
            right_node->children[j] = NULL;
        }

        left_node->children[0] = (*cnode)->children[0];
        left_node->children[1] = (*cnode)->children[1];
        right_node->children[0] = (*cnode)->children[2];
        right_node->children[1] = (*cnode)->children[3];

        //子のparent設定
        for(int j = 0; j < 2; j++) {
            if(left_node->children[j]) left_node->children[j]->parent = left_node;
            if(right_node->children[j]) right_node->children[j]->parent = right_node;
        }

        left_node->parent = (*cnode);
        right_node->parent = (*cnode);

        (*cnode)->number_keys = 1;
        (*cnode)->keys[0] = i;
        (*cnode)->keys[1] = (*cnode)->keys[2] = INT_MAX;

        (*cnode)->children[0] = left_node;
        (*cnode)->children[1] = right_node;
        (*cnode)->children[2] = (*cnode)->children[3] = NULL;

        (*cnode)->parent = NULL;
        tree.root = (*cnode);

        return 0;
    }
    else {

        int idx = -1;
        for (int k = 0; k < 4; k++) {
            if ((*cnode)->parent->children[k] == (*cnode)) {
                idx = k;
                break;
            }
        }

        if (idx == -1) {
            printf("children_index ERROR!!!!!!!!!!\n");
            return -1;
        }

        //親ノードに挿入するための準備
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->number_keys = 1;
        new_node->keys[0] = (*cnode)->keys[0];
        new_node->keys[1] = new_node->keys[2] = INT_MAX;
        new_node->parent = (*cnode)->parent;

        new_node->children[0] = (*cnode)->children[0];
        new_node->children[1] = (*cnode)->children[1];
        new_node->children[2] = new_node->children[3] = NULL;

        if(new_node->children[0]) new_node->children[0]->parent = new_node;
        if(new_node->children[1]) new_node->children[1]->parent = new_node;

        (*cnode)->number_keys = 1;
        (*cnode)->keys[0] = (*cnode)->keys[2];
        (*cnode)->keys[1] = (*cnode)->keys[2] = INT_MAX;
        (*cnode)->children[0] = (*cnode)->children[2];
        (*cnode)->children[1] = (*cnode)->children[3];
        (*cnode)->children[2] = (*cnode)->children[3] = NULL;
        if((*cnode)->children[0]) (*cnode)->children[0]->parent = (*cnode);
        if((*cnode)->children[1]) (*cnode)->children[1]->parent = (*cnode);

        //親にキーと子ノードを挿入
        Node *parent = (*cnode)->parent;
        if(idx == 0){
            parent->keys[2] = parent->keys[1];
            parent->keys[1] = parent->keys[0];
            parent->keys[0] = i;
            parent->children[3] = parent->children[2];
            parent->children[2] = parent->children[1];
            parent->children[1] = (*cnode);
            parent->children[0] = new_node;
        }
        else if(idx == 1){
            parent->keys[2] = parent->keys[1];
            parent->keys[1] = i;
            parent->children[3] = parent->children[2];
            parent->children[2] = (*cnode);
            parent->children[1] = new_node;
        }
        else if(idx == 2){
            parent->keys[2] = i;
            parent->children[3] = (*cnode);
            parent->children[2] = new_node;
        } else {
            printf("Unexpected idx value: %d\n", idx);
            return -1;
        }

        parent->number_keys++;

        //親キーも3つになった場合
        if(parent->number_keys == 3){
            InsertUP(&parent, parent->keys[1], NULL, NULL);
        }

        return 0;
    }
}


int Delete_leaf(Node **cnode, int i,int children_index){
    if(*cnode == NULL) {
        //printf("empty");
        return 0;
    }
    Delete_leaf(&(*cnode)->children[0], i, 0);
    Delete_leaf(&(*cnode)->children[1], i, 1);
    Delete_leaf(&(*cnode)->children[2], i, 2);

    for(int j = 0; j < (*cnode)->number_keys; j++) {
        if((*cnode)->keys[j] == i) {
            //キーが見つかった場合
            //ノードが葉ではない場合
            if((*cnode)->children[0] != NULL) {
                //右の子ノードから最小値を値を取得
                Node *right_child = (*cnode)->children[j+1];
                while(right_child->children[0] != NULL) {
                    right_child = right_child->children[0];
                }
                //最小値を置き換え
                int tmp = (*cnode)->keys[j];
                (*cnode)->keys[j] = right_child->keys[0];
                right_child->keys[0] = tmp;

                Actually_delete_leaf(&right_child, 0);
                break;
            }
            //ノードが葉の場合
            Actually_delete_leaf(cnode, j);
            break;
            
        }
        
    }
    return 0;
    
}

int Delete_internal(Node **cnode, int i,int children_index){
    if(*cnode == NULL) {
        return 0;
    }
    Delete_internal(&(*cnode)->children[0], i, 0);
    Delete_internal(&(*cnode)->children[1], i, 1);
    Delete_internal(&(*cnode)->children[2], i, 2);
    for(int j = 0; j < (*cnode)->number_keys+1; j++) {
        if((*cnode)->keys[j] == INT_MIN) {
            //キーが見つかった場合
            Actually_delete_internal(cnode, j,children_index);
            return 0;

        }
    }
    return 0;
}

int Actually_delete_internal(Node **cnode, int i,int children_index){
    if((*cnode)->number_keys == 1) {
        //兄弟にkey_numberが2のものがないか探す
        //1つ右の兄弟が2つのキーを持っている場合
        if(children_index!=2 && (*cnode)->parent->children[children_index+1] != NULL && (*cnode)->parent->children[children_index+1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->children[children_index+1]->keys[1];
            (*cnode)->parent->children[children_index+1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index+1]->number_keys--;
            (*cnode)->children[1] = (*cnode)->parent->children[children_index+1]->children[0];
            (*cnode)->parent->children[children_index+1]->children[0] =(*cnode)->parent->children[children_index+1]->children[1];
            (*cnode)->parent->children[children_index+1]->children[1] = (*cnode)->parent->children[children_index+1]->children[2];
            (*cnode)->parent->children[children_index+1]->children[2] = NULL;
            return 0;
        }
        //1つ左の兄弟が2つのキーを持っている場合
        else if(children_index!=0 && (*cnode)->parent->children[children_index-1] != NULL && (*cnode)->parent->children[children_index-1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[1];
            (*cnode)->parent->children[children_index-1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-1]->number_keys--;
            (*cnode)->children[1] = (*cnode)->children[0];
            (*cnode)->children[0] = (*cnode)->parent->children[children_index-1]->children[2];
            (*cnode)->parent->children[children_index-1]->children[2] = NULL;
            return 0;
        }
        //2つ右の兄弟が2つのキーを持っている場合
        else if(children_index==0 && (*cnode)->parent->children[children_index+2] != NULL && (*cnode)->parent->children[children_index+2]->number_keys == 2) {
            if((*cnode)->children[0]!=NULL){
            }
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->keys[children_index+1];
            (*cnode)->parent->keys[children_index+1] = INT_MAX;
            (*cnode)->parent->number_keys--;
            (*cnode)->keys[1] = (*cnode)->parent->children[children_index+1]->keys[0];

            (*cnode)->children[1] = (*cnode)->parent->children[children_index+1]->children[0];
            (*cnode)->children[1]->number_keys = (*cnode)->parent->children[children_index+1]->children[0]->number_keys;
            (*cnode)->children[2] = (*cnode)->parent->children[children_index+1]->children[1];
            (*cnode)->parent->children[children_index+1]->children[0] = NULL;
            (*cnode)->parent->children[children_index+1]->children[1] = NULL;
            free((*cnode)->parent->children[children_index+1]);
            (*cnode)->parent->children[children_index+1] = (*cnode)->parent->children[children_index+2];
            (*cnode)->parent->children[children_index+2] = NULL;
            
            return 0;
        }
        //2つ左の兄弟が2つのキーを持っている場合
        else if(children_index==2 && (*cnode)->parent->children[children_index-2] != NULL && (*cnode)->parent->children[children_index-2]->number_keys == 2) {
            (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->children[1]->number_keys = 2;
            (*cnode)->parent->number_keys--;
            (*cnode)->parent->keys[children_index-1] = INT_MAX;
            (*cnode)->parent->children[1]->children[2] = (*cnode)->children[0];
            
            free((*cnode)->children[0]);
            (*cnode)->parent->children[2] = NULL;
            free((*cnode));
           
            return 0;
        }
        //兄弟が全員1キー
        else{
            //親ノードが1つのキーを持っている場合
            if((*cnode)->parent->number_keys == 1) {
                if(children_index==0){
                    (*cnode)->parent->children[0]->keys[0] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->children[1]->keys[0];
                    (*cnode)->parent->children[0]->number_keys = 2;
                    (*cnode)->children[1] = (*cnode)->parent->children[1]->children[0];
                    (*cnode)->children[2] = (*cnode)->parent->children[1]->children[1];
                
                    //親が root なら縮退処理を行う
                    if ((*cnode)->parent->parent == NULL) {
                        tree.root = (*cnode)->parent->children[0];
                        free((*cnode)->parent->children[1]);
                        free((*cnode)->parent);
                        (*cnode)->parent = NULL;
                    } else {
                        //親が root でなければ通常のマージ
                        (*cnode)->parent->keys[0] = INT_MIN;
                        free((*cnode)->parent->children[1]);
                        (*cnode)->parent->children[1] = NULL;
                    }
                    return 0;
                }
                if(children_index==1){
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->number_keys = 2;
                    (*cnode)->parent->children[0]->children[2] = (*cnode)->parent->children[1]->children[0];

                    //親が root なら縮退処理を行う
                    if ((*cnode)->parent->parent == NULL) {
                        tree.root = (*cnode)->parent->children[0];
                        (*cnode)->parent->children[0]->parent = NULL;
                        free((*cnode)->parent);
                    } else {
                        //親が root でなければ通常のマージ
                        (*cnode)->parent->keys[0] = INT_MIN;

                        (*cnode)->parent->children[1]=NULL;
                        free((*cnode));
                    
                    }
                    return 0;
                }
            }

            //親ノードが2つのキーを持っている場合
            else if((*cnode)->parent->number_keys == 2) {
                if(children_index==0){
                    (*cnode)->parent->children[0]->keys[0] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->children[1]->keys[0];
                    (*cnode)->parent->children[0]->number_keys = 2;
                    (*cnode)->parent->children[0]->children[1] = (*cnode)->parent->children[1]->children[0];
                    (*cnode)->parent->children[0]->children[2] = (*cnode)->parent->children[1]->children[1];
                    free((*cnode)->parent->children[1]);
                    (*cnode)->parent->children[1] = (*cnode)->parent->children[2];
                    (*cnode)->parent->children[2]=NULL;
                    (*cnode)->parent->number_keys--;
                    (*cnode)->parent->keys[0] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->keys[1] = INT_MAX;

                    return 0;
                }
                if(children_index==1){
                    (*cnode)->parent->children[1]->keys[0] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->children[2]->keys[0];
                    (*cnode)->parent->children[1]->number_keys = 2;
                    (*cnode)->parent->children[1]->children[1] = (*cnode)->parent->children[2]->children[0];
                    (*cnode)->parent->children[1]->children[2] = (*cnode)->parent->children[2]->children[1];
                    free((*cnode)->parent->children[2]);
                    (*cnode)->parent->children[2]=NULL;
                    (*cnode)->parent->number_keys--;
                    (*cnode)->parent->keys[1] = INT_MAX;
                
                    return 0;
                }
                if(children_index==2){
                    (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->children[1]->number_keys = 2;
                    (*cnode)->parent->children[1]->children[2] = (*cnode)->children[0];
                    (*cnode)->parent->number_keys--;
                    (*cnode)->parent->keys[1] = INT_MAX;


                    (*cnode)->parent->children[2]=NULL;
                    free((*cnode));
                
                    return 0;
                }
            }
            
        }
    }
    else{
        printf("ERROR!! number_keys is not 1\n");
        printf("(*cnode)->number_keys: %d\n", (*cnode)->number_keys);
        Show(&tree.root, 0);
        return -1;
    }
}

int Actually_delete_leaf(Node **cnode, int i){
    int idx = -1;
    //自分の子番号を確かめる
    if((*cnode)->parent!=NULL){
        for (int k = 0; k < 4; k++) {
            if ((*cnode)->parent->children[k] == (*cnode)) {
                idx = k;
                break;
            }
        }
        if (idx == -1) {
            printf("children_index ERROR!!!!!!!!!!\n");
            return -1;
        }
    }
    int children_index = idx;
    if((*cnode)->parent == NULL && (*cnode)->number_keys == 1 && (*cnode)->children[0] == NULL) {
        free(*cnode);
        *cnode = NULL;
        tree.root = NULL;
        return 0;
    }
    //キーが2つの場合
    if((*cnode)->number_keys == 2) {

        if(i == 0) {
            (*cnode)->keys[0] = (*cnode)->keys[1];
            (*cnode)->keys[1] = INT_MAX;
            (*cnode)->number_keys--;

        }
        else if(i == 1) {
            (*cnode)->keys[1] = INT_MAX;
            (*cnode)->number_keys--;
        }
        return 0;
    }
    //キーが1つの場合
    if((*cnode)->number_keys == 1) {
        //兄弟にkey_numberが2のものがないか探す
        //1つ右の兄弟が2つのキーを持っている場合
        if(children_index!=2 && (*cnode)->parent->children[children_index+1] != NULL && (*cnode)->parent->children[children_index+1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->children[children_index+1]->keys[1];
            (*cnode)->parent->children[children_index+1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index+1]->number_keys--;
            return 0;
        }
        //1つ左の兄弟が2つのキーを持っている場合
        else if(children_index!=0 && (*cnode)->parent->children[children_index-1] != NULL && (*cnode)->parent->children[children_index-1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[1];
            (*cnode)->parent->children[children_index-1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-1]->number_keys--;
            return 0;
        }
        //2つ右の兄弟が2つのキーを持っている場合
        else if(children_index==0 && (*cnode)->parent->children[children_index+2] != NULL && (*cnode)->parent->children[children_index+2]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->keys[children_index+1];
            (*cnode)->parent->keys[children_index+1] = (*cnode)->parent->children[children_index+2]->keys[0];
            (*cnode)->parent->children[children_index+2]->keys[0] = (*cnode)->parent->children[children_index+2]->keys[1];
            (*cnode)->parent->children[children_index+2]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index+2]->number_keys--;
            return 0;
        }
        //2つ左の兄弟が2つのキーを持っている場合
        else if(children_index==2 && (*cnode)->parent->children[children_index-2] != NULL && (*cnode)->parent->children[children_index-2]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[0];
            (*cnode)->parent->children[children_index-1]->keys[0] = (*cnode)->parent->keys[children_index-2];
            (*cnode)->parent->keys[children_index-2] = (*cnode)->parent->children[children_index-2]->keys[1];
            (*cnode)->parent->children[children_index-2]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-2]->number_keys--;
            return 0;
        }
        //兄弟のキーが1つの場合
        else{
            if((*cnode)->parent->number_keys==2){
                //兄弟のキーが1つ&&親のキーが2つの場合

                //兄弟のキーを親に移動
                if(children_index==0){
                    (*cnode)->keys[0] = (*cnode)->parent->keys[0];
                    (*cnode)->keys[1] = (*cnode)->parent->children[1]->keys[0];
                    (*cnode)->number_keys = 2;
                    (*cnode)->parent->keys[0] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->keys[1] = INT_MAX;
                    free((*cnode)->parent->children[1]);
                    (*cnode)->parent->children[1] = (*cnode)->parent->children[2];
                    (*cnode)->parent->children[2] = NULL;
                    (*cnode)->parent->number_keys--;
                }
                else if(children_index==1){
                    (*cnode)->keys[1] = (*cnode)->parent->keys[0];
                    (*cnode)->keys[0] = (*cnode)->parent->children[0]->keys[0];
                    (*cnode)->number_keys = 2;
                    (*cnode)->parent->keys[0] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->keys[1] = INT_MAX;
                    free((*cnode)->parent->children[0]);
                    (*cnode)->parent->children[0] = (*cnode)->parent->children[1];
                    (*cnode)->parent->children[1] = (*cnode)->parent->children[2];
                    (*cnode)->parent->children[2] = NULL;
                    (*cnode)->parent->number_keys--;

                }
                else if(children_index==2){
                    (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->keys[1];
                    (*cnode)->parent->children[1]->number_keys = 2;
                    (*cnode)->parent->keys[1] = INT_MAX;
                    (*cnode)->parent->number_keys--;
                    (*cnode)->parent->children[2] = NULL;
                    free((*cnode));
                }
                return 0;
            }
            //兄弟のキーが1つ && 親のキーが1つの場合
            else{
                if(children_index==0){
                    (*cnode)->parent->children[0]->keys[0] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->children[1]->keys[0];
                    (*cnode)->parent->children[0]->number_keys = 2;
                
                    //親が root なら縮退処理を行う
                    if ((*cnode)->parent->parent == NULL) {
                        tree.root = (*cnode)->parent->children[0];

                        (*cnode)->parent->children[0]->parent = NULL;
                        free((*cnode)->parent);
                    } else {
                        //親が root でなければ通常のマージ
                        (*cnode)->parent->keys[0] = INT_MIN;
                        free((*cnode)->parent->children[1]);
                        (*cnode)->parent->children[1] = NULL;
                        (*cnode)->number_keys = 2;
                    }
                    return 0;
                }
                if(children_index==1){
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->number_keys = 2;
                
                    //親が root なら縮退処理を行う
                    if ((*cnode)->parent->parent == NULL) {
                        tree.root = (*cnode)->parent->children[0];
                        (*cnode)->parent->children[0]->parent = NULL;
                        free((*cnode)->parent);
                    } else {
                        //親が root でなければ通常のマージ
                        (*cnode)->parent->keys[0] = INT_MIN;
                        
                        (*cnode)->parent->children[1]=NULL;
                        free((*cnode));
                        //(*cnode)->parent->number_keys--;
                    
                    }
                    return 0;
                }
                printf("ERROR!! children_index is not 0 or 1\n");
                printf("children_index: %d\n", children_index);
            }
        }
        return 0;
    }
    else{
        printf("ERROR!!!! number_keys is not 1 or 2\n");
        printf("(*cnode)->number_keys: %d\n", (*cnode)->number_keys);
        return 0;
    }
}



int main(void){
    int i,q;
    int selection;
    tree.root = NULL; //木の根をNULLに初期化

    while (1){
        printf("\nEnter a command (1:Insert, 2:Delete, 3:Search, 4:Show, 5:Exit): "); 
        scanf("%d", &selection);
        switch(selection){
        case 1 :
            printf("Insert:\n");
            scanf("%d", &i);
            Insert(&tree.root,i, NULL, NULL,0);
            break;
        case 2 :
            printf("Delete:\n");
            scanf("%d", &i);
            Delete_leaf((&tree.root), i,0);
            //Show(&tree.root,0);
            Delete_internal((&tree.root), i,0);
            break;
        case 3 :
            printf("Search:\n");
            scanf("%d", &i);
            q=Search(&tree.root,i);
            if(q==0){
                printf("Key %d not found.\n",i);
            }
            break;
        case 4 :
            printf("Show\n");
            Show(&tree.root,0);
            break;
        case 5 :
            printf("Exit\n");
            return 0;
            break;
        default:
            printf("Invalid selection. Please try again.\n");
            break;
        }
    }
}