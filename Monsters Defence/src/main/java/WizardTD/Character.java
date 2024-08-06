package WizardTD;
import processing.core.PApplet;
import processing.core.PImage;
public abstract class Character {

    protected float x;
    protected float y;
    protected PImage sprite;
    
    protected char[][] array;
    protected Layout layout;

    public final int TILESIZE = 32;

    public Character(float x, float y, PImage sprite, Layout layout) {
        this.layout = layout;
        this.x = x;
        this.y = y;
        this.sprite = sprite;
        this.array = this.layout.getArray();
    }
    /**
     * abstract method for drawing logic each child class will implement
     */
    abstract public void tick();

    /**
     * abstract method for drawing frame by frame, each child class will implement
     * @param app
     */
    abstract public void draw(PApplet app);


}

