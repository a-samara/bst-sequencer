/*
  CSC A48 - Assignment 2 - BSTs, Traversals, a tiny Sequencer

  Starter code (c) 2019 - F. Estrada, M. Ahmadzadeh
*/

#include<stdio.h>
#include<stdlib.h>
#include"NoteSynth.c"

typedef struct BST_Node_Struct {

    double key;
    double freq;
    int bar;
    double index;
    struct BST_Node_Struct *left;
	struct BST_Node_Struct *right;
  
} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index)
{
    /* 
     * This function creates and initializes a new BST_Node 
     * for a note with the given position (bar:index) and
     * the specified frequency. The key value for the node
     * is computed here as
     * 
     * 		(10.0*bar)+index
     */
        
	BST_Node *new_node = NULL;
	new_node = (BST_Node *)calloc(1, sizeof(BST_Node));
	new_node->freq = freq;
	new_node->bar = bar;
	new_node->index = index;
	new_node->key = (10.0*bar)+index;
	new_node->left=NULL;
	new_node->right=NULL;
	
	return new_node;
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node)
{
    /*
     * This function inserts a new node into the BST. The
     * node must already have been initialized with valid
     * note data, and must have its unique key.
     * 
     * The insert function must check that no other node
     * exists in the BST with the same key. If a node with
     * the same key exists, it must print out a message
     * using the following format string
     * 
     * printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n",....);
     * (of course you need to provide the relevant variables to print)
     * 
     * And it must return without inserting anyting in the
     * BST.
     */
    
	
	BST_Node *h = NULL;
	h = root;
	
    if(root == NULL){
		return new_node;
	}
	
	while(h != NULL){
		if(h->key == new_node->key){
			printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n", h->bar, h->index);
			return root;
		}		
		if(h->key < new_node->key){
			h = h->left;
		}
		else{
			h = h-> right;
		}
	}
	
	if(new_node->key < root->key){
		root->left = BST_insert(root->left, new_node);
	}else{
		root->right = BST_insert(root->right, new_node);
	}
	return root;
}

BST_Node *BST_search(BST_Node *root, int bar, double index)
{
    /*
     * This function searches the BST for a note at the
     * speficied position. If found, it must return a
     * pointer to the node that contains it. 
     * Search has to happen according to the BST search
     * process - so you need to figure out what value to
     * use during the search process to decide which branch
     * of the tree to search next.
     */
	
	double key = (10.0*bar)+index;
	 
    if(root == NULL){
		return NULL;
	}
	
	if(root->bar == bar && root->index == index){
		return root;
	}
	
	if(key < root->key){
		return BST_search(root->left, bar, index);
	}else{
		return BST_search(root->right, bar, index);
	}
}

BST_Node *find_successor(BST_Node *right_child_node)
{
    /*
     * This function finds the successor of a node by 
     * searching the right subtree for the node that
     * is most to the left (that will be the node
     * with the smallest key in that subtree)
     */
	
	while(right_child_node->left != NULL){
		right_child_node = right_child_node->left;
	}
	return right_child_node;    
}

BST_Node *BST_delete(BST_Node *root, int bar, double index)
{
    /*
     * Deletes from the BST a note at the specified position.
     * You must implement the three cases of BST deletion
     * we discussed in class. Make sure the function can
     * remove a note at any position without breaking the
     * tree!
     */  
	
	BST_Node *tmp = NULL;
	double key = (10.0*bar)+index;
	
	if(root == NULL){ 
		return NULL;
	}
	
	if(root->bar == bar && root->index == index){
		if (root->left == NULL && root->right == NULL){
			free(root);
			return NULL;
		}else if(root->right == NULL){
			tmp = root->left;
			free(root);
			return tmp;
		}else if(root->left == NULL){
			tmp = root->right;
			free(root);
			return tmp;
		}else{
			tmp = find_successor(root->right);
			root->freq = tmp->freq;
			root->bar = tmp->bar;
			root->index = tmp->index;
			root->key = tmp->key;
			root->right = BST_delete(root->right, tmp->bar, tmp->index);
		}
	}
	
	if(key < root->key){
		root->left = BST_delete(root->left, bar, index);
	}else{
		root->right = BST_delete(root->right, bar, index);
	}
	return root;
}

void BST_makePlayList(BST_Node *root)
{
    /*
     * This function does an in-order traversal of the BST to
     * generate an ordered list of notes to be played. Each
     * note is added to a linked-list (already implemented,
     * you only need to call the insert function) and the 
     * play list is then playable using the code in NoteSynth.c
     * 
     * To insert a note, you need to call the function provided
     * in NoteSynth.c:
     * 
     * playlist_head=playlist_insert(freq,bar,index);
     * 
     * playlist_head is a GLOBAL variable declared in NoteSynth.c
     * precisely for this purpose. Don't worry about intializing
     * it. It's set to NULL.
     * 
     * playlist_insert() takes the frequency, bar, and index, and
     * adds the note to the the *end* of the list - so notes 
     * have to be added in order - hence the in-order traversal
     * this function has to do.
     */
    
	if(root != NULL){
		BST_makePlayList(root->left);
		playlist_head = playlist_insert(playlist_head, root->freq, root->bar, root->index);
		BST_makePlayList(root->right);
	}
}

void BST_inOrder(BST_Node *root, int depth)
{
    /*
     * This function performs an in-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     * 
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */
	
	if(root == NULL){
		return;
	}
	
    BST_inOrder(root->left, depth + 1);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
    BST_inOrder(root->right, depth + 1);
}

