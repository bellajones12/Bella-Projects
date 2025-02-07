package WizardTD;

import java.util.ArrayList;
import processing.core.PApplet;
import processing.core.PImage;

public class Button{
    int buttonX = 650;
    int buttonY;
    int buttonWidth = 40;
    int buttonHeight = 40;
    boolean buttonClicked = false;
    PApplet app;
    String name;
    PImage tower0;
    PImage fireball;
    Layout layout;

    TowerState towerState;
    int tower_cost;
    int initial_tower_range;
    double initial_tower_firing_speed;
    double initial_tower_damage;

    public ArrayList<Tower> towersArray;

    public ArrayList<Monster> monsters;

    public ArrayList<Tower> newTowers;

    Button(int buttonY, PApplet app, String name, int tower_cost, int initial_tower_range, double initial_tower_firing_speed, double initial_tower_damage, PImage tower0, Layout layout, PImage fireball, ArrayList<Monster> monsters) {
        this.buttonY = buttonY;
        this.app = app;
        this.name = name;
        this.tower0 = tower0;
        this.layout = layout;


        this.towersArray = new ArrayList<>();
        this.towerState = TowerState.PLACED;
        this.fireball = fireball;
        this.monsters = monsters;
        this.newTowers = new ArrayList<>();
        this.tower_cost = tower_cost;
        this.initial_tower_range = initial_tower_range;
        this.initial_tower_firing_speed = initial_tower_firing_speed;
        this.initial_tower_damage = initial_tower_damage;
    }

    public void setMonsters(ArrayList<Monster> monsters) {
        for (int i = 0; i < monsters.size(); i++) {
            if (!this.monsters.contains(monsters.get(i))) {
                this.monsters.add(monsters.get(i));
            }
        }
    }

    /**
     * @return if mouse is over the coordinates of the current button
     */
    public boolean isMouseOver(int x, int y) {
        return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonHeight && y <= this.buttonY + buttonHeight;
    }

    /**
     * set mouse coordinates for testing purposes
     */
    public void setMouse(int x, int y) {
        this.app.mouseX = x;
        this.app.mouseY = y;
    }

    /**
     * check if mouse has pressed button
     */
    void checkMousePressed() {
        // checking if mouse is hovering over button
        if (isMouseOver(this.app.mouseX, this.app.mouseY)) {
            app.rect(buttonX, this.buttonY, buttonWidth, buttonHeight);
            this.app.fill(200);
            app.rect(buttonX, this.buttonY, buttonWidth, buttonHeight);
            // checking if button is pressed
            if (app.mousePressed) {
                this.app.fill(255, 255, 0);
                app.rect(buttonX, this.buttonY, buttonWidth, buttonHeight);
                // checking if tower button was pressed
                buttonType();
            }
        } else {
            app.rect(buttonX, this.buttonY, buttonWidth, buttonHeight);
            this.app.fill(161, 128, 60);
            if (app.mousePressed) {
                if (this.name.equals("T")) {
                    if (towerState == TowerState.ACTIVATED) {
                        towerState = TowerState.NOTHING;
                        Tower tower1 = new Tower(this.app, this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.app.mouseX, this.app.mouseY, this.tower0, this.layout, this.fireball, this.monsters);
                        this.newTowers.add(tower1);
                        towerState = TowerState.NOTHING;
                    }
                }
            }
        }

        for (Tower tower : this.towersArray) {
            if ((this.app.mouseX >= tower.getX() && this.app.mouseX <= tower.getX() +20 && this.app.mouseY >= tower.getY() && this.app.mouseY <= tower.getY() + 20)) {
                if (towerState == TowerState.UPGRADE1) {
                    this.app.fill(255, 255, 255);
                    app.rect(650, 650, 80, 50);
                    if (app.mousePressed) {
                        towerState = TowerState.NOTHING;
                        tower.upgrade1();
                    }
                } else if (towerState == TowerState.UPGRADE2) {
                    this.app.fill(255, 255, 255);
                    app.rect(650, 650, 80, 50);
                    if (app.mousePressed) {
                        towerState = TowerState.NOTHING;
                        tower.upgrade2();
                    }
                } else if (towerState == TowerState.UPGRADE3) {
                    this.app.fill(255, 255, 255);
                    app.rect(650, 650, 80, 50);
                    if (app.mousePressed) {
                        towerState = TowerState.NOTHING;
                        tower.upgrade3();
                    }
                }
            }
        }
    }

    /**
     * activate associated tower state based on which button is pressed
     */
    void buttonType() {
        if (this.name.equals("T")) {
            // checking if t was already activated
            if (towerState != TowerState.ACTIVATED) {
                towerState = TowerState.ACTIVATED;
            }
        }
        // checking if upgrade 1 was pressed
        else if (this.name.equals("U1")) {
            if (this.towersArray.size() != 0) {
                towerState = TowerState.UPGRADE1;
            }
        }
        else if (this.name.equals("U2")) {
            if (this.towersArray.size() != 0) {
                towerState = TowerState.UPGRADE2;
            }
        }
        else if (this.name.equals("U3")) {
            if (this.towersArray.size() != 0) {
                towerState = TowerState.UPGRADE3;
            }
        }
    }

    /**
     * iterate through towers and draw
     */
    public void draw() {
        this.towersArray.addAll(this.newTowers);
        this.newTowers.clear();
        for (Tower tower: this.towersArray) {
            tower.tick();
            tower.draw(this.app);
            tower.checkMousePressed();
            tower.checkMonsters();
        }
    }

    /**
     * @return array of towers created
     */
    public ArrayList<Tower> getTowers() {
        return this.towersArray;
    }

    /**
     * update towers array
     */
    public void updateTowers(ArrayList<Tower> towersArray) {
        this.towersArray = towersArray;
    }
}