package WizardTD;

import processing.core.PApplet;
import processing.core.PImage;

import java.util.ArrayList;


public class Monster extends Character{

    public String type;
    public int hp;
    public  double speed;
    public double armour;
    public int mana_gained_on_kill;
    public int quantity;
    public PImage monsterDeath1;
    public PImage monsterDeath2;
    public PImage monsterDeath3;
    public PImage monsterDeath4;


    public boolean death_image;

    public ArrayList<ArrayList<Integer>> path;
    public int pathIndex = 0;

    public int currentHP;
    public float proportion;

    public boolean death;
    public boolean start;

    public int coordX;
    public int coordY;
    public int startX;
    public int startY;
    public int destinationX;
    public int destinationY;

    public int death_counter = 0;

    public Mana mana;

    public Monster(int x, int y, PImage sprite, Layout layout, PImage monsterDeath1, PImage monsterDeath2, PImage monsterDeath3, PImage monsterDeath4, String type, int hp, double speed, double armour, int mana_gained_on_kill, int quantity, Mana mana) {
        super(x, y, sprite, layout);

        this.monsterDeath1 = monsterDeath1;
        this.monsterDeath2 = monsterDeath2;
        this.monsterDeath3 = monsterDeath3;
        this.monsterDeath4 = monsterDeath4;
        this.path = this.layout.getCoordinates();


        this.hp = hp;
        this.currentHP = hp;
        this.proportion = 1;
        this.mana = mana;

        this.death = false;

        this.type = type;
        this.speed = speed;
        this.armour = armour;
        this.mana_gained_on_kill = mana_gained_on_kill;
        this.quantity = quantity;

        this.start = false;

        this.death_image = false;

    }

    /**
     * determining start coordinates based on first path coordinate
     */
    public void start() {
        for (int row = 0; row < this.layout.mapHeight; row++) {
            for (int column = 0; column < this.layout.mapWidth; column++) {
                char current = this.array[row][column];
                if (current == 'X') {
                    this.startX = column*this.TILESIZE;
                    this.x = this.startX;
                    this.startY = row*this.TILESIZE;
                    this.y = this.startY;
                    return;
                }
            }
        }
    }

    /**
     * @return whether monster is dead
     */
    public boolean death() {
        return this.death;
    }

    /**
     * draw monsters off screen if wave is over
     */
    public void waveEnd() {
        this.x = -760;
        this.y = -760;
    }

    /**
     * update wizard's and monster's hp based on if monster is hit
     */
    public void hit() {
        this.currentHP -= this.mana_gained_on_kill;
        this.mana.changeMana(this.mana_gained_on_kill);
    }

    /**
     * @return x-coordinate
     */
    public float getX() {
        return this.x;
    }

    /**
     * @return y-coordinate
     */
    public float getY() {
        return this.y;
    }

    /**
     * Handles monster moving along path and updating x and y coordinates
     */
    public void tick() {
        if (this.start == false) {
            start();
            this.start = true;
        }

        this.destinationX = this.layout.getWizard()[0];
        this.destinationY = this.layout.getWizard()[1];

        this.shortestPath(this.startX, this.startY, this.destinationX, this.destinationY);

        if (pathIndex < this.path.size()) {
            ArrayList<Integer> coordinates = this.path.get(pathIndex);
            this.coordX = coordinates.get(1)*TILESIZE;
            this.coordY = coordinates.get(0)*TILESIZE;

            if (this.x < this.coordX) {
                this.x += this.speed;
                if (this.x > this.coordX) {
                    this.x = this.coordX;
                }
            } else if (this.x > this.coordX) {
                this.x -= this.speed;
                if (this.x < this.coordX) {
                    this.x = this.coordX;
                }
            }
            if (this.y < this.coordY) {
                this.y += this.speed;
                if (this.y > this.coordY) {
                    this.y = this.coordY;
                }
            } else if (this.y > this.coordY) {
                this.y -= this.speed;
                if (this.y < this.coordY) {
                    this.y = this.coordY;
                }
            }

            if (this.x == this.coordX && this.y == this.coordY) {
                pathIndex ++;
            }
        }

        if (pathIndex == this.path.size()) {
            this.x = -760;
            this.y = -760;
            pathIndex = 0;
            this.start = false;
            this.mana.changeMana(-this.mana_gained_on_kill);
        }

        if (this.proportion <= 0) {
            this.death_image = true;        
        }

        if (this.death == true) {
            this.x = -760;
            this.y = -760;
            this.start = false;
            pathIndex = 0;
        }
    }

    /**
     * draw monster and power bar
     */
    public void draw(PApplet app) {
        if (! this.death_image) {
            app.image(this.sprite, this.x+5, this.y + 45);
            this.proportion = ((float)(this.currentHP) / (float)(this.hp));
            app.fill(0, 255, 0);
            app.rect(this.x-5, this.y+40, TILESIZE*this.proportion, 2);
            app.fill(255,0,0);
            app.rect(this.x-5 + this.TILESIZE * this.proportion, this.y+40, TILESIZE*(1-this.proportion), 2);
        }
        // else {
        //     if (this.death_counter < 4) {
        //         app.image(this.monsterDeath1, this.x*TILESIZE, this.y*TILESIZE + 45);
        //     } else if (this.death_counter < 8) {
        //         app.image(this.monsterDeath2, this.x*TILESIZE, this.y*TILESIZE + 45);
        //     } else if (this.death_counter < 12) {
        //         app.image(this.monsterDeath3, this.x*TILESIZE, this.y*TILESIZE + 45);
        //     } else if (this.death_counter < 16) {
        //         app.image(this.monsterDeath4, this.x*TILESIZE, this.y*TILESIZE + 45);
        //     }
        //     this.death_counter ++;
        //     if (this.death_counter == 16) {
        //         this.death = true;
        //         this.x = -760;
        //         this.y = -760;
        //     }
        // }
    }

    /**
     * determine shortest path using AAsterisk method
     */
    public void shortestPath(int sx, int sy, int dx, int dy) {
        AAsterisk aStar = new AAsterisk();
        this.path = aStar.setLayout(this.layout, sx/TILESIZE, sy/TILESIZE, dx, dy);
    }

}  