void BST_preOrder(BST_Node *root, int depth)
{
    /*
     * This function performs an pre-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     * 
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */
    
	if(root == NULL){
		return;
	}
	
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
	BST_preOrder(root->left, depth + 1);
    BST_preOrder(root->right, depth + 1);
}

void BST_postOrder(BST_Node *root,int depth)
{
    /*
     * This function performs an post-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     *
     * Obviously, you must provide the bar, index, and frequency
     * of the note that needs to be printed to complete the
     * statement - we're just giving you the formatting string.
     * 
     * The depth value is increased by 1 for each recursive call
     * so when you print, you can see at what level each node
     * is located! (this should help you debug your code by
     * making it easier to check the shape of your BST).
     */
    
	if(root == NULL){
		return;
	}

	BST_postOrder(root->left, depth + 1);
    BST_postOrder(root->right, depth + 1);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
    
} 

void delete_BST(BST_Node *root)
{
    /*
     * This function deletes the BST and frees all memory used for
     * nodes in it. Recall that there is a specific order in which
     * this needs to be done! (consult the Unit 4 notes as needed)
     */
    
	if (root == NULL){
		return;
	}
	
    delete_BST(root->left);
    delete_BST(root->right);
    free(root);
}

void BST_swapFreq(BST_Node *root, double src_freq, double dst_freq){
	
	if(root == NULL){
		return; 
	}
	
	if(fabs(root->freq - src_freq) < 0.00000001){
		root->freq = dst_freq;
	}
	BST_swapFreq(root->left, src_freq, dst_freq);
	BST_swapFreq(root->right, src_freq, dst_freq);
}


void BST_shiftFreq(BST_Node *root, char note_src[5], char note_dst[5])
{
    /*
     * This function goes over the entries in the BST, finds notes
     * corresponding to note_src (which is a text string describing
     * a note's name in the musical scale), and shifts those notes
     * to the frequency of note_dst (another text string with a note
     * name). Effectively, it replaces one particular type of note
     * by another all through the song.
	 *
     * Expected result: All notes in the BST whose frequency
     * corresponds to the frequency of note_src, should have
     * their frequency changed to that of note_dst.
     */
	
	double src_freq;
	double dst_freq;

	for(int i = 0; i < 100; i++){	
		for(int j = 0; j < 5; j++){
		
			if(note_names[i][j] != note_src[j]){
				break;
			}			
			else if(note_dst[j] == '\0'){
				src_freq = note_freq[i];
			}
		}
	}
	 
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 5; j++){
		
			if(note_names[i][j] != note_dst[j]){
				break;
			}			
			else if(note_dst[j] == '\0'){
				dst_freq = note_freq[i];
			}	
		}
	}
	 
	BST_swapFreq(root, src_freq, dst_freq);
}

BST_Node *copyBST(BST_Node *root){
	
    if(root == NULL)
        return NULL;

    BST_Node *newNode = newBST_Node(root->freq, root->bar, root->index);

    newNode->left = copyBST(root->left);
    newNode->right = copyBST(root->right);

    return newNode;
}

BST_Node *harmonize(BST_Node *root, int semitones, double time_shift, BST_Node *new_root){
	
	if(new_root == NULL){
		return NULL;
	}
	
	harmonize(root, semitones, time_shift, new_root->left);
    harmonize(root, semitones, time_shift, new_root->right);
	
	for(int i = 0; i < 100; i++){
		if(fabs(new_root->freq - note_freq[i]) < 0.00000001){
			if(i + semitones < 0 || i + semitones > 99){
				break;
			}
			if(new_root->index + time_shift < 0 || new_root->index + time_shift >= 1){
				break;
			}
			BST_insert(root, newBST_Node(note_freq[i+semitones], new_root->bar, new_root->index + time_shift));	
		}
	}
	return root;	
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift)
{
   
     
    /* This function traverses the BST, and for each existing
     * note, inserts a new, modified note (i.e. it will add sounds
     * to an already existing song, based on the notes it already has)
     * 
     * The new note has the followin properties:
     * - The frequency is shifted by the specified number of semitones
     *   (A semitone is the difference between one note and the
     *    immediately next one in the musical scale - ot what is the
     *    same, the difference in pitch between a white key and the
     *    black key immediately next to it in a piano)
     * - It plays in the same *bar* as the original note
     * - But its *index* is shifted by the specified time_shift
     *   (this value is between 0 and 1, but you have to check 
     *    that the final index remains between 0 and 1)
     * 
     * Both the 'semitones' and 'time_shift' parameter can be
     * positive or negative. A typical value for semitones
     * could be 4 or 7, corresponding to musical 3rds or
     * musical 5ths - this should produce some interesting
     * harmony! but you can play with this function and try
     * various things for fun.
     * 
     * NOTE: If the value of  j+semitones  is < 0 or > 99
     *       then there is no note with the frequency you
     *       want. In that case you don't insert a new
     *       note. 
     * 
     * You then add a new note with that frequency at
     * bar=10 (same bar!)
     * index=.25 + .1  (the original index plus the time_shift)
     * 
     * NOTE: If the resulting index is less than 0, or >= 1,
     *       then you DO NOT insert the new note.
     * 
     * Expected result: The BST will have about twice as many notes
     *   as before, and the new notes are shifted in pitch and 
     *   in time as specified by the parameters. 
     */
  
	BST_Node *new_root = NULL;
	new_root = copyBST(root);

	return harmonize(root, semitones, time_shift, new_root);	
}

