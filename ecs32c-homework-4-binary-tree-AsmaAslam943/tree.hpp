
// We are going to build a binary search tree with an explicit
// stack based iterator.  We are doing this as a "header-only"
// c++ implementation, as it is templated and therefore needs
// to be included to be used.  This provides basically the same
// high level functionality as std::map, that is, a key/value
// data store with a sorted order

// We need to include the following headers...

#include <memory>
#include <stack>
#include <utility>

#define HERE {std::cout << "IMPLEMENT HERE\n";}

// C++ require declaration before use, so we define
// our three classes here.
template <class K, class V>
class BinaryTree;
template <class K, class V>
class BinaryTreeIterator;
template <class K, class V>
class BinaryTreeNode;

// This iterator is returned for both start and
// end but only the start iterator matters, the end 
// iterator is effectively ignored.

// It is considered "undefined behavior" (that is,
// things are allowed to crash in obscure ways if)

// A: The iterator has a longer lifetime than the 
// underlying tree (translation: don't return the iterator)

// B: Nodes are added or removed from the tree
// (translation: its OK to use the iterator to change
// the data associated with keys using the iterator in a 
// for loop, but it is not OK to
// add new keys or remove keys)
template <class K, class V>
class BinaryTreeIterator : public std::input_iterator_tag
{
    friend class BinaryTree<K, V>;

    // The constructor.  For end you don't actually
    // need to do anything (the end is effectively a dummy).
    // For the start iterator, you should set current to root
    // and then call incr() so that the iterator is ready to 
    // actually start the traversal
    BinaryTreeIterator(BinaryTreeNode<K, V> *root, bool start)
    {
        current = root; //have to set current equal to root 
        if (start){
            incr(); //if start the increase 
        } 
    }

public:
    // This should return TRUE if there is still
    // items left in the iteration:
    // Basically, check to see if the stack is empty
    // and current is null, and, if not, there is 
    // still more work to do...
    bool operator!=(BinaryTreeIterator<K, V> &other)
    {
        (void) other; //set (Void) other so that other is void 
        if (current == nullptr && working_stack.empty()){
            return false; //we have to return false if current is null and if the working_stack is empty 
        } else{
            return true; //otherwise return true 
        }
    }

    // This is the heart of the tree traversal algorithm.
    // It needs to be called once in the constructor and 
    // once by each invocation of the ++ operator.
    // The idea in english...
    //
    // While current is not null, push the current
    // pointer onto the stack and take current->left 
    // and make it the new current.
    // 
    // Once current is null, then take the top of the stack
    // and set it to current...
    void incr()
    {
        while (current != nullptr){ //when current is NOT equal to the nullpointer we have to push current node onto my working stack 
            working_stack.push(current); 
            current = current -> left; //update current to left 
        }
        if (!working_stack.empty()){ //if working_stack is not empty then set current equal to top 
           current = working_stack.top(); 
           working_stack.pop(); //and then pop it 
        }
        else { //otherwise current is nullpointer 
            current = nullptr; 
        }
    }

    // This should just call incr
    void operator++()
    {
        incr();
    }

    // And this visits the node itself...
    // In visiting the node, it should 
    // both set current to be the current node's right
    // and then return a pair constructed from the old 
    // current node's key and value.
    std::pair<K, V> operator*()
    {
        BinaryTreeNode<K,V>*tmp = current; 
        current = current -> right; //continue to update current 
        // This isn't actually what you want to return,
        // its just a placeholder so the compiler doesn't complain.
        return std::pair<K,V>(tmp->key, tmp->value);
    }

private:
    // A pointer to the current node
    BinaryTreeNode<K, V> *current;

    // And a stack for the traversal visit of the tree
    // nodes.
    std::stack<BinaryTreeNode<K, V> *> working_stack;
};


// The class for the binary tree itself.
template <class K, class V>
class BinaryTree
{
public:
    BinaryTree() : root(nullptr)
    {
    }
    
    // The [] operation is for both getting and setting.
    // If the key exists in the tree a reference to the
    // associated value is returned.  Otherwise, it will
    // create a new tree node and return that value.

