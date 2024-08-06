package WizardTD;

import java.util.ArrayList;

import processing.core.PApplet;
import processing.core.PImage;

public class Tower extends Character {
    public int tower_cost;
    public int initial_tower_range;
    public double initial_tower_firing_speed;
    public double initial_tower_damage;
    public PImage fireballimage;
    PApplet app;
    public  ArrayList<Monster> monsters;
    public  ArrayList<Fireball> fireballs;
    public  ArrayList<Fireball> fireballs_to_remove;
    double fireball_timer;


    public Tower(PApplet app, int tower_cost, int initial_tower_range, double initial_tower_firing_speed, double initial_tower_damage, float x, float y, PImage sprite, Layout layout, PImage fireball, ArrayList<Monster> monsters) {
        super(x, y, sprite, layout);
        this.tower_cost = tower_cost;
        this.initial_tower_range = initial_tower_range;
        this.initial_tower_firing_speed = initial_tower_firing_speed;
        this.initial_tower_damage = initial_tower_damage;
        this.fireballimage = fireball;
        this.app = app;        
        this.monsters = monsters;
        this.fireballs = new ArrayList<>();
        this.fireballs_to_remove = new ArrayList<>();
        this.fireball_timer = this.initial_tower_firing_speed;
    }

    
    public void tick() {
    }

    /**
     * set fireball timer
     */
    public void setTimer(double timer) {
        this.fireball_timer = timer;
    }

    /**
     * draw tower
     */
    public void draw(PApplet app) {
        app.image(this.sprite, this.x-10, this.y-10);
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
     * check if mouse is hovering over tower
     */
    void checkMousePressed() {
        // checking if mouse is hovering over tower
        if (this.app.mouseX >= this.getX() && this.app.mouseX <= this.getX() + 20 && this.app.mouseY >= this.getY() && this.app.mouseY <= this.getY() + 20) {
            this.app.stroke(255, 0, 0);
            this.app.noFill();
            app.ellipse(this.getX()+5, this.getY()+5, this.initial_tower_range*2, this.initial_tower_range*2);
            this.app.stroke(0);
        }
    }

    /**
     * check if monsters are within tower range, then create a fireball
     */
    void checkMonsters() {
            this.fireball_timer ++;
            if (this.fireball_timer >= this.initial_tower_firing_speed*60) {
                // System.out.println("within range");
                for (Monster monster : this.monsters) {
                    if ((monster.getX() >= (this.getX() - this.initial_tower_range) && monster.getX() <= (this.getX() + this.initial_tower_range)) && ((monster.getY())+45.00 >= (this.getY() - this.initial_tower_range) && (monster.getY())+45.00 <= (this.getY() + this.initial_tower_range))) {
                        // System.out.println("creating a fireball: " + monster.getX() + ", " + monster.getY());
                        Fireball fireball = new Fireball(this.x, this.y, this.fireballimage, this.layout, this.app, monster.getX(), monster.getY(), monster);
                        this.fireballs.add(fireball);
                        continue;
                    }
                }
                this.fireball_timer = 0;
            }

            if (this.fireballs != null) {
                for (Fireball fireball1 : this.fireballs) {
                    fireball1.tick();
                    fireball1.draw(this.app);  
                    if (fireball1.hit()) {
                        this.fireballs_to_remove.add(fireball1);
                    }
                }

                this.fireballs.removeAll(this.fireballs_to_remove);
                this.fireballs_to_remove.clear();
            }
    }

    /**
     * upgrade towers range
     */
    void upgrade1() {
        this.initial_tower_range += this.TILESIZE;
    }

    /**
     * upgrade towers firing speed
     */
    void upgrade2() {
        this.initial_tower_firing_speed += 0.5;
    }

    /**
     * upgrade towers damage
     */
    void upgrade3() {
        this.initial_tower_damage = this.initial_tower_damage*1.5;
    }
}
