/**
 * Memory Allocator Web API Server
 * 
 * This is a Node.js Express server that wraps the C++ memory allocator
 * and provides a REST API for web-based interaction and visualization.
 * 
 * Install dependencies: npm install express cors body-parser
 */

const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const path = require('path');

const app = express();
const PORT = 3000;

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(express.static('public'));

// In-memory simulation of the memory allocator
// This is a JavaScript implementation that mirrors the C++ logic
class MemoryAllocator {
    constructor(strategy = 'FIRST_FIT') {
        this.MEMORY_SIZE = 1024 * 1024;  // 1MB
        this.MIN_BLOCK_SIZE = 16;
        this.HEADER_SIZE = 16;
        
        this.blocks = [];
        this.strategy = strategy;
        this.stats = {
            totalAllocated: 0,
            totalFree: this.MEMORY_SIZE - this.HEADER_SIZE,
            allocatedBlocks: 0,
            freeBlocks: 1,
            largestFreeBlock: this.MEMORY_SIZE - this.HEADER_SIZE,
            fragmentation: 0.0
        };
        
        // Initialize with one large free block
        this.blocks.push({
            id: 0,
            address: 0,
            size: this.MEMORY_SIZE - this.HEADER_SIZE,
            allocated: false
        });
    }
    
    allocate(size) {
        if (size <= 0) return null;
        
        if (size < this.MIN_BLOCK_SIZE) {
            size = this.MIN_BLOCK_SIZE;
        }
        
        let block = null;
        
        if (this.strategy === 'FIRST_FIT') {
            block = this.findFirstFit(size);
        } else {
            block = this.findBestFit(size);
        }
        
        if (!block) {
            return { error: "Not enough memory" };
        }
        
        // Split block if necessary
        if (block.size > size + this.MIN_BLOCK_SIZE + this.HEADER_SIZE) {
            const newBlockAddress = block.address + size + this.HEADER_SIZE;
            const newBlockSize = block.size - size - this.HEADER_SIZE;
            
            this.blocks.push({
                id: this.getNextBlockId(),
                address: newBlockAddress,
                size: newBlockSize,
                allocated: false
            });
            
            block.size = size;
        }
        
        block.allocated = true;
        this.updateStats();
        
        return { address: block.address, size: block.size };
    }
    
    deallocate(address) {
        const block = this.blocks.find(b => b.address === address);
        
        if (!block || !block.allocated) {
            return { error: "Invalid deallocation" };
        }
        
        block.allocated = false;
        this.coalesceBlocks();
        this.updateStats();
        
        return { success: true };
    }
    
    findFirstFit(size) {
        return this.blocks.find(b => !b.allocated && b.size >= size);
    }
    
    findBestFit(size) {
        let bestBlock = null;
        let bestSize = this.MEMORY_SIZE + 1;
        
        for (let block of this.blocks) {
            if (!block.allocated && block.size >= size && block.size < bestSize) {
                bestBlock = block;
                bestSize = block.size;
            }
        }
        
        return bestBlock;
    }
    
    coalesceBlocks() {
        // Sort blocks by address
        this.blocks.sort((a, b) => a.address - b.address);
        
        for (let i = 0; i < this.blocks.length - 1; i++) {
            const current = this.blocks[i];
            const next = this.blocks[i + 1];
            
            if (!current.allocated && !next.allocated &&
                current.address + current.size + this.HEADER_SIZE === next.address) {
                current.size += next.size + this.HEADER_SIZE;
                this.blocks.splice(i + 1, 1);
                i--;
            }
        }
    }
    
    updateStats() {
        let totalAllocated = 0;
        let totalFree = 0;
        let allocatedBlocks = 0;
        let freeBlocks = 0;
        let largestFreeBlock = 0;
        
        for (let block of this.blocks) {
            if (block.allocated) {
                totalAllocated += block.size;
                allocatedBlocks++;
            } else {
                totalFree += block.size;
                freeBlocks++;
                largestFreeBlock = Math.max(largestFreeBlock, block.size);
            }
        }
        
        this.stats = {
            totalAllocated,
            totalFree,
            allocatedBlocks,
            freeBlocks,
            largestFreeBlock,
            fragmentation: totalFree > 0 ? 1.0 - (largestFreeBlock / totalFree) : 0
        };
    }
    
    setStrategy(strategy) {
        this.strategy = strategy;
    }
    
    getStats() {
        return {
            ...this.stats,
            strategy: this.strategy,
            totalMemory: this.MEMORY_SIZE,
            allocatedPercent: (this.stats.totalAllocated * 100 / this.MEMORY_SIZE).toFixed(2)
        };
    }
    
    getBlocks() {
        return this.blocks.map(b => ({
            address: b.address,
            size: b.size,
            allocated: b.allocated
        }));
    }
    
    getNextBlockId() {
        return Math.max(0, ...this.blocks.map(b => b.id)) + 1;
    }
}

// Create global allocator instance
const allocator = new MemoryAllocator('FIRST_FIT');

// Routes

// Get current statistics
app.get('/api/stats', (req, res) => {
    res.json(allocator.getStats());
});

// Get all blocks
app.get('/api/blocks', (req, res) => {
    res.json(allocator.getBlocks());
});

// Allocate memory
app.post('/api/allocate', (req, res) => {
    const { size } = req.body;
    const result = allocator.allocate(size);
    res.json(result);
});

// Deallocate memory
app.post('/api/deallocate', (req, res) => {
    const { address } = req.body;
    const result = allocator.deallocate(address);
    res.json(result);
});

// Set allocation strategy
app.post('/api/strategy', (req, res) => {
    const { strategy } = req.body;
    if (['FIRST_FIT', 'BEST_FIT'].includes(strategy)) {
        allocator.setStrategy(strategy);
        res.json({ success: true, strategy });
    } else {
        res.status(400).json({ error: 'Invalid strategy' });
    }
});

// Reset allocator
app.post('/api/reset', (req, res) => {
    allocator.blocks = [{
        id: 0,
        address: 0,
        size: allocator.MEMORY_SIZE - allocator.HEADER_SIZE,
        allocated: false
    }];
    allocator.updateStats();
    res.json({ success: true });
});

// Start server
app.listen(PORT, () => {
    console.log(`Memory Allocator Web API running on http://localhost:${PORT}`);
    console.log(`Open http://localhost:${PORT} in your browser to access the interface`);
});
