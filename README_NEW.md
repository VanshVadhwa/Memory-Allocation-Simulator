# Memory Allocator Simulator

An interactive educational tool that demonstrates how modern operating systems manage memory allocation. Learn about different allocation strategies, memory fragmentation, and block management through hands-on experimentation.

## 🎯 Purpose

This project is designed to help students and developers understand:
- **Memory Allocation Strategies** (First Fit vs Best Fit)
- **Memory Fragmentation** and its impact on system performance
- **Linked List Data Structures** for memory management
- **Dynamic Memory Management** principles
- **Resource Tracking** and statistics

## 🚀 Quick Start

### Option 1: Web Version (Recommended for Learning)

```bash
# Install dependencies
npm install

# Start the server
npm start

# Open http://localhost:3000 in your browser
```

### Option 2: C++ Command-Line Version

```bash
# Build
mkdir build && cd build
cmake ..
cmake --build .

# Run
./memory_allocator
```

## 📁 Project Structure

```
Memory Allocator/
├── os.cpp                 # C++ memory allocator implementation
├── CMakeLists.txt         # Build configuration
├── server.js              # Node.js/Express web server
├── package.json           # Node.js dependencies
├── public/
│   ├── index.html         # Interactive web interface
│   ├── styles.css         # Styling
│   └── script.js          # Frontend logic
├── Dockerfile             # Container configuration
└── README.md              # This file
```

## 🎓 Learning Concepts

### First Fit Algorithm
- **How it works**: Scans through free memory blocks and allocates the first one large enough
- **Advantages**: Simple and fast (O(n) average case)
- **Disadvantages**: Can waste space and cause fragmentation

### Best Fit Algorithm
- **How it works**: Finds the smallest free block that can fit the request
- **Advantages**: Minimizes wasted space per allocation
- **Disadvantages**: Slower (O(n)) and can create many small fragments

### Memory Fragmentation
- **Definition**: When free memory is split into small, unusable pieces
- **Impact**: Reduces available contiguous memory and system performance
- **Measurement**: Calculated as (1 - largest_free_block / total_free_memory)
- **Range**: 0% (perfect) to 100% (highly fragmented)

### Block Splitting & Coalescing
- **Splitting**: When a large free block is allocated, the remainder becomes a new free block
- **Coalescing**: Adjacent free blocks are merged to reduce fragmentation

## 💻 Features

### Interactive Web Interface
- ✅ Allocate memory blocks with custom sizes
- ✅ Deallocate blocks individually
- ✅ Switch between allocation strategies in real-time
- ✅ Visual memory map showing allocation status
- ✅ Real-time statistics and fragmentation tracking
- ✅ Automated demo to show key concepts
- ✅ Learning guide with detailed explanations

### Command-Line Interface
- ✅ Manual allocation/deallocation
- ✅ View detailed memory reports
- ✅ Visual memory maps in terminal
- ✅ Block detail information
- ✅ Automated demonstration scenarios

### Key Metrics
- Total memory (1 MB virtual heap)
- Allocated vs Free memory
- Number of allocated/free blocks
- Memory fragmentation percentage
- Largest contiguous free block
- Current allocation strategy

## 🔧 API Reference

### GET /api/stats
Returns current memory statistics

```json
{
  "totalMemory": 1048576,
  "totalAllocated": 102400,
  "totalFree": 946176,
  "allocatedBlocks": 1,
  "freeBlocks": 2,
  "largestFreeBlock": 946176,
  "fragmentation": 0.0,
  "strategy": "FIRST_FIT",
  "allocatedPercent": "9.77"
}
```

### POST /api/allocate
Allocate a memory block

Request:
```json
{ "size": 51200 }
```

Response:
```json
{
  "address": 102400,
  "size": 51200
}
```

### POST /api/deallocate
Deallocate a memory block

Request:
```json
{ "address": 102400 }
```

Response:
```json
{ "success": true }
```

### POST /api/strategy
Set allocation strategy

Request:
```json
{ "strategy": "BEST_FIT" }
```

### GET /api/blocks
Get all memory blocks

Response:
```json
[
  { "address": 0, "size": 51200, "allocated": true },
  { "address": 51200, "size": 997376, "allocated": false }
]
```

## 🧪 Learning Experiments

