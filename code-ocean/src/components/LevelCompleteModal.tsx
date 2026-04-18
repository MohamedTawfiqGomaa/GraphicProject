import { RotateCcw, ArrowRight, Trophy } from 'lucide-react';

interface LevelCompleteModalProps {
    onReplay: () => void;
    onNextLevel: () => void;
}

export const LevelCompleteModal = ({ onReplay, onNextLevel }: LevelCompleteModalProps) => {
    return (
        <div className="absolute inset-0 bg-black/60 backdrop-blur-sm z-50 flex items-center justify-center">
            <div className="bg-ocean-800 border-2 border-ocean-600 rounded-3xl p-8 max-w-md w-full text-center shadow-2xl transform animate-in zoom-in-95 duration-300">
                <div className="relative inline-block mb-6">
                    <div className="absolute inset-0 bg-yellow-400 blur-xl opacity-30 rounded-full animate-pulse" />
                    <div className="bg-gradient-to-br from-yellow-300 to-orange-500 w-24 h-24 rounded-full flex items-center justify-center relative shadow-inner mx-auto ring-4 ring-ocean-900">
                        <Trophy size={48} className="text-white drop-shadow-md" />
                    </div>
                </div>

                <h2 className="text-4xl font-extrabold text-white mb-2 tracking-tight">
                    Level Complete!
                </h2>
                <p className="text-ocean-200 mb-8 text-lg">
                    Great job! You helped the fish find its way.
                </p>

                <div className="flex gap-4 justify-center">
                    <button
                        onClick={onReplay}
                        className="flex-1 py-3 px-4 bg-ocean-700 hover:bg-ocean-600 text-ocean-100 rounded-xl font-bold flex items-center justify-center gap-2 transition-all hover:scale-105 active:scale-95"
                    >
                        <RotateCcw size={20} />
                        Replay
                    </button>
                    <button
                        onClick={onNextLevel}
                        className="flex-1 py-3 px-4 bg-gradient-to-r from-green-500 to-emerald-600 hover:from-green-400 hover:to-emerald-500 text-white rounded-xl font-bold flex items-center justify-center gap-2 shadow-lg shadow-green-500/20 transition-all hover:scale-105 active:scale-95 border-b-4 border-green-700 hover:border-green-600 active:border-b-0 active:translate-y-1"
                    >
                        Next Level
                        <ArrowRight size={20} />
                    </button>
                </div>
            </div>
        </div>
    );
};
