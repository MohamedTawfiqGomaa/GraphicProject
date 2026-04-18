import { useEffect, useRef } from 'react';
import { GameApp } from '../engine/GameApp';

interface GameCanvasProps {
    onMount: (game: GameApp) => void;
    onLevelComplete?: () => void;
}

export const GameCanvas = ({ onMount, onLevelComplete }: GameCanvasProps) => {
    const containerRef = useRef<HTMLDivElement>(null);
    const gameRef = useRef<GameApp | null>(null);

    useEffect(() => {
        if (!containerRef.current) return;

        const initGame = async () => {
            // Prevent double init
            if (gameRef.current) return;

            const game = new GameApp(containerRef.current!, onLevelComplete);
            gameRef.current = game;
            await game.init();
            onMount(game);
        };

        initGame();

        return () => {
            if (gameRef.current) {
                gameRef.current.destroy();
                gameRef.current = null;
            }
        };
    }, []);

    return (
        <div
            ref={containerRef}
            className="w-full h-full bg-ocean-900 overflow-hidden relative"
        />
    );
};
