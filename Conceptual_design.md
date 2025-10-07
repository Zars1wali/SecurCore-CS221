# SecurCore - Conceptual Design Document
## CS221 Data Structures and Algorithms - Deliverable 1

### Team Members
- [Student 1 Name] - [Roll Number 1]
- [Student 2 Name] - [Roll Number 2] 
- [Student 3 Name] - [Roll Number 3]

**Submission Date**: October 7, 2024  
**Course**: CS221 - Data Structures and Algorithms

---

## 1. System Architecture Overview

### 1.1 Core Data Structure Integration

```
Data Flow Architecture:
User Input → SecurityUtils.h → Credential.h → LinkedList.h → Stack.h/Queue.h → main.cpp
```

**Code Integration in `main.cpp`:**
```cpp
#include "Credential.h"
#include "LinkedList.h" 
#include "Stack.h"
#include "Queue.h"
#include "SecurityUtils.h"
// All core data structures integrated in main program
```

### 1.2 System Components

| Component | File | Purpose |
|-----------|------|---------|
| **Data Storage** | `Credential.h` | Credential object definition |
| **Core Storage** | `LinkedList.h` | Primary data organization |
| **Operation History** | `Stack.h` | Undo/redo functionality |
| **Audit Trail** | `Queue.h` | Security event logging |
| **Security** | `SecurityUtils.h` | Password hashing & validation |
| **Integration** | `main.cpp` | System demonstration |

---

## 2. Data Storage Layer Design

### 2.1 Credential Object Structure

**Defined in `Credential.h`:**
```cpp
struct Credential {
    std::string website;       // Key for searching
    std::string username;      // User identification  
    std::string password_hash; // Secure storage (hashed)
    int strength_score;        // Analytics data
    std::time_t created_at;    // Timestamp for auditing
    
    // Pointer-based linked list implementation
    Credential* next;          // Forward traversal
    Credential* prev;          // Backward traversal
    
    // Constructor demonstrates object initialization
    Credential(const std::string& web, const std::string& user, 
               const std::string& pwd_hash, int strength);
    
    // Member function for data display
    void display() const;
};
```

**Educational Concepts Demonstrated:**
- ✅ **Structures and Pointers**: Complex data type with pointer members
- ✅ **Memory Management**: Dynamic allocation in linked lists  
- ✅ **Data Encapsulation**: Member functions for operations
- ✅ **Time Complexity**: O(1) object creation

### 2.2 Linked List Implementation

**Core Structure in `LinkedList.h`:**
```cpp
class LinkedList {
protected:
    Credential* head;    // Pointer to first element
    Credential* tail;    // Pointer to last element  
    int size;           // Size tracking for efficiency
    
public:
    LinkedList();       // Constructor
    virtual ~LinkedList(); // Destructor for memory cleanup
    
    // Basic operations with pointer manipulations
    virtual void append(Credential* cred);     // O(1) insertion
    virtual void prepend(Credential* cred);    // O(1) insertion  
    virtual bool remove(const std::string& website); // O(n) deletion
    virtual Credential* search(const std::string& website) const; // O(n) search
};
```

**Algorithm Complexity - Linear Search:**
```cpp
Credential* LinkedList::search(const std::string& website) const {
    Credential* current = head;
    while (current != nullptr) {          // n iterations worst case
        if (current->website == website) {
            return current;               // Found - return early
        }
        current = current->next;          // Pointer traversal
    }
    return nullptr;                       // Not found
}
```

**Time Complexity Analysis:**
- **Insertion (append/prepend)**: O(1) - Constant time
- **Deletion (remove)**: O(n) - Linear time (search + remove)
- **Search**: O(n) - Linear time traversal
- **Space**: O(n) - Linear space for n elements

---

## 3. Operational Layer Design

### 3.1 Stack - Operation History (LIFO)

**Implementation in `Stack.h`:**
```cpp
class Stack {
private:
    Operation* top;     // Single pointer for LIFO access
    int capacity;       // Size tracking
    
public:
    Stack();           // Constructor
    ~Stack();          // Destructor
    
    // Core stack operations
    void push(const std::string& operation, Credential* data); // O(1)
    Operation* pop();                                          // O(1)
    bool isEmpty() const;                                      // O(1)
};
```

**Stack Visualization:**
```
Operation Stack (LIFO - Last In, First Out):
┌─────────────────┐ ← top (next to pop)
│   DELETE_op3    │ 
├─────────────────┤
│   MODIFY_op2    │
├─────────────────┤
│   ADD_op1       │ ← bottom (first pushed)
└─────────────────┘
```

