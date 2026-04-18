import { useSortable } from '@dnd-kit/sortable';
import { CSS } from '@dnd-kit/utilities';
import { BlockData } from '../engine/BlockRunner';
import { BLOCK_TYPES } from '../engine/BlockDefinitions';
import clsx from 'clsx';
import { GripVertical, Trash2 } from 'lucide-react';

interface BlockProps {
    block: BlockData;
    onDelete?: () => void;
}

export const Block = ({ block, onDelete }: BlockProps) => {
    const {
        attributes,
        listeners,
        setNodeRef,
        transform,
        transition,
        isDragging,
    } = useSortable({ id: block.id });

    const style = {
        transform: CSS.Transform.toString(transform),
        transition,
    };

    const blockInfo = Object.values(BLOCK_TYPES).find(b => b.type === block.type) || BLOCK_TYPES.MOVE_FORWARD;

    return (
        <div
            ref={setNodeRef}
            style={style}
            className={clsx(
                'flex items-center gap-3 p-3 rounded-lg shadow-sm border border-white/10 select-none group',
                blockInfo.color,
                isDragging ? 'opacity-50 z-50' : 'opacity-100',
                'bg-opacity-90 backdrop-blur-sm'
            )}
        >
            <div {...attributes} {...listeners} className="cursor-grab active:cursor-grabbing text-white/50 hover:text-white">
                <GripVertical size={18} />
            </div>

            <span className="font-bold text-white text-sm flex-1">
                {blockInfo.label}
            </span>

            {onDelete && (
                <button
                    onClick={onDelete}
                    className="text-white/50 hover:text-white hover:bg-white/20 p-1 rounded transition-colors"
                >
                    <Trash2 size={16} />
                </button>
            )}
        </div>
    );
};
