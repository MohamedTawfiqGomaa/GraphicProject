export type BlockType = 'move_forward' | 'turn_right' | 'turn_left' | 'repeat';

export interface BlockData {
    id: string;
    type: BlockType;
    count?: number; // For repeat
    children?: BlockData[]; // For repeat body
}

import { Fish } from './Fish';

export class BlockRunner {
    private fish: Fish;
    private checkObstacle: (x: number, y: number) => boolean;
    private isRunning: boolean = false;
    private shouldStop: boolean = false;

    constructor(fish: Fish, checkObstacle: (x: number, y: number) => boolean) {
        this.fish = fish;
        this.checkObstacle = checkObstacle;
    }

    public async run(blocks: BlockData[]) {
        this.isRunning = true;
        this.shouldStop = false;

        for (const block of blocks) {
            if (this.shouldStop) break;
            await this.executeBlock(block);
            await new Promise(resolve => setTimeout(resolve, 500)); // Delay between steps
        }

        this.isRunning = false;
    }

    public stop() {
        this.shouldStop = true;
        this.isRunning = false;
    }

    private async executeBlock(block: BlockData) {
        // Highlight logic could go here (callback)
        console.log(`Executing ${block.type}`);

        switch (block.type) {
            case 'move_forward':
                this.fish.moveForward(this.checkObstacle);
                break;
            case 'turn_right':
                this.fish.turnRight();
                break;
            case 'turn_left':
                this.fish.turnLeft();
                break;
            case 'repeat':
                if (block.count && block.children) {
                    for (let i = 0; i < block.count; i++) {
                        if (this.shouldStop) return;
                        for (const child of block.children) {
                            if (this.shouldStop) return;
                            await this.executeBlock(child);
                            await new Promise(resolve => setTimeout(resolve, 500));
                        }
                    }
                }
                break;
        }
    }
}
