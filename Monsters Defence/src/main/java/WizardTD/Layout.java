package WizardTD;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.io.FileNotFoundException;

import processing.core.PApplet;
import processing.core.PImage;

public class Layout {
    public int mapWidth;
    public int mapHeight;
    public final int TILESIZE = 32;

    public String mapFile;
    public String level;

    public PImage horizontal;
    public PImage vertical;
    public PImage leftTurn;
    public PImage rightTurn;
    public PImage leftTurnUp;
    public PImage rightTurnUp;
    public PImage down;
    public PImage left;
    public PImage up;
    public PImage right;
    public PImage path3;
    public PImage shrub;
    public PImage grass;
    public PImage wizards_house;

    public int[] wizard_coordinate;

    public ArrayList<Monster> monsters;

    private ArrayList<ArrayList<Integer>> coordinates;
    public ArrayList<Integer> coordinate;
    public int [][] arrays;

    
    public char[][] array;

    public Layout(int mapWidth, int mapHeight, String level, PImage horizontal, PImage vertical, PImage leftTurn, PImage rightTurn, PImage leftTurnUp, PImage rightTurnUp, PImage down, PImage left, PImage up, PImage right, PImage path3, PImage shrub, PImage grass, PImage wizards_house) {
        this.mapWidth = mapWidth;
        this.mapHeight = mapHeight;

        this.level = level;
        this.horizontal = horizontal;
        this.vertical = vertical;

        this.leftTurn = leftTurn;
        this.rightTurn = rightTurn;
        this.leftTurnUp = leftTurnUp;
        this.rightTurnUp = rightTurnUp;
        this.down = down;
        this.left = left;
        this.up = up;
        this.right = right;
        this.path3 = path3;
        this.shrub = shrub;
        this.grass = grass;
        this.wizards_house = wizards_house;

        this.monsters = new ArrayList<Monster>();
        this.coordinates = new ArrayList<ArrayList<Integer>>();

        this.arrays = new int[20][20];
        this.wizard_coordinate = new int[2];

    }

    /**
     * Read layout file and convert to array
     */
    public void readMap() {
        try {
            char[][] array = new char[mapHeight][mapWidth];
            File f = new File(level);
            Scanner scan = new Scanner(f);
            while (scan.hasNextLine()) {
                for (int row = 0; row < this.mapHeight; row++) {
                    if (scan.hasNextLine()) {
                        String line = scan.nextLine();
                        for (int column = 0; column < line.length(); column++) {
                            array[row][column] = line.charAt(column);
                        }
                    }
                }
            }
            this.array = array;
            scan.close();
            } catch(FileNotFoundException e) {
                System.err.println("File not found!");
            }
        }
    
    /**
     * Read array and draw images based on character
     */
    public void drawLayout(PApplet app) {
        for (int row = 0; row < this.mapHeight; row++) {
            for (int column = 0; column < this.mapWidth; column++) {
                char current = this.array[row][column];
                if (current == 'S') {
                    drawShrub(app, column, row, TILESIZE);
                } else if (current == ' ') {
                    drawGrass(app, column, row, TILESIZE);
                }
                else if (current == 'X') {
                    ArrayList<Integer> coordinate2 = new ArrayList<>(Arrays.asList(column, row));
                    this.coordinates.add(coordinate2);
                    drawHorizontal(app, column, row, TILESIZE);
                    // if has path on left
                    if (isValid(row, column-1)) {
                        // if has path on right
                        if (isValid(row, column+1)) {
                            // if path above
                            if (isValid(row-1, column)) {
                                // if path below
                                if (isValid(row+1, column)) {
                                    drawPath3(app, column, row, TILESIZE);
                                    continue;
                                } else {
                                    drawUp(app, column, row, TILESIZE);
                                    continue;
                                }
                            // no path above
                            } else {
                                // if path below
                                if (isValid(row+1, column)) {
                                    drawDown(app, column, row, TILESIZE);
                                    continue;
                                } else {
                                    drawHorizontal(app, column, row, TILESIZE);
                                    continue;
                                }
                            }
                        // no path on right
                        } else {
                            // if path above
                            if (isValid(row-1, column)) {
                                // if path below
                                if (isValid(row+1, column)) {
                                    drawLeft(app, column, row, TILESIZE);
                                    continue;
                                } else {
                                    drawleftTurnUp(app, column, row, TILESIZE);
                                    continue;
                                }
                            // no path above
                            } else if (isValid(row, column-1)) {
                                drawleftTurn(app, column, row, TILESIZE);
                                continue;
                            }
                        }
                    // no path on left
                    } else {
                        // if path on right
                        if (isValid(row, column+1)) {
                            // if path above
                            if (isValid(row-1, column)) {
                                // if path below
                                if (isValid(row+1, column)) {
                                    drawRight(app, column, row, TILESIZE);
                                    continue;
                                } else {
                                    drawrightTurnUp(app, column, row, TILESIZE);
                                    continue;
                                }
                            } else if (isValid(row+1, column)) {
                                drawrightTurn(app, column, row, TILESIZE);
                                continue;
                            }
                        } else {
                            drawVertical(app, column, row, TILESIZE);
                            continue;
                        }
                    }
                }
                else {
                    if (row < 20 && column < 20) {
                        drawGrass(app, column, row, TILESIZE);
                    }
                }
            }
        }

        for (int row = 0; row < this.mapHeight; row++) {
            for (int column = 0; column < this.mapWidth; column++) {
                char current = this.array[row][column];
                if (current == 'W') {
                    this.wizard_coordinate[0] = column;
                    this.wizard_coordinate[1] = row;
                    drawGrass(app, column, row, TILESIZE);
                    drawWizardHouse(app, column, row, TILESIZE);
                    break;
                }
            }
        }
    }
    
