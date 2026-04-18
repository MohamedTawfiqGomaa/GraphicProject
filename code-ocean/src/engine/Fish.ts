import { Container, Sprite, Texture, Assets } from 'pixi.js';

export class Fish extends Container {
    private sprite: Sprite | null = null;
    public gridX: number = 0;
    public gridY: number = 0;
    public direction: number = 0; // 0: Right, 1: Down, 2: Left, 3: Up

    // Animation state
    private isMoving: boolean = false;
    private targetX: number = 0;
    private targetY: number = 0;
    private targetRotation: number = 0;
    private readonly TILE_SIZE: number = 80;
    private readonly MOVE_SPEED: number = 0.1; // Interpolation factor

    constructor() {
        super();
    }

    public async load() {
        const texture = await Assets.load('/src/assets/images/fish.png');
        this.sprite = new Sprite(texture);
        this.sprite.anchor.set(0.5);
        this.sprite.scale.set(0.25); // Adjusted scale
        this.addChild(this.sprite);
    }

    public setGridPosition(x: number, y: number) {
        this.gridX = x;
        this.gridY = y;
        this.x = x * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.y = y * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.targetX = this.x;
        this.targetY = this.y;
    }

    public setDirection(dir: number) {
        this.direction = dir;
        this.rotation = dir * (Math.PI / 2);
        this.targetRotation = this.rotation;
    }

    public moveForward(checkObstacle?: (x: number, y: number) => boolean) {
        if (this.isMoving) return;

        let nextX = this.gridX;
        let nextY = this.gridY;

        switch (this.direction) {
            case 0: nextX++; break; // Right
            case 1: nextY++; break; // Down
            case 2: nextX--; break; // Left
            case 3: nextY--; break; // Up
        }

        if (checkObstacle && checkObstacle(nextX, nextY)) {
            // Hit obstacle animation?
            return;
        }

        this.gridX = nextX;
        this.gridY = nextY;


        this.targetX = this.gridX * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.targetY = this.gridY * this.TILE_SIZE + this.TILE_SIZE / 2;
        this.isMoving = true;
    }

    public turnRight() {
        this.direction = (this.direction + 1) % 4;
        this.targetRotation += Math.PI / 2;
    }

    public turnLeft() {
        this.direction = (this.direction - 1 + 4) % 4;
        this.targetRotation -= Math.PI / 2;
    }

    public update(delta: number) {
        // Smooth movement
        if (Math.abs(this.x - this.targetX) > 1 || Math.abs(this.y - this.targetY) > 1) {
            this.x += (this.targetX - this.x) * this.MOVE_SPEED * delta;
            this.y += (this.targetY - this.y) * this.MOVE_SPEED * delta;
        } else {
            this.x = this.targetX;
            this.y = this.targetY;
            this.isMoving = false;
        }

        // Smooth rotation
        if (Math.abs(this.rotation - this.targetRotation) > 0.01) {
            this.rotation += (this.targetRotation - this.rotation) * 0.1 * delta;
        } else {
            this.rotation = this.targetRotation;
        }

        // Bobbing animation
        if (this.sprite) {
            // this.sprite.y = Math.sin(Date.now() * 0.005) * 5; 
        }
    }
}
