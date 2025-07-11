//2つ左の兄弟が2つのキーを持っている場合　の書き換えから
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define down 0
#define up 1

typedef struct Node {
    int number_keys;//キーの数
    int keys[3]; //キーを入れる
    struct Node *children[4]; //子ノードを入れる
    struct Node *parent; //親ノードへのポインタ
} Node;

typedef struct Tree {
    Node *root;
} Tree;
Tree tree; //木を作成
int Actually_delete_leaf(Node **cnode, int i,int children_index);
int Delete_internal(Node **cnode, int i,int children_index);
int Delete_leaf(Node **cnode, int i,int children_index);
int Actually_delete_internal(Node **cnode,int j,int children_index);
int InsertUP(Node **cnode, int i, Node *left, Node *right,int state,int children_index);



int Search(Node **cnode,int space){
    if (*cnode == NULL) {
        //printf("empty ");
        return 0;
    }
    for(int i = 0; i < space; i++) {
        printf(" ");
    }
    if((*cnode)->keys[0] == INT_MIN || (*cnode)->keys[0] == INT_MAX) {
        printf("ERROR show");
    }
    else if((*cnode)->keys[1] == INT_MIN || (*cnode)->keys[1] == INT_MAX){
        printf("%d \n", (*cnode)->keys[0]);
    }
    else{
        printf("%d %d\n", (*cnode)->keys[0],(*cnode)->keys[1]);
        //printf("number_keys: %d\n", (*cnode)->number_keys);
    }
    //printf("\nkey ");
    //for(int i = 0; i < 3; i++) {
    //    if ((*cnode)->keys[i] != INT_MAX) {
    //        printf("%2d ", (*cnode)->keys[i]);
    //    }
        //else{
        //    printf("- ");
        //}
    //}   
    //printf("\n");

    for (int i = 0; i < 4; i++) {
        if ((*cnode)->children[i] != NULL) {
            Search(&(*cnode)->children[i],space + 2 + 1);
        } else {
            // 末端の子ノードはemptyを出す
            //printf("empty ");
        }
    }
    return 0;
}