    // What you should do is first check if the root is null
    // and, if so, just create a new node.
    // then simply invoke root->find(key) and return
    // that value as find is already set up to return
    // a reference.
    V &operator[](const K &key)
    {
        if (root == nullptr){ //We need to check if the root is null 
            root = new BinaryTreeNode<K, V>(key);  
        }
        // THis is just to keep the compiler happy
        // so your code compiles, this is not what you
        // actually want to return
        return root->find(key); //this returns the root's value key if it is NOT null 
    }

    // This should return false if the tree
    // is empty, otherwise return root->contains
    bool contains(const K &key)
    {
        if (root == nullptr){
            return false; //since it's empty returns false 
        }
        return root->contains(key); //otherwise it returns root->contains key 
    }

    // Erases a node if a key matches.  If the
    // key does not match it simply is an operation
    // that does nothing.
    void erase(const K &key)
    {
        if (root == nullptr){ //if root is equal to nullptr simply return 
            return;
        } else{ //but if we have smthng in then we have to call on erase(key)
            root = root->erase(key); //otherwise we call erase on it
        }
    }

    // And the destructor for the binary tree.
    // In order to prevent memory leaks and keep with
    // the C++ "RAII" convention, it should see
    // if there is a root and, if so, call freetree()
    // on the root.
    ~BinaryTree()
    {
        if (root != nullptr){
            root-> freetree(); //we have to call on freetree to delete all nodes  
        }
        root = nullptr; //set root to nullptr 
    
    }

    // This returns the iterators.
    BinaryTreeIterator<K, V> begin()
    {
        return BinaryTreeIterator(root, true);
    }
    BinaryTreeIterator<K, V> end()
    {
        return BinaryTreeIterator(root, false);
    }

protected:
    BinaryTreeNode<K, V> *root;
};

// And the class for the binary tree node itself.
template <class K, class V>
class BinaryTreeNode
{
    friend class BinaryTree<K, V>;
    friend class BinaryTreeIterator<K, V>;

public:
    // The constructor, it simply setts the key and the left/right pointers.
    // Data defaults to whatever the default value is for the data type.
    BinaryTreeNode(const K &keyin) : key(keyin), left(nullptr), right(nullptr), height(1) //set height to 1 
    {
    }

    // This should recursively free the tree.
    // It should call freetree on left and 
    // right and then, as the last act,
    // call delete on this.  

    // Yes, you can "suicide" an object in C++,
    // and this is a case where you want to do it.
    void freetree()
    {
        if (left){ //if left does exxist then free and same for right 
            left ->freetree(); 
        }
        if (right){
            right->freetree();
        }
        delete this; //then delete this 
    }

protected:

    // Removing a node from a binary tree, returning
    // a pointer to the now modified tree.

    // If you aren't at the node you are looking for, just set
    // your new left or right pointer (as approriate, and if it exists) 
    // to the  result of recursively calling erase, and then return this.

    // If this node does match the key however, check the
    // child nodes.  If there is no left, assign the right node to a temporary
    // value, delete this, and return the right node.
    // If there is no right node, do the same thing.
    // If there is both a left and right node, check if the left node
    // doesn't have a right node of its own.  If so, set that node's right
    // to the current right, assign left to a temporary, and delete this and
    // return that left node.

    // Finally, if the left node DOES have a right node, keep
    // following the right hand side of that subtree until you
    // find a node without a right hand branch.  In that case set
    // the node above it's right to that node's left, assign that
    // node to a temporary, have its left point to the current node's left
    // its right to the current node's right, delete this and return that
    // node.
    BinaryTreeNode<K, V> *erase(const K &k)
    {
        if (k < key){ //if k < key then make sure left = left->erase(k )
            if (left){
                left = left ->erase(k); 
            }
        }
        else if (k > key){
            if (right){ //if right then make sure right = right->erase(k)
                right = right->erase(k); 
            }
        }
        else {
            if (!left){
                auto temp = right; 
                delete this; 
                return temp; //if left is null make sure auto temp is set to right and delete current node 

            }
            if (!right){ //if right is null then set to left and delete current node so it returns the temporary 
                auto temp = left; 
                delete this; 
                return temp; 
            }

            BinaryTreeNode<K,V>*temp = left; 
            if (!temp->right){ //if its not null then make sure temp->right is set to right 
                temp ->right = right; 
                delete this; 
                return temp; 
            }

            while (temp ->right ->right){ //make sure temp->right->right then temp = temp->right 
                temp = temp->right; 
            }

            BinaryTreeNode<K,V>*previous = temp ->right; //set a previous node 
            temp ->right = previous ->left; //set temporary to right and previous to left 
            previous->left = left; 
            previous ->right = right; 

            delete this; 
            return previous; //return previous 
        }
        // Again, not what you will always want to return...
        return rebalance(); //to pass the performance test we need an O(nlogn) complexity so we need to call the function at the bottom 
    }