**Real-World Application - Undo Functionality:**
```cpp
void undoLastOperation(LinkedList& credentialList) {
    Operation* lastOp = pop();
    if (lastOp != nullptr) {
        // Reverse the operation
        if (lastOp->type == "ADD") {
            credentialList.remove(lastOp->data->website);
        }
        delete lastOp;
    }
}
```

### 3.2 Queue - Audit Trail System (FIFO)

**Implementation in `Queue.h`:**
```cpp
class Queue {
private:
    AuditEvent* front;  // Pointer to first element
    AuditEvent* rear;   // Pointer to last element
    int size;           // Size tracking
    
public:
    Queue();            // Constructor
    ~Queue();           // Destructor
    
    // Core queue operations  
    void enqueue(const std::string& type, const std::string& desc); // O(1)
    AuditEvent* dequeue();                                         // O(1)
    bool isEmpty() const;                                          // O(1)
};
```

**Queue Visualization:**
```
Audit Queue (FIFO - First In, First Out):
front → ┌─────────────────┐ → ┌─────────────────┐ → ┌─────────────────┐ ← rear
        │   Login_Event   │   │ Password_Change │   │  Security_Alert │
        └─────────────────┘   └─────────────────┘   └─────────────────┘
```

**Cybersecurity Application:**
```cpp
void logSecurityEvent(const std::string& event, const Credential* cred = nullptr) {
    std::string description = "Security: " + event;
    if (cred != nullptr) {
        description += " for " + cred->website;
    }
    enqueue("SECURITY", description); // O(1) operation
}
```

---

## 4. Security Layer Design

### 4.1 Password Hashing & Validation

**Security Algorithms in `SecurityUtils.h`:**
```cpp
class SecurityUtils {
public:
    // Hashing algorithm for password protection - O(n)
    static std::string hashPassword(const std::string& password) {
        unsigned int hash = 5381;
        for (char c : password) {                    // n iterations
            hash = ((hash << 5) + hash) + c;        // hash * 33 + c
        }
        return std::to_string(hash);
    }
    
    // Password strength analysis - O(n) complexity
    static int calculateStrength(const std::string& password) {
        int score = 0;
        if (hasMinimumLength(password)) score += 2;  // O(1)
        if (hasUpperCase(password)) score += 2;      // O(n)
        if (hasLowerCase(password)) score += 2;      // O(n)
        if (hasNumbers(password)) score += 2;        // O(n)
        if (hasSpecialChars(password)) score += 2;   // O(n)
        return std::min(score, 10);
    }
};
```

**Complexity Analysis:**
- **Hashing**: O(n) - Linear time based on password length
- **Strength Calculation**: O(n) - Multiple linear traversals
- **Validation Checks**: O(n) - Character-by-character analysis

---

## 5. Data Processing Pipeline

### 5.1 Complete Workflow Integration

**Demonstrated in `main.cpp`:**
```cpp
void demonstrateDataFlow() {
    // 1. INPUT: Create credential object
    Credential* newCred = new Credential(
        "google.com", "user123", 
        SecurityUtils::hashPassword("mypass123"), 
        SecurityUtils::calculateStrength("mypass123")
    );
    
    // 2. PROCESSING: Store in linked list - O(1)
    LinkedList credentialList;
    credentialList.append(newCred);
    
    // 3. OPERATIONS: Track in stack - O(1)
    Stack operationStack;
    operationStack.push("ADD", newCred);
    
    // 4. AUDITING: Log in queue - O(1)
    Queue auditQueue;
    auditQueue.enqueue("ADD", "New credential for google.com");
    
    // 5. OUTPUT: Display results
    newCred->display();
}
```

### 5.2 Memory Management Strategy

**Proper Resource Handling:**
```cpp
void demonstrateMemoryManagement() {
    LinkedList* list = new LinkedList(); // Dynamic allocation
    
    // Add multiple credentials
    for (int i = 0; i < 5; i++) {
        Credential* cred = new Credential(
            "site" + std::to_string(i) + ".com",
            "user" + std::to_string(i),
            SecurityUtils::hashPassword("pass" + std::to_string(i)),
            7
        );
        list->append(cred); // Transfer ownership to list
    }
    
    // Automatic cleanup through destructor
    delete list; // ~LinkedList() deletes all Credential objects
}
```

