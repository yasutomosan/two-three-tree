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
int InsertUP(Node **cnode, int i,int state);



int Search(Node **cnode,int space){
    if(tree.root == NULL) {
        printf("Tree is empty.\n");
    }
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
    //printf("This is insert & i: %d  state: %d.\n", i,state);

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
    
    //printf("current->number_keys: %d\n", current->number_keys);
    //printf("keys %d j: %d\n", current->keys[0], j);
    current->keys[j+1] = i;
    //printf("current->keys[j+1]: %d j:%d\n", current->keys[j+1],j);
    current->number_keys++;
    //printf("current->number_keys: %d\n", current->number_keys);
    //Search(&tree.root,0);

    //printf("dividebefore\n");
    //Search(&tree.root,0);
    // 分割処理
    if(current->number_keys == 3) {
        // その後、親ノードに新しいキーを挿入
        //printf("current->keys[1]: %d\n", current->keys[1]);
        //printf("current->keys[2]: %d\n", current->keys[2]);
        if(current->parent!= NULL) {
        //printf("current->parent->keys[0]: %d\n", current->parent->keys[0]);
        }
        InsertUP(&current, current->keys[1], up);

    }
    //Search(&tree.root,0);
    return 0;
}

int InsertUP(Node **cnode, int i,int state){
    //printf("This is insertUP & i: %d  state: %d.\n", i,state);
    //printf("keys: %d %d %d\n", (*cnode)->keys[0], (*cnode)->keys[1], (*cnode)->keys[2]);

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

        // 子のparent設定
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

        printf("up\n");
        return 0;
    }
    else {
        //printf("parentkeys: %d %d %d\n", (*cnode)->parent->keys[0], (*cnode)->parent->keys[1], (*cnode)->parent->keys[2]);

        int idx = -1;
        for (int k = 0; k < 4; k++) {
            if ((*cnode)->parent->children[k] == (*cnode)) {
                idx = k;
                break;
            }
        }

        if (idx == -1) {
            printf("children_index error: parent->childrenにcnodeが見つかりません\n");
            return -1;
        }

        // 親ノードに挿入するための準備
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

        // 親にキーと子ノードを挿入
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

        // 再帰的に親ノードもオーバーフローした場合
        if(parent->number_keys == 3){
            InsertUP(&parent, parent->keys[1], up);
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
    //printf("This is delete & i: %d.\n", i);
    //葉ノードの削除
    for(int j = 0; j < (*cnode)->number_keys; j++) {
        if((*cnode)->keys[j] == i) {
            // キーが見つかった場合
            //printf("Key %d found and deleted.\n", i);
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
                //printf("ddd1dd%d\n", right_child->keys[0]);
                //printf("ddd1dd%d\n", children_index);
                //Actually_delete_leaf(&right_child, 0,children_index);
                Actually_delete_leaf(&right_child, 0,1);
                //printf("return1\n");
                break;
            }
            //ノードが葉の場合
            Actually_delete_leaf(cnode, j,children_index);
            //printf("return2\n");
            break;
            return 0;

        }

    }
    //printf("No key %d found in this node.\n", i);
    return 0;
    
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
            //printf("INT_MIN found and deleted.\n");
            Actually_delete_internal(cnode, j,children_index);
            return 0;

        }
    }
    return 0;
}