    /**
     * @return wizard house coordinates
     */
    public int[] getWizard() {
        return this.wizard_coordinate;
    }

    /**
     * @return array of path coordinates
     */
    public int[][] array() {
        for (int row = 0; row < this.mapHeight; row++) {
            for (int column = 0; column < this.mapWidth; column++) {
                char current = this.array[row][column];
                if (current == 'X') {
                    ArrayList<Integer> coordinate2 = new ArrayList<>(Arrays.asList(column, row));
                    this.coordinates.add(coordinate2);
                }
            }
        }
        
        for (ArrayList<Integer> row : this.coordinates) {
            int coordX = row.get(1);
            int coordY = row.get(0);
            arrays[coordX][coordY] = 1;
        }

        return arrays;        
    }

    /**
     * @return array of layout
     */
    public char[][] getArray() {
        return this.array;
    }

    /**
     * @return array of coordinates of path
     */
    public ArrayList<ArrayList<Integer>> getCoordinates() {
        return this.coordinates;
    }
    
    /**
     * @return whether current coordiante is a path, and is not on the outer edge of the map
     */
    public boolean isValid(int row, int column) {
        if (row >=0 && row < this.mapHeight && column >= 0 && column < this.mapWidth) {
            if (this.array[row][column] == 'X') {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * draw shrub
     */
    public void drawShrub(PApplet app, int column, int row, int imageSize) {
        app.image(this.shrub, column*imageSize, row*imageSize+40);
    }

    /**
     * draw horizontal path
     */
    public void drawHorizontal(PApplet app, int column, int row, int imageSize) {
        app.image(this.horizontal, column*imageSize, row*imageSize+40);
    }

    /**
     * draw vertical path
     */
    public void drawVertical(PApplet app, int column, int row, int imageSize) {
        app.image(this.vertical, column*imageSize, row*imageSize+40);
    }

    /**
     * draw left turn path
     */
    public void drawleftTurn(PApplet app, int column, int row, int imageSize) {
        app.image(this.leftTurn, column*imageSize, row*imageSize+40);
    }

    /**
     * draw right turn path
     */
    public void drawrightTurn(PApplet app, int column, int row, int imageSize) {
        app.image(this.rightTurn, column*imageSize, row*imageSize+40);
    }

    /**
     * draw right turn up path
     */
    public void drawrightTurnUp(PApplet app, int column, int row, int imageSize) {
        app.image(this.rightTurnUp, column*imageSize, row*imageSize+40);
    }

    /**
     * draw left turn up path
     */
    public void drawleftTurnUp(PApplet app, int column, int row, int imageSize) {
        app.image(this.leftTurnUp, column*imageSize, row*imageSize+40);
    }

    /**
     * draw down t-junction path
     */
    public void drawDown(PApplet app, int column, int row, int imageSize) {
        app.image(this.down, column*imageSize, row*imageSize+40);
    }

    /**
     * draw left t-junction path
     */
    public void drawLeft(PApplet app, int column, int row, int imageSize) {
        app.image(this.left, column*imageSize, row*imageSize+40);
    }

    /**
     * draw up t-junction path
     */
    public void drawUp(PApplet app, int column, int row, int imageSize) {
        app.image(this.up, column*imageSize, row*imageSize+40);
    }

    /**
     * draw right t-junction path
     */
    public void drawRight(PApplet app, int column, int row, int imageSize) {
        app.image(this.right, column*imageSize, row*imageSize+40);
    }

    /**
     * draw connecting path
     */
    public void drawPath3(PApplet app, int column, int row, int imageSize) {
        app.image(this.path3, column*imageSize, row*imageSize+40);
    }

    /**
     * draw wizard house
     */
    public void drawWizardHouse(PApplet app, int column, int row, int imageSize) {
        app.image(this.wizards_house, column*imageSize, row*imageSize+40);
    }

    /**
     * draw grass
     */
    public void drawGrass(PApplet app, int column, int row, int imageSize) {
        app.image(this.grass, column*imageSize, row*imageSize+40);
    }
}
