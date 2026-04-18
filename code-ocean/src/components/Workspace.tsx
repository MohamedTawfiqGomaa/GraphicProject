import { DndContext, closestCenter, DragEndEvent } from '@dnd-kit/core';
import { SortableContext, verticalListSortingStrategy } from '@dnd-kit/sortable';
import { useBlockStore } from '../stores/blockStore';
import { Block } from './Block';

export const Workspace = () => {
    const { blocks, moveBlock, removeBlock } = useBlockStore();

    const handleDragEnd = (event: DragEndEvent) => {
        const { active, over } = event;

        if (active.id !== over?.id && over) {
            moveBlock(active.id as string, over.id as string);
        }
    };

    return (
        <div className="flex-1 bg-ocean-800/30 p-4 overflow-y-auto">
            <h2 className="text-ocean-200 font-semibold mb-4 text-sm uppercase tracking-wider">Workspace</h2>

            <DndContext
                collisionDetection={closestCenter}
                onDragEnd={handleDragEnd}
            >
                <SortableContext
                    items={blocks.map(b => b.id)}
                    strategy={verticalListSortingStrategy}
                >
                    <div className="space-y-2 min-h-[200px] pb-20">
                        {blocks.length === 0 && (
                            <div className="text-center py-10 text-ocean-400 border-2 border-dashed border-ocean-700 rounded-lg">
                                Drag blocks here implicitly (click to add for now)
                            </div>
                        )}
                        {blocks.map((block) => (
                            <Block
                                key={block.id}
                                block={block}
                                onDelete={() => removeBlock(block.id)}
                            />
                        ))}
                    </div>
                </SortableContext>
            </DndContext>
        </div>
    );
};
