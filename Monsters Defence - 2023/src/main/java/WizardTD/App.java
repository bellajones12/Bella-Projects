package WizardTD;

import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.io.File;


import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PImage;
import processing.event.MouseEvent;
import processing.data.JSONArray;
import processing.data.JSONObject;

public class App extends PApplet {

    public static final int CELLSIZE = 32;
    public static final int SIDEBAR = 120;
    public static final int TOPBAR = 40;
    public static final int BOARD_WIDTH = 20;

    public static int WIDTH = CELLSIZE*BOARD_WIDTH+SIDEBAR;
    public static int HEIGHT = BOARD_WIDTH*CELLSIZE+TOPBAR;

    public PImage grass;
    public PImage shrub;
    public PImage horizontal;
    public PImage vertical;
    public PImage leftTurn;
    public PImage rightTurn;
    public PImage leftTurnUp;
    public PImage rightTurnUp;
    public PImage left;
    public PImage right;
    public PImage down;
    public PImage up;
    public PImage path3;
    public PImage wizard_house;
    public PImage monsterImage;

    public PImage monsterDeath1;
    public PImage monsterDeath2;
    public PImage monsterDeath3;
    public PImage monsterDeath4;

    public PImage tower0;
    public PImage tower1;
    public PImage tower2;

    public PImage fireball;

    public static final int FPS = 60;

    public Layout layout;

    public JSONObject config;
    public String configPath;
    public String JSONLevel;
    public String JSONwave;

    public JSONArray JSONWaves;

    public Waves[] waves;
    
    public static PFont font;

    public Monster monster;
    public Tower tower;

    public  ArrayList<Monster> monsters;
    public  ArrayList<Monster> monsters_final;
    public  ArrayList<Tower> towersArray;

    public Button buttonFF;
    public Button buttonP;
    public Button buttonT;
    public Button buttonU1;
    public Button buttonU2;
    public Button buttonU3;
    public Button buttonM;
    public ArrayList<Button> buttons;

    public Mana mana;

    int initial_tower_range;
    double initial_tower_firing_speed;
    int initial_tower_damage;
    int initial_mana;
    int initial_mana_cap;
    int initial_mana_gained_per_second;
    int tower_cost;
    int mana_pool_spell_initial_cost;
    int mana_pool_spell_cost_increase_per_use;
    double mana_pool_spell_cap_multiplier;
    double mana_pool_spell_mana_gained_multiplier;

    // wave attributes
    int duration;
    double pre_wave_pause;

    // monster attributes
    String type;
    int hp;
    double speed;
    double armour;
    int mana_gained_on_kill;
    int quantity;

    ArrayList<Waves> waveslist;

    int monster_timer;
    int monster_increment;
    int monster_seconds_between_moves = 2;
    int wave_increment;


    public App() {
        this.configPath = "config.json";
    }

    /**
     * Initialise the setting of the window size.
     */
    public void settings() {
        size(WIDTH, HEIGHT);
    }

