import { useRef, useState } from 'react';
import { GameCanvas } from './components/GameCanvas';
import { Sidebar } from './components/Sidebar';
import { Workspace } from './components/Workspace';
import { useBlockStore } from './stores/blockStore';
import { GameApp } from './engine/GameApp';
import { Play, RotateCcw } from 'lucide-react';
import { LevelCompleteModal } from './components/LevelCompleteModal';

function App() {
  const gameAppRef = useRef<GameApp | null>(null);
  const { blocks, isRunning, setRunning } = useBlockStore();
  const [isLevelComplete, setIsLevelComplete] = useState(false);

  const handleLevelComplete = () => {
    setIsLevelComplete(true);
    setRunning(false);
  };

  const handleRun = async () => {
    if (!gameAppRef.current) return;
    setRunning(true);
    await gameAppRef.current.runCode(blocks);
    setRunning(false);
  };

  const handleReset = () => {
    if (gameAppRef.current) {
      gameAppRef.current.stopCode();
      // Reset fish position (hacky way for now)
      if (gameAppRef.current.levelManager) {
        gameAppRef.current.levelManager.fish.setGridPosition(1, 3);
        gameAppRef.current.levelManager.fish.setDirection(0);
      }
    }
    setRunning(false);
  };

  return (
    <div className="w-screen h-screen flex flex-col bg-ocean-900">
      {/* Header */}
      <div className="h-14 bg-ocean-800 border-b border-ocean-700 flex items-center px-6 shadow-md z-10 justify-between">
        <h1 className="text-xl font-bold bg-gradient-to-r from-yellow-300 to-orange-400 bg-clip-text text-transparent flex items-end gap-2">
          <span>Code Ocean</span>
          <span className="text-xs text-ocean-300 font-normal pb-0.5">Educational Edition</span>
        </h1>
      </div>

      {/* Main Content */}
      <div className="flex-1 flex overflow-hidden">
        <Sidebar />

        <div className="flex-1 flex flex-col min-w-0">
          <div className="flex-1 relative flex">
            {/* Game View */}
            <div className="flex-1 relative border-r border-ocean-700 bg-ocean-900">
              <GameCanvas
                onMount={(game) => gameAppRef.current = game}
                onLevelComplete={handleLevelComplete}
              />

              {isLevelComplete && (
                <LevelCompleteModal
                  onReplay={() => {
                    setIsLevelComplete(false);
                    handleReset();
                  }}
                  onNextLevel={() => {
                    setIsLevelComplete(false);
                    handleReset();
                    alert("Next level coming soon!");
                  }}
                />
              )}

              {/* Overlay Controls */}
              <div className="absolute bottom-6 right-6 flex gap-3">
                <button
                  onClick={handleReset}
                  className="p-3 bg-red-500 hover:bg-red-400 text-white rounded-full shadow-lg transition-transform hover:scale-105 active:scale-95 disabled:opacity-50"
                  disabled={!isRunning}
                >
                  <RotateCcw size={24} color="white" />
                </button>
                <button
                  onClick={handleRun}
                  className={`px-6 py-3 ${isRunning ? 'bg-gray-500 cursor-not-allowed' : 'bg-green-500 hover:bg-green-400'} text-white font-bold rounded-full shadow-lg transition-transform hover:scale-105 active:scale-95 flex items-center gap-2`}
                  disabled={isRunning}
                >
                  <Play size={24} fill="currentColor" />
                  {isRunning ? 'Running...' : 'Run Code'}
                </button>
              </div>
            </div>

            <div className="w-[400px] flex flex-col border-l border-ocean-700 bg-ocean-900/50 backdrop-blur-sm z-10 shadow-xl">
              <Workspace />
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