    // This seeks to recursively find a key, creating
    // a new node if necessary.
    // If k == this node's key, return the value.
    // If k < this key it is to the left.
    // If there is no left node, create it with k as the key.
    // Then recursively return find on the left.  Similar for
    // the right. 
    V &find(const K &k)
    {
        if (k == key){ //if k == key then return the right value 
            return value; 
        }
        if (k < key){
            if (!left){ //if left is null then we need to create a left = new BinaryTreeNode(K)
                left = new BinaryTreeNode(k); //couldn't use <K,V> so I used (k) instead 
            }
            left = left->rebalance(); //again call rebalance to maintain O(nlogn)
            return left->find(k); //lastly find 
        }
        if (!right){
            right = new BinaryTreeNode(k); 
        }
        right = right->rebalance(); //rebalance and then returns updated right 
        return right->find(k); 
        // THis is just to keep the compiler happy
        // so your code compiles, this is not what you
        // actually want to return
    }

    // And contains is a recursive search that doesn't
    // create new nodes, just checks if the key exists.
    bool contains(const K &k)
    {
        if (k == key){ //if k == key then u have to return true 
            return true; 
        }
        if (k<key && left){ //ptherwise check if they exist 
            return left->contains(k);  
        }
        if (k>key && right){
            return right ->contains(k); 
        }
        return false; //this means no key is here 
        
    }
protected: 
    K key;
    V value;
    BinaryTreeNode<K, V> *left;
    BinaryTreeNode<K, V> *right;

    int height; 

    int getHeight(BinaryTreeNode*node){
        if (node == nullptr){ //using lecture 24 slides. if node is null then it just returns 0 otherwise node->height 
            return 0; 
        } else{
            return node->height; 
        }
    }
    
    void updateHeight(){
        height = 1 + std::max(getHeight(left),getHeight(right)); //we need to find the max of getHeight(left), getHeight(right)
    } 

    int balance(){ //return the balance by getting difference of right and left 
        return getHeight(right)- getHeight(left); 
    }

    
    BinaryTreeNode<K,V>*rotateleft(){
        BinaryTreeNode<K,V>*top = right; //we have to make sure to rotate left that we first grab the top  
        right = top ->left; 
        top -> left = this; //once right = top->left then "this" is updated to top->left 
        this -> updateHeight(); 
        top->updateHeight(); 

        return top; 
    }

    BinaryTreeNode<K,V>*rotateright(){
        BinaryTreeNode<K,V>*top = left; //create node*top such that its named left 
        left = top ->right; //set left equal to top->right 
        top ->right = this; //and set top->right equal to this 

        this -> updateHeight(); //ask this to continue updating the height and do same for top 
        top ->updateHeight(); 

        return top; //return top to rotate right 
    }

    BinaryTreeNode<K,V>*rebalance(){
        updateHeight(); //call on updateheight void 

        int node_bal = balance(); //grab the balance again 

        if (node_bal == 2){ //using lecture 24 pseudocode i found if the node balance is equal to 2 (right heavy) then 
            if (right && right->balance()< 0 ){
                right = right ->rotateright(); //we have to update right so right=right->rotateright()
            }
            return rotateleft(); //tehn we rotateleft because the right is much heavier 
        }
        if (node_bal == -2){ //if the left side is heavy hence more negative then we rotate right 
            if (left && left->balance()> 0){ //but if the balance is greater than 0
                left = left ->rotateleft(); //rotate left 
            }
            return rotateright(); //since we're left heavy we will rotate right 
        }

        return this; 
    }




}; 