int Actually_delete_internal(Node **cnode, int i, int children_index) {
    Node *parent = (*cnode)->parent;

    // 兄弟から借りる（右）
    if (children_index != 2 && parent->children[children_index + 1] &&
        parent->children[children_index + 1]->number_keys == 2) {

        Node *right = parent->children[children_index + 1];

        (*cnode)->keys[0] = parent->keys[children_index];
        parent->keys[children_index] = right->keys[0];

        right->keys[0] = right->keys[1];
        right->keys[1] = INT_MAX;
        right->number_keys--;

        (*cnode)->children[1] = right->children[0];
        if ((*cnode)->children[1]) (*cnode)->children[1]->parent = *cnode;

        right->children[0] = right->children[1];
        right->children[1] = right->children[2];
        right->children[2] = NULL;

        //printf("Borrowed from right sibling.\n");
        return 0;
    }

    // 兄弟から借りる（左）
    if (children_index != 0 && parent->children[children_index - 1] &&
        parent->children[children_index - 1]->number_keys == 2) {

        Node *left = parent->children[children_index - 1];

        (*cnode)->keys[0] = parent->keys[children_index - 1];
        parent->keys[children_index - 1] = left->keys[1];

        left->keys[1] = INT_MAX;
        left->number_keys--;

        // shift children
        (*cnode)->children[1] = (*cnode)->children[0];
        (*cnode)->children[0] = left->children[2];
        if ((*cnode)->children[0]) (*cnode)->children[0]->parent = *cnode;
        left->children[2] = NULL;

        //printf("Borrowed from left sibling.\n");
        return 0;
    }

    // マージ with right
    if (children_index != 2 && parent->children[children_index + 1]) {
        Node *right = parent->children[children_index + 1];

        (*cnode)->keys[1] = parent->keys[children_index];
        (*cnode)->number_keys = 2;

        (*cnode)->children[2] = right->children[0];
        if ((*cnode)->children[2]) (*cnode)->children[2]->parent = *cnode;

        // 親の keys/children を詰める
        parent->keys[children_index] = parent->keys[children_index + 1];
        parent->keys[children_index + 1] = INT_MAX;

        parent->children[children_index + 1] = parent->children[children_index + 2];
        parent->children[children_index + 2] = NULL;

        parent->number_keys--;

        free(right);
        //printf("Merged with right sibling.\n");
    }
    // マージ with left
    else if (children_index != 0 && parent->children[children_index - 1]) {
        Node *left = parent->children[children_index - 1];

        left->keys[1] = parent->keys[children_index - 1];
        left->number_keys = 2;

        left->children[2] = (*cnode)->children[0];
        if (left->children[2]) left->children[2]->parent = left;

        parent->keys[children_index - 1] = parent->keys[children_index];
        parent->keys[children_index] = INT_MAX;

        parent->children[children_index] = parent->children[children_index + 1];
        parent->children[children_index + 1] = NULL;

        parent->number_keys--;

        free(*cnode);
        *cnode = NULL;

        //printf("Merged with left sibling.\n");
    }

    // --- root 縮退処理を最後にチェック ---
    if (parent == NULL && (*cnode)->number_keys == 0) {
        int hasChild = 0;
        for (int i = 0; i < 4; i++) {
            if ((*cnode)->children[i] != NULL) {
                hasChild = 1;
                break;
            }
        }

        if (hasChild) {
            for (int i = 0; i < 4; i++) {
                if ((*cnode)->children[i]) {
                    tree.root = (*cnode)->children[i];
                    tree.root->parent = NULL;
                    break;
                }
            }
        } else {
            tree.root = NULL;
        }

        free(*cnode);
        *cnode = NULL;
        //printf("Root node removed. Tree is now empty or shrunk.\n");
        return 0;
    }

    return 0;
}


