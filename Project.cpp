
#include <iostream>
using namespace std;

// ==================== DATA STRUCTURES ====================

// Credential structure
struct Credential {
    string website;
    string username;
    string password_hash;
    Credential* next;
    Credential* prev;
};

// BST Node structure
struct BSTNode {
    string website;
    Credential* credential_ptr; // Points to credential in linked list
    BSTNode* left;
    BSTNode* right;
};

// Binary Search Tree manager
struct BinarySearchTree {
    BSTNode* root;
    int size;
};

// Stack node for undo operations
struct StackNode {
    string operation_type;
    Credential* credential_data;
    StackNode* next;
};

// Queue node for audit trail
struct QueueNode {
    string event_type;
    string description;
    QueueNode* next;
};

// Linked List manager
struct LinkedList {
    Credential* head;
    Credential* tail;
    int size;
};

// Stack manager
struct OperationStack {
    StackNode* top;
    int size;
};

// Queue manager
struct AuditQueue {
    QueueNode* front;
    QueueNode* rear;
    int size;
};

// ==================== UTILITY FUNCTIONS ====================

// Simple hash function for passwords
string hashPassword(const string& password) {
    unsigned int hash = 5381;
    
    for (int i = 0; i < password.length(); i++) {
        hash = ((hash << 5) + hash) + password[i]; // hash * 33 + char
    }
    
    // Convert to string
    string result = "";
    unsigned int n = hash;
    
    while (n > 0) {
        char digit = '0' + (n % 10);
        result = digit + result;
        n = n / 10;
    }
    
    if (result == "") result = "0";
    return result;
}

// ==================== CREDENTIAL FUNCTIONS ====================

// Create a new credential
Credential* createCredential(const string& website, 
                           const string& username, 
                           const string& password_hash) {
    Credential* new_cred = new Credential;
    
    new_cred->website = website;
    new_cred->username = username;
    new_cred->password_hash = password_hash;
    new_cred->next = 0;
    new_cred->prev = 0;
    
    return new_cred;
}

// Display credential details
void displayCredential(const Credential* cred) {
    if (cred == 0) {
        cout << "Credential is null!" << endl;
        return;
    }
    
    cout << "\n=== Credential Details ===" << endl;
    cout << "Website: " << cred->website << endl;
    cout << "Username: " << cred->username << endl;
    cout << "Password Hash: " << cred->password_hash << endl;
}

// Free credential memory
void freeCredential(Credential* cred) {
    if (cred != 0) {
        delete cred;
    }
}

// ==================== BINARY SEARCH TREE FUNCTIONS ====================

// Create a new BST node
BSTNode* createBSTNode(const string& website, Credential* cred_ptr) {
    BSTNode* new_node = new BSTNode;
    new_node->website = website;
    new_node->credential_ptr = cred_ptr;
    new_node->left = 0;
    new_node->right = 0;
    return new_node;
}

// Initialize BST
void initBST(BinarySearchTree* bst) {
    bst->root = 0;
    bst->size = 0;
}

// Insert node into BST
BSTNode* insertBSTNode(BSTNode* root, const string& website, Credential* cred_ptr) {
    if (root == 0) {
        return createBSTNode(website, cred_ptr);
    }
    
    if (website < root->website) {
        root->left = insertBSTNode(root->left, website, cred_ptr);
    } else if (website > root->website) {
        root->right = insertBSTNode(root->right, website, cred_ptr);
    }
    
    return root;
}

// Insert into BST (wrapper function)
void insertBST(BinarySearchTree* bst, const string& website, Credential* cred_ptr) {
    bst->root = insertBSTNode(bst->root, website, cred_ptr);
    bst->size++;
}

// Search in BST
BSTNode* searchBSTNode(BSTNode* root, const string& website) {
    if (root == 0 || root->website == website) {
        return root;
    }
    
    if (website < root->website) {
        return searchBSTNode(root->left, website);
    }
    
    return searchBSTNode(root->right, website);
}

// Search in BST (wrapper function)
BSTNode* searchBST(BinarySearchTree* bst, const string& website) {
    return searchBSTNode(bst->root, website);
}

// Find minimum value node in BST
BSTNode* findMinBSTNode(BSTNode* node) {
    BSTNode* current = node;
    while (current && current->left != 0) {
        current = current->left;
    }
    return current;
}

// Delete node from BST
BSTNode* deleteBSTNode(BSTNode* root, const string& website) {
    if (root == 0) return root;
    
    if (website < root->website) {
        root->left = deleteBSTNode(root->left, website);
    } else if (website > root->website) {
        root->right = deleteBSTNode(root->right, website);
    } else {
        // Node found
        if (root->left == 0) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == 0) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        
        // Node with two children
        BSTNode* temp = findMinBSTNode(root->right);
        root->website = temp->website;
        root->credential_ptr = temp->credential_ptr;
        root->right = deleteBSTNode(root->right, temp->website);
    }
    return root;
}

