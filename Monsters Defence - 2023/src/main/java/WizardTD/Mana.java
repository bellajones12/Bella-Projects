package WizardTD;

import processing.core.PApplet;
import processing.core.PFont;


public class Mana {
    int mana;
    int mana_cap;
    int initial_mana;
    float proportion;
    public static PFont font;

    public Mana(int mana, int mana_cap) {
        this.initial_mana = mana;
        this.mana = mana;
        this.mana_cap = mana_cap;
    } 

    /**
     * @return mana
     */
    public int getMana() {
        return this.mana;
    }

    /**
     * update mana based on whether monster was hit, or monster was banished
     */
    public void changeMana(int change_in_points) {
        this.mana += change_in_points;
        if (this.mana > this.mana_cap) {
            this.mana = this.mana_cap;
        }
        if (this.mana < 0) {
            this.mana = 0;
        }
    }

    /**
     * mana powerbar
     */
    public void draw(PApplet app) {
        this.proportion = ((float)(this.mana) / (float)(this.mana_cap));
        app.fill(255, 255,230);
        app.rect(450, 10, 250,20);
        app.fill(0, 150, 255);
        app.rect(450, 10, 250*this.proportion, 20);
    }
}
