import { Application } from 'pixi.js';
import { LevelManager } from './LevelManager';
import { BlockRunner, type BlockData } from './BlockRunner';

export class GameApp {
    private app: Application;
    private viewContainer: HTMLElement;
    public levelManager: LevelManager | null = null;
    private blockRunner: BlockRunner | null = null;
    public onLevelComplete?: () => void;

    constructor(container: HTMLElement, onLevelComplete?: () => void) {
        this.viewContainer = container;
        this.onLevelComplete = onLevelComplete;
        this.app = new Application();
    }

    public async init() {
        await this.app.init({
            background: '#1099bb',
            resizeTo: this.viewContainer,
            resolution: window.devicePixelRatio || 1,
            autoDensity: true,
            antialias: true,
        });

        this.viewContainer.appendChild(this.app.canvas);

        await this.setupScene();
    }

    private async setupScene() {
        this.levelManager = new LevelManager(this.app);

        // Wire up events
        this.levelManager.onWin = () => {
            if (this.onLevelComplete) this.onLevelComplete();
        };

        await this.levelManager.init();

        this.blockRunner = new BlockRunner(this.levelManager.fish, (x, y) => {
            return this.levelManager!.isObstacle(x, y);
        });

        this.app.ticker.add((ticker) => {
            if (this.levelManager) {
                this.levelManager.update(ticker.deltaTime);
            }
        });
    }

    public async runCode(blocks: BlockData[]) {
        if (!this.blockRunner) return;
        await this.blockRunner.run(blocks);
    }

    public stopCode() {
        if (!this.blockRunner) return;
        this.blockRunner.stop();
    }

    public destroy() {
        this.app.destroy(true, { children: true, texture: true });
    }
}
