#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstring>

// Constants
constexpr size_t MEMORY_SIZE = 1024 * 1024; // 1MB virtual heap
constexpr size_t MIN_BLOCK_SIZE = 16;       // Minimum block size (bytes)
constexpr size_t HEADER_SIZE = sizeof(size_t) * 2; // Size for block header

// Allocation strategies
enum class AllocationStrategy {
    FIRST_FIT,
    BEST_FIT
};

// Memory block structure
struct MemoryBlock {
    size_t size;         // Size of the block in bytes (excluding header)
    bool allocated;      // Whether the block is allocated or free
    MemoryBlock* next;   // Pointer to next block in the linked list
    MemoryBlock* prev;   // Pointer to previous block in the linked list

    // Get pointer to the data area of this block
    void* GetData() {
        return reinterpret_cast<void*>(reinterpret_cast<char*>(this) + HEADER_SIZE);
    }

    // Get pointer to the next block based on address arithmetic
    MemoryBlock* GetPhysicalNext() {
        if (size == 0) return nullptr; // End of memory
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(GetData()) + size);
    }
};

// Memory Allocator class
class MemoryAllocator {
private:
    char memory[MEMORY_SIZE];    // The virtual heap
    MemoryBlock* firstBlock;     // Start of the memory blocks linked list
    AllocationStrategy strategy; // Current allocation strategy
    
    // Statistics
    size_t totalAllocated;
    size_t totalFree;
    size_t allocatedBlocks;
    size_t freeBlocks;
    size_t largestFreeBlock;
    double fragmentation;

public:
    // Constructor
    MemoryAllocator(AllocationStrategy strat = AllocationStrategy::FIRST_FIT)
        : strategy(strat), totalAllocated(0), totalFree(MEMORY_SIZE - HEADER_SIZE), 
          allocatedBlocks(0), freeBlocks(1), largestFreeBlock(MEMORY_SIZE - HEADER_SIZE),
          fragmentation(0.0) {
        
        // Initialize the first block (entire memory is free)
        firstBlock = reinterpret_cast<MemoryBlock*>(memory);
        firstBlock->size = MEMORY_SIZE - HEADER_SIZE;
        firstBlock->allocated = false;
        firstBlock->next = nullptr;
        firstBlock->prev = nullptr;
    }

    // Set allocation strategy
    void SetStrategy(AllocationStrategy strat) {
        strategy = strat;
    }

    // Allocate memory block
    void* Allocate(size_t size) {
        if (size == 0) return nullptr;

        // Round up size to minimum block size if needed
        if (size < MIN_BLOCK_SIZE) {
            size = MIN_BLOCK_SIZE;
        }

        // Find a suitable block using the selected strategy
        MemoryBlock* block = nullptr;
        
        if (strategy == AllocationStrategy::FIRST_FIT) {
            block = FindFirstFit(size);
        } else { // BEST_FIT
            block = FindBestFit(size);
        }

        if (!block) {
            std::cout << "ERROR: Memory allocation failed. Not enough free memory.\n";
            return nullptr;
        }

        // Split the block if needed
        SplitBlock(block, size);
        
        // Mark block as allocated
        block->allocated = true;
        
        // Update statistics
        totalAllocated += block->size;
        totalFree -= block->size;
        allocatedBlocks++;
        freeBlocks--;
        UpdateStats();

        return block->GetData();
    }

    // Deallocate memory block
    bool Deallocate(void* ptr) {
        if (!ptr) return false;

        // Calculate the block address from the data pointer
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(
            reinterpret_cast<char*>(ptr) - HEADER_SIZE
        );

        // Validate the block
        if (!IsValidBlock(block) || !block->allocated) {
            std::cout << "ERROR: Invalid deallocation request.\n";
            return false;
        }

        // Mark block as free
        block->allocated = false;
        
        // Update statistics
        totalAllocated -= block->size;
        totalFree += block->size;
        allocatedBlocks--;
        freeBlocks++;

        // Attempt to coalesce with adjacent blocks
        CoalesceBlocks(block);
        UpdateStats();

        return true;
    }

    // Print memory usage report
    void PrintMemoryReport() const {
        std::cout << "\n===== MEMORY ALLOCATOR REPORT =====\n";
        std::cout << "Total Memory: " << MEMORY_SIZE << " bytes\n";
        std::cout << "Allocation Strategy: " << (strategy == AllocationStrategy::FIRST_FIT ? "First Fit" : "Best Fit") << "\n";
        std::cout << "Total Allocated: " << totalAllocated << " bytes (" 
                  << std::fixed << std::setprecision(2) << (totalAllocated * 100.0 / MEMORY_SIZE) << "%)\n";
        std::cout << "Total Free: " << totalFree << " bytes (" 
                  << std::fixed << std::setprecision(2) << (totalFree * 100.0 / MEMORY_SIZE) << "%)\n";
        std::cout << "Allocated Blocks: " << allocatedBlocks << "\n";
        std::cout << "Free Blocks: " << freeBlocks << "\n";
        std::cout << "Largest Free Block: " << largestFreeBlock << " bytes\n";
        std::cout << "Memory Fragmentation: " << std::fixed << std::setprecision(2) 
                  << (fragmentation * 100.0) << "%\n";
        std::cout << "==================================\n\n";
    }