### Experiment 1: First Fit Fragmentation
1. Use **First Fit** strategy
2. Allocate: 100KB, 50KB, 100KB
3. Deallocate the 50KB block
4. Try to allocate 75KB
5. Observe: It uses the leftover 100KB block, leaving 25KB wasted

### Experiment 2: Best Fit Benefits
1. Run Experiment 1 again with **Best Fit**
2. Try to allocate 40KB after deallocation
3. Observe: Best Fit uses the 50KB gap, wasting only 10KB

### Experiment 3: Coalescing
1. Allocate three 100KB blocks
2. Deallocate the first and second
3. Observe: They merge into one 200KB block
4. This recovered memory is now usable

### Experiment 4: Edge Cases
1. Allocate blocks of various sizes
2. Create a highly fragmented state
3. Try to allocate a large block
4. See what happens when memory is fragmented but technically available

## 📚 Educational Resources

### Key Concepts
- **Virtual Memory**: The simulator uses a 1MB virtual heap
- **Memory Header**: 16 bytes per block for metadata (size, allocation status)
- **Minimum Block Size**: 16 bytes (prevents tiny unusable fragments)
- **Linked List**: Blocks are connected via next/previous pointers

### Related Topics
- Operating Systems memory management
- Heap allocation in malloc/free
- Garbage collection algorithms
- Virtual memory paging
- Memory pool allocators

## 🐳 Deployment

### Docker Deployment
```bash
# Build image
docker build -t memory-allocator .

# Run container
docker run -p 3000:3000 memory-allocator

# Access at http://localhost:3000
```

### Cloud Deployment (Heroku)
```bash
# Login to Heroku
heroku login

# Create app
heroku create your-app-name

# Deploy
git push heroku main

# View logs
heroku logs --tail
```

### Cloud Deployment (AWS, Azure, GCP)
Use the provided Dockerfile with:
- AWS ECS / EKS
- Azure Container Instances / App Service
- Google Cloud Run / App Engine

## 📋 System Requirements

### Web Version
- Node.js 12+ 
- Modern web browser (Chrome, Firefox, Safari, Edge)
- 50 MB disk space

### C++ Version
- C++17 compiler
- CMake 3.10+
- 20 MB disk space

## 🔍 Code Quality

- **Well-documented**: Every function and algorithm has detailed comments
- **Educational**: Code is written for clarity, not just performance
- **Portable**: Works on Windows, Linux, and macOS
- **Extensible**: Easy to add new allocation strategies

## 🤝 Contributing

Contributions are welcome! Consider:
- Adding new allocation strategies (Next Fit, Buddy System, etc.)
- Improving visualizations
- Adding more learning materials
- Fixing bugs or improving performance

## 📖 Algorithm Details

### Memory Block Structure
```cpp
struct MemoryBlock {
    size_t size;          // Block size in bytes
    bool allocated;       // Is this block allocated?
    MemoryBlock* next;    // Next block in linked list
    MemoryBlock* prev;    // Previous block in linked list
};
```

### First Fit Pseudocode
```
function firstFit(size):
    for each block in memory:
        if block is free and block.size >= size:
            return block
    return null
```

### Best Fit Pseudocode
```
function bestFit(size):
    bestBlock = null
    bestSize = infinity
    for each block in memory:
        if block is free and block.size >= size:
            if block.size < bestSize:
                bestBlock = block
                bestSize = block.size
    return bestBlock
```

## 📊 Performance Characteristics

| Strategy  | Allocation Time | Fragmentation | Memory Waste |
|-----------|-----------------|----------------|--------------|
| First Fit | O(n)            | High           | High         |
| Best Fit  | O(n)            | Medium         | Low          |

## 🎯 Learning Path

1. **Beginner**: Run the web interface, allocate/deallocate some blocks
2. **Intermediate**: Compare First Fit and Best Fit with the same sequence
3. **Advanced**: Study the C++ source code to understand implementations
4. **Expert**: Implement your own allocation strategy

## 🔗 Related Projects

- GNU Malloc: https://github.com/bminor/glibc
- Jemalloc: https://github.com/jemalloc/jemalloc
- Memory Pool Allocator: https://github.com/cacay/MemoryPool

## 📝 License

This project is open source and available under the MIT License.

## ✉️ Contact

For questions, suggestions, or collaborations, please open an issue or contact the project maintainers.

---

**Happy Learning!** 🎓 Start exploring how operating systems manage memory allocation.
