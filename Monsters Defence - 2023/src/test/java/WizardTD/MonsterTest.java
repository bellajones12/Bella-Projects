package WizardTD;

import processing.core.PImage;
import processing.core.PApplet;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;


public class MonsterTest {
    PImage sprite;
    Layout layout = new Layout(760, 680, "level1.txt", sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite);
    Mana mana = new Mana(50, 50);
    Monster monster = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 500, 1, 50, 50,  5, mana);

    
    @Test
    public void loadSprite() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        assertNotNull(sprite);
    }
    

    @Test
    public void hitTest() {
        monster.hit();
        assertEquals(450, monster.currentHP);
    }


    @Test
    public void deathTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        assertNotNull(sprite);
        assertFalse(monster.death());
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        assertNotNull(monster1.pathIndex);
        monster1.tick();
        monster1.hit();
        assertEquals(0, monster1.currentHP);
        monster1.tick();
        monster1.draw(app);
        monster1.tick();
        assertTrue(monster1.death_image);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        monster1.draw(app);
        assertEquals(16, monster1.death_counter);
        assertTrue(monster1.death);
        assertEquals(-760, monster1.getX());
        assertEquals(-760, monster1.getY());
    }


    @Test
    public void tickTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        assertFalse(monster.start);
        Monster monster1 = new Monster(5, 5, sprite, layout, null, sprite, sprite, sprite, "gremlin", 50, 1, 50, 5,  5, mana);
        assertNotNull(monster1.array);
        monster1.tick();
        assertEquals(0, monster1.x);
        assertEquals(96, monster1.y);
        assertTrue(monster1.start);
        assertEquals(1, monster1.pathIndex);
    }

}