// Delete from BST (wrapper function)
bool deleteBST(BinarySearchTree* bst, const string& website) {
    int initial_size = bst->size;
    bst->root = deleteBSTNode(bst->root, website);
    if (bst->size == initial_size) {
        bst->size--; // Decrement only if node was found and deleted
        return true;
    }
    return false;
}

// In-order traversal of BST
void inorderTraversal(BSTNode* root) {
    if (root != 0) {
        inorderTraversal(root->left);
        cout << "  • " << root->website << endl;
        inorderTraversal(root->right);
    }
}

// Display BST structure
void displayBST(BinarySearchTree* bst) {
    cout << "\n=== BINARY SEARCH TREE STRUCTURE (" << bst->size << " items) ===" << endl;
    cout << "Websites in alphabetical order:" << endl;
    
    if (bst->root == 0) {
        cout << "BST is empty." << endl;
        return;
    }
    
    inorderTraversal(bst->root);
}

// Free BST memory
void freeBSTNode(BSTNode* node) {
    if (node == 0) return;
    
    freeBSTNode(node->left);
    freeBSTNode(node->right);
    delete node;
}

void freeBST(BinarySearchTree* bst) {
    freeBSTNode(bst->root);
    bst->root = 0;
    bst->size = 0;
}

// ==================== LINKED LIST FUNCTIONS ====================

