#include<iostream>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map> 
using namespace std; 
  
// An AVL tree node  
class Node  
{  
    public: 
    int key;  
    Node *left;  
    Node *right;  

    int height;  
    // int distance;
    int h_s;   //start index of the hole
    int h_e;   //end index of the hole
    int h_n;   //the total size of all the holes in the subtree rooted but its right child
};  
Node* root;

unordered_map<int,int> dic;  //checking existance of nodes
map<int, int> result;   //store results of reuse distance
int max(int a,int b);
Node* insert(Node* node, int key, int index);
Node * minValueNode(Node* node);
void printInorder(Node* node);
Node* deleteNode(Node* root, int key);

int max(int a, int b)  
{  
    return (a > b)? a : b;  
}  


Node* newNode(int key, int index)  
{  
    Node* node = new Node(); 
    node->key = key;  
    node->left = NULL;  
    node->right = NULL;  
    node->height = 1; // new node is initially 
                      // added at leaf  
    // node->distance = 0;
    node->h_s = index;
    node->h_e = index;
    node->h_n = 0;
    return(node);  
}  
// A utility function to get the  
// height of the tree  
int height(Node *n)  
{  
    if (n == NULL)  
        return 0;  
    return n->height;  
}  
//update subtree node count
void update_sub(Node* node){
    if(node->left!=NULL && node->right!=NULL){
        node->h_n=node->left->h_n+node->right->h_n+2;
    }else if(node->left!=NULL&&node->right==NULL){
        node->h_n = node->left->h_n+1;
    }else if(node->left==NULL && node->right!=NULL){
        node->h_n= node->right->h_n+1;
    }else{
        node->h_n=0;
    }
}

// A utility function to right 
// rotate subtree rooted with y  
// See the diagram given above.  
Node *rightRotate(Node *z)  
{  
    Node *y = z->left;  
    Node *T3 = y->right;  
  
    // Perform rotation  
    y->right = z; 
    z->left = T3; 
  
    // Update heights  
    z->height = max(height(z->left), 
                    height(z->right)) + 1;  
    y->height = max(height(y->left), 
                    height(y->right)) + 1;  
    update_sub(z);
    // cout<<z->key <<": hn is"<<z->h_n<<endl;
    update_sub(y);
    // cout<<y->key <<": hn is"<<y->h_n<<endl;
    // Return new root  
    return y;  
} 

Node *leftRotate(Node *z)  
{  
    Node *y = z->right;  
    Node *T2 = y->left;  
  
    // Perform rotation  
    y->left = z;  

    z->right = T2; 
  
    // Update heights  
    z->height = max(height(z->left),     
                    height(z->right)) + 1;  
    y->height = max(height(y->left),  
                    height(y->right)) + 1;  
    update_sub(z);
    update_sub(y);
    // Return new root  
    return y;  
}  
  
// Get Balance factor of node N  
int getBalance(Node *N)  
{  
    if (N == NULL)  
        return 0;  
    return height(N->left) - height(N->right);  
}  



/**
 * 1. Count reuse distance from the tree
 * 
 * Update:
 * 2. Delete the old node and perform balancing and update the augmented field
 * 3. Insert the new node into the left most node and update the augmented field
 **/