int Insert(Node **cnode, int i, Node *left, Node *right,int state,int children_index) {
    printf("This is insert & i: %d  state: %d.\n", i,state);

    // cnodeがNULLなら新しいノードを作成
    if(*cnode == NULL) {
        Node *new_node = (Node *)malloc(sizeof(Node));
        //メモリ確保ができない場合
        if (new_node == NULL) {
            printf("Memory allocation failed.\n");
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
        new_node->parent = NULL; // 新しいノードの親はNULL
        
        //new_node->parent = NULL;
        //if(left){
        //    new_node->children[0] = left;
        //}
        //if(right) new_node->children[1] = right;
        //if(left) left->parent = new_node;
        //if(right) right->parent = new_node;
        //*cnode = new_node;
        tree.root = new_node; // 新しいノードを木の根に設定
        return 0;
        
    
    }
    Node *current = *cnode;
    //葉ではなかったら葉へ移動
    if(current->children[0] != NULL && state == down) {
        for(int j = 0; j < current->number_keys; j++) {
            if (current->keys[j] > i) {
                Insert(&current->children[j], i, NULL, NULL,down,j);
                return 0;
            }
        }
        Insert(&current->children[current->number_keys], i, NULL, NULL,down,current->number_keys);
        return 0;
    }
    // 葉ノードへの挿入
    int j = current->number_keys - 1;
    while(j >= 0 && current->keys[j] > i) {
        current->keys[j+1] = current->keys[j];
        
        j--;
    }
    
    printf("current->number_keys: %d\n", current->number_keys);
    printf("keys %d j: %d\n", current->keys[0], j);
    current->keys[j+1] = i;
    printf("current->keys[j+1]: %d j:%d\n", current->keys[j+1],j);
    current->number_keys++;
    printf("current->number_keys: %d\n", current->number_keys);
    //Search(&tree.root,0);

    printf("dividebefore\n");
    //Search(&tree.root,0);
    // 分割処理
    if(current->number_keys == 3) {
        //printf("divide\n");
        //Node *left = (Node *)malloc(sizeof(Node));
        //Node *right = (Node *)malloc(sizeof(Node));
        //left->number_keys = 1;
        //left->keys[0] = current->keys[0];
        //left->keys[1] = left->keys[2] = INT_MAX;
        //right->number_keys = 1;
        //right->keys[0] = current->keys[2];
        //right->keys[1] = right->keys[2] = INT_MAX;


        //for(int k=0;k<4;k++) left->children[k]=right->children[k]=NULL;
        //left->parent = right->parent = current->parent;
        //// 子ノードがある場合は分配
        //if(current->children[0]) {
        //    left->children[0] = current->children[0];
        //    left->children[1] = current->children[1];
        //    if(left->children[0]) left->children[0]->parent = left;
        //    if(left->children[1]) left->children[1]->parent = left;
        //    right->children[0] = current->children[2];
        //    printf("fffright->children[0] keys: %d %d %d\n", right->children[0]->keys[0], right->children[0]->keys[1], right->children[0]->keys[2]);
        //    right->children[1] = current->children[3];
        //    if(right->children[0]) right->children[0]->parent = right;
        //    if(right->children[1]) right->children[1]->parent = right;
        //}

        // 親がNULLなら新しい根を作る
        //if(current->parent == NULL) {
        //    printf("parent is null\n");
        //    Node *new_root = NULL;
        //    InsertUP(&new_root, current->keys[1], left, right,up,0);
        //    left->parent = new_root;
        //    right->parent = new_root;
        //    *cnode = new_root;

        //    free(current);
        //    Search(&tree.root,0);
            
        //    return 0;
    //    } else {
    //        printf("parent is not null\n");
    //        Node *parent = current->parent;
    //        int idx = -1;
    //        for (int k = 0; k < 4; k++) {
    //            if (parent->children[k] == current) {
    //                idx = k;
    //                break;
    //            }
    //        }

    //        // まず親ノードのchildrenを右にずらして空きを作る
    //        for (int k = 3; k > idx; k--) {
    //            parent->children[k] = parent->children[k-1];
    //        }
    //        printf("idx: %d\n", idx);
    //        printf("children_index: %d\n", children_index);
    //        // currentをleft/rightに置き換え
    //        parent->children[idx] = left;
    //        parent->children[idx+1] = right;
    //        left->parent = parent;
    //        right->parent = parent;
    //        Search(&tree.root,0);


    //        // その後、親ノードに新しいキーを挿入
            printf("current->keys[1]: %d\n", current->keys[1]);
            printf("current->keys[2]: %d\n", current->keys[2]);
            if(current->parent!= NULL) {
            printf("current->parent->keys[0]: %d\n", current->parent->keys[0]);
            }
            InsertUP(&current, current->keys[1], left, right, up,children_index);

    //        free(current);
    //        Search(&tree.root,0);
    //        return 0;
    //    }
    }
    Search(&tree.root,0);
    return 0;
}

int InsertUP(Node **cnode, int i, Node *left, Node *right,int state,int children_index){
    printf("This is insertUP & i: %d  state: %d.\n", i,state);
    printf("keys: %d %d %d\n", (*cnode)->keys[0], (*cnode)->keys[1], (*cnode)->keys[2]);
    if((*cnode)->parent == NULL){
        //Node *new_node = (Node *)malloc(sizeof(Node));
        Node *left_node = (Node *)malloc(sizeof(Node));
        Node *right_node = (Node *)malloc(sizeof(Node));
        left_node->parent = (*cnode);
        right_node->parent = (*cnode);
        left_node->number_keys = 1;
        right_node->number_keys = 1;
        left_node->keys[0] = (*cnode)->keys[0];
        right_node->keys[0] = (*cnode)->keys[2];
        left_node->keys[1] = left_node->keys[2] = INT_MAX;
        right_node->keys[1] = right_node->keys[2] = INT_MAX;
        (*cnode)->number_keys = 1;
        (*cnode)->keys[0] = i;
        (*cnode)->keys[1] = INT_MAX;
        (*cnode)->keys[2] = INT_MAX;
        left_node->children[0] = (*cnode)->children[0];
        left_node->children[1] = (*cnode)->children[1];
        right_node->children[0] = (*cnode)->children[2];
        right_node->children[1] = (*cnode)->children[3];
        for(int j = 2; j < 4; j++) {
            left_node->children[j] = NULL;
            right_node->children[j] = NULL;
        }
        (*cnode)->children[0] = left_node;
        (*cnode)->children[1] = right_node;
        (*cnode)->children[2] = (*cnode)->children[3] = NULL;
        //(*cnode)->parent = NULL;
        printf("up\n");
        
        tree.root = (*cnode); // 新しいノードを木の根に設定
        printf("tree.root:keys: %d %d %d\n", tree.root->keys[0], tree.root->keys[1], tree.root->keys[2]);

        return 0;
    }
    else{
        printf("parentkeys: %d %d %d\n", (*cnode)->parent->keys[0], (*cnode)->parent->keys[1], (*cnode)->parent->keys[2]);

        int idx=-1;
        for (int k = 0; k < 4; k++) {
            if ((*cnode)->parent->children[k] == (*cnode)) {
            //if ((*cnode)->parent->children[k]->keys[0] == (*cnode)->keys[0]) {
                idx = k;
                break;
            }
        }
        if (idx == -1) {
            printf("children_index error: parent->childrenにcnodeが見つかりません\n");
            for (int k = 0; k < 4; k++) {
                printf("parent->children[%d]=%p, cnode=%p\n", k, (void*)(*cnode)->parent->children[k], (void*)(*cnode));
            }
            return -1;
        }
        printf("idx: %d i;%d\n", idx,i);
        printf("(*cnode)->parent->children[0]->keys[0]: %d\n", (*cnode)->parent->children[0]->keys[0]);
        printf("(*cnode)->parent->children[1]->keys[0]: %d\n", (*cnode)->parent->children[1]->keys[0]);
        printf("(*cnode)->keys[0]: %d\n", (*cnode)->keys[0]);

        if(idx == 0) {
            //printf("(*cnode)->children[0]->parent->keys[0]: %d\n", (*cnode)->children[0]->parent->keys[0]);
            //printf("children_index: %d\n", children_index);
            //printf("left: %d right: %d\n", left->keys[0], right->keys[0]);
            //親にキーを追加
            (*cnode)->parent->keys[2] = (*cnode)->parent->keys[1];
            (*cnode)->parent->keys[1] = (*cnode)->parent->keys[0];
            (*cnode)->parent->keys[0] = i;
            (*cnode)->parent->number_keys++;
            //新ノードを親のchildren[0]へ
            Node *new_node = (Node *)malloc(sizeof(Node));
            //(*cnode)->children[0]->parent = (*cnode)->children[1]->parent = new_node;
            new_node->number_keys = 1;
            new_node->keys[0] = (*cnode)->keys[0];
            new_node->keys[1] = new_node->keys[2] = INT_MAX;
            new_node->parent = (*cnode)->parent;
            new_node->children[0] = (*cnode)->children[0];
            new_node->children[1] = (*cnode)->children[1];
            new_node->children[2] = new_node->children[3] = NULL;
            if (new_node->children[0]) new_node->children[0]->parent = new_node;
            if (new_node->children[1]) new_node->children[1]->parent = new_node;
            //new_node->children[0]->parent = new_node;
            //new_node->children[1]->parent = new_node;
            //new_node->children[0] = NULL;
            
            //現在のノードは親のchildren[1]へ
            (*cnode)->number_keys = 1;
            (*cnode)->keys[0] = (*cnode)->keys[2];
            (*cnode)->keys[1] = (*cnode)->keys[2] = INT_MAX;
            (*cnode)->children[0] = (*cnode)->children[2];
            (*cnode)->children[1] = (*cnode)->children[3];
            (*cnode)->children[2] = (*cnode)->children[3] = NULL;
            if ((*cnode)->children[0]) (*cnode)->children[0]->parent = (*cnode);
            if ((*cnode)->children[1]) (*cnode)->children[1]->parent = (*cnode);

            (*cnode)->parent->children[3] = (*cnode)->parent->children[2];
            (*cnode)->parent->children[2] = (*cnode)->parent->children[1];
            (*cnode)->parent->children[1] = (*cnode); 
            (*cnode)->parent->children[0] = new_node; 
        }
        else if(idx == 1) {
            (*cnode)->parent->keys[2] = (*cnode)->parent->keys[1];
            (*cnode)->parent->keys[1] = i;
            (*cnode)->parent->number_keys++;
            //新ノードを親のchildren[1]へ
            Node *new_node = (Node *)malloc(sizeof(Node));
            //(*cnode)->children[0]->parent = (*cnode)->children[1]->parent = new_node;
            new_node->number_keys = 1;
            new_node->keys[0] = (*cnode)->keys[0];
            new_node->keys[1] = new_node->keys[2] = INT_MAX;
            new_node->parent = (*cnode)->parent;
            new_node->children[0] = (*cnode)->children[0];
            new_node->children[1] = (*cnode)->children[1];
            new_node->children[2] = new_node->children[3] = NULL;
            new_node->children[0]->parent = new_node;
            new_node->children[1]->parent = new_node;
            if (new_node->children[0]) new_node->children[0]->parent = new_node;
            if (new_node->children[1]) new_node->children[1]->parent = new_node;
            
            //現在のノードは親のchildren[2]へ
            (*cnode)->number_keys = 1;
            (*cnode)->keys[0] = (*cnode)->keys[2];
            (*cnode)->keys[1] = (*cnode)->keys[2] = INT_MAX;
            (*cnode)->children[0] = (*cnode)->children[2];
            (*cnode)->children[1] = (*cnode)->children[3];
            (*cnode)->children[2] = (*cnode)->children[3] = NULL;
            if ((*cnode)->children[0]) (*cnode)->children[0]->parent = (*cnode);
            if ((*cnode)->children[1]) (*cnode)->children[1]->parent = (*cnode);

            (*cnode)->parent->children[3] = (*cnode)->parent->children[2];
            (*cnode)->parent->children[2] = (*cnode);
            (*cnode)->parent->children[1] = new_node; 
        }
        else if(idx == 2) {
            (*cnode)->parent->keys[2] = i;
            (*cnode)->parent->number_keys++;
            //新ノードを親のchildren[2]へ
            Node *new_node = (Node *)malloc(sizeof(Node));
            //(*cnode)->children[0]->parent = (*cnode)->children[1]->parent = new_node;
            new_node->number_keys = 1;
            new_node->keys[0] = (*cnode)->keys[0];
            new_node->keys[1] = new_node->keys[2] = INT_MAX;
            new_node->parent = (*cnode)->parent;
            new_node->children[0] = (*cnode)->children[0];
            new_node->children[1] = (*cnode)->children[1];
            new_node->children[2] = new_node->children[3] = NULL;
            new_node->children[0]->parent = new_node;
            new_node->children[1]->parent = new_node;
            if (new_node->children[0]) new_node->children[0]->parent = new_node;
            if (new_node->children[1]) new_node->children[1]->parent = new_node;
            
            //現在のノードは親のchildren[3]へ
            (*cnode)->number_keys = 1;
            (*cnode)->keys[0] = (*cnode)->keys[2];
            (*cnode)->keys[1] = (*cnode)->keys[2] = INT_MAX;
            (*cnode)->children[0] = (*cnode)->children[2];
            (*cnode)->children[1] = (*cnode)->children[3];
            (*cnode)->children[2] = (*cnode)->children[3] = NULL;
            if ((*cnode)->children[0]) (*cnode)->children[0]->parent = (*cnode);
            if ((*cnode)->children[1]) (*cnode)->children[1]->parent = (*cnode);

            (*cnode)->parent->children[3] = (*cnode);
            (*cnode)->parent->children[2] = new_node; 
        }
        else{
            printf("children_index error\n");
            return -1;
        }
        //親ノードのkeyが3つになった場合
        if((*cnode)->parent->number_keys == 3) {
            InsertUP(&(*cnode)->parent, (*cnode)->parent->keys[1],NULL, NULL, up,children_index);
        }
    return 0;
    }
}


int Delete_leaf(Node **cnode, int i,int children_index){
    if(*cnode == NULL) {
        printf("empty");
        return 0;
    }
    Delete_leaf(&(*cnode)->children[0], i, 0);
    Delete_leaf(&(*cnode)->children[1], i, 1);
    Delete_leaf(&(*cnode)->children[2], i, 2);
    printf("This is delete & i: %d.\n", i);
    //葉ノードの削除
    for(int j = 0; j < (*cnode)->number_keys; j++) {
        if((*cnode)->keys[j] == i) {
            // キーが見つかった場合
            printf("Key %d found and deleted.\n", i);
            //ノードが葉ではない場合
            if((*cnode)->children[0] != NULL) {
                // 右の子ノードから最小値を値を取得
                //Node *right_child = (*cnode)->children[(*cnode)->number_keys];
                Node *right_child = (*cnode)->children[j+1];
                while(right_child->children[0] != NULL) {
                    right_child = right_child->children[0];
                }
                // 最小値を置き換え
                int tmp = (*cnode)->keys[j];
                (*cnode)->keys[j] = right_child->keys[0];
                right_child->keys[0] = tmp;
                printf("ddd1dd%d\n", right_child->keys[0]);
                printf("ddd1dd%d\n", children_index);
                //Actually_delete_leaf(&right_child, 0,children_index);
                Actually_delete_leaf(&right_child, 0,1);
                printf("return1\n");
                break;
            }
            //ノードが葉の場合
            Actually_delete_leaf(cnode, j,children_index);
            printf("return2\n");
            break;
            //return 0;

        }

    }
    
}

int Delete_internal(Node **cnode, int i,int children_index){
    //printf("children_index: %d\n", children_index);
    if(*cnode == NULL) {
        //printf("empty");
        //printf("delete leaf not found.\n");
        return 0;
    }
    Delete_internal(&(*cnode)->children[0], i, 0);
    Delete_internal(&(*cnode)->children[1], i, 1);
    Delete_internal(&(*cnode)->children[2], i, 2);
    //printf("internal node delete\n");
    //printf("number_keys::%d\n",(*cnode)->number_keys);
    for(int j = 0; j < (*cnode)->number_keys; j++) {
        //printf("number_keys:%d keys[j]:%d\n",(*cnode)->number_keys,j);
        if((*cnode)->keys[j] == INT_MIN) {
            // キーが見つかった場合
            printf("INT_MIN found and deleted.\n");
            Actually_delete_internal(cnode, j,children_index);
            return 0;

        }
    }
    return 0;
}

int Actually_delete_internal(Node **cnode, int i,int children_index){
    if((*cnode)->number_keys == 1) {
        //兄弟にkey_numberが2のものがないか探す
        // 1つ右の兄弟が2つのキーを持っている場合
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
            printf("Leaf node is 1 key, borrowed from right sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 1つ左の兄弟が2つのキーを持っている場合
        else if(children_index!=0 && (*cnode)->parent->children[children_index-1] != NULL && (*cnode)->parent->children[children_index-1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[1];
            (*cnode)->parent->children[children_index-1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-1]->number_keys--;
            (*cnode)->children[1] = (*cnode)->children[0];
            (*cnode)->children[0] = (*cnode)->parent->children[children_index-1]->children[2];
            (*cnode)->parent->children[children_index-1]->children[2] = NULL;
            printf("Leaf node is 1 key, borrowed from left sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 2つ右の兄弟が2つのキーを持っている場合
        else if(children_index==0 && (*cnode)->parent->children[children_index+2] != NULL && (*cnode)->parent->children[children_index+2]->number_keys == 2) {
            //(*cnode)->children[0]->keys[1] = (*cnode)->children[1]->keys[0];
            //(*cnode)->children[0]->number_keys = 2;
            //(*cnode)->children[1]->keys[0] = (*cnode)->parent->children[children_index+1]->keys[0];
            //(*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            //(*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            //(*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->keys[children_index+1];
            //(*cnode)->parent->keys[children_index+1] = (*cnode)->parent->children[children_index+2]->keys[0];
            //(*cnode)->parent->children[children_index+2]->keys[0] = (*cnode)->parent->children[children_index+2]->keys[1];
            //(*cnode)->parent->children[children_index+2]->keys[1] = INT_MAX;
            //(*cnode)->parent->children[children_index+2]->number_keys--;
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->keys[children_index+1];
            (*cnode)->parent->keys[children_index+1] = INT_MAX;
            (*cnode)->parent->number_keys--;
            (*cnode)->keys[1] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->children[1] = (*cnode)->parent->children[children_index+1]->children[0];
            (*cnode)->children[2] = (*cnode)->parent->children[children_index+1]->children[1];
            (*cnode)->parent->children[children_index+1]->children[0] = NULL;
            (*cnode)->parent->children[children_index+1]->children[1] = NULL;
            free((*cnode)->parent->children[children_index+1]);
            (*cnode)->parent->children[children_index+1] = (*cnode)->parent->children[children_index+2];
            (*cnode)->parent->children[children_index+2] = NULL;
            
            //printf("Leaf node is 1 key, borrowed from right sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 2つ左の兄弟が2つのキーを持っている場合
        else if(children_index==2 && (*cnode)->parent->children[children_index-2] != NULL && (*cnode)->parent->children[children_index-2]->number_keys == 2) {
            printf("aaaaa\n");
            (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->keys[children_index-1];
            printf("bbbbb\n");
        
            (*cnode)->parent->children[1]->number_keys = 2;
            printf("children[1]->number_keys: %d\n", (*cnode)->parent->children[1]->number_keys);
            
            //(*cnode)->parent->keys[children_index-1] = INT_MAX;
            printf("ccccc\n");
            
            (*cnode)->parent->number_keys--;
            printf("ddddd\n");
            
            (*cnode)->parent->keys[children_index-1] = INT_MAX;
            printf("eeeee\n");
            
            (*cnode)->parent->children[1]->children[2] = (*cnode)->children[0];
            printf("%d",(*cnode)->parent->children[1]->children[2]->number_keys);
            printf("fffff\n");
            
            
            //printf("children[0]: %d\n", (*cnode)->children[0]->keys[0]);
            free((*cnode)->children[0]);
            printf("ggggg\n");
            
            (*cnode)->parent->children[2] = NULL;
            printf("hhhhh\n");

            free((*cnode));
            //(*cnode)->keys[1] = (*cnode)->parent->children[children_index+1]->keys[0];
            //(*cnode)->children[1] = (*cnode)->parent->children[children_index+1]->children[0];
            //(*cnode)->children[2] = (*cnode)->parent->children[children_index+1]->children[1];
            //(*cnode)->parent->children[children_index+1]->children[0] = NULL;
            //(*cnode)->parent->children[children_index+1]->children[1] = NULL;
            //free((*cnode)->parent->children[children_index+1]);
            //(*cnode)->parent->children[children_index+1] = (*cnode)->parent->children[children_index+2];
            //(*cnode)->parent->children[children_index+2] = NULL;
            
            //printf("Leaf node is 1 key, borrowed from left sibling.\n");
            printf("method 2.\n");
            return 0;
        }
    }
}

int Actually_delete_leaf(Node **cnode, int i,int children_index){
    printf("children_index: %d\n", children_index);
    printf("i: %d\n", i);
    //if(*cnode == NULL) {
    //    printf("delete leaf not found.\n");
    //    return 0;
    //}
    //キーが2つの場合
    if((*cnode)->number_keys == 2) {
        if(i == 0) {
            (*cnode)->keys[0] = (*cnode)->keys[1];
            (*cnode)->keys[1] = INT_MAX;
            (*cnode)->number_keys--;
        //printf("method 1-1.\n");

        }
        else if(i == 1) {
            (*cnode)->keys[1] = INT_MAX;
            (*cnode)->number_keys--;
            //printf("method 1-2.\n");
        }
        printf("method 1.\n");
        return 0;
    }
    // キーが1つの場合
    if((*cnode)->number_keys == 1) {
        //兄弟にkey_numberが2のものがないか探す
        // 1つ右の兄弟が2つのキーを持っている場合
        if(children_index!=2 && (*cnode)->parent->children[children_index+1] != NULL && (*cnode)->parent->children[children_index+1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->children[children_index+1]->keys[1];
            (*cnode)->parent->children[children_index+1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index+1]->number_keys--;
            printf("Leaf node is 1 key, borrowed from right sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 1つ左の兄弟が2つのキーを持っている場合
        else if(children_index!=0 && (*cnode)->parent->children[children_index-1] != NULL && (*cnode)->parent->children[children_index-1]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[1];
            (*cnode)->parent->children[children_index-1]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-1]->number_keys--;
            printf("Leaf node is 1 key, borrowed from left sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 2つ右の兄弟が2つのキーを持っている場合
        else if(children_index==0 && (*cnode)->parent->children[children_index+2] != NULL && (*cnode)->parent->children[children_index+2]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index];
            (*cnode)->parent->keys[children_index] = (*cnode)->parent->children[children_index+1]->keys[0];
            (*cnode)->parent->children[children_index+1]->keys[0] = (*cnode)->parent->keys[children_index+1];
            (*cnode)->parent->keys[children_index+1] = (*cnode)->parent->children[children_index+2]->keys[0];
            (*cnode)->parent->children[children_index+2]->keys[0] = (*cnode)->parent->children[children_index+2]->keys[1];
            (*cnode)->parent->children[children_index+2]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index+2]->number_keys--;
            printf("Leaf node is 1 key, borrowed from right sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        // 2つ左の兄弟が2つのキーを持っている場合
        else if(children_index==2 && (*cnode)->parent->children[children_index-2] != NULL && (*cnode)->parent->children[children_index-2]->number_keys == 2) {
            (*cnode)->keys[0] = (*cnode)->parent->keys[children_index-1];
            (*cnode)->parent->keys[children_index-1] = (*cnode)->parent->children[children_index-1]->keys[0];
            (*cnode)->parent->children[children_index-1]->keys[0] = (*cnode)->parent->keys[children_index-2];
            (*cnode)->parent->keys[children_index-2] = (*cnode)->parent->children[children_index-2]->keys[1];
            (*cnode)->parent->children[children_index-2]->keys[1] = INT_MAX;
            (*cnode)->parent->children[children_index-2]->number_keys--;
            printf("Leaf node is 1 key, borrowed from left sibling.\n");
            printf("method 2.\n");
            return 0;
        }
        //兄弟のキーが1つの場合
        else{
            if((*cnode)->parent->number_keys==2){
                //親のキーが2つの場合
                printf("Leaf node is 1 key, parent has 2 keys.\n");
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
                    printf("parent->number_keys%d\n", (*cnode)->parent->number_keys);

                }
                else if(children_index==2){
                    printf("aaaaa\n");
                    (*cnode)->parent->children[1]->keys[1] = (*cnode)->parent->keys[1];
                    printf("parent->children[1]->keys[1]:%d",(*cnode)->parent->children[1]->keys[1]);
                    (*cnode)->parent->children[1]->number_keys = 2;
                    (*cnode)->parent->keys[1] = INT_MAX;
                    (*cnode)->parent->number_keys--;
                    (*cnode)->parent->children[2] = NULL;
                    //free((*cnode)->parent->children[2]);
                    free((*cnode));
                }
                printf("method 3.\n");
                return 0;
            }
            else{
                //親のキーが1つの場合
                printf("Leaf node is 1 key, parent has 1 key.\n");
                if(children_index==0){
                    printf("qqqqq\n");
                    (*cnode)->parent->children[0]->keys[0] = (*cnode)->parent->keys[0];
                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->children[1]->keys[0];
                }
                if(children_index==1){
                    printf("ppppp\n");

                    (*cnode)->parent->children[0]->keys[1] = (*cnode)->parent->keys[0];

                }
                (*cnode)->parent->children[0]->number_keys = 2;
                (*cnode)->parent->keys[0] = INT_MIN;
                //(*cnode)->parent->number_keys--;
                Node *tmp;
                tmp = (*cnode)->parent->children[1];
                (*cnode)->parent->children[1] = NULL;
                free(tmp);
                printf("method 4.\n");
            }
        }
        return 0;
    }
    else{
        printf("Leaf node have not key.\n");
        return 0;
    }
}



int main(void){
    int i;
    int selection;
    tree.root = NULL; //木の根をNULLに初期化
    //Insert(&tree.root,5, NULL, NULL,down,0);
    //Insert(&tree.root,3, NULL, NULL,down,0);
    //Insert(&tree.root,8, NULL, NULL,down,0);
    //Insert(&tree.root,9, NULL, NULL,down,0);
    //Insert(&tree.root,10, NULL, NULL,down,0);
    //Insert(&tree.root,11, NULL, NULL,down,0);
    //Insert(&tree.root,12, NULL, NULL,down,0);
    //Insert(&tree.root,13, NULL, NULL,down,0);
    //Insert(&tree.root,6, NULL, NULL,down,0);
    //Insert(&tree.root,7, NULL, NULL,down,0);
    //Insert(&tree.root,1, NULL, NULL,down,0);
    //Insert(&tree.root,2, NULL, NULL,down,0);
    //Insert(&tree.root,14, NULL, NULL,down,0);
    //Insert(&tree.root,15, NULL, NULL,down,0);
    Insert(&tree.root,11, NULL, NULL,down,0);
    //Search(&tree.root,0);

    Insert(&tree.root,10, NULL, NULL,down,0);
    //Search(&tree.root,0);

    Insert(&tree.root,9, NULL, NULL,down,0);
    Insert(&tree.root,8, NULL, NULL,down,0);
    Insert(&tree.root,7, NULL, NULL,down,0);
    Insert(&tree.root,6, NULL, NULL,down,0);
    Insert(&tree.root,5, NULL, NULL,down,0);
    Insert(&tree.root,4, NULL, NULL,down,0);
    Insert(&tree.root,12, NULL, NULL,down,0);
    Insert(&tree.root,13, NULL, NULL,down,0);
    Insert(&tree.root,14, NULL, NULL,down,0);
    Insert(&tree.root,15, NULL, NULL,down,0);
    Insert(&tree.root,3, NULL, NULL,down,0);
    Insert(&tree.root,2, NULL, NULL,down,0);




    while (1){
        printf("\nEnter a command (1: Insert, 2: Search, 3: Delete, 4: Exit): "); 
        scanf("%d", &selection);
        switch(selection){
        case 1 :
            printf("Insert:\n");
            scanf("%d", &i);
            Insert(&tree.root,i, NULL, NULL,down,0);
            break;
        case 2 :
            printf("Search\n");
            Search(&tree.root,0);
            break;
        case 3 :
            printf("Delete:\n");
            scanf("%d", &i);
            Delete_leaf((&tree.root), i,0);
            Search(&tree.root,0);
            printf("oooooo:\n");
            Delete_internal((&tree.root), i,0);
            break;
        case 4 :
            printf("Exit\n");
            return 0;
            break;
        default:
            printf("Invalid selection. Please try again.\n");
            break;
        }
    }
}