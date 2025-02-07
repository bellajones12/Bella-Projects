package WizardTD;

import processing.core.PImage;
import processing.core.PApplet;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;


public class TowerTest {
    PImage sprite;
    Layout layout = new Layout(760, 680, "level1.txt", sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite);
    Mana mana = new Mana(50, 50);
    Monster monster = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 500, 1, 50, 50,  5, mana);
    ArrayList<Monster> monsters;
    
    @Test
    public void upgrade1Test() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower0.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        assertEquals(96, tower.initial_tower_range);
        tower.upgrade1();
        assertEquals(128, tower.initial_tower_range);
    }

    @Test
    public void upgrade2Test() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower1.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        assertEquals(1.5, tower.initial_tower_firing_speed);
        tower.upgrade2();
        assertEquals(2.0, tower.initial_tower_firing_speed);
    }

    @Test
    public void upgrade3Test() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower2.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        assertEquals(40, tower.initial_tower_damage);
        tower.upgrade3();
        assertEquals(60, tower.initial_tower_damage);
    }

    @Test
    public void checkMonstersTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        Monster monster2 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        monsters = new ArrayList<>();
        monsters.add(monster1);
        monsters.add(monster2);
        Tower tower = new Tower(app, 100, 96, 1.5, 40, 0, 0, sprite, layout, sprite, monsters);
        tower.checkMonsters();
        tower.setTimer(tower.initial_tower_firing_speed*60);
        assertEquals(tower.fireball_timer, tower.initial_tower_firing_speed*60);
        tower.checkMonsters();
        assertEquals(tower.app.mouseX, tower.getX());
        assertEquals(tower.app.mouseY, tower.getY());
    }

    @Test
    public void checkMousePressedTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/gremlin.png");
        layout.readMap();
        Monster monster1 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        Monster monster2 = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 5, 1, 50, 5,  5, mana);
        monsters = new ArrayList<>();
        monsters.add(monster1);
        monsters.add(monster2);
        Tower tower = new Tower(app, 100, 96, 1.5, 40, 0, 0, sprite, layout, sprite, monsters);
        tower.checkMousePressed();
    }

}