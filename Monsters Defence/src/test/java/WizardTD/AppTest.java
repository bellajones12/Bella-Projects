package WizardTD;

import org.junit.jupiter.api.Test;

import processing.core.PApplet;

public class AppTest {

    @Test
    public void mousePressedTest() {
        App app = new App();
        app.loop();
        PApplet.runSketch(new String[] { "App" }, app);
        app.setup();
        app.delay(1000);
        app.mousePressed();
    }


    @Test
    public void testFill() {
        App app = new App();
        app.loop();
        PApplet.runSketch(new String[] { "App" }, app);
        app.setup();
        app.delay(1000);
        app.fill(0);
    }


}