// Initialize linked list
void initLinkedList(LinkedList* list) {
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

// Add credential to end of list
void addEnd(LinkedList* list, Credential* cred) {
    if (cred == 0) return;
    
    cred->next = 0;
    cred->prev = list->tail;
    
    if (list->head == 0) {
        // First element
        list->head = cred;
        list->tail = cred;
    } else {
        // Add to end
        list->tail->next = cred;
        list->tail = cred;
    }
    list->size++;
}

// Add credential to beginning of list
void addBeginning(LinkedList* list, Credential* cred) {
    if (cred == 0) return;
    
    cred->prev = 0;
    cred->next = list->head;
    
    if (list->head == 0) {
        // First element
        list->head = cred;
        list->tail = cred;
    } else {
        // Add to beginning
        list->head->prev = cred;
        list->head = cred;
    }
    list->size++;
}

// Search for credential by website (linear search - kept for backward compatibility)
Credential* searchCredential(LinkedList* list, const string& website) {
    Credential* current = list->head;
    
    while (current != 0) {
        if (current->website == website) {
            return current;
        }
        current = current->next;
    }
    
    return 0;
}

// Delete credential by website
int deleteCredential(LinkedList* list, const string& website) {
    Credential* current = list->head;
    
    while (current != 0) {
        if (current->website == website) {
            // Update links
            if (current->prev != 0) {
                current->prev->next = current->next;
            } else {
                // Deleting head
                list->head = current->next;
            }
            
            if (current->next != 0) {
                current->next->prev = current->prev;
            } else {
                // Deleting tail
                list->tail = current->prev;
            }
            
            list->size--;
            return 1; // Success
        }
        current = current->next;
    }
    
    return 0; // Not found
}

// Display all credentials
void displayAllCredentials(LinkedList* list) {
    Credential* current = list->head;
    int count = 1;
    
    cout << "\n=== STORED CREDENTIALS (" << list->size << " items) ===" << endl;
    
    if (current == 0) {
        cout << "No credentials stored." << endl;
        return;
    }
    
    while (current != 0) {
        cout << "\n" << count++ << ". Website: " << current->website << endl;
        cout << "   Username: " << current->username << endl;
        cout << "   Password Hash: " << current->password_hash << endl;
        
        current = current->next;
    }
}

// Get list size
int getListSize(LinkedList* list) {
    return list->size;
}

// Free all memory used by linked list
void freeLinkedList(LinkedList* list) {
    Credential* current = list->head;
    Credential* next;
    
    while (current != 0) {
        next = current->next;
        delete current;
        current = next;
    }
    
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

// ==================== STACK FUNCTIONS ====================

// Initialize stack
void initStack(OperationStack* stack) {
    stack->top = 0;
    stack->size = 0;
}

// Push operation to stack
void pushOperation(OperationStack* stack, const string& operation_type, Credential* cred) {
    StackNode* new_node = new StackNode;
    
    new_node->operation_type = operation_type;
    new_node->credential_data = cred;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;
}

// Pop operation from stack
StackNode* popOperation(OperationStack* stack) {
    if (stack->top == 0) {
        return 0;
    }
    
    StackNode* popped = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    popped->next = 0;
    
    return popped;
}

// Check if stack is empty
int isStackEmpty(OperationStack* stack) {
    return (stack->top == 0);
}

// Display stack contents
void displayStack(OperationStack* stack) {
    StackNode* current = stack->top;
    int count = 1;
    
    cout << "\n=== OPERATION HISTORY (LIFO - Last " << stack->size << " operations) ===" << endl;
    
    if (current == 0) {
        cout << "Stack is empty." << endl;
        return;
    }
    
    while (current != 0) {
        cout << count++ << ". " << current->operation_type;
        if (current->credential_data != 0) {
            cout << " - " << current->credential_data->website;
        }
        cout << endl;
        current = current->next;
    }
}

// Free stack memory
void freeStack(OperationStack* stack) {
    StackNode* current = stack->top;
    StackNode* next;
    
    while (current != 0) {
        next = current->next;
        delete current;
        current = next;
    }
    
    stack->top = 0;
    stack->size = 0;
}

// ==================== QUEUE FUNCTIONS ====================

// Initialize queue
void initQueue(AuditQueue* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

// Enqueue audit event
void enqueueEvent(AuditQueue* queue, const string& event_type, const string& description) {
    QueueNode* new_node = new QueueNode;
    
    new_node->event_type = event_type;
    new_node->description = description;
    new_node->next = 0;
    
    if (queue->rear == 0) {
        // First element
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        // Add to end
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    
    queue->size++;
}

// Dequeue audit event
QueueNode* dequeueEvent(AuditQueue* queue) {
    if (queue->front == 0) {
        return 0;
    }
    
    QueueNode* dequeued = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == 0) {
        queue->rear = 0;
    }
    
    queue->size--;
    dequeued->next = 0;
    
    return dequeued;
}

// Check if queue is empty
int isQueueEmpty(AuditQueue* queue) {
    return (queue->front == 0);
}

// Display queue contents
void displayQueue(AuditQueue* queue) {
    QueueNode* current = queue->front;
    int count = 1;
    
    cout << "\n=== AUDIT TRAIL (FIFO - " << queue->size << " events) ===" << endl;
    
    if (current == 0) {
        cout << "Queue is empty." << endl;
        return;
    }
    
    while (current != 0) {
        cout << count++ << ". " << current->event_type 
             << ": " << current->description << endl;
        current = current->next;
    }
}

// Free queue memory
void freeQueue(AuditQueue* queue) {
    QueueNode* current = queue->front;
    QueueNode* next;
    
    while (current != 0) {
        next = current->next;
        delete current;
        current = next;
    }
    
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

// ==================== MENU & MAIN PROGRAM ====================

// Function to show menu
void showMenu() {
    cout << "\n-------------------------------------------" << endl;
    cout << "          SECURCORE PASSWORD MANAGER       " << endl;
    cout << "-------------------------------------------" << endl;
    cout << "1. Add New Credential" << endl;
    cout << "2. Search Credential (BST Fast Search)" << endl;
    cout << "3. Delete Credential" << endl;
    cout << "4. Display All Credentials (Linked List)" << endl;
    cout << "5. Display BST Structure (Alphabetical)" << endl;
    cout << "6. View Operation History (Stack)" << endl;
    cout << "7. View Audit Trail (Queue)" << endl;
    cout << "8. Clear All Data" << endl;
    cout << "0. Exit" << endl;
    cout << "-------------------------------------------" << endl;
}

// Main interactive system
int main() {
    cout << "-------------------------------------------" << endl;
    cout << "     SECURCORE - PASSWORD MANAGER          " << endl;
    cout << "     CS221 Data Structures Project         " << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Data Structures Used:" << endl;
    cout << "  • Linked Lists (addBeginning/addEnd)" << endl;
    cout << "  • Binary Search Trees (Fast Search O(log n))" << endl;
    cout << "  • Stacks (LIFO for undo operations)" << endl;
    cout << "  • Queues (FIFO for audit trail)" << endl;
    cout << "-------------------------------------------" << endl;
    
    // Initialize data structures
    LinkedList cred_list;
    BinarySearchTree cred_tree;
    OperationStack op_stack;
    AuditQueue audit_queue;
    
    initLinkedList(&cred_list);
    initBST(&cred_tree);
    initStack(&op_stack);
    initQueue(&audit_queue);
    
    int choice;
    
    do {
        showMenu();
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline
        
        switch (choice) {
            case 1: { // Add credential
                int addChoice;
                cout << "\nWhere to add the credential?" << endl;
                cout << "1. Add at End (normal)" << endl;
                cout << "2. Add at Beginning" << endl;
                cout << "Choice: ";
                cin >> addChoice;
                cin.ignore();
                
                string website, username, password, hash;
                
                cout << "\nWebsite: ";
                getline(cin, website);
                
                cout << "Username: ";
                getline(cin, username);
                
                cout << "Password: ";
                getline(cin, password);
                
                // Hash password
                hash = hashPassword(password);
                
                // Create credential
                Credential* new_cred = createCredential(website, username, hash);
                if (new_cred) {
                    if (addChoice == 1) {
                        // Add at end of linked list
                        addEnd(&cred_list, new_cred);
                        // Insert into BST for fast search
                        insertBST(&cred_tree, website, new_cred);
                        
                        pushOperation(&op_stack, "ADD at End", new_cred);
                        string audit_desc = "Added credential at end for " + website;
                        enqueueEvent(&audit_queue, "ADD_END", audit_desc);
                        cout << "\n Credential added at END successfully!" << endl;
                    } else if (addChoice == 2) {
                        // Add at beginning of linked list
                        addBeginning(&cred_list, new_cred);
                        // Insert into BST for fast search
                        insertBST(&cred_tree, website, new_cred);
                        
                        pushOperation(&op_stack, "ADD at Beginning", new_cred);
                        string audit_desc = "Added credential at beginning for " + website;
                        enqueueEvent(&audit_queue, "ADD_BEGIN", audit_desc);
                        cout << "\n Credential added at BEGINNING successfully!" << endl;
                    } else {
                        cout << "\n Invalid choice. Added at end by default." << endl;
                        addEnd(&cred_list, new_cred);  // Default to end
                        insertBST(&cred_tree, website, new_cred);
                    }
                }
                break;
            }
                
            case 2: { // Search using BST (Fast Search)
                string website;
                
                cout << "\nEnter website to search: ";
                getline(cin, website);
                
                // Search using BST (O(log n) time)
                BSTNode* foundNode = searchBST(&cred_tree, website);
                
                if (foundNode && foundNode->credential_ptr) {
                    cout << "\n Credential Found (using BST):" << endl;
                    displayCredential(foundNode->credential_ptr);
                } else {
                    cout << "\n Credential not found." << endl;
                }
                break;
            }
                
            case 3: { // Delete
                string website;
                
                cout << "\nEnter website to delete: ";
                getline(cin, website);
                
                // Search using BST first
                BSTNode* foundNode = searchBST(&cred_tree, website);
                
                if (foundNode && foundNode->credential_ptr) {
                    displayCredential(foundNode->credential_ptr);
                    
                    cout << "\nAre you sure you want to delete this credential? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore();
                    
                    if (confirm == 'y' || confirm == 'Y') {
                        // Delete from linked list
                        if (deleteCredential(&cred_list, website)) {
                            // Delete from BST
                            deleteBST(&cred_tree, website);
                            
                            pushOperation(&op_stack, "DELETE Credential", foundNode->credential_ptr);
                            
                            string audit_desc = "Deleted credential for " + website;
                            enqueueEvent(&audit_queue, "DELETE", audit_desc);
                            
                            cout << "\n Credential deleted successfully from both structures." << endl;
                            freeCredential(foundNode->credential_ptr);
                        }
                    } else {
                        cout << "\nDeletion cancelled." << endl;
                    }
                } else {
                    cout << "\n✗ Credential not found." << endl;
                }
                break;
            }
                
            case 4: // Display all credentials from linked list
                displayAllCredentials(&cred_list);
                break;
                
            case 5: // Display BST structure
                displayBST(&cred_tree);
                break;
                
            case 6: // Operation history
                displayStack(&op_stack);
                break;
                
            case 7: // Audit trail
                displayQueue(&audit_queue);
                break;
                
            case 8: { // Clear all data
                cout << "\nAre you sure you want to clear ALL data? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();
                
                if (confirm == 'y' || confirm == 'Y') {
                    freeLinkedList(&cred_list);
                    freeBST(&cred_tree);
                    freeStack(&op_stack);
                    freeQueue(&audit_queue);
                    
                    initLinkedList(&cred_list);
                    initBST(&cred_tree);
                    initStack(&op_stack);
                    initQueue(&audit_queue);
                    
                    cout << "\n All data cleared from all structures." << endl;
                } else {
                    cout << "\nClear operation cancelled." << endl;
                }
                break;
            }
                
            case 0: // Exit
                cout << "\nExiting SecurCore..." << endl;
                break;
                
            default:
                cout << "\n Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
        
    } while (choice != 0);
    
    // Cleanup before exit
    freeLinkedList(&cred_list);
    freeBST(&cred_tree);
    freeStack(&op_stack);
    freeQueue(&audit_queue);
    
    cout << "\nThank you for using SecurCore!" << endl;
    
    return 0;
}