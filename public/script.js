/**
 * Memory Allocator Web Interface - JavaScript
 * Handles API calls, UI updates, and visualization
 */

let currentAllocations = [];
let currentStrategy = 'FIRST_FIT';

/**
 * Initialize the interface
 */
function init() {
    updateDisplay();
    setInterval(updateDisplay, 500);
}

/**
 * Allocate memory
 */
async function allocateMemory() {
    const sizeInput = document.getElementById('allocateSize');
    const size = parseInt(sizeInput.value);
    
    if (isNaN(size) || size <= 0) {
        alert('Please enter a valid size');
        return;
    }
    
    try {
        const response = await fetch('/api/allocate', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ size })
        });
        
        const result = await response.json();
        
        if (result.error) {
            alert('Error: ' + result.error);
        } else {
            currentAllocations.push({
                address: result.address,
                size: result.size
            });
            sizeInput.value = '';
            updateDisplay();
        }
    } catch (error) {
        alert('Error: ' + error.message);
    }
}

/**
 * Deallocate memory
 */
async function deallocateMemory(address) {
    try {
        const response = await fetch('/api/deallocate', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ address })
        });
        
        const result = await response.json();
        
        if (result.error) {
            alert('Error: ' + result.error);
        } else {
            currentAllocations = currentAllocations.filter(a => a.address !== address);
            updateDisplay();
        }
    } catch (error) {
        alert('Error: ' + error.message);
    }
}

/**
 * Set allocation strategy
 */
async function setStrategy(strategy) {
    try {
        const response = await fetch('/api/strategy', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ strategy })
        });
        
        const result = await response.json();
        
        if (result.success) {
            currentStrategy = strategy;
            
            // Update button styles
            document.querySelectorAll('.strategy-btn').forEach(btn => {
                btn.classList.remove('active');
            });
            event.target.classList.add('active');
            
            // Update info text
            const infoText = strategy === 'FIRST_FIT' 
                ? '<strong>First Fit:</strong> Allocates the first block that fits. Fast but may cause fragmentation.'
                : '<strong>Best Fit:</strong> Uses the smallest block that fits. Reduces waste but slower.';
            document.getElementById('strategyInfo').innerHTML = infoText;
            
            updateDisplay();
        }
    } catch (error) {
        alert('Error: ' + error.message);
    }
}

/**
 * Run automated demo
 */
async function runDemo() {
    await resetMemory();
    
    // Demo sequence
    const demoSequence = [
        { action: 'info', text: 'Demo: Allocating blocks...' },
        { action: 'allocate', size: 102400 },
        { action: 'delay', time: 500 },
        { action: 'allocate', size: 51200 },
        { action: 'delay', time: 500 },
        { action: 'allocate', size: 102400 },
        { action: 'delay', time: 500 },
        { action: 'info', text: 'Deallocating middle block to create fragmentation...' },
        { action: 'deallocate', index: 1 },
        { action: 'delay', time: 500 },
        { action: 'info', text: 'Trying to allocate 50KB...' },
        { action: 'allocate', size: 51200 },
        { action: 'delay', time: 500 },
        { action: 'info', text: 'Demo complete!' }
    ];
    
    for (const step of demoSequence) {
        if (step.action === 'allocate') {
            await allocateMemory();
            document.getElementById('allocateSize').value = step.size;
            await allocateMemory();
        } else if (step.action === 'deallocate' && step.index < currentAllocations.length) {
            await deallocateMemory(currentAllocations[step.index].address);
        } else if (step.action === 'delay') {
            await new Promise(resolve => setTimeout(resolve, step.time));
        } else if (step.action === 'info') {
            console.log(step.text);
        }
    }
}

/**
 * Reset memory
 */
async function resetMemory() {
    try {
        await fetch('/api/reset', { method: 'POST' });
        currentAllocations = [];
        updateDisplay();
    } catch (error) {
        alert('Error: ' + error.message);
    }
}

/**
 * Update all display elements
 */
async function updateDisplay() {
    try {
        const statsResponse = await fetch('/api/stats');
        const stats = await statsResponse.json();
        
        const blocksResponse = await fetch('/api/blocks');
        const blocks = await blocksResponse.json();
        
        // Update statistics
        updateStats(stats);
        
        // Update memory map
        updateMemoryMap(blocks);
        
        // Update block details
        updateBlockDetails(blocks);
        
        // Update blocks list
        updateBlocksList(currentAllocations);
    } catch (error) {
        console.error('Error updating display:', error);
    }
}

/**
 * Update statistics display
 */
function updateStats(stats) {
    document.getElementById('totalMemory').textContent = formatBytes(stats.totalMemory);
    document.getElementById('allocatedMem').textContent = formatBytes(stats.totalAllocated);
    document.getElementById('freeMem').textContent = formatBytes(stats.totalFree);
    document.getElementById('allocBlocks').textContent = stats.allocatedBlocks;
    document.getElementById('freeBlocks').textContent = stats.freeBlocks;
    document.getElementById('largestFree').textContent = formatBytes(stats.largestFreeBlock);
    document.getElementById('fragmentation').textContent = (stats.fragmentation * 100).toFixed(1) + '%';
    document.getElementById('allocPercent').textContent = stats.allocatedPercent + '%';
}

/**
 * Update memory map visualization
 */
function updateMemoryMap(blocks) {
    const mapContainer = document.getElementById('memoryMap');
    mapContainer.innerHTML = '';
    
    blocks.forEach(block => {
        const blockElement = document.createElement('div');
        blockElement.className = 'memory-block ' + (block.allocated ? 'allocated' : 'free');
        blockElement.title = `Address: ${block.address}, Size: ${formatBytes(block.size)}, Status: ${block.allocated ? 'Allocated' : 'Free'}`;
        mapContainer.appendChild(blockElement);
    });
}

/**
 * Update block details table
 */
function updateBlockDetails(blocks) {
    const detailsContainer = document.getElementById('blockDetails');
    detailsContainer.innerHTML = '';
    
    blocks.slice(0, 10).forEach((block, index) => {
        const row = document.createElement('div');
        row.className = 'detail-row';
        row.innerHTML = `
            <strong>${block.allocated ? 'Alloc #' + (index+1) : 'Free'}</strong>
            <span>${formatBytes(block.size)}</span>
        `;
        detailsContainer.appendChild(row);
    });
}

/**
 * Update allocated blocks list
 */
function updateBlocksList(allocations) {
    const listContainer = document.getElementById('blocksList');
    listContainer.innerHTML = '';
    
    if (allocations.length === 0) {
        listContainer.innerHTML = '<p style="color: #999; text-align: center;">No allocations yet</p>';
        return;
    }
    
    allocations.forEach((allocation, index) => {
        const blockItem = document.createElement('div');
        blockItem.className = 'block-item';
        blockItem.innerHTML = `
            <div>
                <strong>Block #${index + 1}</strong><br>
                <small>${formatBytes(allocation.size)}</small>
            </div>
            <button onclick="deallocateMemory(${allocation.address})">Free</button>
        `;
        listContainer.appendChild(blockItem);
    });
}

/**
 * Format bytes to human-readable format
 */
function formatBytes(bytes) {
    const units = ['B', 'KB', 'MB', 'GB'];
    let size = bytes;
    let unitIndex = 0;
    
    while (size >= 1024 && unitIndex < units.length - 1) {
        size /= 1024;
        unitIndex++;
    }
    
    return size.toFixed(1) + ' ' + units[unitIndex];
}

// Initialize on page load
document.addEventListener('DOMContentLoaded', init);
