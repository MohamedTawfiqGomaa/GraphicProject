import { Container, Graphics, Application, Sprite, Assets } from 'pixi.js';
import { Fish } from './Fish';

interface LevelConfig {
    cols: number;
    rows: number;
    start: { x: number, y: number };
    goal: { x: number, y: number };
    obstacles: { x: number, y: number }[];
}

export class LevelManager {
    private app: Application;
    private container: Container;
    private grid: Graphics;
    public fish: Fish;
    private obstacles: Sprite[] = [];
    private goal: Sprite | null = null;
    private readonly TILE_SIZE = 80;
    private currentLevel!: LevelConfig;
    public onWin?: () => void;

    constructor(app: Application) {
        this.app = app;
        this.container = new Container();
        this.grid = new Graphics();
        this.fish = new Fish();

        this.app.stage.addChild(this.container);
        this.container.addChild(this.grid);
        this.container.addChild(this.fish);
    }

    public async init() {
        // Default level for now
        this.currentLevel = {
            cols: 10,
            rows: 8,
            start: { x: 1, y: 3 },
            goal: { x: 8, y: 3 },
            obstacles: [
                { x: 4, y: 2 },
                { x: 4, y: 3 },
                { x: 4, y: 4 },
                { x: 6, y: 1 },
                { x: 6, y: 5 },
            ]
        };

        this.drawGrid(this.currentLevel.cols, this.currentLevel.rows);

        await this.loadLevelAssets();

        await this.fish.load();
        this.fish.setGridPosition(this.currentLevel.start.x, this.currentLevel.start.y);
    }

    private async loadLevelAssets() {
        const rockTexture = await Assets.load('/src/assets/images/rock.png');
        const starTexture = await Assets.load('/src/assets/images/star.png');

        // Allow multiple loads without error if texture cached

        // Spawn Goal
        this.goal = new Sprite(starTexture);
        this.goal.anchor.set(0.5);
        this.goal.scale.set(0.3); // Scale down
        this.goal.x = this.currentLevel.goal.x * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.goal.y = this.currentLevel.goal.y * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.container.addChild(this.goal);

        // Spawn Obstacles
        for (const obs of this.currentLevel.obstacles) {
            const rock = new Sprite(rockTexture);
            rock.anchor.set(0.5);
            rock.scale.set(0.35);
            rock.x = obs.x * this.TILE_SIZE + this.TILE_SIZE / 2;
            rock.y = obs.y * this.TILE_SIZE + this.TILE_SIZE / 2;
            this.container.addChild(rock);
            this.obstacles.push(rock);
        }
    }

    private drawGrid(cols: number, rows: number) {
        this.grid.clear();
        this.grid.lineStyle(2, 0xFFFFFF, 0.1);

        for (let i = 0; i <= cols; i++) {
            this.grid.moveTo(i * this.TILE_SIZE, 0);
            this.grid.lineTo(i * this.TILE_SIZE, rows * this.TILE_SIZE);
        }

        for (let i = 0; i <= rows; i++) {
            this.grid.moveTo(0, i * this.TILE_SIZE);
            this.grid.lineTo(cols * this.TILE_SIZE, i * this.TILE_SIZE);
        }

        // Center the grid in the view
        this.container.x = (this.app.screen.width - cols * this.TILE_SIZE) / 2;
        this.container.y = (this.app.screen.height - rows * this.TILE_SIZE) / 2;
    }

    public update(delta: number) {
        this.fish.update(delta);

        // Basic Collision Check (Goal)
        if (this.fish.gridX === this.currentLevel.goal.x && this.fish.gridY === this.currentLevel.goal.y) {
            if (this.goal) this.goal.rotation += 0.05; // Spin goal if reached

            // Trigger Win
            if (this.onWin) {
                this.onWin();
                this.onWin = undefined; // Trigger once
            }
        }
    }

    public isObstacle(x: number, y: number): boolean {
        return this.obstacles.some(obs => {
            const obsGridX = Math.floor(obs.x / this.TILE_SIZE);
            const obsGridY = Math.floor(obs.y / this.TILE_SIZE);
            return obsGridX === x && obsGridY === y;
        });
    }
}