int Actually_delete_leaf(Node **cnode, int i, int children_index){
    //printf("children_index: %d\n", children_index);
    printf("i: %d\n", i);
    if((*cnode)->parent == NULL && (*cnode)->number_keys == 1 && (*cnode)->children[0] == NULL) {
        free(*cnode);
        *cnode = NULL;
        tree.root = NULL;
        //printf("Root node removed. Tree is now empty.\n");
        return 0;
    }
    if((*cnode)->number_keys == 2) {
        if(i == 0) {
            (*cnode)->keys[0] = (*cnode)->keys[1];
            (*cnode)->keys[1] = INT_MAX;
        } else if(i == 1) {
            (*cnode)->keys[1] = INT_MAX;
        }
        (*cnode)->number_keys--;
        //printf("method 1.\n");
        return 0;
    }

    if((*cnode)->number_keys == 1) {
        Node *parent = (*cnode)->parent;

        // 借用（右）
        if(children_index != 2 && parent->children[children_index + 1] &&
           parent->children[children_index + 1]->number_keys == 2) {
            Node *right = parent->children[children_index + 1];
            (*cnode)->keys[0] = parent->keys[children_index];
            parent->keys[children_index] = right->keys[0];
            right->keys[0] = right->keys[1];
            right->keys[1] = INT_MAX;
            right->number_keys--;
            //printf("method 2: borrow from right.\n");
            return 0;
        }

        // 借用（左）
        if(children_index != 0 && parent->children[children_index - 1] &&
           parent->children[children_index - 1]->number_keys == 2) {
            Node *left = parent->children[children_index - 1];
            (*cnode)->keys[0] = parent->keys[children_index - 1];
            parent->keys[children_index - 1] = left->keys[1];
            left->keys[1] = INT_MAX;
            left->number_keys--;
            //printf("method 2: borrow from left.\n");
            return 0;
        }

        // マージ（兄弟も1キー） → 親のキーを取り込んで2キーにする
        if(parent->number_keys == 2) {
            //printf("Leaf node is 1 key, parent has 2 keys.\n");

            if(children_index == 2) {
                Node *left = parent->children[1];
                left->keys[1] = parent->keys[1];
                left->number_keys = 2;
                parent->keys[1] = INT_MAX;
                parent->number_keys--;
                parent->children[2] = NULL;
                free(*cnode);
                *cnode = NULL;
                //printf("method 3.\n");
                return 0;
            }
            if(children_index == 1) {
                Node *left = parent->children[0];
                left->keys[1] = parent->keys[0];
                left->number_keys = 2;
                parent->keys[0] = parent->keys[1];
                parent->keys[1] = INT_MAX;
                parent->number_keys--;
                free(*cnode);
                parent->children[1] = parent->children[2];
                parent->children[2] = NULL;
                *cnode = NULL;
                //printf("method 3.\n");
                return 0;
            }
            if(children_index == 0) {
                Node *right = parent->children[1];
                (*cnode)->keys[1] = parent->keys[0];
                (*cnode)->number_keys = 2;
                parent->keys[0] = parent->keys[1];
                parent->keys[1] = INT_MAX;
                parent->number_keys--;
                free(right);
                parent->children[1] = parent->children[2];
                parent->children[2] = NULL;
                //printf("method 3.\n");
                return 0;
            }
        }
        else if(parent->number_keys == 1) {
            //printf("Leaf node is 1 key, parent has 1 key.\n");

            Node *sibling = NULL;
            if(children_index == 1) {
                //printf("ppppp\n");
                sibling = parent->children[0];
                sibling->keys[1] = parent->keys[0];
                sibling->number_keys = 2;
            }
            else if(children_index == 0) {
                //printf("qqqqq\n");
                sibling = (*cnode); // *cnode は左の子自身（残る側）
                sibling->keys[1] = parent->keys[0];
                sibling->number_keys = 2;

                // 親が root なら縮退処理を行う
                if (parent->parent == NULL) {
                    tree.root = sibling;
                    sibling->parent = NULL;
                    free(parent);
                    //printf("method 4 (left): root replaced by merged child.\n");
                } else {
                    // 親が root でなければ通常のマージ
                    parent->keys[0] = INT_MIN;
                    parent->number_keys--;
                
                    // 親の children を詰める
                    for(int k = children_index; k < 3; k++) {
                        parent->children[k] = parent->children[k + 1];
                    }
                    parent->children[3] = NULL;
                }
            
                free(*cnode);
                *cnode = NULL;
                return 0;
            }



            // 親が root の場合は縮退
            if(parent->parent == NULL) {
                tree.root = sibling;
                sibling->parent = NULL;
                free(parent);
                //printf("method 4: root replaced by merged child.\n");
            } else {
                parent->keys[0] = INT_MIN;
                parent->number_keys--;
                // 親の children を整理（安全性向上のため）
                for(int k = children_index; k < 3; k++) {
                    parent->children[k] = parent->children[k + 1];
                }
                parent->children[3] = NULL;
            }

            free(*cnode);
            *cnode = NULL;
            return 0;
        }

        //printf("method 4.\n");
        return 0;
    }

    //printf("Leaf node has no key.\n");
    return 0;
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
    //Insert(&tree.root,11, NULL, NULL,down,0);
    ////Search(&tree.root,0);

    //Insert(&tree.root,10, NULL, NULL,down,0);
    ////Search(&tree.root,0);

    //Insert(&tree.root,9, NULL, NULL,down,0);
    //Insert(&tree.root,8, NULL, NULL,down,0);
    //Insert(&tree.root,7, NULL, NULL,down,0);
    //Insert(&tree.root,6, NULL, NULL,down,0);
    //Insert(&tree.root,5, NULL, NULL,down,0);
    //Insert(&tree.root,4, NULL, NULL,down,0);
    //Insert(&tree.root,12, NULL, NULL,down,0);
    //Insert(&tree.root,13, NULL, NULL,down,0);
    //Insert(&tree.root,14, NULL, NULL,down,0);
    //Insert(&tree.root,15, NULL, NULL,down,0);
    //Insert(&tree.root,3, NULL, NULL,down,0);
    //Insert(&tree.root,2, NULL, NULL,down,0);




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
            //printf("oooooo:\n");
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