    // Print memory map (visual representation of blocks)
    void PrintMemoryMap() const {
        std::cout << "\n===== MEMORY MAP =====\n";
        std::cout << "Each symbol represents " << (MEMORY_SIZE / 100) << " bytes\n";
        std::cout << "[A] = Allocated, [F] = Free\n";
        
        MemoryBlock* current = firstBlock;
        int symbolCount = 0;
        int lineCount = 0;
        
        while (current) {
            size_t blockSymbols = current->size / (MEMORY_SIZE / 100);
            if (blockSymbols == 0) blockSymbols = 1;
            
            for (size_t i = 0; i < blockSymbols; i++) {
                std::cout << (current->allocated ? "A" : "F");
                symbolCount++;
                
                if (symbolCount % 50 == 0) {
                    std::cout << " " << (lineCount * 50 + 1) << "-" << (lineCount + 1) * 50 << "\n";
                    lineCount++;
                }
            }
            
            current = current->next;
        }
        
        if (symbolCount % 50 != 0) {
            std::cout << " " << ((lineCount * 50) + 1) << "-" << symbolCount << "\n";
        }
        std::cout << "====================\n\n";
    }

    // Print detailed block information
    void PrintBlockDetails() const {
        std::cout << "\n===== BLOCK DETAILS =====\n";
        std::cout << std::left << std::setw(20) << "Block Address" 
                  << std::setw(15) << "Size (bytes)" 
                  << std::setw(12) << "Status" 
                  << "Data Address\n";
        std::cout << std::string(60, '-') << "\n";
        
        MemoryBlock* current = firstBlock;
        while (current) {
            std::cout << std::left << std::setw(20) << current 
                      << std::setw(15) << current->size 
                      << std::setw(12) << (current->allocated ? "Allocated" : "Free") 
                      << current->GetData() << "\n";
            current = current->next;
        }
        std::cout << "=======================\n\n";
    }

private:
    // Find the first block that can fit the requested size
    MemoryBlock* FindFirstFit(size_t size) {
        MemoryBlock* current = firstBlock;
        while (current) {
            if (!current->allocated && current->size >= size) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Find the best fitting block for the requested size
    MemoryBlock* FindBestFit(size_t size) {
        MemoryBlock* bestBlock = nullptr;
        size_t bestSize = MEMORY_SIZE + 1; // Initialize with a value larger than possible
        
        MemoryBlock* current = firstBlock;
        while (current) {
            if (!current->allocated && current->size >= size) {
                if (current->size < bestSize) {
                    bestSize = current->size;
                    bestBlock = current;
                }
            }
            current = current->next;
        }
        
        return bestBlock;
    }

    // Split a block if it's larger than needed (plus minimum block size)
    void SplitBlock(MemoryBlock* block, size_t size) {
        if (!block) return;
        
        // Only split if the remainder would be large enough for another block
        size_t remainingSize = block->size - size;
        if (remainingSize < MIN_BLOCK_SIZE + HEADER_SIZE) {
            return; // Don't split if remainder is too small
        }
        
        // Create a new block at the end of the current block's data area
        char* blockEnd = reinterpret_cast<char*>(block->GetData()) + size;
        MemoryBlock* newBlock = reinterpret_cast<MemoryBlock*>(blockEnd);
        
        // Set up the new block
        newBlock->size = remainingSize - HEADER_SIZE;
        newBlock->allocated = false;
        newBlock->next = block->next;
        newBlock->prev = block;
        
        // Update the original block
        block->size = size;
        
        // Fix next block's prev pointer if it exists
        if (block->next) {
            block->next->prev = newBlock;
        }
        
        // Connect the original block to the new one
        block->next = newBlock;
        
        // Update statistics
        freeBlocks++;
    }

    // Combine adjacent free blocks to reduce fragmentation
    void CoalesceBlocks(MemoryBlock* block) {
        if (!block) return;
        
        // Try to merge with the next block (if it's free)
        if (block->next && !block->next->allocated) {
            // Calculate the combined size
            block->size += block->next->size + HEADER_SIZE;
            
            // Update the linked list
            MemoryBlock* nextNext = block->next->next;
            block->next = nextNext;
            
            if (nextNext) {
                nextNext->prev = block;
            }
            
            // Update statistics
            freeBlocks--;
        }
        
        // Try to merge with the previous block (if it's free)
        if (block->prev && !block->prev->allocated) {
            // Calculate the combined size
            block->prev->size += block->size + HEADER_SIZE;
            
            // Update the linked list
            block->prev->next = block->next;
            
            if (block->next) {
                block->next->prev = block->prev;
            }
            
            // Update statistics
            freeBlocks--;
        }
    }

    // Check if a block pointer is valid
    bool IsValidBlock(MemoryBlock* block) const {
        if (!block) return false;
        
        // Check if the block is within the memory bounds
        char* blockAddr = reinterpret_cast<char*>(block);
        if (blockAddr < memory || blockAddr >= memory + MEMORY_SIZE) {
            return false;
        }
        
        // Validate block by traversing the list
        MemoryBlock* current = firstBlock;
        while (current) {
            if (current == block) {
                return true;
            }
            current = current->next;
        }
        
        return false;
    }

    // Update memory statistics
    void UpdateStats() {
        // Find largest free block and calculate fragmentation
        largestFreeBlock = 0;
        size_t freeBytesSum = 0;
        
        MemoryBlock* current = firstBlock;
        while (current) {
            if (!current->allocated) {
                freeBytesSum += current->size;
                if (current->size > largestFreeBlock) {
                    largestFreeBlock = current->size;
                }
            }
            current = current->next;
        }
        
        // Calculate fragmentation as (1 - largest_free_block / total_free_memory)
        if (totalFree > 0) {
            fragmentation = 1.0 - (static_cast<double>(largestFreeBlock) / totalFree);
        } else {
            fragmentation = 0.0;
        }
    }
};

// Main function with user interaction
int main() {
    std::cout << "Memory Allocator Simulator\n";
    std::cout << "========================\n\n";
    
    // Initialize default strategy
    AllocationStrategy currentStrategy = AllocationStrategy::FIRST_FIT;
    
    // Create memory allocator
    MemoryAllocator allocator(currentStrategy);
    
    // Store allocated pointers
    std::vector<std::pair<void*, size_t>> allocatedBlocks;
    
    bool running = true;
    int choice;
    
    while (running) {
        // Display menu
        std::cout << "\n==== MENU ====\n";
        std::cout << "1. Allocate memory\n";
        std::cout << "2. Deallocate memory\n";
        std::cout << "3. Deallocate all memory\n";
        std::cout << "4. Print memory report\n";
        std::cout << "5. Print block details\n";
        std::cout << "6. Print memory map\n";
        std::cout << "7. Switch allocation strategy (Current: " 
                  << (currentStrategy == AllocationStrategy::FIRST_FIT ? "First Fit" : "Best Fit") << ")\n";
        std::cout << "8. Run automated demo\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter your choice: ";
        
        // Get user choice
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        // Process user choice
        switch (choice) {
            case 1: { // Allocate memory
                size_t size;
                std::cout << "Enter block size (bytes): ";
                if (!(std::cin >> size)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid size. Please enter a number.\n";
                    continue;
                }
                
                void* ptr = allocator.Allocate(size);
                if (ptr) {
                    allocatedBlocks.push_back({ptr, size});
                    std::cout << "Block #" << allocatedBlocks.size() << " allocated at address " << ptr 
                              << " with size " << size << " bytes\n";
                }
                break;
            }
            
            case 2: { // Deallocate memory
                if (allocatedBlocks.empty()) {
                    std::cout << "No blocks to deallocate.\n";
                    break;
                }
                
                std::cout << "Allocated blocks:\n";
                for (size_t i = 0; i < allocatedBlocks.size(); i++) {
                    if (allocatedBlocks[i].first != nullptr) {
                        std::cout << "#" << (i+1) << ": Address " << allocatedBlocks[i].first 
                                  << ", Size " << allocatedBlocks[i].second << " bytes\n";
                    }
                }
                
                int blockIndex;
                std::cout << "Enter block number to deallocate (0 to cancel): ";
                if (!(std::cin >> blockIndex) || blockIndex < 0 || blockIndex > static_cast<int>(allocatedBlocks.size())) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid block number.\n";
                    continue;
                }
                
                if (blockIndex == 0) {
                    std::cout << "Deallocation canceled.\n";
                    break;
                }
                
                // Deallocate the selected block
                if (allocatedBlocks[blockIndex-1].first != nullptr) {
                    if (allocator.Deallocate(allocatedBlocks[blockIndex-1].first)) {
                        std::cout << "Block #" << blockIndex << " deallocated successfully.\n";
                        allocatedBlocks[blockIndex-1].first = nullptr; // Mark as deallocated
                    }
                } else {
                    std::cout << "Block #" << blockIndex << " is already deallocated.\n";
                }
                break;
            }
            
            case 3: { // Deallocate all memory
                int deallocatedCount = 0;
                for (auto& block : allocatedBlocks) {
                    if (block.first != nullptr) {
                        if (allocator.Deallocate(block.first)) {
                            block.first = nullptr;
                            deallocatedCount++;
                        }
                    }
                }
                std::cout << deallocatedCount << " blocks deallocated successfully.\n";
                break;
            }
            
            case 4: // Print memory report
                allocator.PrintMemoryReport();
                break;
                
            case 5: // Print block details
                allocator.PrintBlockDetails();
                break;
                
            case 6: // Print memory map
                allocator.PrintMemoryMap();
                break;
                
            case 7: { // Switch allocation strategy
                if (currentStrategy == AllocationStrategy::FIRST_FIT) {
                    currentStrategy = AllocationStrategy::BEST_FIT;
                    allocator.SetStrategy(AllocationStrategy::BEST_FIT);
                    std::cout << "Switched to Best Fit allocation strategy.\n";
                } else {
                    currentStrategy = AllocationStrategy::FIRST_FIT;
                    allocator.SetStrategy(AllocationStrategy::FIRST_FIT);
                    std::cout << "Switched to First Fit allocation strategy.\n";
                }
                break;
            }
            
            case 8: { // Run automated demo
                std::cout << "\n=== Running Automated Demo ===\n";
                
                // Clear any existing allocations
                for (auto& block : allocatedBlocks) {
                    if (block.first != nullptr) {
                        allocator.Deallocate(block.first);
                        block.first = nullptr;
                    }
                }
                allocatedBlocks.clear();
                
                // First Fit demonstration
                std::cout << "Setting First Fit strategy...\n";
                allocator.SetStrategy(AllocationStrategy::FIRST_FIT);
                currentStrategy = AllocationStrategy::FIRST_FIT;
                
                std::cout << "Initial memory state:\n";
                allocator.PrintMemoryReport();
                
                // Demo allocations
                std::cout << "Allocating blocks of various sizes...\n";
                void* block1 = allocator.Allocate(1024);       // 1KB
                void* block2 = allocator.Allocate(10 * 1024);  // 10KB
                void* block3 = allocator.Allocate(100 * 1024); // 100KB
                void* block4 = allocator.Allocate(5 * 1024);   // 5KB
                
                // Store allocated blocks for cleanup
                if (block1) allocatedBlocks.push_back({block1, 1024});
                if (block2) allocatedBlocks.push_back({block2, 10 * 1024});
                if (block3) allocatedBlocks.push_back({block3, 100 * 1024});
                if (block4) allocatedBlocks.push_back({block4, 5 * 1024});
                
                // Print state
                allocator.PrintMemoryReport();
                allocator.PrintBlockDetails();
                allocator.PrintMemoryMap();
                
                // Create fragmentation
                std::cout << "Deallocating blocks to create fragmentation...\n";
                if (block2) allocator.Deallocate(block2);
                if (block4) allocator.Deallocate(block4);
                
                // Mark as deallocated in our tracking
                for (auto& block : allocatedBlocks) {
                    if (block.first == block2 || block.first == block4) {
                        block.first = nullptr;
                    }
                }
                
                // Print state
                allocator.PrintMemoryReport();
                allocator.PrintBlockDetails();
                allocator.PrintMemoryMap();
                
                // First Fit allocation in fragmented memory
                std::cout << "Allocating a block with First Fit...\n";
                void* block5 = allocator.Allocate(3 * 1024);
                if (block5) allocatedBlocks.push_back({block5, 3 * 1024});
                
                // Print state
                allocator.PrintMemoryReport();
                allocator.PrintBlockDetails();
                
                // Switch to Best Fit
                std::cout << "Switching to Best Fit allocation strategy...\n";
                allocator.SetStrategy(AllocationStrategy::BEST_FIT);
                currentStrategy = AllocationStrategy::BEST_FIT;
                
                // Best Fit allocation
                void* block6 = allocator.Allocate(1 * 1024);
                if (block6) allocatedBlocks.push_back({block6, 1 * 1024});
                
                // Print state
                allocator.PrintMemoryReport();
                allocator.PrintBlockDetails();
                allocator.PrintMemoryMap();
                
                // Demonstrate coalescing
                std::cout << "Deallocating all blocks to demonstrate coalescing...\n";
                for (auto& block : allocatedBlocks) {
                    if (block.first != nullptr) {
                        allocator.Deallocate(block.first);
                        block.first = nullptr;
                    }
                }
                allocatedBlocks.clear();
                
                // Print final state
                allocator.PrintMemoryReport();
                allocator.PrintBlockDetails();
                allocator.PrintMemoryMap();
                
                std::cout << "Demo completed successfully.\n";
                break;
            }
            
            case 9: // Exit
                running = false;
                std::cout << "Exiting memory allocator simulator.\n";
                break;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}