---

## 6. Algorithm Complexity Analysis

### 6.1 Time Complexity Summary

| Operation | Data Structure | Time Complexity | Space Complexity |
|-----------|---------------|-----------------|------------------|
| Insertion | Linked List | O(1) | O(1) per element |
| Search | Linked List | O(n) | O(1) |
| Deletion | Linked List | O(n) | O(1) |
| Push | Stack | O(1) | O(1) per operation |
| Pop | Stack | O(1) | O(1) |
| Enqueue | Queue | O(1) | O(1) per event |
| Dequeue | Queue | O(1) | O(1) |
| Hashing | SecurityUtils | O(n) | O(1) |
| Strength Check | SecurityUtils | O(n) | O(1) |

### 6.2 Space Complexity Analysis

- **Credential Object**: O(1) per credential
- **Linked List**: O(n) total storage
- **Stack**: O(m) for m operations
- **Queue**: O(k) for k audit events
- **Overall System**: O(n + m + k) linear space complexity

---

## 7. Educational Value & CS221 Integration

### 7.1 Course Concept Mapping

| CS221 Concept | Implementation | Code Reference |
|---------------|----------------|----------------|
| **Pointers & Memory** | Linked list nodes, stack/queue elements | All header files |
| **Structures** | Credential object definition | `Credential.h` |
| **Linked Lists** | Single/Double linked list | `LinkedList.h` |
| **Stacks** | LIFO operation history | `Stack.h` |
| **Queues** | FIFO audit trail | `Queue.h` |
| **Algorithm Analysis** | Time/Space complexity | Code comments |
| **Sorting Algorithms** | Multiple sort methods | `LinkedList.h` |
| **Searching Algorithms** | Linear search | `LinkedList::search()` |
| **Hashing** | Password hashing | `SecurityUtils.h` |

### 7.2 Real-World Cybersecurity Application

```cpp
class SecurityManager {
private:
    LinkedList credentials;    // O(n) storage
    Stack operationHistory;    // O(m) operations  
    Queue securityLog;         // O(k) audit events
    SecurityUtils utils;       // O(1) utilities
    
public:
    void addCredential(const std::string& site, const std::string& user, 
                      const std::string& password) {
        // Security processing - O(n)
        std::string hash = utils.hashPassword(password);
        int strength = utils.calculateStrength(password);
        
        // Data storage - O(1)
        Credential* cred = new Credential(site, user, hash, strength);
        credentials.append(cred);
        
        // Operation tracking - O(1)
        operationHistory.push("ADD", cred);
        
        // Security auditing - O(1)
        securityLog.enqueue("ADD", "New credential for " + site);
    }
};
```

---

## 8. Implementation Roadmap

### Phase 1: Core Foundation (Deliverable 1 - Current)
- ✅ Basic data structure definitions
- ✅ Object-oriented design patterns
- ✅ Memory management implementation
- ✅ Algorithm complexity analysis
- ✅ Cybersecurity context integration

### Phase 2: Advanced Features (Deliverable 2)
```cpp
// Planned enhancements for Deliverable 2
class AdvancedFeatures {
    void implementSortingAlgorithms() {
        // Bubble sort, insertion sort, selection sort
        // Performance comparison and visualization
    }
    
    void addSearchOptimizations() {
        // Binary search trees for O(log n) operations
        // Hash tables for O(1) lookups
    }
};
```

### Phase 3: Optimization & Analytics (Deliverable 3)
- Graph algorithms for password relationship analysis
- Advanced sorting (merge sort, quick sort, heap sort)
- Performance benchmarking and visualization
- Enhanced security analytics dashboard

### Phase 4: Final Integration (Deliverable 4)
- Complete system integration
- User interface development
- Comprehensive testing and documentation
- Performance optimization

---

## 9. Conclusion

This conceptual design demonstrates a comprehensive approach to applying data structures and algorithms in a practical cybersecurity context. The SecurCore system provides:

1. **Educational Foundation**: Complete coverage of CS221 concepts
2. **Practical Application**: Real-world password management system
3. **Performance Awareness**: Clear complexity analysis throughout
4. **Scalable Architecture**: Foundation for future enhancements
5. **Security Focus**: Cybersecurity context for academic concepts

The implementation follows best practices in memory management, algorithm design, and software architecture while maintaining educational clarity and practical utility.

---
*CS221 Data Structures and Algorithms - Deliverable 1*  
*Submitted: October 7, 2024*
