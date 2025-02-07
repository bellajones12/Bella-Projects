package WizardTD;

import java.util.ArrayList;
import processing.core.PApplet;
import processing.core.PImage;

public class Fireball extends Character {
    public int speed;
    public float destinationX;
    public float destinationY;
    public float sourceX;
    public float sourceY;
    public Monster monster;

    Tower tower;
    PApplet app;

    ArrayList<float[]> coordinates;
    ArrayList<float[]> coordinatesArray;

    float slope;
    float y_int;

    float startX;
    float endX;

    public int coordinateIndex;

    boolean start;
    boolean dead;
    
    public Fireball(float x, float y, PImage sprite, Layout layout, PApplet app, float monsterX, float monsterY, Monster monster) {
        super(x, y, sprite, layout);
        this.speed = 2;
        this.app = app;
        this.destinationX = monsterX + 5;
        this.destinationY = (monsterY) + 5+45;
        this.sourceX = this.x+5;
        this.sourceY = this.y+5;
        this.coordinates = new ArrayList<>();
        this.coordinatesArray = determinePath();
        this.coordinateIndex = 0;
        this.start = false;
        this.dead = false;
        this.monster = monster;
    }

    /**
     * determining starting coordinates based on the towers coordinates
     */
    public void start() {
        this.x = this.sourceX;
        this.y = this.sourceY;
    }

    /**
     * Handles fireball moving along path towards monster
     */
    public void tick(){
        if (this.start == false) {
            start();
            this.start = true;
        }

        if (coordinateIndex < this.coordinates.size()) {
            float[] coordinates = this.coordinates.get(coordinateIndex);
            float coordX = coordinates[0];
            float coordY = coordinates[1];

            this.x = coordX;
            this.y = coordY;
            coordinateIndex ++;

        } else {
            this.x = -760;
            this.y = -760;
        }
    }

    /**
     * draw fireball frame by frame
     */
    public void draw(PApplet app){
        app.image(this.sprite, this.x, this.y);
    }

    /**
     * @return path from tower to monster for fireball to travel along
     */
    public ArrayList<float[]> determinePath() {
        // calculate slope
        slope = (this.destinationY - this.sourceY) / (this.destinationX - this.sourceX);

        // calculate y-intercept
        y_int = (this.sourceY - (slope * this.sourceX));

        if (this.destinationX < this.sourceX) {
            for (float x = this.sourceX; x >= this.destinationX; x -= this.speed) {
                float y = ((slope * x) + y_int);
                this.coordinates.add(new float[]{Math.round(x), Math.round(y)});
                if (this.destinationX > this.sourceX) {
                    this.coordinates.add(new float[]{this.destinationX, this.destinationY});
                    break;
                }
            } 
        } else {
            for (float x = this.sourceX; x <= this.destinationX; x += this.speed) {
                float y = ((slope * x) + y_int);
                this.coordinates.add(new float[]{Math.round(x), Math.round(y)});
                if (this.destinationX < this.sourceX) {
                    this.coordinates.add(new float[]{this.destinationX, this.destinationY});
                    break;
                }
            }
        }
        return this.coordinates;
    }

    /**
     * @return whether monster was hit by fireball
     */
    public boolean hit() {
        if (this.x <= (this.monster.getX()+16) || this.x >= (this.monster.getX() -16) && this.y >= this.monster.getY() - 16 && this.y <= this.monster.getY() + 16) {
            this.monster.hit();
            return true;
        } else {
            return false;
        }
    }
}