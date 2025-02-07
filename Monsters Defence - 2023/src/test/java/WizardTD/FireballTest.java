package WizardTD;

import processing.core.PImage;
import processing.core.PApplet;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;


public class FireballTest {
    PImage sprite;
    PApplet app = new App();
    Layout layout = new Layout(760, 680, "level1.txt", sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite);
    Mana mana = new Mana(50, 50);
    Monster monster = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 500, 1, 50, 50,  5, mana);
    
    @Test
    public void startTest() {
        App app = new App();
        App.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        assertNotNull(sprite);
        assertFalse(monster.death());
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        Fireball fireball = new Fireball(5, 5, sprite, layout, app, monster1.getX(), monster1.getY(), monster1);
        fireball.start();
        assertEquals(10, fireball.x);
        assertEquals(10, fireball.y);
    }


    @Test
    public void tickTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        Fireball fireball = new Fireball(10, 10, sprite, layout, app, monster1.getX(), monster1.getY(), monster1);
        assertFalse(fireball.start);
        fireball.tick();
        assertTrue(fireball.start);
    }

    @Test
    public void pathTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        Fireball fireball = new Fireball(10, 10, sprite, layout, app, monster1.getX(), monster1.getY(), monster1);
        assertEquals(10, fireball.destinationX);
        assertEquals(55, fireball.destinationY);
        assertEquals(15, fireball.sourceX);
        assertEquals(15, fireball.sourceY);
        fireball.determinePath();
        assertEquals(-8, fireball.slope);
        assertEquals(135, fireball.y_int);
    }

    @Test
    public void hitTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        Fireball fireball = new Fireball(10, 10, sprite, layout, app, monster1.getX(), monster1.getY(), monster1);
        assertTrue(fireball.hit());
        Monster monster2 = new Monster(500, 500, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        Fireball fireball1 = new Fireball(10, 10, sprite, layout, app, monster2.getX(), monster2.getY(), monster2);
        assertTrue(fireball1.hit());
    }

    @Test
    public void fireballTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        Fireball fireball = new Fireball(10, 10, sprite, layout, app, monster1.getX(), monster1.getY(), monster1);
        fireball.coordinateIndex = 500;
        fireball.tick();
        assertEquals(-760, fireball.x);
        assertEquals(-760, fireball.y);
    }

}