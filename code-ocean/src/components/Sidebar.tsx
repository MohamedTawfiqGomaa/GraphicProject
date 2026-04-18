import { BLOCK_TYPES } from '../engine/BlockDefinitions';
import { useBlockStore } from '../stores/blockStore';
import { Plus } from 'lucide-react';

export const Sidebar = () => {
    const addBlock = useBlockStore(state => state.addBlock);

    return (
        <div className="w-64 bg-ocean-900 border-r border-ocean-700 p-4 flex flex-col gap-4">
            <h2 className="text-ocean-100 font-bold mb-2">Blocks</h2>

            <div className="space-y-3">
                {Object.values(BLOCK_TYPES).map((blockType) => (
                    <button
                        key={blockType.type}
                        onClick={() => addBlock(blockType.type as any)}
                        className={`w-full text-left p-3 rounded-lg shadow-sm ${blockType.color} 
                        hover:brightness-110 active:scale-95 transition-all text-white font-medium flex items-center justify-between group`}
                    >
                        {blockType.label}
                        <Plus size={16} className="opacity-0 group-hover:opacity-100 transition-opacity" />
                    </button>
                ))}
            </div>

            <div className="mt-auto p-4 bg-ocean-800 rounded-xl">
                <p className="text-xs text-ocean-300 text-center">
                    Click blocks to add them to your workspace.
                </p>
            </div>
        </div>
    );
};
