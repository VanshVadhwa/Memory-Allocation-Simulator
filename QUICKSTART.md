# Quick Start Guide

Get the Memory Allocator Simulator running in 5 minutes!

## 🚀 Web Version (Easiest)

### Prerequisites
- Node.js 12+ installed
- Command line/terminal access

### Installation
```bash
# 1. Navigate to project directory
cd your-project-directory

# 2. Install dependencies
npm install

# 3. Start server
npm start
```

### Access
Open your browser to: **http://localhost:3000**

Done! 🎉

## 💻 C++ Command-Line Version

### Prerequisites
- C++17 compiler (Visual Studio, GCC, or Clang)
- CMake 3.10+

### Build & Run
```bash
# 1. Create build directory
mkdir build
cd build

# 2. Generate build files
cmake ..

# 3. Build
cmake --build .

# 4. Run
./memory_allocator
```

## 🐳 Docker Version

### Prerequisites
- Docker installed

### Run
```bash
# Build image
docker build -t memory-allocator .

# Run container
docker run -p 3000:3000 memory-allocator

# Access at http://localhost:3000
```

## 📚 First Steps

1. **Open the Web Interface** (http://localhost:3000)

2. **Try Basic Allocation**
   - Enter "102400" (100KB) in the size field
   - Click "Allocate"
   - See the memory map update

3. **Deallocate**
   - Click "Free" button on allocated block
   - Watch the memory coalesce

4. **Switch Strategies**
   - Try "First Fit" vs "Best Fit"
   - Allocate same-sized blocks
   - Compare fragmentation

5. **Run Demo**
   - Click "Run Demo" to see automated example
   - Observe block splitting and coalescing

## 🎯 Learning Path

### Beginner (5 minutes)
- Allocate and deallocate a few blocks
- Observe memory map changes

### Intermediate (15 minutes)
- Compare First Fit vs Best Fit
- Create fragmentation intentionally
- Try to allocate in fragmented memory

### Advanced (30 minutes)
- Study the C++ source code
- Understand linked list implementation
- Trace allocation algorithm step-by-step

## 🔍 Key Features to Explore

| Feature | Location | What It Shows |
|---------|----------|---------------|
| Memory Map | Center panel | Visual block layout (Blue=Allocated, Green=Free) |
| Statistics | Top right | Real-time metrics and percentages |
| Block Details | Bottom right | Individual block information |
| Allocated Blocks | Left panel | Your active allocations |
| Strategy Buttons | Left panel | Switch algorithms |
| Learning Guide | Bottom | Detailed explanations |

## 📞 Troubleshooting

**Port 3000 already in use?**
```bash
# Change port in server.js (line 12):
# const PORT = 3001;
```

**npm install fails?**
```bash
npm cache clean --force
npm install
```

**Docker won't start?**
```bash
docker ps  # Check running containers
docker logs container-id  # View logs
```

## 🌐 Deploy Online

Want to share it online? See [DEPLOYMENT.md](DEPLOYMENT.md) for:
- Heroku (easiest)
- Google Cloud Run
- AWS
- Azure
- DigitalOcean

## 📖 Full Documentation

- **[README.md](README.md)** - Complete documentation
- **[DEPLOYMENT.md](DEPLOYMENT.md)** - Deployment guide
- **[os.cpp](os.cpp)** - Source code with comments

## 💡 Common Experiments

### Create Fragmentation
```
1. First Fit strategy
2. Allocate: 100KB, 50KB, 100KB
3. Deallocate: 50KB (middle)
4. Try to allocate 75KB
5. Observe: Wastes 25KB
```

### Compare Strategies
```
1. Same allocations with First Fit vs Best Fit
2. Compare fragmentation percentage
3. Notice Best Fit reduces waste
```

### Coalescing Demo
```
1. Allocate three 50KB blocks
2. Deallocate first and second
3. Watch them merge into 100KB
```

## ✅ Next Steps

1. Explore the web interface
2. Try the learning experiments
3. Read the Learning Guide (at bottom of page)
4. Study the C++ source code
5. Deploy online to share with others

---

**Need help?** Check the README.md for comprehensive documentation or review the inline code comments in os.cpp.

Happy Learning! 🎓
