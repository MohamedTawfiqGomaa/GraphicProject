import { create } from 'zustand';
import { BlockData } from '../engine/BlockRunner';
import { v4 as uuidv4 } from 'uuid';

interface BlockStore {
    blocks: BlockData[];
    isRunning: boolean;
    addBlock: (type: BlockData['type']) => void;
    removeBlock: (id: string) => void;
    moveBlock: (activeId: string, overId: string) => void;
    setRunning: (running: boolean) => void;
    clearBlocks: () => void;
}

export const useBlockStore = create<BlockStore>((set) => ({
    blocks: [],
    isRunning: false,
    addBlock: (type) => set((state) => ({
        blocks: [...state.blocks, { id: uuidv4(), type }]
    })),
    removeBlock: (id) => set((state) => ({
        blocks: state.blocks.filter(b => b.id !== id)
    })),
    moveBlock: (activeId, overId) => set((state) => {
        const oldIndex = state.blocks.findIndex(b => b.id === activeId);
        const newIndex = state.blocks.findIndex(b => b.id === overId);

        const newBlocks = [...state.blocks];
        const [movedBlock] = newBlocks.splice(oldIndex, 1);
        newBlocks.splice(newIndex, 0, movedBlock);

        return { blocks: newBlocks };
    }),
    setRunning: (running) => set({ isRunning: running }),
    clearBlocks: () => set({ blocks: [] }),
}));