Node* count_reuse(int value, int old_val, int new_val)
{
    Node* r = root;
    if(r==NULL)
        return NULL;
    int reuse = r->h_n;
    int left = -1;
    int rot = 1;
    // printInorder(root);
    while(r->key!=value){
        if(rot==1){
            //keep going left from root
            if(old_val>r->h_e&&(left==1||left==-1)){
                left=1;
                r=r->left;
                continue;
            }
            if(old_val<r->h_e&&(left==0||left==-1)){
                left=0;
                r=r->right;
                continue;
            }
            rot=0;
            if(left==1){
                reuse = 0;
             
            }else{
                reuse = root->h_n;
                
            }
        }
        if(old_val>r->h_e){
            //going left
            if(left==0){//left flag is 0 on turn
                reuse= reuse - r->h_n;
                r=r->left;
                left=1;
                continue;
            }else{//keep
                r=r->left;
            } 
        }else{
            //going right
            if(left==1){//left flag is 1 on turn
                reuse = reuse+r->h_n;
                r=r->right;
                left=0;
                continue;
            }else{
                r=r->right;
            }
        }
    }
    //count reuse distance and add to result
    if(rot==1){
        if(left==1){
            reuse = r->left==NULL?0:(r->left->h_n+1);  
            if(result.find(reuse)==result.end()){
                result.insert(make_pair(reuse,1));
            }else{
                result.at(reuse)+=1;
            }
        }else if(left==0){
            reuse = root->h_n - ((r->right==NULL)?0:(r->right->h_n+1)); 
            if(result.find(reuse)==result.end()){
                result.insert(make_pair(reuse,1));
            }else{
                result.at(reuse)+=1;
            }
        }else{
            reuse = r->left==NULL?0:(r->left->h_n+1);
            if(result.find(reuse)==result.end()){
                result.insert(make_pair(reuse,1));
            }else{
                result.at(reuse)+=1;
            }
        }
    }else{
        int le = r->left==NULL?0:(r->left->h_n+1);
        reuse = reuse+ le;
        if(result.find(reuse)==result.end()){
            result.insert(make_pair(reuse,1));
        }else{
            result.at(reuse)+=1;
        }
    }
    // cout<<root->key<<": root hn is "<<root->h_n<<endl;
    // cout<<"reuse distance is "<<reuse<<endl;
    //delete node and insert back
    root = deleteNode(root, old_val);
    root = insert(root,value, new_val);   
    return root;
}


Node* insert(Node* node, int key, int index)  
{  
    //if root is null
    if (node == NULL)  
        return(newNode(key, index));  
    Node* left = minValueNode(node);
    // cout<<key<<" value coming"<<" index is"<< index<<"\n";
    if(key==-1){//if duplicate
        left->h_e = index;
        return node;
    }
    //if new node
    /* 2. Update height of this ancestor node */
    if (index > node->h_e){  
        node->left = insert(node->left, key, index);  
    }else if (index < node->h_e){  
        node->right = insert(node->right, key, index);  
    }
    node->height = 1 + max(height(node->left),  
                        height(node->right)); 
    // cout<<"height is"<<node->height<<"\n"; 
    //update #of node under the subtree
    // if(node->left!=NULL && node->right!=NULL){
    //     node->h_n=node->left->h_n+node->right->h_n+2;
    // }else if(node->left!=NULL&&node->right==NULL){
    //     node->h_n = node->left->h_n+1;
    // }else if(node->left==NULL && node->right!=NULL){
    //     node->h_n= node->right->h_n+1;
    // }else{
    //     node->h_n=0;
    // }
    update_sub(node);
    // cout<<node->key<<": h_n is"<<node->h_n<<"\n";

    /* 3. Get the balance factor of this ancestor  
        node to check whether this node became  
        unbalanced */
    int balance = getBalance(node);  
    // If this node becomes unbalanced, then  
    // there are 4 cases  
  
    // Left Left Case  
    
    if (balance > 1 && index > node->left->h_e){  
        // cout<<"right rotate"<<endl;
        return rightRotate(node);  
    }
    
    // Right Right Case  
    if (balance < -1 && index < node->right->h_e)  
        return leftRotate(node);  
  
    // Left Right Case  
    if (balance > 1 && index < node->left->h_e)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Left Case  
    if (balance < -1 && index > node->right->h_e)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    }  
  
    /* return the (unchanged) node pointer */
    return node;  
} 

/* Given a non-empty binary search tree,  
return the node with minimum key value  
found in that tree. Note that the entire  
tree does not need to be searched. */
Node * minValueNode(Node* node)  
{  
    Node* current = node;  
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)  
        current = current->left;  
  
    return current;  
}

