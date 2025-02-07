package WizardTD;

import processing.core.PImage;
import processing.core.PApplet;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;


public class ButtonTest {
    PImage sprite;
    PApplet app1;
    Layout layout = new Layout(760, 680, "level1.txt", sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite, sprite);
    Mana mana = new Mana(50, 50);
    Monster monster = new Monster(5, 5, sprite, layout, sprite, sprite, sprite, sprite, "gremlin", 500, 1, 50, 50,  5, mana);
    ArrayList<Monster> monsters;
    Fireball fireball = new Fireball(5, 5, sprite, layout, app1, monster.getX(), monster.getY(), monster);

    @Test
    void mouseOverTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower0.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        Button button = new Button(20, app1, "T", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        assertNotNull(button);
        assertEquals(650, button.buttonX);
        assertEquals(20, button.buttonY);
        assertTrue(button.isMouseOver(690, 60));
    }

    @Test
    void mousePressedTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower0.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        Button button = new Button(0, app, "T", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        assertTrue(button.isMouseOver(690, 40));
        assertNotNull(button);
        assertNotNull(button.app);
        button.setMouse(650, 0);
        assertEquals(button.buttonY, button.app.mouseY);
        assertEquals(button.buttonX, button.app.mouseX);
    }

    @Test
    void buttonTypeTest() {
        // loading app and images
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower0.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, app.mouseX, app.mouseY, sprite, layout, sprite, monsters);
        Button button = new Button(0, app, "T", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        
        // testing button type
        assertEquals(TowerState.PLACED, button.towerState);
        button.buttonType();
        assertEquals(TowerState.ACTIVATED, button.towerState);
        button.app.mousePressed = true;
        assertTrue(button.app.mousePressed);
        button.setMouse(650, 0);
        button.checkMousePressed();

        // checking tower state is updated correclty
        Button button1 = new Button(0, app, "U1", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        Button button2 = new Button(0, app, "U2", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        Button button3 = new Button(0, app, "U3", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        button1.towersArray.add(tower);
        button2.towersArray.add(tower);
        button3.towersArray.add(tower);
        button1.buttonType();
        assertEquals(TowerState.UPGRADE1, button1.towerState);
        button2.buttonType();
        assertEquals(TowerState.UPGRADE2, button2.towerState);
        button3.buttonType();
        assertEquals(TowerState.UPGRADE3, button3.towerState);

        // checking tower state is updated back to base correclty
        button1.setMouse(650, 0);
        assertEquals(1, button1.towersArray.size());
        button1.app.mousePressed = true;
        button1.checkMousePressed();
    }

    @Test
    public void checkMousePressedTest() {
        App app = new App();
        PApplet.runSketch(new String[] { "App" }, app);
        app.delay(1000);
        sprite = app.loadImage("src/main/resources/WizardTD/tower0.png");
        layout.readMap();
        Tower tower = new Tower(app, 100, 96, 1.5, 40, 690, 40, sprite, layout, sprite, monsters);
        Button button = new Button(0, app, "T", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        
        // testing button type
        button.app.mousePressed = true;
        assertTrue(button.app.mousePressed);
        button.setMouse(690, 40);
        assertTrue(button.isMouseOver(button.app.mouseX, button.app.mouseY));

        
        // testing if statements
        Button button1 = new Button(0, app, "U1", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        Button button2 = new Button(0, app, "U2", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        Button button3 = new Button(0, app, "U3", tower.tower_cost, tower.initial_tower_range, tower.initial_tower_firing_speed, tower.initial_tower_damage, sprite, layout, sprite, monsters);
        button1.towersArray.add(tower);
        button2.towersArray.add(tower);
        button3.towersArray.add(tower);
        button1.buttonType();
        assertEquals(TowerState.UPGRADE1, button1.towerState);
        button1.checkMousePressed();
        assertEquals(1, button1.towersArray.size());
        assertEquals(button1.app.mouseX, tower.getX());
        assertEquals(button1.app.mouseY, tower.getY());
        button1.app.mousePressed = true;
        assertEquals(TowerState.NOTHING, button1.towerState);

    }
}