    /**
     * Load all resources such as images. Initialise the elements such as the player, enemies and map elements.
     */
    public void setup() {
        frameRate(FPS);
        this.wave_increment = 0;
        
        // Load images
        this.horizontal = loadImage("src/main/resources/WizardTD/path0.png");
        this.leftTurn = loadImage("src/main/resources/WizardTD/path1.png");
        this.leftTurnUp = rotateImageByDegrees(this.leftTurn, 90);
        this.down = loadImage("src/main/resources/WizardTD/path2.png");
        this.left = rotateImageByDegrees(this.down, 90);
        this.up = rotateImageByDegrees(this.down, 180);
        this.right = rotateImageByDegrees(this.down, 270);
        this.grass = loadImage("src/main/resources/WizardTD/grass.png");
        this.shrub = loadImage("src/main/resources/WizardTD/shrub.png");
        this.path3 = loadImage("src/main/resources/WizardTD/path3.png");
        this.vertical = rotateImageByDegrees(this.horizontal, 90);
        this.rightTurnUp = rotateImageByDegrees(this.leftTurn, 180);
        this.rightTurn = rotateImageByDegrees(this.leftTurn, 270);
        this.wizard_house = loadImage("src/main/resources/WizardTD/wizard_house.png");
        this.wizard_house.resize(48,48);
        this.monsterImage = loadImage("src/main/resources/WizardTD/gremlin.png");
        this.monsterDeath1 = loadImage("src/main/resources/WizardTD/gremlin1.png");
        this.monsterDeath2 = loadImage("src/main/resources/WizardTD/gremlin2.png");
        this.monsterDeath3 = loadImage("src/main/resources/WizardTD/gremlin3.png");
        this.monsterDeath4 = loadImage("src/main/resources/WizardTD/gremlin4.png");
        this.tower0 = loadImage("src/main/resources/WizardTD/tower0.png");
        this.fireball = loadImage("src/main/resources/WizardTD/fireball.png");
        
        // get data from JSON config file
        this.config = loadJSONObject(new File(this.configPath));  

        this.JSONLevel = config.getString("layout");

        this.initial_tower_range = config.getInt("initial_tower_range");
        this.initial_tower_firing_speed = config.getDouble("initial_tower_firing_speed");
        this.initial_tower_damage = config.getInt("initial_tower_damage");

        this.initial_mana = config.getInt("initial_mana");
        this.initial_mana_cap = config.getInt("initial_mana_cap");
        this.initial_mana_gained_per_second = config.getInt("initial_mana_gained_per_second");

        this.tower_cost = config.getInt("tower_cost");

        this.mana_pool_spell_initial_cost = config.getInt("mana_pool_spell_initial_cost");
        this.mana_pool_spell_cost_increase_per_use = config.getInt("mana_pool_spell_cost_increase_per_use");
        this.mana_pool_spell_cap_multiplier = config.getDouble("mana_pool_spell_cap_multiplier");
        this.mana_pool_spell_mana_gained_multiplier = config.getDouble("mana_pool_spell_mana_gained_multiplier");

        // draw layout
        this.layout = new Layout(WIDTH, HEIGHT, this.JSONLevel, this.horizontal, this.vertical, this.leftTurn, this.rightTurn, this.leftTurnUp, this.rightTurnUp, this.down, this.left, this.up, this.right, this.path3, this.shrub, this.grass, this.wizard_house);
        this.layout.readMap();


        this.monster_increment = 0;
        this.monster_timer = monster_seconds_between_moves;

        this.monsters_final = new ArrayList<>();

        this.towersArray = new ArrayList<>();
        
        JSONArray wavesArray = config.getJSONArray("waves");
        this.waveslist = new ArrayList<>();

        this.mana = new Mana(200, 1000);

        // create object for each wave
        for (int i = 0; i < wavesArray.size(); i++) {
            JSONObject waveObject = wavesArray.getJSONObject(i);
            this.duration = waveObject.getInt("duration");
            this.pre_wave_pause = waveObject.getDouble("pre_wave_pause");
            
            JSONArray monstersArray = waveObject.getJSONArray("monsters");
            this.monsters = new ArrayList<>();
            
            // create object for each monster
            for (int j = 0; j < monstersArray.size(); j++) {
                JSONObject monsterObject = monstersArray.getJSONObject(j);
                this.type = monsterObject.getString("type");
                this.hp = monsterObject.getInt("hp");
                this.speed = monsterObject.getDouble("speed");
                this.armour = monsterObject.getDouble("armour");
                this.mana_gained_on_kill = monsterObject.getInt("mana_gained_on_kill");
                this.quantity = monsterObject.getInt("quantity");
        
                Monster monster = new Monster(-WIDTH, -HEIGHT, this.monsterImage, this.layout, this.monsterDeath1, this.monsterDeath2, this.monsterDeath3, this.monsterDeath4, this.type, this.hp, this.speed, this.armour, this.mana_gained_on_kill, this.quantity, this.mana);
                Monster monster1 = new Monster(-WIDTH, -HEIGHT, this.monsterImage, this.layout, this.monsterDeath1, this.monsterDeath2, this.monsterDeath3, this.monsterDeath4, this.type, this.hp, this.speed, this.armour, this.mana_gained_on_kill, this.quantity, this.mana);
                Monster monster2 = new Monster(-WIDTH, -HEIGHT, this.monsterImage, this.layout, this.monsterDeath1, this.monsterDeath2, this.monsterDeath3, this.monsterDeath4, this.type, this.hp, this.speed, this.armour, this.mana_gained_on_kill, this.quantity, this.mana);
                Monster monster3 = new Monster(-WIDTH, -HEIGHT, this.monsterImage, this.layout, this.monsterDeath1, this.monsterDeath2, this.monsterDeath3, this.monsterDeath4, this.type, this.hp, this.speed, this.armour, this.mana_gained_on_kill, this.quantity, this.mana);
                this.monsters.add(monster);
                this.monsters.add(monster1);
                this.monsters.add(monster2);
                this.monsters.add(monster3);
            }
            
            Waves wave = new Waves(this.duration, this.pre_wave_pause, monsters);
            waveslist.add(wave);       
        }

        // set background colour
        background(161, 128, 60);


        // set up font
        font = createFont("Arial", 16, true);
        textFont(font,16);
        fill(0);

        this.buttons = new ArrayList<>();

        buttonFF = new Button(50, this, "FF", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonP = new Button(110, this, "P", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonT = new Button(170, this, "T", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonU1 = new Button(230, this, "U1", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonU2 = new Button(300, this, "U2", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonU3 = new Button(360, this, "U3", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);
        buttonM = new Button(420, this, "M", this.tower_cost, this.initial_tower_range, this.initial_tower_firing_speed, this.initial_tower_damage, this.tower0, this.layout, this.fireball, this.monsters);

        this.buttons.add(buttonFF);
        this.buttons.add(buttonP);
        this.buttons.add(buttonT);
        this.buttons.add(buttonU1);
        this.buttons.add(buttonU2);
        this.buttons.add(buttonU3);
        this.buttons.add(buttonM);
    }

    /**
     * Receive key pressed signal from the keyboard.
     */
	@Override
    public void keyPressed(){
    }

    /**
     * Receive key released signal from the keyboard.
     */
	@Override
    public void keyReleased(){
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    public void mousePressed() {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseDragged(MouseEvent e) {
    }

    /**
     * Draw all elements in the game by current frame.
     */
    public void draw() {
        background(161, 128, 60);
        layout.drawLayout(this);
        this.mana.draw(this);
        textFont(font,20);
        fill(0);
        textAlign(PApplet.CENTER, PApplet.CENTER);
        text("MANA: " + this.mana.mana + "/" + this.mana.mana_cap, 350, 20);

        if (wave_increment >= this.waveslist.size()) {
            wave_increment = (this.waveslist.size()-1);
        }

        Waves wave = this.waveslist.get(wave_increment);
        text("Wave " + (wave_increment+2) + " starts " + wave.duration/60, 100, 20);
        for (Button button : this.buttons) {
            button.setMonsters(wave.monsters_final);
        }
            
        if (this.monster_timer >= monster_seconds_between_moves*FPS && (this.monster_increment < wave.monsters.size())) {
            // create new monster
            wave.monsters_final.add(wave.monsters.get(this.monster_increment));
            this.monster_timer = 0;
            this.monster_increment++;
        } else if (this.monster_increment >= wave.monsters.size()) {
            this.monster_increment = 0;
        }

        this.monster_timer ++;
        if (wave.pre_wave_pause > 0) {
            wave.pre_wave_pause --;
        }
            

        if (wave.duration > 0) {
            for (int i = wave.monsters_final.size() - 1; i >= 0; i--) {
                Monster monster = wave.monsters_final.get(i);
                monster.tick();
                monster.draw(this);

            }
            wave.duration --;
        } else {
            wave_increment += 1;
        }

        for (int i = 0; i < wave_increment; i++) {
            Waves wave1 = this.waveslist.get(i);
            for (int a = wave1.monsters_final.size() - 1; a >= 0; a--) {
                Monster monster = wave1.monsters_final.get(a);
                monster.tick();
                monster.draw(this);
            }
        }

        // display buttons
        for (Button button : this.buttons) {
            fill(161, 128, 60);
            // button.display();
            button.checkMousePressed();
            button.draw();
            if (button.name == "T") {
                this.towersArray = button.getTowers();
            } else {
                button.updateTowers(this.towersArray);
            }
        }

        textFont(font,20);
        fill(0);
        textAlign(PApplet.CENTER, PApplet.CENTER);
        text("FF", 650+25/2, 50+25/2);
        text("P", 650+25/2, 110+25/2);
        text("T", 650+25/2, 170+25/2);
        text("U1", 650+25/2, 230+25/2);
        text("U2", 650+25/2, 300+25/2);
        text("U3", 650+25/2, 360+25/2);
        text("M", 650+25/2, 420+25/2);
    }

    public static void main(String[] args) {
        PApplet.main("WizardTD.App");
    }

    /**
     * Source: https://stackoverflow.com/questions/37758061/rotate-a-buffered-image-in-java
     * @param pimg The image to be rotated
     * @param angle between 0 and 360 degrees
     * @return the new rotated image
     */
    public PImage rotateImageByDegrees(PImage pimg, double angle) {
        BufferedImage img = (BufferedImage) pimg.getNative();
        double rads = Math.toRadians(angle);
        double sin = Math.abs(Math.sin(rads)), cos = Math.abs(Math.cos(rads));
        int w = img.getWidth();
        int h = img.getHeight();
        int newWidth = (int) Math.floor(w * cos + h * sin);
        int newHeight = (int) Math.floor(h * cos + w * sin);

        PImage result = this.createImage(newWidth, newHeight, ARGB);
        //BufferedImage rotated = new BufferedImage(newWidth, newHeight, BufferedImage.TYPE_INT_ARGB);
        BufferedImage rotated = (BufferedImage) result.getNative();
        Graphics2D g2d = rotated.createGraphics();
        AffineTransform at = new AffineTransform();
        at.translate((newWidth - w) / 2, (newHeight - h) / 2);

        int x = w / 2;
        int y = h / 2;

        at.rotate(rads, x, y);
        g2d.setTransform(at);
        g2d.drawImage(img, 0, 0, null);
        g2d.dispose();
        for (int i = 0; i < newWidth; i++) {
            for (int j = 0; j < newHeight; j++) {
                result.set(i, j, rotated.getRGB(i, j));
            }
        }

        return result;
    }
}