Node* deleteNode(Node* node, int key)  
{  
      
    // STEP 1: PERFORM STANDARD BST DELETE  
    
    if (node== NULL)  
        return node;  
  
    // If the key to be deleted is smaller  
    // than the root's key, then it lies 
    // in left subtree  
    if ( key > node->h_e )  
        node->left = deleteNode(node->left, key);  
  
    // If the key to be deleted is greater  
    // than the root's key, then it lies  
    // in right subtree  
    else if( key < node->h_e)  
        node->right = deleteNode(node->right, key);  
  
    // if key is same as root's key, then  
    // This is the node to be deleted  
    else
    {  
        // node with only one child or no child  
        if( (node->left == NULL) || (node->right == NULL) )  
        {  
            Node *temp = node->left ?  
                         node->left :  
                         node->right;  
  
            // No child case  
            if (temp == NULL)  
            {  
                temp = node;  
                node = NULL;  
            }  
            else // One child case  
            *node = *temp; // Copy the contents of  
                           // the non-empty child  
            free(temp);  
        }  
        else
        {  
            // node with two children: Get the inorder  
            // successor (smallest in the right subtree)  
            Node* temp = minValueNode(node->right);  
  
            // Copy the inorder successor's  
            // data to this node  
            node->h_e = temp->h_e;  
            node->key = temp->key;
            // Delete the inorder successor  
            node->right = deleteNode(node->right,  
                                     temp->h_e);  
        }  
    }  
  
    // If the tree had only one node 
    // then return  
    if (node == NULL)  
    return node;  
  
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE  
    node->height = 1 + max(height(node->left),  
                           height(node->right));  
    update_sub(node);
    
    // STEP 3: GET THE BALANCE FACTOR OF  
    // THIS NODE (to check whether this  
    // node became unbalanced)  
    int balance = getBalance(node);  
  
    // If this node becomes unbalanced,  
    // then there are 4 cases  
  
    // Left Left Case  
    if (balance > 1 &&  getBalance(node->left) >= 0)  
        return rightRotate(node);  
  
    // Left Right Case  
    if (balance > 1 &&  getBalance(node->left) < 0)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Right Case  
    if (balance < -1 &&  getBalance(node->right) <= 0)  
        return leftRotate(node);  
  
    // Right Left Case  
    if (balance < -1 &&  getBalance(node->right) > 0)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    }  
  
    return node;  
}
void printInorder(Node* node) 
{ 
    if (node == NULL) 
        return; 
    /* first recur on left child */
    printInorder(node->left); 
  
    /* then print the data of node */
    cout << node->key << " "; 
    
    /* now recur on right child */
    printInorder(node->right); 
    
}
int main()  
{  
    root = NULL;  

    fstream newfile;
    newfile.open("trace.txt",ios::in);
    if (newfile.is_open()){   
        string line;
        int index=0;
        while(getline(newfile, line)){ 
        //    cout << line << "\n"; //print the data of the string
            int value = stoi(line);
            //get the number
            //if not in dictionary 
         
            if(dic.find(value)==dic.end()){
                dic.insert(make_pair(value,index));   //insert new node into dic
                root=insert(root, value,index);     //insert new node into tree
            }else{ //in the tree
                int last = dic.at(value);
                if(last==(index-1)){ //duplicate
                    dic.at(value)=index;        //update dic
                    insert(root,-1, index);  //insert duplicate 
                }else{// new node
                    root=count_reuse(value,last,index); //reuse -> delete old -> insert new
                    dic.at(value)=index;    //update dictionary
                    // printInorder(root);
                }
            }
            index++;
        }
      newfile.close(); //close the file object.
    }
    cout << "Inorder traversal"<<endl;  
    printInorder(root); 
    cout<<"\n";
    for(map<int,int>::iterator it = result.begin();it != result.end(); ++it){
        std::cout << " " << (*it).first << " " << (*it).second << "\n";
    }
    return 0;  
}